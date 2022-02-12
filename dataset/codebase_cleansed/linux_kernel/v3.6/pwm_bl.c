static int pwm_backlight_update_status(struct backlight_device *bl)
{
struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);
int brightness = bl->props.brightness;
int max = bl->props.max_brightness;
if (bl->props.power != FB_BLANK_UNBLANK)
brightness = 0;
if (bl->props.fb_blank != FB_BLANK_UNBLANK)
brightness = 0;
if (pb->notify)
brightness = pb->notify(pb->dev, brightness);
if (brightness == 0) {
pwm_config(pb->pwm, 0, pb->period);
pwm_disable(pb->pwm);
} else {
int duty_cycle;
if (pb->levels) {
duty_cycle = pb->levels[brightness];
max = pb->levels[max];
} else {
duty_cycle = brightness;
}
duty_cycle = pb->lth_brightness +
(duty_cycle * (pb->period - pb->lth_brightness) / max);
pwm_config(pb->pwm, duty_cycle, pb->period);
pwm_enable(pb->pwm);
}
if (pb->notify_after)
pb->notify_after(pb->dev, brightness);
return 0;
}
static int pwm_backlight_get_brightness(struct backlight_device *bl)
{
return bl->props.brightness;
}
static int pwm_backlight_check_fb(struct backlight_device *bl,
struct fb_info *info)
{
struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);
return !pb->check_fb || pb->check_fb(pb->dev, info);
}
static int pwm_backlight_parse_dt(struct device *dev,
struct platform_pwm_backlight_data *data)
{
struct device_node *node = dev->of_node;
struct property *prop;
int length;
u32 value;
int ret;
if (!node)
return -ENODEV;
memset(data, 0, sizeof(*data));
prop = of_find_property(node, "brightness-levels", &length);
if (!prop)
return -EINVAL;
data->max_brightness = length / sizeof(u32);
if (data->max_brightness > 0) {
size_t size = sizeof(*data->levels) * data->max_brightness;
data->levels = devm_kzalloc(dev, size, GFP_KERNEL);
if (!data->levels)
return -ENOMEM;
ret = of_property_read_u32_array(node, "brightness-levels",
data->levels,
data->max_brightness);
if (ret < 0)
return ret;
ret = of_property_read_u32(node, "default-brightness-level",
&value);
if (ret < 0)
return ret;
if (value >= data->max_brightness) {
dev_warn(dev, "invalid default brightness level: %u, using %u\n",
value, data->max_brightness - 1);
value = data->max_brightness - 1;
}
data->dft_brightness = value;
data->max_brightness--;
}
return 0;
}
static int pwm_backlight_parse_dt(struct device *dev,
struct platform_pwm_backlight_data *data)
{
return -ENODEV;
}
static int pwm_backlight_probe(struct platform_device *pdev)
{
struct platform_pwm_backlight_data *data = pdev->dev.platform_data;
struct platform_pwm_backlight_data defdata;
struct backlight_properties props;
struct backlight_device *bl;
struct pwm_bl_data *pb;
unsigned int max;
int ret;
if (!data) {
ret = pwm_backlight_parse_dt(&pdev->dev, &defdata);
if (ret < 0) {
dev_err(&pdev->dev, "failed to find platform data\n");
return ret;
}
data = &defdata;
}
if (data->init) {
ret = data->init(&pdev->dev);
if (ret < 0)
return ret;
}
pb = devm_kzalloc(&pdev->dev, sizeof(*pb), GFP_KERNEL);
if (!pb) {
dev_err(&pdev->dev, "no memory for state\n");
ret = -ENOMEM;
goto err_alloc;
}
if (data->levels) {
max = data->levels[data->max_brightness];
pb->levels = data->levels;
} else
max = data->max_brightness;
pb->notify = data->notify;
pb->notify_after = data->notify_after;
pb->check_fb = data->check_fb;
pb->exit = data->exit;
pb->dev = &pdev->dev;
pb->pwm = pwm_get(&pdev->dev, NULL);
if (IS_ERR(pb->pwm)) {
dev_err(&pdev->dev, "unable to request PWM, trying legacy API\n");
pb->pwm = pwm_request(data->pwm_id, "pwm-backlight");
if (IS_ERR(pb->pwm)) {
dev_err(&pdev->dev, "unable to request legacy PWM\n");
ret = PTR_ERR(pb->pwm);
goto err_alloc;
}
}
dev_dbg(&pdev->dev, "got pwm for backlight\n");
if (data->pwm_period_ns > 0)
pwm_set_period(pb->pwm, data->pwm_period_ns);
pb->period = pwm_get_period(pb->pwm);
pb->lth_brightness = data->lth_brightness * (pb->period / max);
memset(&props, 0, sizeof(struct backlight_properties));
props.type = BACKLIGHT_RAW;
props.max_brightness = data->max_brightness;
bl = backlight_device_register(dev_name(&pdev->dev), &pdev->dev, pb,
&pwm_backlight_ops, &props);
if (IS_ERR(bl)) {
dev_err(&pdev->dev, "failed to register backlight\n");
ret = PTR_ERR(bl);
goto err_bl;
}
bl->props.brightness = data->dft_brightness;
backlight_update_status(bl);
platform_set_drvdata(pdev, bl);
return 0;
err_bl:
pwm_put(pb->pwm);
err_alloc:
if (data->exit)
data->exit(&pdev->dev);
return ret;
}
static int pwm_backlight_remove(struct platform_device *pdev)
{
struct backlight_device *bl = platform_get_drvdata(pdev);
struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);
backlight_device_unregister(bl);
pwm_config(pb->pwm, 0, pb->period);
pwm_disable(pb->pwm);
pwm_put(pb->pwm);
if (pb->exit)
pb->exit(&pdev->dev);
return 0;
}
static int pwm_backlight_suspend(struct device *dev)
{
struct backlight_device *bl = dev_get_drvdata(dev);
struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);
if (pb->notify)
pb->notify(pb->dev, 0);
pwm_config(pb->pwm, 0, pb->period);
pwm_disable(pb->pwm);
if (pb->notify_after)
pb->notify_after(pb->dev, 0);
return 0;
}
static int pwm_backlight_resume(struct device *dev)
{
struct backlight_device *bl = dev_get_drvdata(dev);
backlight_update_status(bl);
return 0;
}
