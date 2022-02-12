static irqreturn_t migor_ts_isr(int irq, void *dev_id)
{
struct migor_ts_priv *priv = dev_id;
unsigned short xpos, ypos;
unsigned char event;
u_int8_t buf[16];
memset(buf, 0, sizeof(buf));
buf[0] = 0;
if (i2c_master_send(priv->client, buf, 1) != 1) {
dev_err(&priv->client->dev, "Unable to write i2c index\n");
goto out;
}
if (i2c_master_recv(priv->client, buf, sizeof(buf)) != sizeof(buf)) {
dev_err(&priv->client->dev, "Unable to read i2c page\n");
goto out;
}
ypos = ((buf[9] & 0x03) << 8 | buf[8]);
xpos = ((buf[11] & 0x03) << 8 | buf[10]);
event = buf[12];
switch (event) {
case EVENT_PENDOWN:
case EVENT_REPEAT:
input_report_key(priv->input, BTN_TOUCH, 1);
input_report_abs(priv->input, ABS_X, ypos);
input_report_abs(priv->input, ABS_Y, xpos);
input_sync(priv->input);
break;
case EVENT_PENUP:
input_report_key(priv->input, BTN_TOUCH, 0);
input_sync(priv->input);
break;
}
out:
return IRQ_HANDLED;
}
static int migor_ts_open(struct input_dev *dev)
{
struct migor_ts_priv *priv = input_get_drvdata(dev);
struct i2c_client *client = priv->client;
int count;
count = i2c_master_send(client, migor_ts_ena_seq,
sizeof(migor_ts_ena_seq));
if (count != sizeof(migor_ts_ena_seq)) {
dev_err(&client->dev, "Unable to enable touchscreen.\n");
return -ENXIO;
}
return 0;
}
static void migor_ts_close(struct input_dev *dev)
{
struct migor_ts_priv *priv = input_get_drvdata(dev);
struct i2c_client *client = priv->client;
disable_irq(priv->irq);
i2c_master_send(client, migor_ts_dis_seq, sizeof(migor_ts_dis_seq));
enable_irq(priv->irq);
}
static int migor_ts_probe(struct i2c_client *client,
const struct i2c_device_id *idp)
{
struct migor_ts_priv *priv;
struct input_dev *input;
int error;
priv = kzalloc(sizeof(*priv), GFP_KERNEL);
input = input_allocate_device();
if (!priv || !input) {
dev_err(&client->dev, "failed to allocate memory\n");
error = -ENOMEM;
goto err_free_mem;
}
priv->client = client;
priv->input = input;
priv->irq = client->irq;
input->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
__set_bit(BTN_TOUCH, input->keybit);
input_set_abs_params(input, ABS_X, 95, 955, 0, 0);
input_set_abs_params(input, ABS_Y, 85, 935, 0, 0);
input->name = client->name;
input->id.bustype = BUS_I2C;
input->dev.parent = &client->dev;
input->open = migor_ts_open;
input->close = migor_ts_close;
input_set_drvdata(input, priv);
error = request_threaded_irq(priv->irq, NULL, migor_ts_isr,
IRQF_TRIGGER_LOW | IRQF_ONESHOT,
client->name, priv);
if (error) {
dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
goto err_free_mem;
}
error = input_register_device(input);
if (error)
goto err_free_irq;
i2c_set_clientdata(client, priv);
device_init_wakeup(&client->dev, 1);
return 0;
err_free_irq:
free_irq(priv->irq, priv);
err_free_mem:
input_free_device(input);
kfree(priv);
return error;
}
static int migor_ts_remove(struct i2c_client *client)
{
struct migor_ts_priv *priv = i2c_get_clientdata(client);
free_irq(priv->irq, priv);
input_unregister_device(priv->input);
kfree(priv);
dev_set_drvdata(&client->dev, NULL);
return 0;
}
static int migor_ts_suspend(struct device *dev)
{
struct i2c_client *client = to_i2c_client(dev);
struct migor_ts_priv *priv = i2c_get_clientdata(client);
if (device_may_wakeup(&client->dev))
enable_irq_wake(priv->irq);
return 0;
}
static int migor_ts_resume(struct device *dev)
{
struct i2c_client *client = to_i2c_client(dev);
struct migor_ts_priv *priv = i2c_get_clientdata(client);
if (device_may_wakeup(&client->dev))
disable_irq_wake(priv->irq);
return 0;
}
static int __init migor_ts_init(void)
{
return i2c_add_driver(&migor_ts_driver);
}
static void __exit migor_ts_exit(void)
{
i2c_del_driver(&migor_ts_driver);
}
