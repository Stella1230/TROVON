static void kmx61_set_data(struct iio_dev *indio_dev, struct kmx61_data *data)
{
struct kmx61_data **priv = iio_priv(indio_dev);
*priv = data;
}
static struct kmx61_data *kmx61_get_data(struct iio_dev *indio_dev)
{
return *(struct kmx61_data **)iio_priv(indio_dev);
}
static int kmx61_convert_freq_to_bit(int val, int val2)
{
int i;
for (i = 0; i < ARRAY_SIZE(kmx61_samp_freq_table); i++)
if (val == kmx61_samp_freq_table[i].val &&
val2 == kmx61_samp_freq_table[i].val2)
return i;
return -EINVAL;
}
static int kmx61_convert_wake_up_odr_to_bit(int val, int val2)
{
int i;
for (i = 0; i < ARRAY_SIZE(kmx61_wake_up_odr_table); ++i)
if (kmx61_wake_up_odr_table[i].val == val &&
kmx61_wake_up_odr_table[i].val2 == val2)
return kmx61_wake_up_odr_table[i].odr_bits;
return -EINVAL;
}
static int kmx61_set_mode(struct kmx61_data *data, u8 mode, u8 device,
bool update)
{
int ret;
int acc_stby = -1, mag_stby = -1;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_STBY);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_stby\n");
return ret;
}
if (device & KMX61_ACC) {
if (mode & KMX61_ACC_STBY_BIT) {
ret |= KMX61_ACC_STBY_BIT;
acc_stby = 1;
} else {
ret &= ~KMX61_ACC_STBY_BIT;
acc_stby = 0;
}
}
if (device & KMX61_MAG) {
if (mode & KMX61_MAG_STBY_BIT) {
ret |= KMX61_MAG_STBY_BIT;
mag_stby = 1;
} else {
ret &= ~KMX61_MAG_STBY_BIT;
mag_stby = 0;
}
}
if (mode & KMX61_ACT_STBY_BIT)
ret |= KMX61_ACT_STBY_BIT;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_STBY, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_stby\n");
return ret;
}
if (acc_stby != -1 && update)
data->acc_stby = acc_stby;
if (mag_stby != -1 && update)
data->mag_stby = mag_stby;
return 0;
}
static int kmx61_get_mode(struct kmx61_data *data, u8 *mode, u8 device)
{
int ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_STBY);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_stby\n");
return ret;
}
*mode = 0;
if (device & KMX61_ACC) {
if (ret & KMX61_ACC_STBY_BIT)
*mode |= KMX61_ACC_STBY_BIT;
else
*mode &= ~KMX61_ACC_STBY_BIT;
}
if (device & KMX61_MAG) {
if (ret & KMX61_MAG_STBY_BIT)
*mode |= KMX61_MAG_STBY_BIT;
else
*mode &= ~KMX61_MAG_STBY_BIT;
}
return 0;
}
static int kmx61_set_wake_up_odr(struct kmx61_data *data, int val, int val2)
{
int ret, odr_bits;
odr_bits = kmx61_convert_wake_up_odr_to_bit(val, val2);
if (odr_bits < 0)
return odr_bits;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_CTRL2,
odr_bits);
if (ret < 0)
dev_err(&data->client->dev, "Error writing reg_ctrl2\n");
return ret;
}
static int kmx61_set_odr(struct kmx61_data *data, int val, int val2, u8 device)
{
int ret;
u8 mode;
int lodr_bits, odr_bits;
ret = kmx61_get_mode(data, &mode, KMX61_ACC | KMX61_MAG);
if (ret < 0)
return ret;
lodr_bits = kmx61_convert_freq_to_bit(val, val2);
if (lodr_bits < 0)
return lodr_bits;
ret = kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG,
true);
if (ret < 0)
return ret;
odr_bits = 0;
if (device & KMX61_ACC)
odr_bits |= lodr_bits << KMX61_ACC_ODR_SHIFT;
if (device & KMX61_MAG)
odr_bits |= lodr_bits << KMX61_MAG_ODR_SHIFT;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_ODCNTL,
odr_bits);
if (ret < 0)
return ret;
data->odr_bits = odr_bits;
if (device & KMX61_ACC) {
ret = kmx61_set_wake_up_odr(data, val, val2);
if (ret)
return ret;
}
return kmx61_set_mode(data, mode, KMX61_ACC | KMX61_MAG, true);
}
static int kmx61_get_odr(struct kmx61_data *data, int *val, int *val2,
u8 device)
{
u8 lodr_bits;
if (device & KMX61_ACC)
lodr_bits = (data->odr_bits >> KMX61_ACC_ODR_SHIFT) &
KMX61_ACC_ODR_MASK;
else if (device & KMX61_MAG)
lodr_bits = (data->odr_bits >> KMX61_MAG_ODR_SHIFT) &
KMX61_MAG_ODR_MASK;
else
return -EINVAL;
if (lodr_bits >= ARRAY_SIZE(kmx61_samp_freq_table))
return -EINVAL;
*val = kmx61_samp_freq_table[lodr_bits].val;
*val2 = kmx61_samp_freq_table[lodr_bits].val2;
return 0;
}
static int kmx61_set_range(struct kmx61_data *data, u8 range)
{
int ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_CTRL1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ctrl1\n");
return ret;
}
ret &= ~KMX61_REG_CTRL1_GSEL_MASK;
ret |= range & KMX61_REG_CTRL1_GSEL_MASK;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_CTRL1, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_ctrl1\n");
return ret;
}
data->range = range;
return 0;
}
static int kmx61_set_scale(struct kmx61_data *data, u16 uscale)
{
int ret, i;
u8 mode;
for (i = 0; i < ARRAY_SIZE(kmx61_uscale_table); i++) {
if (kmx61_uscale_table[i] == uscale) {
ret = kmx61_get_mode(data, &mode,
KMX61_ACC | KMX61_MAG);
if (ret < 0)
return ret;
ret = kmx61_set_mode(data, KMX61_ALL_STBY,
KMX61_ACC | KMX61_MAG, true);
if (ret < 0)
return ret;
ret = kmx61_set_range(data, i);
if (ret < 0)
return ret;
return kmx61_set_mode(data, mode,
KMX61_ACC | KMX61_MAG, true);
}
}
return -EINVAL;
}
static int kmx61_chip_init(struct kmx61_data *data)
{
int ret, val, val2;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_WHO_AM_I);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading who_am_i\n");
return ret;
}
if (ret != KMX61_CHIP_ID) {
dev_err(&data->client->dev,
"Wrong chip id, got %x expected %x\n",
ret, KMX61_CHIP_ID);
return -EINVAL;
}
ret = kmx61_set_range(data, KMX61_RANGE_4G);
if (ret < 0)
return ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_ODCNTL);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_odcntl\n");
return ret;
}
data->odr_bits = ret;
ret = kmx61_get_odr(data, &val, &val2, KMX61_ACC);
if (ret < 0)
return ret;
ret = kmx61_set_wake_up_odr(data, val, val2);
if (ret < 0)
return ret;
ret = kmx61_set_mode(data, 0, KMX61_ACC | KMX61_MAG, true);
if (ret < 0)
return ret;
data->wake_thresh = KMX61_DEFAULT_WAKE_THRESH;
data->wake_duration = KMX61_DEFAULT_WAKE_DURATION;
return 0;
}
static int kmx61_setup_new_data_interrupt(struct kmx61_data *data,
bool status, u8 device)
{
u8 mode;
int ret;
ret = kmx61_get_mode(data, &mode, KMX61_ACC | KMX61_MAG);
if (ret < 0)
return ret;
ret = kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG, true);
if (ret < 0)
return ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INC1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ctrl1\n");
return ret;
}
if (status) {
ret |= KMX61_REG_INC1_BIT_IEN;
if (device & KMX61_ACC)
ret |= KMX61_REG_INC1_BIT_DRDYA;
if (device & KMX61_MAG)
ret |= KMX61_REG_INC1_BIT_DRDYM;
} else {
ret &= ~KMX61_REG_INC1_BIT_IEN;
if (device & KMX61_ACC)
ret &= ~KMX61_REG_INC1_BIT_DRDYA;
if (device & KMX61_MAG)
ret &= ~KMX61_REG_INC1_BIT_DRDYM;
}
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_INC1, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_int_ctrl1\n");
return ret;
}
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_CTRL1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ctrl1\n");
return ret;
}
if (status)
ret |= KMX61_REG_CTRL1_BIT_DRDYE;
else
ret &= ~KMX61_REG_CTRL1_BIT_DRDYE;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_CTRL1, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_ctrl1\n");
return ret;
}
return kmx61_set_mode(data, mode, KMX61_ACC | KMX61_MAG, true);
}
static int kmx61_chip_update_thresholds(struct kmx61_data *data)
{
int ret;
ret = i2c_smbus_write_byte_data(data->client,
KMX61_REG_WUF_TIMER,
data->wake_duration);
if (ret < 0) {
dev_err(&data->client->dev, "Errow writing reg_wuf_timer\n");
return ret;
}
ret = i2c_smbus_write_byte_data(data->client,
KMX61_REG_WUF_THRESH,
data->wake_thresh);
if (ret < 0)
dev_err(&data->client->dev, "Error writing reg_wuf_thresh\n");
return ret;
}
static int kmx61_setup_any_motion_interrupt(struct kmx61_data *data,
bool status)
{
u8 mode;
int ret;
ret = kmx61_get_mode(data, &mode, KMX61_ACC | KMX61_MAG);
if (ret < 0)
return ret;
ret = kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG, true);
if (ret < 0)
return ret;
ret = kmx61_chip_update_thresholds(data);
if (ret < 0)
return ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INC1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_inc1\n");
return ret;
}
if (status)
ret |= (KMX61_REG_INC1_BIT_IEN | KMX61_REG_INC1_BIT_WUFS);
else
ret &= ~(KMX61_REG_INC1_BIT_IEN | KMX61_REG_INC1_BIT_WUFS);
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_INC1, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_inc1\n");
return ret;
}
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_CTRL1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ctrl1\n");
return ret;
}
if (status)
ret |= KMX61_REG_CTRL1_BIT_WUFE | KMX61_REG_CTRL1_BIT_BTSE;
else
ret &= ~(KMX61_REG_CTRL1_BIT_WUFE | KMX61_REG_CTRL1_BIT_BTSE);
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_CTRL1, ret);
if (ret < 0) {
dev_err(&data->client->dev, "Error writing reg_ctrl1\n");
return ret;
}
mode |= KMX61_ACT_STBY_BIT;
return kmx61_set_mode(data, mode, KMX61_ACC | KMX61_MAG, true);
}
static int kmx61_set_power_state(struct kmx61_data *data, bool on, u8 device)
{
#ifdef CONFIG_PM
int ret;
if (device & KMX61_ACC) {
if (on && !data->acc_ps && !data->mag_stby) {
ret = kmx61_set_mode(data, 0, KMX61_ACC, true);
if (ret < 0)
return ret;
}
data->acc_ps = on;
}
if (device & KMX61_MAG) {
if (on && !data->mag_ps && !data->acc_stby) {
ret = kmx61_set_mode(data, 0, KMX61_MAG, true);
if (ret < 0)
return ret;
}
data->mag_ps = on;
}
if (on) {
ret = pm_runtime_get_sync(&data->client->dev);
} else {
pm_runtime_mark_last_busy(&data->client->dev);
ret = pm_runtime_put_autosuspend(&data->client->dev);
}
if (ret < 0) {
dev_err(&data->client->dev,
"Failed: kmx61_set_power_state for %d, ret %d\n",
on, ret);
if (on)
pm_runtime_put_noidle(&data->client->dev);
return ret;
}
#endif
return 0;
}
static int kmx61_read_measurement(struct kmx61_data *data, u8 base, u8 offset)
{
int ret;
u8 reg = base + offset * 2;
ret = i2c_smbus_read_word_data(data->client, reg);
if (ret < 0)
dev_err(&data->client->dev, "failed to read reg at %x\n", reg);
return ret;
}
static int kmx61_read_raw(struct iio_dev *indio_dev,
struct iio_chan_spec const *chan, int *val,
int *val2, long mask)
{
int ret;
u8 base_reg;
struct kmx61_data *data = kmx61_get_data(indio_dev);
switch (mask) {
case IIO_CHAN_INFO_RAW:
switch (chan->type) {
case IIO_ACCEL:
base_reg = KMX61_ACC_XOUT_L;
break;
case IIO_MAGN:
base_reg = KMX61_MAG_XOUT_L;
break;
default:
return -EINVAL;
}
mutex_lock(&data->lock);
ret = kmx61_set_power_state(data, true, chan->address);
if (ret) {
mutex_unlock(&data->lock);
return ret;
}
ret = kmx61_read_measurement(data, base_reg, chan->scan_index);
if (ret < 0) {
kmx61_set_power_state(data, false, chan->address);
mutex_unlock(&data->lock);
return ret;
}
*val = sign_extend32(ret >> chan->scan_type.shift,
chan->scan_type.realbits - 1);
ret = kmx61_set_power_state(data, false, chan->address);
mutex_unlock(&data->lock);
if (ret)
return ret;
return IIO_VAL_INT;
case IIO_CHAN_INFO_SCALE:
switch (chan->type) {
case IIO_ACCEL:
*val = 0;
*val2 = kmx61_uscale_table[data->range];
return IIO_VAL_INT_PLUS_MICRO;
case IIO_MAGN:
*val = 0;
*val2 = 1465;
return IIO_VAL_INT_PLUS_MICRO;
default:
return -EINVAL;
}
case IIO_CHAN_INFO_SAMP_FREQ:
if (chan->type != IIO_ACCEL && chan->type != IIO_MAGN)
return -EINVAL;
mutex_lock(&data->lock);
ret = kmx61_get_odr(data, val, val2, chan->address);
mutex_unlock(&data->lock);
if (ret)
return -EINVAL;
return IIO_VAL_INT_PLUS_MICRO;
}
return -EINVAL;
}
static int kmx61_write_raw(struct iio_dev *indio_dev,
struct iio_chan_spec const *chan, int val,
int val2, long mask)
{
int ret;
struct kmx61_data *data = kmx61_get_data(indio_dev);
switch (mask) {
case IIO_CHAN_INFO_SAMP_FREQ:
if (chan->type != IIO_ACCEL && chan->type != IIO_MAGN)
return -EINVAL;
mutex_lock(&data->lock);
ret = kmx61_set_odr(data, val, val2, chan->address);
mutex_unlock(&data->lock);
return ret;
case IIO_CHAN_INFO_SCALE:
switch (chan->type) {
case IIO_ACCEL:
if (val != 0)
return -EINVAL;
mutex_lock(&data->lock);
ret = kmx61_set_scale(data, val2);
mutex_unlock(&data->lock);
return ret;
default:
return -EINVAL;
}
default:
return -EINVAL;
}
}
static int kmx61_read_event(struct iio_dev *indio_dev,
const struct iio_chan_spec *chan,
enum iio_event_type type,
enum iio_event_direction dir,
enum iio_event_info info,
int *val, int *val2)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
*val2 = 0;
switch (info) {
case IIO_EV_INFO_VALUE:
*val = data->wake_thresh;
return IIO_VAL_INT;
case IIO_EV_INFO_PERIOD:
*val = data->wake_duration;
return IIO_VAL_INT;
default:
return -EINVAL;
}
}
static int kmx61_write_event(struct iio_dev *indio_dev,
const struct iio_chan_spec *chan,
enum iio_event_type type,
enum iio_event_direction dir,
enum iio_event_info info,
int val, int val2)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
if (data->ev_enable_state)
return -EBUSY;
switch (info) {
case IIO_EV_INFO_VALUE:
data->wake_thresh = val;
return IIO_VAL_INT;
case IIO_EV_INFO_PERIOD:
data->wake_duration = val;
return IIO_VAL_INT;
default:
return -EINVAL;
}
}
static int kmx61_read_event_config(struct iio_dev *indio_dev,
const struct iio_chan_spec *chan,
enum iio_event_type type,
enum iio_event_direction dir)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
return data->ev_enable_state;
}
static int kmx61_write_event_config(struct iio_dev *indio_dev,
const struct iio_chan_spec *chan,
enum iio_event_type type,
enum iio_event_direction dir,
int state)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
int ret = 0;
if (state && data->ev_enable_state)
return 0;
mutex_lock(&data->lock);
if (!state && data->motion_trig_on) {
data->ev_enable_state = false;
goto err_unlock;
}
ret = kmx61_set_power_state(data, state, KMX61_ACC);
if (ret < 0)
goto err_unlock;
ret = kmx61_setup_any_motion_interrupt(data, state);
if (ret < 0) {
kmx61_set_power_state(data, false, KMX61_ACC);
goto err_unlock;
}
data->ev_enable_state = state;
err_unlock:
mutex_unlock(&data->lock);
return ret;
}
static int kmx61_acc_validate_trigger(struct iio_dev *indio_dev,
struct iio_trigger *trig)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
if (data->acc_dready_trig != trig && data->motion_trig != trig)
return -EINVAL;
return 0;
}
static int kmx61_mag_validate_trigger(struct iio_dev *indio_dev,
struct iio_trigger *trig)
{
struct kmx61_data *data = kmx61_get_data(indio_dev);
if (data->mag_dready_trig != trig)
return -EINVAL;
return 0;
}
static int kmx61_data_rdy_trigger_set_state(struct iio_trigger *trig,
bool state)
{
int ret = 0;
u8 device;
struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);
struct kmx61_data *data = kmx61_get_data(indio_dev);
mutex_lock(&data->lock);
if (!state && data->ev_enable_state && data->motion_trig_on) {
data->motion_trig_on = false;
goto err_unlock;
}
if (data->acc_dready_trig == trig || data->motion_trig == trig)
device = KMX61_ACC;
else
device = KMX61_MAG;
ret = kmx61_set_power_state(data, state, device);
if (ret < 0)
goto err_unlock;
if (data->acc_dready_trig == trig || data->mag_dready_trig == trig)
ret = kmx61_setup_new_data_interrupt(data, state, device);
else
ret = kmx61_setup_any_motion_interrupt(data, state);
if (ret < 0) {
kmx61_set_power_state(data, false, device);
goto err_unlock;
}
if (data->acc_dready_trig == trig)
data->acc_dready_trig_on = state;
else if (data->mag_dready_trig == trig)
data->mag_dready_trig_on = state;
else
data->motion_trig_on = state;
err_unlock:
mutex_unlock(&data->lock);
return ret;
}
static int kmx61_trig_try_reenable(struct iio_trigger *trig)
{
struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);
struct kmx61_data *data = kmx61_get_data(indio_dev);
int ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INL);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_inl\n");
return ret;
}
return 0;
}
static irqreturn_t kmx61_event_handler(int irq, void *private)
{
struct kmx61_data *data = private;
struct iio_dev *indio_dev = data->acc_indio_dev;
int ret;
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INS1);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ins1\n");
goto ack_intr;
}
if (ret & KMX61_REG_INS1_BIT_WUFS) {
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INS2);
if (ret < 0) {
dev_err(&data->client->dev, "Error reading reg_ins2\n");
goto ack_intr;
}
if (ret & KMX61_REG_INS2_BIT_XN)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_X,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_FALLING),
0);
if (ret & KMX61_REG_INS2_BIT_XP)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_X,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_RISING),
0);
if (ret & KMX61_REG_INS2_BIT_YN)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_Y,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_FALLING),
0);
if (ret & KMX61_REG_INS2_BIT_YP)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_Y,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_RISING),
0);
if (ret & KMX61_REG_INS2_BIT_ZN)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_Z,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_FALLING),
0);
if (ret & KMX61_REG_INS2_BIT_ZP)
iio_push_event(indio_dev,
IIO_MOD_EVENT_CODE(IIO_ACCEL,
0,
IIO_MOD_Z,
IIO_EV_TYPE_THRESH,
IIO_EV_DIR_RISING),
0);
}
ack_intr:
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_CTRL1);
if (ret < 0)
dev_err(&data->client->dev, "Error reading reg_ctrl1\n");
ret |= KMX61_REG_CTRL1_BIT_RES;
ret = i2c_smbus_write_byte_data(data->client, KMX61_REG_CTRL1, ret);
if (ret < 0)
dev_err(&data->client->dev, "Error writing reg_ctrl1\n");
ret = i2c_smbus_read_byte_data(data->client, KMX61_REG_INL);
if (ret < 0)
dev_err(&data->client->dev, "Error reading reg_inl\n");
return IRQ_HANDLED;
}
static irqreturn_t kmx61_data_rdy_trig_poll(int irq, void *private)
{
struct kmx61_data *data = private;
if (data->acc_dready_trig_on)
iio_trigger_poll(data->acc_dready_trig);
if (data->mag_dready_trig_on)
iio_trigger_poll(data->mag_dready_trig);
if (data->motion_trig_on)
iio_trigger_poll(data->motion_trig);
if (data->ev_enable_state)
return IRQ_WAKE_THREAD;
return IRQ_HANDLED;
}
static irqreturn_t kmx61_trigger_handler(int irq, void *p)
{
struct iio_poll_func *pf = p;
struct iio_dev *indio_dev = pf->indio_dev;
struct kmx61_data *data = kmx61_get_data(indio_dev);
int bit, ret, i = 0;
u8 base;
s16 buffer[8];
if (indio_dev == data->acc_indio_dev)
base = KMX61_ACC_XOUT_L;
else
base = KMX61_MAG_XOUT_L;
mutex_lock(&data->lock);
for_each_set_bit(bit, indio_dev->active_scan_mask,
indio_dev->masklength) {
ret = kmx61_read_measurement(data, base, bit);
if (ret < 0) {
mutex_unlock(&data->lock);
goto err;
}
buffer[i++] = ret;
}
mutex_unlock(&data->lock);
iio_push_to_buffers(indio_dev, buffer);
err:
iio_trigger_notify_done(indio_dev->trig);
return IRQ_HANDLED;
}
static const char *kmx61_match_acpi_device(struct device *dev)
{
const struct acpi_device_id *id;
id = acpi_match_device(dev->driver->acpi_match_table, dev);
if (!id)
return NULL;
return dev_name(dev);
}
static struct iio_dev *kmx61_indiodev_setup(struct kmx61_data *data,
const struct iio_info *info,
const struct iio_chan_spec *chan,
int num_channels,
const char *name)
{
struct iio_dev *indio_dev;
indio_dev = devm_iio_device_alloc(&data->client->dev, sizeof(data));
if (!indio_dev)
return ERR_PTR(-ENOMEM);
kmx61_set_data(indio_dev, data);
indio_dev->dev.parent = &data->client->dev;
indio_dev->channels = chan;
indio_dev->num_channels = num_channels;
indio_dev->name = name;
indio_dev->modes = INDIO_DIRECT_MODE;
indio_dev->info = info;
return indio_dev;
}
static struct iio_trigger *kmx61_trigger_setup(struct kmx61_data *data,
struct iio_dev *indio_dev,
const char *tag)
{
struct iio_trigger *trig;
int ret;
trig = devm_iio_trigger_alloc(&data->client->dev,
"%s-%s-dev%d",
indio_dev->name,
tag,
indio_dev->id);
if (!trig)
return ERR_PTR(-ENOMEM);
trig->dev.parent = &data->client->dev;
trig->ops = &kmx61_trigger_ops;
iio_trigger_set_drvdata(trig, indio_dev);
ret = iio_trigger_register(trig);
if (ret)
return ERR_PTR(ret);
return trig;
}
static int kmx61_probe(struct i2c_client *client,
const struct i2c_device_id *id)
{
int ret;
struct kmx61_data *data;
const char *name = NULL;
data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
if (!data)
return -ENOMEM;
i2c_set_clientdata(client, data);
data->client = client;
mutex_init(&data->lock);
if (id)
name = id->name;
else if (ACPI_HANDLE(&client->dev))
name = kmx61_match_acpi_device(&client->dev);
else
return -ENODEV;
data->acc_indio_dev =
kmx61_indiodev_setup(data, &kmx61_acc_info,
kmx61_acc_channels,
ARRAY_SIZE(kmx61_acc_channels),
name);
if (IS_ERR(data->acc_indio_dev))
return PTR_ERR(data->acc_indio_dev);
data->mag_indio_dev =
kmx61_indiodev_setup(data, &kmx61_mag_info,
kmx61_mag_channels,
ARRAY_SIZE(kmx61_mag_channels),
name);
if (IS_ERR(data->mag_indio_dev))
return PTR_ERR(data->mag_indio_dev);
ret = kmx61_chip_init(data);
if (ret < 0)
return ret;
if (client->irq > 0) {
ret = devm_request_threaded_irq(&client->dev, client->irq,
kmx61_data_rdy_trig_poll,
kmx61_event_handler,
IRQF_TRIGGER_RISING,
KMX61_IRQ_NAME,
data);
if (ret)
goto err_chip_uninit;
data->acc_dready_trig =
kmx61_trigger_setup(data, data->acc_indio_dev,
"dready");
if (IS_ERR(data->acc_dready_trig)) {
ret = PTR_ERR(data->acc_dready_trig);
goto err_chip_uninit;
}
data->mag_dready_trig =
kmx61_trigger_setup(data, data->mag_indio_dev,
"dready");
if (IS_ERR(data->mag_dready_trig)) {
ret = PTR_ERR(data->mag_dready_trig);
goto err_trigger_unregister_acc_dready;
}
data->motion_trig =
kmx61_trigger_setup(data, data->acc_indio_dev,
"any-motion");
if (IS_ERR(data->motion_trig)) {
ret = PTR_ERR(data->motion_trig);
goto err_trigger_unregister_mag_dready;
}
ret = iio_triggered_buffer_setup(data->acc_indio_dev,
&iio_pollfunc_store_time,
kmx61_trigger_handler,
NULL);
if (ret < 0) {
dev_err(&data->client->dev,
"Failed to setup acc triggered buffer\n");
goto err_trigger_unregister_motion;
}
ret = iio_triggered_buffer_setup(data->mag_indio_dev,
&iio_pollfunc_store_time,
kmx61_trigger_handler,
NULL);
if (ret < 0) {
dev_err(&data->client->dev,
"Failed to setup mag triggered buffer\n");
goto err_buffer_cleanup_acc;
}
}
ret = pm_runtime_set_active(&client->dev);
if (ret < 0)
goto err_buffer_cleanup_mag;
pm_runtime_enable(&client->dev);
pm_runtime_set_autosuspend_delay(&client->dev, KMX61_SLEEP_DELAY_MS);
pm_runtime_use_autosuspend(&client->dev);
ret = iio_device_register(data->acc_indio_dev);
if (ret < 0) {
dev_err(&client->dev, "Failed to register acc iio device\n");
goto err_buffer_cleanup_mag;
}
ret = iio_device_register(data->mag_indio_dev);
if (ret < 0) {
dev_err(&client->dev, "Failed to register mag iio device\n");
goto err_iio_unregister_acc;
}
return 0;
err_iio_unregister_acc:
iio_device_unregister(data->acc_indio_dev);
err_buffer_cleanup_mag:
if (client->irq > 0)
iio_triggered_buffer_cleanup(data->mag_indio_dev);
err_buffer_cleanup_acc:
if (client->irq > 0)
iio_triggered_buffer_cleanup(data->acc_indio_dev);
err_trigger_unregister_motion:
iio_trigger_unregister(data->motion_trig);
err_trigger_unregister_mag_dready:
iio_trigger_unregister(data->mag_dready_trig);
err_trigger_unregister_acc_dready:
iio_trigger_unregister(data->acc_dready_trig);
err_chip_uninit:
kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG, true);
return ret;
}
static int kmx61_remove(struct i2c_client *client)
{
struct kmx61_data *data = i2c_get_clientdata(client);
iio_device_unregister(data->acc_indio_dev);
iio_device_unregister(data->mag_indio_dev);
pm_runtime_disable(&client->dev);
pm_runtime_set_suspended(&client->dev);
pm_runtime_put_noidle(&client->dev);
if (client->irq > 0) {
iio_triggered_buffer_cleanup(data->acc_indio_dev);
iio_triggered_buffer_cleanup(data->mag_indio_dev);
iio_trigger_unregister(data->acc_dready_trig);
iio_trigger_unregister(data->mag_dready_trig);
iio_trigger_unregister(data->motion_trig);
}
mutex_lock(&data->lock);
kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG, true);
mutex_unlock(&data->lock);
return 0;
}
static int kmx61_suspend(struct device *dev)
{
int ret;
struct kmx61_data *data = i2c_get_clientdata(to_i2c_client(dev));
mutex_lock(&data->lock);
ret = kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG,
false);
mutex_unlock(&data->lock);
return ret;
}
static int kmx61_resume(struct device *dev)
{
u8 stby = 0;
struct kmx61_data *data = i2c_get_clientdata(to_i2c_client(dev));
if (data->acc_stby)
stby |= KMX61_ACC_STBY_BIT;
if (data->mag_stby)
stby |= KMX61_MAG_STBY_BIT;
return kmx61_set_mode(data, stby, KMX61_ACC | KMX61_MAG, true);
}
static int kmx61_runtime_suspend(struct device *dev)
{
struct kmx61_data *data = i2c_get_clientdata(to_i2c_client(dev));
int ret;
mutex_lock(&data->lock);
ret = kmx61_set_mode(data, KMX61_ALL_STBY, KMX61_ACC | KMX61_MAG, true);
mutex_unlock(&data->lock);
return ret;
}
static int kmx61_runtime_resume(struct device *dev)
{
struct kmx61_data *data = i2c_get_clientdata(to_i2c_client(dev));
u8 stby = 0;
if (!data->acc_ps)
stby |= KMX61_ACC_STBY_BIT;
if (!data->mag_ps)
stby |= KMX61_MAG_STBY_BIT;
return kmx61_set_mode(data, stby, KMX61_ACC | KMX61_MAG, true);
}
