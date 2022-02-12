static enum drm_connector_status
exynos_dpi_detect(struct drm_connector *connector, bool force)
{
struct exynos_dpi *ctx = connector_to_dpi(connector);
if (ctx->panel && !ctx->panel->connector)
drm_panel_attach(ctx->panel, &ctx->connector);
return connector_status_connected;
}
static void exynos_dpi_connector_destroy(struct drm_connector *connector)
{
drm_sysfs_connector_remove(connector);
drm_connector_cleanup(connector);
}
static int exynos_dpi_get_modes(struct drm_connector *connector)
{
struct exynos_dpi *ctx = connector_to_dpi(connector);
if (ctx->vm) {
struct drm_display_mode *mode;
mode = drm_mode_create(connector->dev);
if (!mode) {
DRM_ERROR("failed to create a new display mode\n");
return 0;
}
drm_display_mode_from_videomode(ctx->vm, mode);
mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
drm_mode_probed_add(connector, mode);
return 1;
}
if (ctx->panel)
return ctx->panel->funcs->get_modes(ctx->panel);
return 0;
}
static struct drm_encoder *
exynos_dpi_best_encoder(struct drm_connector *connector)
{
struct exynos_dpi *ctx = connector_to_dpi(connector);
return ctx->encoder;
}
static int exynos_dpi_create_connector(struct exynos_drm_display *display,
struct drm_encoder *encoder)
{
struct exynos_dpi *ctx = display->ctx;
struct drm_connector *connector = &ctx->connector;
int ret;
ctx->encoder = encoder;
connector->polled = DRM_CONNECTOR_POLL_HPD;
ret = drm_connector_init(encoder->dev, connector,
&exynos_dpi_connector_funcs,
DRM_MODE_CONNECTOR_VGA);
if (ret) {
DRM_ERROR("failed to initialize connector with drm\n");
return ret;
}
drm_connector_helper_add(connector, &exynos_dpi_connector_helper_funcs);
drm_sysfs_connector_add(connector);
drm_mode_connector_attach_encoder(connector, encoder);
return 0;
}
static void exynos_dpi_poweron(struct exynos_dpi *ctx)
{
if (ctx->panel)
drm_panel_enable(ctx->panel);
}
static void exynos_dpi_poweroff(struct exynos_dpi *ctx)
{
if (ctx->panel)
drm_panel_disable(ctx->panel);
}
static void exynos_dpi_dpms(struct exynos_drm_display *display, int mode)
{
struct exynos_dpi *ctx = display->ctx;
switch (mode) {
case DRM_MODE_DPMS_ON:
if (ctx->dpms_mode != DRM_MODE_DPMS_ON)
exynos_dpi_poweron(ctx);
break;
case DRM_MODE_DPMS_STANDBY:
case DRM_MODE_DPMS_SUSPEND:
case DRM_MODE_DPMS_OFF:
if (ctx->dpms_mode == DRM_MODE_DPMS_ON)
exynos_dpi_poweroff(ctx);
break;
default:
break;
}
ctx->dpms_mode = mode;
}
static struct device_node *
of_get_child_by_name_reg(struct device_node *parent, const char *name, u32 reg)
{
struct device_node *np;
for_each_child_of_node(parent, np) {
u32 r;
if (!np->name || of_node_cmp(np->name, name))
continue;
if (of_property_read_u32(np, "reg", &r) < 0)
r = 0;
if (reg == r)
break;
}
return np;
}
static struct device_node *of_graph_get_port_by_reg(struct device_node *parent,
u32 reg)
{
struct device_node *ports, *port;
ports = of_get_child_by_name(parent, "ports");
if (ports)
parent = ports;
port = of_get_child_by_name_reg(parent, "port", reg);
of_node_put(ports);
return port;
}
static struct device_node *
of_graph_get_endpoint_by_reg(struct device_node *port, u32 reg)
{
return of_get_child_by_name_reg(port, "endpoint", reg);
}
static struct device_node *
of_graph_get_remote_port_parent(const struct device_node *node)
{
struct device_node *np;
unsigned int depth;
np = of_parse_phandle(node, "remote-endpoint", 0);
for (depth = 3; depth && np; depth--) {
np = of_get_next_parent(np);
if (depth == 2 && of_node_cmp(np->name, "ports"))
break;
}
return np;
}
static struct device_node *exynos_dpi_of_find_panel_node(struct device *dev)
{
struct device_node *np, *ep;
np = of_graph_get_port_by_reg(dev->of_node, FIMD_PORT_RGB);
if (!np)
return NULL;
ep = of_graph_get_endpoint_by_reg(np, 0);
of_node_put(np);
if (!ep)
return NULL;
np = of_graph_get_remote_port_parent(ep);
of_node_put(ep);
return np;
}
static int exynos_dpi_parse_dt(struct exynos_dpi *ctx)
{
struct device *dev = ctx->dev;
struct device_node *dn = dev->of_node;
struct device_node *np;
ctx->panel_node = exynos_dpi_of_find_panel_node(dev);
np = of_get_child_by_name(dn, "display-timings");
if (np) {
struct videomode *vm;
int ret;
of_node_put(np);
vm = devm_kzalloc(dev, sizeof(*ctx->vm), GFP_KERNEL);
if (!vm)
return -ENOMEM;
ret = of_get_videomode(dn, vm, 0);
if (ret < 0) {
devm_kfree(dev, vm);
return ret;
}
ctx->vm = vm;
return 0;
}
if (!ctx->panel_node)
return -EINVAL;
return 0;
}
struct exynos_drm_display *exynos_dpi_probe(struct device *dev)
{
struct exynos_dpi *ctx;
int ret;
ret = exynos_drm_component_add(dev,
EXYNOS_DEVICE_TYPE_CONNECTOR,
exynos_dpi_display.type);
if (ret)
return ERR_PTR(ret);
ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
if (!ctx)
goto err_del_component;
ctx->dev = dev;
exynos_dpi_display.ctx = ctx;
ctx->dpms_mode = DRM_MODE_DPMS_OFF;
ret = exynos_dpi_parse_dt(ctx);
if (ret < 0) {
devm_kfree(dev, ctx);
goto err_del_component;
}
if (ctx->panel_node) {
ctx->panel = of_drm_find_panel(ctx->panel_node);
if (!ctx->panel) {
exynos_drm_component_del(dev,
EXYNOS_DEVICE_TYPE_CONNECTOR);
return ERR_PTR(-EPROBE_DEFER);
}
}
return &exynos_dpi_display;
err_del_component:
exynos_drm_component_del(dev, EXYNOS_DEVICE_TYPE_CONNECTOR);
return NULL;
}
int exynos_dpi_remove(struct device *dev)
{
struct drm_encoder *encoder = exynos_dpi_display.encoder;
struct exynos_dpi *ctx = exynos_dpi_display.ctx;
exynos_dpi_dpms(&exynos_dpi_display, DRM_MODE_DPMS_OFF);
encoder->funcs->destroy(encoder);
drm_connector_cleanup(&ctx->connector);
exynos_drm_component_del(dev, EXYNOS_DEVICE_TYPE_CONNECTOR);
return 0;
}
