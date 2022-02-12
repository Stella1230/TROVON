static int wl1273_fm_read_reg(struct wl1273_core *core, u8 reg, u16 *value)
{
struct i2c_client *client = core->client;
u8 b[2];
int r;
r = i2c_smbus_read_i2c_block_data(client, reg, sizeof(b), b);
if (r != 2) {
dev_err(&client->dev, "%s: Read: %d fails.\n", __func__, reg);
return -EREMOTEIO;
}
*value = (u16)b[0] << 8 | b[1];
return 0;
}
static int wl1273_fm_write_cmd(struct wl1273_core *core, u8 cmd, u16 param)
{
struct i2c_client *client = core->client;
u8 buf[] = { (param >> 8) & 0xff, param & 0xff };
int r;
r = i2c_smbus_write_i2c_block_data(client, cmd, sizeof(buf), buf);
if (r) {
dev_err(&client->dev, "%s: Cmd: %d fails.\n", __func__, cmd);
return r;
}
return 0;
}
static int wl1273_fm_write_data(struct wl1273_core *core, u8 *data, u16 len)
{
struct i2c_client *client = core->client;
struct i2c_msg msg;
int r;
msg.addr = client->addr;
msg.flags = 0;
msg.buf = data;
msg.len = len;
r = i2c_transfer(client->adapter, &msg, 1);
if (r != 1) {
dev_err(&client->dev, "%s: write error.\n", __func__);
return -EREMOTEIO;
}
return 0;
}
static int wl1273_fm_set_audio(struct wl1273_core *core, unsigned int new_mode)
{
int r = 0;
if (core->mode == WL1273_MODE_OFF ||
core->mode == WL1273_MODE_SUSPENDED)
return -EPERM;
if (core->mode == WL1273_MODE_RX && new_mode == WL1273_AUDIO_DIGITAL) {
r = wl1273_fm_write_cmd(core, WL1273_PCM_MODE_SET,
WL1273_PCM_DEF_MODE);
if (r)
goto out;
r = wl1273_fm_write_cmd(core, WL1273_I2S_MODE_CONFIG_SET,
core->i2s_mode);
if (r)
goto out;
r = wl1273_fm_write_cmd(core, WL1273_AUDIO_ENABLE,
WL1273_AUDIO_ENABLE_I2S);
if (r)
goto out;
} else if (core->mode == WL1273_MODE_RX &&
new_mode == WL1273_AUDIO_ANALOG) {
r = wl1273_fm_write_cmd(core, WL1273_AUDIO_ENABLE,
WL1273_AUDIO_ENABLE_ANALOG);
if (r)
goto out;
} else if (core->mode == WL1273_MODE_TX &&
new_mode == WL1273_AUDIO_DIGITAL) {
r = wl1273_fm_write_cmd(core, WL1273_I2S_MODE_CONFIG_SET,
core->i2s_mode);
if (r)
goto out;
r = wl1273_fm_write_cmd(core, WL1273_AUDIO_IO_SET,
WL1273_AUDIO_IO_SET_I2S);
if (r)
goto out;
} else if (core->mode == WL1273_MODE_TX &&
new_mode == WL1273_AUDIO_ANALOG) {
r = wl1273_fm_write_cmd(core, WL1273_AUDIO_IO_SET,
WL1273_AUDIO_IO_SET_ANALOG);
if (r)
goto out;
}
core->audio_mode = new_mode;
out:
return r;
}
static int wl1273_fm_set_volume(struct wl1273_core *core, unsigned int volume)
{
int r;
if (volume > WL1273_MAX_VOLUME)
return -EINVAL;
if (core->volume == volume)
return 0;
r = wl1273_fm_write_cmd(core, WL1273_VOLUME_SET, volume);
if (r)
return r;
core->volume = volume;
return 0;
}
static int wl1273_core_remove(struct i2c_client *client)
{
dev_dbg(&client->dev, "%s\n", __func__);
mfd_remove_devices(&client->dev);
return 0;
}
static int wl1273_core_probe(struct i2c_client *client,
const struct i2c_device_id *id)
{
struct wl1273_fm_platform_data *pdata = dev_get_platdata(&client->dev);
struct wl1273_core *core;
struct mfd_cell *cell;
int children = 0;
int r = 0;
dev_dbg(&client->dev, "%s\n", __func__);
if (!pdata) {
dev_err(&client->dev, "No platform data.\n");
return -EINVAL;
}
if (!(pdata->children & WL1273_RADIO_CHILD)) {
dev_err(&client->dev, "Cannot function without radio child.\n");
return -EINVAL;
}
core = devm_kzalloc(&client->dev, sizeof(*core), GFP_KERNEL);
if (!core)
return -ENOMEM;
core->pdata = pdata;
core->client = client;
mutex_init(&core->lock);
i2c_set_clientdata(client, core);
dev_dbg(&client->dev, "%s: Have V4L2.\n", __func__);
cell = &core->cells[children];
cell->name = "wl1273_fm_radio";
cell->platform_data = &core;
cell->pdata_size = sizeof(core);
children++;
core->read = wl1273_fm_read_reg;
core->write = wl1273_fm_write_cmd;
core->write_data = wl1273_fm_write_data;
core->set_audio = wl1273_fm_set_audio;
core->set_volume = wl1273_fm_set_volume;
if (pdata->children & WL1273_CODEC_CHILD) {
cell = &core->cells[children];
dev_dbg(&client->dev, "%s: Have codec.\n", __func__);
cell->name = "wl1273-codec";
cell->platform_data = &core;
cell->pdata_size = sizeof(core);
children++;
}
dev_dbg(&client->dev, "%s: number of children: %d.\n",
__func__, children);
r = mfd_add_devices(&client->dev, -1, core->cells,
children, NULL, 0, NULL);
if (r)
goto err;
return 0;
err:
pdata->free_resources();
dev_dbg(&client->dev, "%s\n", __func__);
return r;
}
static int __init wl1273_core_init(void)
{
int r;
r = i2c_add_driver(&wl1273_core_driver);
if (r) {
pr_err(WL1273_FM_DRIVER_NAME
": driver registration failed\n");
return r;
}
return r;
}
static void __exit wl1273_core_exit(void)
{
i2c_del_driver(&wl1273_core_driver);
}
