static void pxa2xx_map_inval_cache(struct map_info *map, unsigned long from,
ssize_t len)
{
unsigned long start = (unsigned long)map->cached + from;
unsigned long end = start + len;
start &= ~(CACHELINESIZE - 1);
while (start < end) {
asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (start));
start += CACHELINESIZE;
}
}
static int __devinit pxa2xx_flash_probe(struct platform_device *pdev)
{
struct flash_platform_data *flash = pdev->dev.platform_data;
struct pxa2xx_flash_info *info;
struct resource *res;
res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
if (!res)
return -ENODEV;
info = kzalloc(sizeof(struct pxa2xx_flash_info), GFP_KERNEL);
if (!info)
return -ENOMEM;
info->map.name = (char *) flash->name;
info->map.bankwidth = flash->width;
info->map.phys = res->start;
info->map.size = resource_size(res);
info->map.virt = ioremap(info->map.phys, info->map.size);
if (!info->map.virt) {
printk(KERN_WARNING "Failed to ioremap %s\n",
info->map.name);
return -ENOMEM;
}
info->map.cached =
ioremap_cached(info->map.phys, info->map.size);
if (!info->map.cached)
printk(KERN_WARNING "Failed to ioremap cached %s\n",
info->map.name);
info->map.inval_cache = pxa2xx_map_inval_cache;
simple_map_init(&info->map);
printk(KERN_NOTICE
"Probing %s at physical address 0x%08lx"
" (%d-bit bankwidth)\n",
info->map.name, (unsigned long)info->map.phys,
info->map.bankwidth * 8);
info->mtd = do_map_probe(flash->map_name, &info->map);
if (!info->mtd) {
iounmap((void *)info->map.virt);
if (info->map.cached)
iounmap(info->map.cached);
return -EIO;
}
info->mtd->owner = THIS_MODULE;
mtd_device_parse_register(info->mtd, probes, NULL, flash->parts,
flash->nr_parts);
platform_set_drvdata(pdev, info);
return 0;
}
static int __devexit pxa2xx_flash_remove(struct platform_device *dev)
{
struct pxa2xx_flash_info *info = platform_get_drvdata(dev);
platform_set_drvdata(dev, NULL);
mtd_device_unregister(info->mtd);
map_destroy(info->mtd);
iounmap(info->map.virt);
if (info->map.cached)
iounmap(info->map.cached);
kfree(info);
return 0;
}
static void pxa2xx_flash_shutdown(struct platform_device *dev)
{
struct pxa2xx_flash_info *info = platform_get_drvdata(dev);
if (info && mtd_suspend(info->mtd) == 0)
mtd_resume(info->mtd);
}
