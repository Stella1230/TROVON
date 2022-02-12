static inline unsigned int socinfo_to_major(u32 socinfo)
{
return FIELD_GET(SOCINFO_MAJOR, socinfo);
}
static inline unsigned int socinfo_to_minor(u32 socinfo)
{
return FIELD_GET(SOCINFO_MINOR, socinfo);
}
static inline unsigned int socinfo_to_pack(u32 socinfo)
{
return FIELD_GET(SOCINFO_PACK, socinfo);
}
static inline unsigned int socinfo_to_misc(u32 socinfo)
{
return FIELD_GET(SOCINFO_MISC, socinfo);
}
static const char *socinfo_to_package_id(u32 socinfo)
{
unsigned int pack = socinfo_to_pack(socinfo) & 0xf0;
unsigned int major = socinfo_to_major(socinfo);
int i;
for (i = 0 ; i < ARRAY_SIZE(soc_packages) ; ++i) {
if (soc_packages[i].major_id == major &&
soc_packages[i].pack_id == pack)
return soc_packages[i].name;
}
return "Unknown";
}
static const char *socinfo_to_soc_id(u32 socinfo)
{
unsigned int id = socinfo_to_major(socinfo);
int i;
for (i = 0 ; i < ARRAY_SIZE(soc_ids) ; ++i) {
if (soc_ids[i].id == id)
return soc_ids[i].name;
}
return "Unknown";
}
int __init meson_gx_socinfo_init(void)
{
struct soc_device_attribute *soc_dev_attr;
struct soc_device *soc_dev;
struct device_node *np;
struct regmap *regmap;
unsigned int socinfo;
struct device *dev;
int ret;
np = of_find_compatible_node(NULL, NULL, "amlogic,meson-gx-ao-secure");
if (!np)
return -ENODEV;
if (!of_device_is_available(np))
return -ENODEV;
if (!of_property_read_bool(np, "amlogic,has-chip-id"))
return -ENODEV;
regmap = syscon_node_to_regmap(np);
of_node_put(np);
if (IS_ERR(regmap)) {
pr_err("%s: failed to get regmap\n", __func__);
return -ENODEV;
}
ret = regmap_read(regmap, AO_SEC_SOCINFO_OFFSET, &socinfo);
if (ret < 0)
return ret;
if (!socinfo) {
pr_err("%s: invalid chipid value\n", __func__);
return -EINVAL;
}
soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
if (!soc_dev_attr)
return -ENODEV;
soc_dev_attr->family = "Amlogic Meson";
np = of_find_node_by_path("/");
of_property_read_string(np, "model", &soc_dev_attr->machine);
of_node_put(np);
soc_dev_attr->revision = kasprintf(GFP_KERNEL, "%x:%x - %x:%x",
socinfo_to_major(socinfo),
socinfo_to_minor(socinfo),
socinfo_to_pack(socinfo),
socinfo_to_misc(socinfo));
soc_dev_attr->soc_id = kasprintf(GFP_KERNEL, "%s (%s)",
socinfo_to_soc_id(socinfo),
socinfo_to_package_id(socinfo));
soc_dev = soc_device_register(soc_dev_attr);
if (IS_ERR(soc_dev)) {
kfree(soc_dev_attr->revision);
kfree_const(soc_dev_attr->soc_id);
kfree(soc_dev_attr);
return PTR_ERR(soc_dev);
}
dev = soc_device_to_device(soc_dev);
dev_info(dev, "Amlogic Meson %s Revision %x:%x (%x:%x) Detected\n",
soc_dev_attr->soc_id,
socinfo_to_major(socinfo),
socinfo_to_minor(socinfo),
socinfo_to_pack(socinfo),
socinfo_to_misc(socinfo));
return 0;
}
