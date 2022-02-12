static void rtlx_interrupt(void)
{
int i;
struct rtlx_info *info;
struct rtlx_info **p = vpe_get_shared(aprp_cpu_index());
if (p == NULL || *p == NULL)
return;
info = *p;
if (info->ap_int_pending == 1 && smp_processor_id() == 0) {
for (i = 0; i < RTLX_CHANNELS; i++) {
wake_up(&channel_wqs[i].lx_queue);
wake_up(&channel_wqs[i].rt_queue);
}
info->ap_int_pending = 0;
}
}
void _interrupt_sp(void)
{
smp_send_reschedule(aprp_cpu_index());
}
int __init rtlx_module_init(void)
{
struct device *dev;
int i, err;
if (!cpu_has_mipsmt) {
pr_warn("VPE loader: not a MIPS MT capable processor\n");
return -ENODEV;
}
if (num_possible_cpus() - aprp_cpu_index() < 1) {
pr_warn("No TCs reserved for AP/SP, not initializing RTLX.\n"
"Pass maxcpus=<n> argument as kernel argument\n");
return -ENODEV;
}
major = register_chrdev(0, RTLX_MODULE_NAME, &rtlx_fops);
if (major < 0) {
pr_err("rtlx_module_init: unable to register device\n");
return major;
}
for (i = 0; i < RTLX_CHANNELS; i++) {
init_waitqueue_head(&channel_wqs[i].rt_queue);
init_waitqueue_head(&channel_wqs[i].lx_queue);
atomic_set(&channel_wqs[i].in_open, 0);
mutex_init(&channel_wqs[i].mutex);
dev = device_create(mt_class, NULL, MKDEV(major, i), NULL,
"%s%d", RTLX_MODULE_NAME, i);
if (IS_ERR(dev)) {
while (i--)
device_destroy(mt_class, MKDEV(major, i));
err = PTR_ERR(dev);
goto out_chrdev;
}
}
rtlx_notify.start = rtlx_starting;
rtlx_notify.stop = rtlx_stopping;
vpe_notify(aprp_cpu_index(), &rtlx_notify);
if (cpu_has_vint) {
aprp_hook = rtlx_interrupt;
} else {
pr_err("APRP RTLX init on non-vectored-interrupt processor\n");
err = -ENODEV;
goto out_class;
}
return 0;
out_class:
for (i = 0; i < RTLX_CHANNELS; i++)
device_destroy(mt_class, MKDEV(major, i));
out_chrdev:
unregister_chrdev(major, RTLX_MODULE_NAME);
return err;
}
void __exit rtlx_module_exit(void)
{
int i;
for (i = 0; i < RTLX_CHANNELS; i++)
device_destroy(mt_class, MKDEV(major, i));
unregister_chrdev(major, RTLX_MODULE_NAME);
aprp_hook = NULL;
}
