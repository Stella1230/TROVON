static int isl6423_write(struct isl6423_dev *isl6423, u8 reg)
{
struct i2c_adapter *i2c = isl6423->i2c;
u8 addr = isl6423->config->addr;
int err = 0;
struct i2c_msg msg = { .addr = addr, .flags = 0, .buf = &reg, .len = 1 };
dprintk(FE_DEBUG, 1, "write reg %02X", reg);
err = i2c_transfer(i2c, &msg, 1);
if (err < 0)
goto exit;
return 0;
exit:
dprintk(FE_ERROR, 1, "I/O error <%d>", err);
return err;
}
static int isl6423_set_modulation(struct dvb_frontend *fe)
{
struct isl6423_dev *isl6423 = (struct isl6423_dev *) fe->sec_priv;
const struct isl6423_config *config = isl6423->config;
int err = 0;
u8 reg_2 = 0;
reg_2 = 0x01 << 5;
if (config->mod_extern)
reg_2 |= (1 << 3);
else
reg_2 |= (1 << 4);
err = isl6423_write(isl6423, reg_2);
if (err < 0)
goto exit;
return 0;
exit:
dprintk(FE_ERROR, 1, "I/O error <%d>", err);
return err;
}
static int isl6423_voltage_boost(struct dvb_frontend *fe, long arg)
{
struct isl6423_dev *isl6423 = (struct isl6423_dev *) fe->sec_priv;
u8 reg_3 = isl6423->reg_3;
u8 reg_4 = isl6423->reg_4;
int err = 0;
if (arg) {
reg_4 |= (1 << 4);
reg_4 |= 0x1;
reg_3 |= (1 << 3);
} else {
reg_4 |= (1 << 4);
reg_4 &= ~0x1;
reg_3 |= (1 << 3);
}
err = isl6423_write(isl6423, reg_3);
if (err < 0)
goto exit;
err = isl6423_write(isl6423, reg_4);
if (err < 0)
goto exit;
isl6423->reg_3 = reg_3;
isl6423->reg_4 = reg_4;
return 0;
exit:
dprintk(FE_ERROR, 1, "I/O error <%d>", err);
return err;
}
static int isl6423_set_voltage(struct dvb_frontend *fe,
enum fe_sec_voltage voltage)
{
struct isl6423_dev *isl6423 = (struct isl6423_dev *) fe->sec_priv;
u8 reg_3 = isl6423->reg_3;
u8 reg_4 = isl6423->reg_4;
int err = 0;
switch (voltage) {
case SEC_VOLTAGE_OFF:
reg_4 &= ~(1 << 4);
break;
case SEC_VOLTAGE_13:
reg_4 |= (1 << 4);
reg_4 &= ~0x3;
reg_3 |= (1 << 3);
break;
case SEC_VOLTAGE_18:
reg_4 |= (1 << 4);
reg_4 |= 0x2;
reg_4 &= ~0x1;
reg_3 |= (1 << 3);
break;
default:
break;
}
err = isl6423_write(isl6423, reg_3);
if (err < 0)
goto exit;
err = isl6423_write(isl6423, reg_4);
if (err < 0)
goto exit;
isl6423->reg_3 = reg_3;
isl6423->reg_4 = reg_4;
return 0;
exit:
dprintk(FE_ERROR, 1, "I/O error <%d>", err);
return err;
}
static int isl6423_set_current(struct dvb_frontend *fe)
{
struct isl6423_dev *isl6423 = (struct isl6423_dev *) fe->sec_priv;
u8 reg_3 = isl6423->reg_3;
const struct isl6423_config *config = isl6423->config;
int err = 0;
switch (config->current_max) {
case SEC_CURRENT_275m:
reg_3 &= ~0x3;
break;
case SEC_CURRENT_515m:
reg_3 &= ~0x2;
reg_3 |= 0x1;
break;
case SEC_CURRENT_635m:
reg_3 &= ~0x1;
reg_3 |= 0x2;
break;
case SEC_CURRENT_800m:
reg_3 |= 0x3;
break;
}
err = isl6423_write(isl6423, reg_3);
if (err < 0)
goto exit;
switch (config->curlim) {
case SEC_CURRENT_LIM_ON:
reg_3 &= ~0x10;
break;
case SEC_CURRENT_LIM_OFF:
reg_3 |= 0x10;
break;
}
err = isl6423_write(isl6423, reg_3);
if (err < 0)
goto exit;
isl6423->reg_3 = reg_3;
return 0;
exit:
dprintk(FE_ERROR, 1, "I/O error <%d>", err);
return err;
}
static void isl6423_release(struct dvb_frontend *fe)
{
isl6423_set_voltage(fe, SEC_VOLTAGE_OFF);
kfree(fe->sec_priv);
fe->sec_priv = NULL;
}
struct dvb_frontend *isl6423_attach(struct dvb_frontend *fe,
struct i2c_adapter *i2c,
const struct isl6423_config *config)
{
struct isl6423_dev *isl6423;
isl6423 = kzalloc(sizeof(struct isl6423_dev), GFP_KERNEL);
if (!isl6423)
return NULL;
isl6423->config = config;
isl6423->i2c = i2c;
fe->sec_priv = isl6423;
isl6423->reg_3 = 0x02 << 5;
isl6423->reg_4 = 0x03 << 5;
if (isl6423_set_current(fe))
goto exit;
if (isl6423_set_modulation(fe))
goto exit;
fe->ops.release_sec = isl6423_release;
fe->ops.set_voltage = isl6423_set_voltage;
fe->ops.enable_high_lnb_voltage = isl6423_voltage_boost;
isl6423->verbose = verbose;
return fe;
exit:
kfree(isl6423);
fe->sec_priv = NULL;
return NULL;
}
