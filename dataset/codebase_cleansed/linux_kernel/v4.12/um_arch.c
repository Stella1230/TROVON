static void __init add_arg(char *arg)
{
if (strlen(command_line) + strlen(arg) + 1 > COMMAND_LINE_SIZE) {
printf("add_arg: Too many command line arguments!\n");
exit(1);
}
if (strlen(command_line) > 0)
strcat(command_line, " ");
strcat(command_line, arg);
}
static int show_cpuinfo(struct seq_file *m, void *v)
{
int index = 0;
seq_printf(m, "processor\t: %d\n", index);
seq_printf(m, "vendor_id\t: User Mode Linux\n");
seq_printf(m, "model name\t: UML\n");
seq_printf(m, "mode\t\t: skas\n");
seq_printf(m, "host\t\t: %s\n", host_info);
seq_printf(m, "bogomips\t: %lu.%02lu\n\n",
loops_per_jiffy/(500000/HZ),
(loops_per_jiffy/(5000/HZ)) % 100);
return 0;
}
static void *c_start(struct seq_file *m, loff_t *pos)
{
return *pos < NR_CPUS ? cpu_data + *pos : NULL;
}
static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
++*pos;
return c_start(m, pos);
}
static void c_stop(struct seq_file *m, void *v)
{
}
static int __init uml_version_setup(char *line, int *add)
{
printf("%s\n", init_utsname()->release);
exit(0);
return 0;
}
static int __init uml_root_setup(char *line, int *add)
{
have_root = 1;
return 0;
}
static int __init no_skas_debug_setup(char *line, int *add)
{
printf("'debug' is not necessary to gdb UML in skas mode - run \n");
printf("'gdb linux'\n");
return 0;
}
static int __init Usage(char *line, int *add)
{
const char **p;
printf(usage_string, init_utsname()->release);
p = &__uml_help_start;
while (p < &__uml_help_end) {
printf("%s", *p);
p++;
}
exit(0);
return 0;
}
static void __init uml_checksetup(char *line, int *add)
{
struct uml_param *p;
p = &__uml_setup_start;
while (p < &__uml_setup_end) {
size_t n;
n = strlen(p->str);
if (!strncmp(line, p->str, n) && p->setup_func(line + n, add))
return;
p++;
}
}
static void __init uml_postsetup(void)
{
initcall_t *p;
p = &__uml_postsetup_start;
while (p < &__uml_postsetup_end) {
(*p)();
p++;
}
return;
}
static int panic_exit(struct notifier_block *self, unsigned long unused1,
void *unused2)
{
kmsg_dump(KMSG_DUMP_PANIC);
bust_spinlocks(1);
bust_spinlocks(0);
uml_exitcode = 1;
os_dump_core();
return 0;
}
void uml_finishsetup(void)
{
atomic_notifier_chain_register(&panic_notifier_list,
&panic_exit_notifier);
uml_postsetup();
new_thread_handler();
}
int __init linux_main(int argc, char **argv)
{
unsigned long avail, diff;
unsigned long virtmem_size, max_physmem;
unsigned long stack;
unsigned int i;
int add;
for (i = 1; i < argc; i++) {
if ((i == 1) && (argv[i][0] == ' '))
continue;
add = 1;
uml_checksetup(argv[i], &add);
if (add)
add_arg(argv[i]);
}
if (have_root == 0)
add_arg(DEFAULT_COMMAND_LINE);
host_task_size = os_get_top_address();
task_size = host_task_size & PGDIR_MASK;
os_early_checks();
brk_start = (unsigned long) sbrk(0);
diff = UML_ROUND_UP(brk_start) - UML_ROUND_UP(&_end);
if (diff > 1024 * 1024) {
printf("Adding %ld bytes to physical memory to account for "
"exec-shield gap\n", diff);
physmem_size += UML_ROUND_UP(brk_start) - UML_ROUND_UP(&_end);
}
uml_physmem = (unsigned long) __binary_start & PAGE_MASK;
uml_reserved = ROUND_4M(brk_start) + (1 << 22);
setup_machinename(init_utsname()->machine);
highmem = 0;
iomem_size = (iomem_size + PAGE_SIZE - 1) & PAGE_MASK;
max_physmem = TASK_SIZE - uml_physmem - iomem_size - MIN_VMALLOC;
max_physmem &= ~((1 << (PAGE_SHIFT + MAX_ORDER)) - 1);
if (physmem_size + iomem_size > max_physmem) {
highmem = physmem_size + iomem_size - max_physmem;
physmem_size -= highmem;
}
high_physmem = uml_physmem + physmem_size;
end_iomem = high_physmem + iomem_size;
high_memory = (void *) end_iomem;
start_vm = VMALLOC_START;
virtmem_size = physmem_size;
stack = (unsigned long) argv;
stack &= ~(1024 * 1024 - 1);
avail = stack - start_vm;
if (physmem_size > avail)
virtmem_size = avail;
end_vm = start_vm + virtmem_size;
if (virtmem_size < physmem_size)
printf("Kernel virtual memory size shrunk to %lu bytes\n",
virtmem_size);
os_flush_stdout();
return start_uml();
}
int __init __weak read_initrd(void)
{
return 0;
}
void __init setup_arch(char **cmdline_p)
{
stack_protections((unsigned long) &init_thread_info);
setup_physmem(uml_physmem, uml_reserved, physmem_size, highmem);
mem_total_pages(physmem_size, iomem_size, highmem);
read_initrd();
paging_init();
strlcpy(boot_command_line, command_line, COMMAND_LINE_SIZE);
*cmdline_p = command_line;
setup_hostinfo(host_info, sizeof host_info);
}
void __init check_bugs(void)
{
arch_check_bugs();
os_check_bugs();
}
void apply_alternatives(struct alt_instr *start, struct alt_instr *end)
{
}
