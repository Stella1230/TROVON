static unsigned int cpm_set(unsigned int cpm_reg, unsigned int mask)
{
unsigned int value;
value = dcr_read(cpm.dcr_host, cpm.dcr_offset[cpm_reg]);
dcr_write(cpm.dcr_host, cpm.dcr_offset[cpm_reg], value | mask);
return value;
}
static void cpm_idle_wait(void)
{
unsigned long msr_save;
msr_save = mfmsr();
mb();
mtmsr(msr_save|MSR_WE|MSR_EE|MSR_CE|MSR_DE);
isync();
mtmsr(msr_save);
isync();
}
static void cpm_idle_sleep(unsigned int mask)
{
unsigned int er_save;
er_save = cpm_set(CPM_ER, mask);
cpm_idle_wait();
dcr_write(cpm.dcr_host, cpm.dcr_offset[CPM_ER], er_save);
}
static void cpm_idle_doze(void)
{
cpm_idle_sleep(cpm.idle_doze);
}
static void cpm_idle_config(int mode)
{
int i;
if (idle_mode[mode].enabled)
return;
for (i = 0; i < ARRAY_SIZE(idle_mode); i++)
idle_mode[i].enabled = 0;
idle_mode[mode].enabled = 1;
}
static ssize_t cpm_idle_show(struct kobject *kobj,
struct kobj_attribute *attr, char *buf)
{
char *s = buf;
int i;
for (i = 0; i < ARRAY_SIZE(idle_mode); i++) {
if (idle_mode[i].enabled)
s += sprintf(s, "[%s] ", idle_mode[i].name);
else
s += sprintf(s, "%s ", idle_mode[i].name);
}
*(s-1) = '\n';
return s - buf;
}
static ssize_t cpm_idle_store(struct kobject *kobj,
struct kobj_attribute *attr,
const char *buf, size_t n)
{
int i;
char *p;
int len;
p = memchr(buf, '\n', n);
len = p ? p - buf : n;
for (i = 0; i < ARRAY_SIZE(idle_mode); i++) {
if (strncmp(buf, idle_mode[i].name, len) == 0) {
cpm_idle_config(i);
return n;
}
}
return -EINVAL;
}
static void cpm_idle_config_sysfs(void)
{
struct device *dev;
unsigned long ret;
dev = get_cpu_device(0);
ret = sysfs_create_file(&dev->kobj,
&cpm_idle_attr.attr);
if (ret)
printk(KERN_WARNING
"cpm: failed to create idle sysfs entry\n");
}
static void cpm_idle(void)
{
if (idle_mode[CPM_IDLE_DOZE].enabled)
cpm_idle_doze();
else
cpm_idle_wait();
}
static int cpm_suspend_valid(suspend_state_t state)
{
switch (state) {
case PM_SUSPEND_STANDBY:
return !!cpm.standby;
case PM_SUSPEND_MEM:
return !!cpm.suspend;
default:
return 0;
}
}
static void cpm_suspend_standby(unsigned int mask)
{
unsigned long tcr_save;
tcr_save = mfspr(SPRN_TCR);
mtspr(SPRN_TCR, tcr_save & ~TCR_DIE);
cpm_idle_sleep(mask);
mtspr(SPRN_TCR, tcr_save);
}
static int cpm_suspend_enter(suspend_state_t state)
{
switch (state) {
case PM_SUSPEND_STANDBY:
cpm_suspend_standby(cpm.standby);
break;
case PM_SUSPEND_MEM:
cpm_suspend_standby(cpm.suspend);
break;
}
return 0;
}
static int cpm_get_uint_property(struct device_node *np,
const char *name)
{
int len;
const unsigned int *prop = of_get_property(np, name, &len);
if (prop == NULL || len < sizeof(u32))
return 0;
return *prop;
}
static int __init cpm_init(void)
{
struct device_node *np;
int dcr_base, dcr_len;
int ret = 0;
if (!cpm.powersave_off) {
cpm_idle_config(CPM_IDLE_WAIT);
ppc_md.power_save = &cpm_idle;
}
np = of_find_compatible_node(NULL, NULL, "ibm,cpm");
if (!np) {
ret = -EINVAL;
goto out;
}
dcr_base = dcr_resource_start(np, 0);
dcr_len = dcr_resource_len(np, 0);
if (dcr_base == 0 || dcr_len == 0) {
printk(KERN_ERR "cpm: could not parse dcr property for %s\n",
np->full_name);
ret = -EINVAL;
goto out;
}
cpm.dcr_host = dcr_map(np, dcr_base, dcr_len);
if (!DCR_MAP_OK(cpm.dcr_host)) {
printk(KERN_ERR "cpm: failed to map dcr property for %s\n",
np->full_name);
ret = -EINVAL;
goto out;
}
if (cpm_get_uint_property(np, "er-offset") == 0) {
cpm.dcr_offset[CPM_ER] = 0;
cpm.dcr_offset[CPM_FR] = 1;
cpm.dcr_offset[CPM_SR] = 2;
} else {
cpm.dcr_offset[CPM_ER] = 1;
cpm.dcr_offset[CPM_FR] = 2;
cpm.dcr_offset[CPM_SR] = 0;
}
cpm.unused = cpm_get_uint_property(np, "unused-units");
cpm.idle_doze = cpm_get_uint_property(np, "idle-doze");
cpm.standby = cpm_get_uint_property(np, "standby");
cpm.suspend = cpm_get_uint_property(np, "suspend");
if (cpm.unused) {
cpm_set(CPM_ER, cpm.unused);
cpm_set(CPM_FR, cpm.unused);
}
if (!cpm.powersave_off && cpm.idle_doze)
cpm_idle_config_sysfs();
if (cpm.standby || cpm.suspend)
suspend_set_ops(&cpm_suspend_ops);
out:
if (np)
of_node_put(np);
return ret;
}
static int __init cpm_powersave_off(char *arg)
{
cpm.powersave_off = 1;
return 0;
}
