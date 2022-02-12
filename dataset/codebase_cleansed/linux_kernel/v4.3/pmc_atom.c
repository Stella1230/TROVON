static inline u32 pmc_reg_read(struct pmc_dev *pmc, int reg_offset)
{
return readl(pmc->regmap + reg_offset);
}
static inline void pmc_reg_write(struct pmc_dev *pmc, int reg_offset, u32 val)
{
writel(val, pmc->regmap + reg_offset);
}
int pmc_atom_read(int offset, u32 *value)
{
struct pmc_dev *pmc = &pmc_device;
if (!pmc->init)
return -ENODEV;
*value = pmc_reg_read(pmc, offset);
return 0;
}
int pmc_atom_write(int offset, u32 value)
{
struct pmc_dev *pmc = &pmc_device;
if (!pmc->init)
return -ENODEV;
pmc_reg_write(pmc, offset, value);
return 0;
}
static void pmc_power_off(void)
{
u16 pm1_cnt_port;
u32 pm1_cnt_value;
pr_info("Preparing to enter system sleep state S5\n");
pm1_cnt_port = acpi_base_addr + PM1_CNT;
pm1_cnt_value = inl(pm1_cnt_port);
pm1_cnt_value &= SLEEP_TYPE_MASK;
pm1_cnt_value |= SLEEP_TYPE_S5;
pm1_cnt_value |= SLEEP_ENABLE;
outl(pm1_cnt_value, pm1_cnt_port);
}
static void pmc_hw_reg_setup(struct pmc_dev *pmc)
{
pmc_reg_write(pmc, PMC_S0IX_WAKE_EN, (u32)PMC_WAKE_EN_SETTING);
}
static void pmc_dev_state_print(struct seq_file *s, int reg_index,
u32 sts, const struct pmc_bit_map *sts_map,
u32 fd, const struct pmc_bit_map *fd_map)
{
int offset = PMC_REG_BIT_WIDTH * reg_index;
int index;
for (index = 0; sts_map[index].name; index++) {
seq_printf(s, "Dev: %-2d - %-32s\tState: %s [%s]\n",
offset + index, sts_map[index].name,
fd_map[index].bit_mask & fd ? "Disabled" : "Enabled ",
sts_map[index].bit_mask & sts ? "D3" : "D0");
}
}
static int pmc_dev_state_show(struct seq_file *s, void *unused)
{
struct pmc_dev *pmc = s->private;
const struct pmc_reg_map *m = pmc->map;
u32 func_dis, func_dis_2;
u32 d3_sts_0, d3_sts_1;
func_dis = pmc_reg_read(pmc, PMC_FUNC_DIS);
func_dis_2 = pmc_reg_read(pmc, PMC_FUNC_DIS_2);
d3_sts_0 = pmc_reg_read(pmc, PMC_D3_STS_0);
d3_sts_1 = pmc_reg_read(pmc, PMC_D3_STS_1);
pmc_dev_state_print(s, 0, d3_sts_0, m->d3_sts_0, func_dis, m->func_dis);
pmc_dev_state_print(s, 1, d3_sts_1, m->d3_sts_1, func_dis_2, m->func_dis_2);
return 0;
}
static int pmc_dev_state_open(struct inode *inode, struct file *file)
{
return single_open(file, pmc_dev_state_show, inode->i_private);
}
static int pmc_pss_state_show(struct seq_file *s, void *unused)
{
struct pmc_dev *pmc = s->private;
const struct pmc_bit_map *map = pmc->map->pss;
u32 pss = pmc_reg_read(pmc, PMC_PSS);
int index;
for (index = 0; map[index].name; index++) {
seq_printf(s, "Island: %-2d - %-32s\tState: %s\n",
index, map[index].name,
map[index].bit_mask & pss ? "Off" : "On");
}
return 0;
}
static int pmc_pss_state_open(struct inode *inode, struct file *file)
{
return single_open(file, pmc_pss_state_show, inode->i_private);
}
static int pmc_sleep_tmr_show(struct seq_file *s, void *unused)
{
struct pmc_dev *pmc = s->private;
u64 s0ir_tmr, s0i1_tmr, s0i2_tmr, s0i3_tmr, s0_tmr;
s0ir_tmr = (u64)pmc_reg_read(pmc, PMC_S0IR_TMR) << PMC_TMR_SHIFT;
s0i1_tmr = (u64)pmc_reg_read(pmc, PMC_S0I1_TMR) << PMC_TMR_SHIFT;
s0i2_tmr = (u64)pmc_reg_read(pmc, PMC_S0I2_TMR) << PMC_TMR_SHIFT;
s0i3_tmr = (u64)pmc_reg_read(pmc, PMC_S0I3_TMR) << PMC_TMR_SHIFT;
s0_tmr = (u64)pmc_reg_read(pmc, PMC_S0_TMR) << PMC_TMR_SHIFT;
seq_printf(s, "S0IR Residency:\t%lldus\n", s0ir_tmr);
seq_printf(s, "S0I1 Residency:\t%lldus\n", s0i1_tmr);
seq_printf(s, "S0I2 Residency:\t%lldus\n", s0i2_tmr);
seq_printf(s, "S0I3 Residency:\t%lldus\n", s0i3_tmr);
seq_printf(s, "S0 Residency:\t%lldus\n", s0_tmr);
return 0;
}
static int pmc_sleep_tmr_open(struct inode *inode, struct file *file)
{
return single_open(file, pmc_sleep_tmr_show, inode->i_private);
}
static void pmc_dbgfs_unregister(struct pmc_dev *pmc)
{
debugfs_remove_recursive(pmc->dbgfs_dir);
}
static int pmc_dbgfs_register(struct pmc_dev *pmc)
{
struct dentry *dir, *f;
dir = debugfs_create_dir("pmc_atom", NULL);
if (!dir)
return -ENOMEM;
pmc->dbgfs_dir = dir;
f = debugfs_create_file("dev_state", S_IFREG | S_IRUGO,
dir, pmc, &pmc_dev_state_ops);
if (!f)
goto err;
f = debugfs_create_file("pss_state", S_IFREG | S_IRUGO,
dir, pmc, &pmc_pss_state_ops);
if (!f)
goto err;
f = debugfs_create_file("sleep_state", S_IFREG | S_IRUGO,
dir, pmc, &pmc_sleep_tmr_ops);
if (!f)
goto err;
return 0;
err:
pmc_dbgfs_unregister(pmc);
return -ENODEV;
}
static int pmc_dbgfs_register(struct pmc_dev *pmc)
{
return 0;
}
static int pmc_setup_dev(struct pci_dev *pdev, const struct pci_device_id *ent)
{
struct pmc_dev *pmc = &pmc_device;
const struct pmc_reg_map *map = (struct pmc_reg_map *)ent->driver_data;
int ret;
pci_read_config_dword(pdev, ACPI_BASE_ADDR_OFFSET, &acpi_base_addr);
acpi_base_addr &= ACPI_BASE_ADDR_MASK;
if (acpi_base_addr != 0 && pm_power_off == NULL)
pm_power_off = pmc_power_off;
pci_read_config_dword(pdev, PMC_BASE_ADDR_OFFSET, &pmc->base_addr);
pmc->base_addr &= PMC_BASE_ADDR_MASK;
pmc->regmap = ioremap_nocache(pmc->base_addr, PMC_MMIO_REG_LEN);
if (!pmc->regmap) {
dev_err(&pdev->dev, "error: ioremap failed\n");
return -ENOMEM;
}
pmc->map = map;
pmc_hw_reg_setup(pmc);
ret = pmc_dbgfs_register(pmc);
if (ret)
dev_warn(&pdev->dev, "debugfs register failed\n");
pmc->init = true;
return ret;
}
static int __init pmc_atom_init(void)
{
struct pci_dev *pdev = NULL;
const struct pci_device_id *ent;
for_each_pci_dev(pdev) {
ent = pci_match_id(pmc_pci_ids, pdev);
if (ent)
return pmc_setup_dev(pdev, ent);
}
return -ENODEV;
}
