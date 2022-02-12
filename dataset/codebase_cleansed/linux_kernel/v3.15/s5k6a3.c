static inline struct s5k6a3 *sd_to_s5k6a3(struct v4l2_subdev *sd)
{
return container_of(sd, struct s5k6a3, subdev);
}
static const struct v4l2_mbus_framefmt *find_sensor_format(
struct v4l2_mbus_framefmt *mf)
{
int i;
for (i = 0; i < ARRAY_SIZE(s5k6a3_formats); i++)
if (mf->code == s5k6a3_formats[i].code)
return &s5k6a3_formats[i];
return &s5k6a3_formats[0];
}
static int s5k6a3_enum_mbus_code(struct v4l2_subdev *sd,
struct v4l2_subdev_fh *fh,
struct v4l2_subdev_mbus_code_enum *code)
{
if (code->index >= ARRAY_SIZE(s5k6a3_formats))
return -EINVAL;
code->code = s5k6a3_formats[code->index].code;
return 0;
}
static void s5k6a3_try_format(struct v4l2_mbus_framefmt *mf)
{
const struct v4l2_mbus_framefmt *fmt;
fmt = find_sensor_format(mf);
mf->code = fmt->code;
v4l_bound_align_image(&mf->width, S5K6A3_SENSOR_MIN_WIDTH,
S5K6A3_SENSOR_MAX_WIDTH, 0,
&mf->height, S5K6A3_SENSOR_MIN_HEIGHT,
S5K6A3_SENSOR_MAX_HEIGHT, 0, 0);
}
static struct v4l2_mbus_framefmt *__s5k6a3_get_format(
struct s5k6a3 *sensor, struct v4l2_subdev_fh *fh,
u32 pad, enum v4l2_subdev_format_whence which)
{
if (which == V4L2_SUBDEV_FORMAT_TRY)
return fh ? v4l2_subdev_get_try_format(fh, pad) : NULL;
return &sensor->format;
}
static int s5k6a3_set_fmt(struct v4l2_subdev *sd,
struct v4l2_subdev_fh *fh,
struct v4l2_subdev_format *fmt)
{
struct s5k6a3 *sensor = sd_to_s5k6a3(sd);
struct v4l2_mbus_framefmt *mf;
s5k6a3_try_format(&fmt->format);
mf = __s5k6a3_get_format(sensor, fh, fmt->pad, fmt->which);
if (mf) {
mutex_lock(&sensor->lock);
if (fmt->which == V4L2_SUBDEV_FORMAT_ACTIVE)
*mf = fmt->format;
mutex_unlock(&sensor->lock);
}
return 0;
}
static int s5k6a3_get_fmt(struct v4l2_subdev *sd,
struct v4l2_subdev_fh *fh,
struct v4l2_subdev_format *fmt)
{
struct s5k6a3 *sensor = sd_to_s5k6a3(sd);
struct v4l2_mbus_framefmt *mf;
mf = __s5k6a3_get_format(sensor, fh, fmt->pad, fmt->which);
mutex_lock(&sensor->lock);
fmt->format = *mf;
mutex_unlock(&sensor->lock);
return 0;
}
static int s5k6a3_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
struct v4l2_mbus_framefmt *format = v4l2_subdev_get_try_format(fh, 0);
*format = s5k6a3_formats[0];
format->width = S5K6A3_DEFAULT_WIDTH;
format->height = S5K6A3_DEFAULT_HEIGHT;
return 0;
}
static int __s5k6a3_power_on(struct s5k6a3 *sensor)
{
int i = S5K6A3_SUPP_VDDA;
int ret;
ret = clk_set_rate(sensor->clock, sensor->clock_frequency);
if (ret < 0)
return ret;
ret = pm_runtime_get(sensor->dev);
if (ret < 0)
return ret;
ret = regulator_enable(sensor->supplies[i].consumer);
if (ret < 0)
goto error_rpm_put;
ret = clk_prepare_enable(sensor->clock);
if (ret < 0)
goto error_reg_dis;
for (i++; i < S5K6A3_NUM_SUPPLIES; i++) {
ret = regulator_enable(sensor->supplies[i].consumer);
if (ret < 0)
goto error_reg_dis;
}
gpio_set_value(sensor->gpio_reset, 1);
usleep_range(600, 800);
gpio_set_value(sensor->gpio_reset, 0);
usleep_range(600, 800);
gpio_set_value(sensor->gpio_reset, 1);
msleep(20);
return 0;
error_reg_dis:
for (--i; i >= 0; --i)
regulator_disable(sensor->supplies[i].consumer);
error_rpm_put:
pm_runtime_put(sensor->dev);
return ret;
}
static int __s5k6a3_power_off(struct s5k6a3 *sensor)
{
int i;
gpio_set_value(sensor->gpio_reset, 0);
for (i = S5K6A3_NUM_SUPPLIES - 1; i >= 0; i--)
regulator_disable(sensor->supplies[i].consumer);
clk_disable_unprepare(sensor->clock);
pm_runtime_put(sensor->dev);
return 0;
}
static int s5k6a3_s_power(struct v4l2_subdev *sd, int on)
{
struct s5k6a3 *sensor = sd_to_s5k6a3(sd);
int ret = 0;
mutex_lock(&sensor->lock);
if (sensor->power_count == !on) {
if (on)
ret = __s5k6a3_power_on(sensor);
else
ret = __s5k6a3_power_off(sensor);
if (ret == 0)
sensor->power_count += on ? 1 : -1;
}
mutex_unlock(&sensor->lock);
return ret;
}
static int s5k6a3_probe(struct i2c_client *client,
const struct i2c_device_id *id)
{
struct device *dev = &client->dev;
struct s5k6a3 *sensor;
struct v4l2_subdev *sd;
int gpio, i, ret;
sensor = devm_kzalloc(dev, sizeof(*sensor), GFP_KERNEL);
if (!sensor)
return -ENOMEM;
mutex_init(&sensor->lock);
sensor->gpio_reset = -EINVAL;
sensor->clock = ERR_PTR(-EINVAL);
sensor->dev = dev;
sensor->clock = devm_clk_get(sensor->dev, S5K6A3_CLK_NAME);
if (IS_ERR(sensor->clock))
return PTR_ERR(sensor->clock);
gpio = of_get_gpio_flags(dev->of_node, 0, NULL);
if (!gpio_is_valid(gpio))
return gpio;
ret = devm_gpio_request_one(dev, gpio, GPIOF_OUT_INIT_LOW,
S5K6A3_DRV_NAME);
if (ret < 0)
return ret;
sensor->gpio_reset = gpio;
if (of_property_read_u32(dev->of_node, "clock-frequency",
&sensor->clock_frequency)) {
sensor->clock_frequency = S5K6A3_DEFAULT_CLK_FREQ;
dev_info(dev, "using default %u Hz clock frequency\n",
sensor->clock_frequency);
}
for (i = 0; i < S5K6A3_NUM_SUPPLIES; i++)
sensor->supplies[i].supply = s5k6a3_supply_names[i];
ret = devm_regulator_bulk_get(&client->dev, S5K6A3_NUM_SUPPLIES,
sensor->supplies);
if (ret < 0)
return ret;
sd = &sensor->subdev;
v4l2_i2c_subdev_init(sd, client, &s5k6a3_subdev_ops);
sensor->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
sd->internal_ops = &s5k6a3_sd_internal_ops;
sensor->format.code = s5k6a3_formats[0].code;
sensor->format.width = S5K6A3_DEFAULT_WIDTH;
sensor->format.height = S5K6A3_DEFAULT_HEIGHT;
sensor->pad.flags = MEDIA_PAD_FL_SOURCE;
ret = media_entity_init(&sd->entity, 1, &sensor->pad, 0);
if (ret < 0)
return ret;
pm_runtime_no_callbacks(dev);
pm_runtime_enable(dev);
ret = v4l2_async_register_subdev(sd);
if (ret < 0) {
pm_runtime_disable(&client->dev);
media_entity_cleanup(&sd->entity);
}
return ret;
}
static int s5k6a3_remove(struct i2c_client *client)
{
struct v4l2_subdev *sd = i2c_get_clientdata(client);
pm_runtime_disable(&client->dev);
v4l2_async_unregister_subdev(sd);
media_entity_cleanup(&sd->entity);
return 0;
}
