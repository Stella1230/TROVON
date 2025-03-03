static int __xipram cfi_probe_chip(struct map_info *map, __u32 base,
unsigned long *chip_map, struct cfi_private *cfi)
{
int i;
if ((base + 0) >= map->size) {
printk(KERN_NOTICE
"Probe at base[0x00](0x%08lx) past the end of the map(0x%08lx)\n",
(unsigned long)base, map->size -1);
return 0;
}
if ((base + 0xff) >= map->size) {
printk(KERN_NOTICE
"Probe at base[0x55](0x%08lx) past the end of the map(0x%08lx)\n",
(unsigned long)base + 0x55, map->size -1);
return 0;
}
xip_disable();
if (!cfi_qry_mode_on(base, map, cfi)) {
xip_enable(base, map, cfi);
return 0;
}
if (!cfi->numchips) {
return cfi_chip_setup(map, cfi);
}
for (i=0; i < (base >> cfi->chipshift); i++) {
unsigned long start;
if(!test_bit(i, chip_map)) {
continue;
}
start = i << cfi->chipshift;
if (cfi_qry_present(map, start, cfi)) {
cfi_qry_mode_off(start, map, cfi);
if (!cfi_qry_present(map, start, cfi)) {
xip_allowed(base, map);
printk(KERN_DEBUG "%s: Found an alias at 0x%x for the chip at 0x%lx\n",
map->name, base, start);
return 0;
}
cfi_qry_mode_off(base, map, cfi);
if (cfi_qry_present(map, base, cfi)) {
xip_allowed(base, map);
printk(KERN_DEBUG "%s: Found an alias at 0x%x for the chip at 0x%lx\n",
map->name, base, start);
return 0;
}
}
}
set_bit((base >> cfi->chipshift), chip_map);
cfi->numchips++;
cfi_qry_mode_off(base, map, cfi);
xip_allowed(base, map);
printk(KERN_INFO "%s: Found %d x%d devices at 0x%x in %d-bit bank\n",
map->name, cfi->interleave, cfi->device_type*8, base,
map->bankwidth*8);
return 1;
}
static int __xipram cfi_chip_setup(struct map_info *map,
struct cfi_private *cfi)
{
int ofs_factor = cfi->interleave*cfi->device_type;
__u32 base = 0;
int num_erase_regions = cfi_read_query(map, base + (0x10 + 28)*ofs_factor);
int i;
int addr_unlock1 = 0x555, addr_unlock2 = 0x2AA;
xip_enable(base, map, cfi);
#ifdef DEBUG_CFI
printk("Number of erase regions: %d\n", num_erase_regions);
#endif
if (!num_erase_regions)
return 0;
cfi->cfiq = kmalloc(sizeof(struct cfi_ident) + num_erase_regions * 4, GFP_KERNEL);
if (!cfi->cfiq) {
printk(KERN_WARNING "%s: kmalloc failed for CFI ident structure\n", map->name);
return 0;
}
memset(cfi->cfiq,0,sizeof(struct cfi_ident));
cfi->cfi_mode = CFI_MODE_CFI;
cfi->sector_erase_cmd = CMD(0x30);
xip_disable_qry(base, map, cfi);
for (i=0; i<(sizeof(struct cfi_ident) + num_erase_regions * 4); i++)
((unsigned char *)cfi->cfiq)[i] = cfi_read_query(map,base + (0x10 + i)*ofs_factor);
cfi->cfiq->P_ID = le16_to_cpu(cfi->cfiq->P_ID);
cfi->cfiq->P_ADR = le16_to_cpu(cfi->cfiq->P_ADR);
cfi->cfiq->A_ID = le16_to_cpu(cfi->cfiq->A_ID);
cfi->cfiq->A_ADR = le16_to_cpu(cfi->cfiq->A_ADR);
cfi->cfiq->InterfaceDesc = le16_to_cpu(cfi->cfiq->InterfaceDesc);
cfi->cfiq->MaxBufWriteSize = le16_to_cpu(cfi->cfiq->MaxBufWriteSize);
#ifdef DEBUG_CFI
print_cfi_ident(cfi->cfiq);
#endif
for (i=0; i<cfi->cfiq->NumEraseRegions; i++) {
cfi->cfiq->EraseRegionInfo[i] = le32_to_cpu(cfi->cfiq->EraseRegionInfo[i]);
#ifdef DEBUG_CFI
printk(" Erase Region #%d: BlockSize 0x%4.4X bytes, %d blocks\n",
i, (cfi->cfiq->EraseRegionInfo[i] >> 8) & ~0xff,
(cfi->cfiq->EraseRegionInfo[i] & 0xffff) + 1);
#endif
}
if (cfi->cfiq->P_ID == P_ID_SST_OLD) {
addr_unlock1 = 0x5555;
addr_unlock2 = 0x2AAA;
}
cfi_send_gen_cmd(0xf0, 0, base, map, cfi, cfi->device_type, NULL);
cfi_send_gen_cmd(0xaa, addr_unlock1, base, map, cfi, cfi->device_type, NULL);
cfi_send_gen_cmd(0x55, addr_unlock2, base, map, cfi, cfi->device_type, NULL);
cfi_send_gen_cmd(0x90, addr_unlock1, base, map, cfi, cfi->device_type, NULL);
cfi->mfr = cfi_read_query16(map, base);
cfi->id = cfi_read_query16(map, base + ofs_factor);
if (cfi->mfr == CFI_MFR_AMD && (cfi->id & 0xff) == 0x7e)
cfi->id = cfi_read_query(map, base + 0xe * ofs_factor) << 8 |
cfi_read_query(map, base + 0xf * ofs_factor);
cfi_qry_mode_off(base, map, cfi);
xip_allowed(base, map);
printk(KERN_INFO "%s: Found %d x%d devices at 0x%x in %d-bit bank. Manufacturer ID %#08x Chip ID %#08x\n",
map->name, cfi->interleave, cfi->device_type*8, base,
map->bankwidth*8, cfi->mfr, cfi->id);
return 1;
}
static char *vendorname(__u16 vendor)
{
switch (vendor) {
case P_ID_NONE:
return "None";
case P_ID_INTEL_EXT:
return "Intel/Sharp Extended";
case P_ID_AMD_STD:
return "AMD/Fujitsu Standard";
case P_ID_INTEL_STD:
return "Intel/Sharp Standard";
case P_ID_AMD_EXT:
return "AMD/Fujitsu Extended";
case P_ID_WINBOND:
return "Winbond Standard";
case P_ID_ST_ADV:
return "ST Advanced";
case P_ID_MITSUBISHI_STD:
return "Mitsubishi Standard";
case P_ID_MITSUBISHI_EXT:
return "Mitsubishi Extended";
case P_ID_SST_PAGE:
return "SST Page Write";
case P_ID_SST_OLD:
return "SST 39VF160x/39VF320x";
case P_ID_INTEL_PERFORMANCE:
return "Intel Performance Code";
case P_ID_INTEL_DATA:
return "Intel Data";
case P_ID_RESERVED:
return "Not Allowed / Reserved for Future Use";
default:
return "Unknown";
}
}
static void print_cfi_ident(struct cfi_ident *cfip)
{
#if 0
if (cfip->qry[0] != 'Q' || cfip->qry[1] != 'R' || cfip->qry[2] != 'Y') {
printk("Invalid CFI ident structure.\n");
return;
}
#endif
printk("Primary Vendor Command Set: %4.4X (%s)\n", cfip->P_ID, vendorname(cfip->P_ID));
if (cfip->P_ADR)
printk("Primary Algorithm Table at %4.4X\n", cfip->P_ADR);
else
printk("No Primary Algorithm Table\n");
printk("Alternative Vendor Command Set: %4.4X (%s)\n", cfip->A_ID, vendorname(cfip->A_ID));
if (cfip->A_ADR)
printk("Alternate Algorithm Table at %4.4X\n", cfip->A_ADR);
else
printk("No Alternate Algorithm Table\n");
printk("Vcc Minimum: %2d.%d V\n", cfip->VccMin >> 4, cfip->VccMin & 0xf);
printk("Vcc Maximum: %2d.%d V\n", cfip->VccMax >> 4, cfip->VccMax & 0xf);
if (cfip->VppMin) {
printk("Vpp Minimum: %2d.%d V\n", cfip->VppMin >> 4, cfip->VppMin & 0xf);
printk("Vpp Maximum: %2d.%d V\n", cfip->VppMax >> 4, cfip->VppMax & 0xf);
}
else
printk("No Vpp line\n");
printk("Typical byte/word write timeout: %d Ã&#8218;Âµs\n", 1<<cfip->WordWriteTimeoutTyp);
printk("Maximum byte/word write timeout: %d Ã&#8218;Âµs\n", (1<<cfip->WordWriteTimeoutMax) * (1<<cfip->WordWriteTimeoutTyp));
if (cfip->BufWriteTimeoutTyp || cfip->BufWriteTimeoutMax) {
printk("Typical full buffer write timeout: %d Ã&#8218;Âµs\n", 1<<cfip->BufWriteTimeoutTyp);
printk("Maximum full buffer write timeout: %d Ã&#8218;Âµs\n", (1<<cfip->BufWriteTimeoutMax) * (1<<cfip->BufWriteTimeoutTyp));
}
else
printk("Full buffer write not supported\n");
printk("Typical block erase timeout: %d ms\n", 1<<cfip->BlockEraseTimeoutTyp);
printk("Maximum block erase timeout: %d ms\n", (1<<cfip->BlockEraseTimeoutMax) * (1<<cfip->BlockEraseTimeoutTyp));
if (cfip->ChipEraseTimeoutTyp || cfip->ChipEraseTimeoutMax) {
printk("Typical chip erase timeout: %d ms\n", 1<<cfip->ChipEraseTimeoutTyp);
printk("Maximum chip erase timeout: %d ms\n", (1<<cfip->ChipEraseTimeoutMax) * (1<<cfip->ChipEraseTimeoutTyp));
}
else
printk("Chip erase not supported\n");
printk("Device size: 0x%X bytes (%d MiB)\n", 1 << cfip->DevSize, 1<< (cfip->DevSize - 20));
printk("Flash Device Interface description: 0x%4.4X\n", cfip->InterfaceDesc);
switch(cfip->InterfaceDesc) {
case CFI_INTERFACE_X8_ASYNC:
printk(" - x8-only asynchronous interface\n");
break;
case CFI_INTERFACE_X16_ASYNC:
printk(" - x16-only asynchronous interface\n");
break;
case CFI_INTERFACE_X8_BY_X16_ASYNC:
printk(" - supports x8 and x16 via BYTE# with asynchronous interface\n");
break;
case CFI_INTERFACE_X32_ASYNC:
printk(" - x32-only asynchronous interface\n");
break;
case CFI_INTERFACE_X16_BY_X32_ASYNC:
printk(" - supports x16 and x32 via Word# with asynchronous interface\n");
break;
case CFI_INTERFACE_NOT_ALLOWED:
printk(" - Not Allowed / Reserved\n");
break;
default:
printk(" - Unknown\n");
break;
}
printk("Max. bytes in buffer write: 0x%x\n", 1<< cfip->MaxBufWriteSize);
printk("Number of Erase Block Regions: %d\n", cfip->NumEraseRegions);
}
struct mtd_info *cfi_probe(struct map_info *map)
{
return mtd_do_chip_probe(map, &cfi_chip_probe);
}
static int __init cfi_probe_init(void)
{
register_mtd_chip_driver(&cfi_chipdrv);
return 0;
}
static void __exit cfi_probe_exit(void)
{
unregister_mtd_chip_driver(&cfi_chipdrv);
}
