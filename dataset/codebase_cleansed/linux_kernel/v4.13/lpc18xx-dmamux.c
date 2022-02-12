static void lpc18xx_dmamux_free(struct device *dev, void *route_data)
{
struct lpc18xx_dmamux_data *dmamux = dev_get_drvdata(dev);
struct lpc18xx_dmamux *mux = route_data;
unsigned long flags;
spin_lock_irqsave(&dmamux->lock, flags);
mux->busy = false;
spin_unlock_irqrestore(&dmamux->lock, flags);
}
static void *lpc18xx_dmamux_reserve(struct of_phandle_args *dma_spec,
struct of_dma *ofdma)
{
struct platform_device *pdev = of_find_device_by_node(ofdma->of_node);
struct lpc18xx_dmamux_data *dmamux = platform_get_drvdata(pdev);
unsigned long flags;
unsigned mux;
if (dma_spec->args_count != 3) {
dev_err(&pdev->dev, "invalid number of dma mux args\n");
return ERR_PTR(-EINVAL);
}
mux = dma_spec->args[0];
if (mux >= dmamux->dma_master_requests) {
dev_err(&pdev->dev, "invalid mux number: %d\n",
dma_spec->args[0]);
return ERR_PTR(-EINVAL);
}
if (dma_spec->args[1] > LPC18XX_DMAMUX_MAX_VAL) {
dev_err(&pdev->dev, "invalid dma mux value: %d\n",
dma_spec->args[1]);
return ERR_PTR(-EINVAL);
}
dma_spec->np = of_parse_phandle(ofdma->of_node, "dma-masters", 0);
if (!dma_spec->np) {
dev_err(&pdev->dev, "can't get dma master\n");
return ERR_PTR(-EINVAL);
}
spin_lock_irqsave(&dmamux->lock, flags);
if (dmamux->muxes[mux].busy) {
spin_unlock_irqrestore(&dmamux->lock, flags);
dev_err(&pdev->dev, "dma request %u busy with %u.%u\n",
mux, mux, dmamux->muxes[mux].value);
of_node_put(dma_spec->np);
return ERR_PTR(-EBUSY);
}
dmamux->muxes[mux].busy = true;
dmamux->muxes[mux].value = dma_spec->args[1];
regmap_update_bits(dmamux->reg, LPC18XX_CREG_DMAMUX,
LPC18XX_DMAMUX_MASK(mux),
LPC18XX_DMAMUX_VAL(dmamux->muxes[mux].value, mux));
spin_unlock_irqrestore(&dmamux->lock, flags);
dma_spec->args[1] = dma_spec->args[2];
dma_spec->args_count = 2;
dev_dbg(&pdev->dev, "mapping dmamux %u.%u to dma request %u\n", mux,
dmamux->muxes[mux].value, mux);
return &dmamux->muxes[mux];
}
static int lpc18xx_dmamux_probe(struct platform_device *pdev)
{
struct device_node *dma_np, *np = pdev->dev.of_node;
struct lpc18xx_dmamux_data *dmamux;
int ret;
dmamux = devm_kzalloc(&pdev->dev, sizeof(*dmamux), GFP_KERNEL);
if (!dmamux)
return -ENOMEM;
dmamux->reg = syscon_regmap_lookup_by_compatible("nxp,lpc1850-creg");
if (IS_ERR(dmamux->reg)) {
dev_err(&pdev->dev, "syscon lookup failed\n");
return PTR_ERR(dmamux->reg);
}
ret = of_property_read_u32(np, "dma-requests",
&dmamux->dma_mux_requests);
if (ret) {
dev_err(&pdev->dev, "missing dma-requests property\n");
return ret;
}
dma_np = of_parse_phandle(np, "dma-masters", 0);
if (!dma_np) {
dev_err(&pdev->dev, "can't get dma master\n");
return -ENODEV;
}
ret = of_property_read_u32(dma_np, "dma-requests",
&dmamux->dma_master_requests);
of_node_put(dma_np);
if (ret) {
dev_err(&pdev->dev, "missing master dma-requests property\n");
return ret;
}
dmamux->muxes = devm_kcalloc(&pdev->dev, dmamux->dma_master_requests,
sizeof(struct lpc18xx_dmamux),
GFP_KERNEL);
if (!dmamux->muxes)
return -ENOMEM;
spin_lock_init(&dmamux->lock);
platform_set_drvdata(pdev, dmamux);
dmamux->dmarouter.dev = &pdev->dev;
dmamux->dmarouter.route_free = lpc18xx_dmamux_free;
return of_dma_router_register(np, lpc18xx_dmamux_reserve,
&dmamux->dmarouter);
}
static int __init lpc18xx_dmamux_init(void)
{
return platform_driver_register(&lpc18xx_dmamux_driver);
}
