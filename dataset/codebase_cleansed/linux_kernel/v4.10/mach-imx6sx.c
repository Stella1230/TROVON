static int ar8031_phy_fixup(struct phy_device *dev)
{
u16 val;
phy_write(dev, 0x1d, 0x1f);
phy_write(dev, 0x1e, 0x8);
phy_write(dev, 0x1d, 0x5);
val = phy_read(dev, 0x1e);
val |= 0x0100;
phy_write(dev, 0x1e, val);
return 0;
}
static void __init imx6sx_enet_phy_init(void)
{
if (IS_BUILTIN(CONFIG_PHYLIB))
phy_register_fixup_for_uid(PHY_ID_AR8031, 0xffffffff,
ar8031_phy_fixup);
}
static void __init imx6sx_enet_clk_sel(void)
{
struct regmap *gpr;
gpr = syscon_regmap_lookup_by_compatible("fsl,imx6sx-iomuxc-gpr");
if (!IS_ERR(gpr)) {
regmap_update_bits(gpr, IOMUXC_GPR1,
IMX6SX_GPR1_FEC_CLOCK_MUX_SEL_MASK, 0);
regmap_update_bits(gpr, IOMUXC_GPR1,
IMX6SX_GPR1_FEC_CLOCK_PAD_DIR_MASK, 0);
} else {
pr_err("failed to find fsl,imx6sx-iomux-gpr regmap\n");
}
}
static inline void imx6sx_enet_init(void)
{
imx6sx_enet_phy_init();
imx6sx_enet_clk_sel();
}
static void __init imx6sx_init_machine(void)
{
struct device *parent;
parent = imx_soc_device_init();
if (parent == NULL)
pr_warn("failed to initialize soc device\n");
of_platform_default_populate(NULL, NULL, parent);
imx6sx_enet_init();
imx_anatop_init();
imx6sx_pm_init();
}
static void __init imx6sx_init_irq(void)
{
imx_gpc_check_dt();
imx_init_revision_from_anatop();
imx_init_l2cache();
imx_src_init();
irqchip_init();
imx6_pm_ccm_init("fsl,imx6sx-ccm");
}
static void __init imx6sx_init_late(void)
{
imx6sx_cpuidle_init();
if (IS_ENABLED(CONFIG_ARM_IMX6Q_CPUFREQ))
platform_device_register_simple("imx6q-cpufreq", -1, NULL, 0);
}
