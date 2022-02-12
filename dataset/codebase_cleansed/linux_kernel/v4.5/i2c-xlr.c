static inline void xlr_i2c_wreg(u32 __iomem *base, unsigned int reg, u32 val)
{
__raw_writel(val, base + reg);
}
static inline u32 xlr_i2c_rdreg(u32 __iomem *base, unsigned int reg)
{
return __raw_readl(base + reg);
}
static int xlr_i2c_busy(struct xlr_i2c_private *priv, u32 status)
{
return (status & XLR_I2C_BUS_BUSY) == priv->cfg->status_busy;
}
static int xlr_i2c_idle(struct xlr_i2c_private *priv)
{
return !xlr_i2c_busy(priv, xlr_i2c_rdreg(priv->iobase, XLR_I2C_STATUS));
}
static int xlr_i2c_wait(struct xlr_i2c_private *priv, unsigned long timeout)
{
int status;
int t;
t = wait_event_timeout(priv->wait, xlr_i2c_idle(priv),
msecs_to_jiffies(timeout));
if (!t)
return -ETIMEDOUT;
status = xlr_i2c_rdreg(priv->iobase, XLR_I2C_STATUS);
return status & XLR_I2C_ACK_ERR ? -EIO : 0;
}
static void xlr_i2c_tx_irq(struct xlr_i2c_private *priv, u32 status)
{
struct i2c_msg *msg = priv->msg;
if (status & XLR_I2C_SDOEMPTY)
xlr_i2c_wreg(priv->iobase, XLR_I2C_DATAOUT,
msg->buf[priv->pos++]);
}
static void xlr_i2c_rx_irq(struct xlr_i2c_private *priv, u32 status)
{
struct i2c_msg *msg = priv->msg;
if (status & XLR_I2C_RXRDY)
msg->buf[priv->pos++] =
xlr_i2c_rdreg(priv->iobase, XLR_I2C_DATAIN);
}
static irqreturn_t xlr_i2c_irq(int irq, void *dev_id)
{
struct xlr_i2c_private *priv = dev_id;
struct i2c_msg *msg = priv->msg;
u32 int_stat, status;
int_stat = xlr_i2c_rdreg(priv->iobase, XLR_I2C_INT_STAT);
if (!int_stat)
return IRQ_NONE;
xlr_i2c_wreg(priv->iobase, XLR_I2C_INT_STAT, int_stat);
if (!msg)
return IRQ_HANDLED;
status = xlr_i2c_rdreg(priv->iobase, XLR_I2C_STATUS);
if (priv->pos < msg->len) {
if (msg->flags & I2C_M_RD)
xlr_i2c_rx_irq(priv, status);
else
xlr_i2c_tx_irq(priv, status);
}
if (!xlr_i2c_busy(priv, status))
wake_up(&priv->wait);
return IRQ_HANDLED;
}
static int xlr_i2c_tx(struct xlr_i2c_private *priv, u16 len,
u8 *buf, u16 addr)
{
struct i2c_adapter *adap = &priv->adap;
unsigned long timeout, stoptime, checktime;
u32 i2c_status;
int pos, timedout;
u8 offset;
u32 xfer;
if (!len)
return -EOPNOTSUPP;
offset = buf[0];
xlr_i2c_wreg(priv->iobase, XLR_I2C_ADDR, offset);
xlr_i2c_wreg(priv->iobase, XLR_I2C_DEVADDR, addr);
xlr_i2c_wreg(priv->iobase, XLR_I2C_CFG,
XLR_I2C_CFG_ADDR | priv->cfg->cfg_extra);
timeout = msecs_to_jiffies(XLR_I2C_TIMEOUT);
stoptime = jiffies + timeout;
timedout = 0;
if (len == 1) {
xlr_i2c_wreg(priv->iobase, XLR_I2C_BYTECNT, len - 1);
xfer = XLR_I2C_STARTXFR_ND;
pos = 1;
} else {
xlr_i2c_wreg(priv->iobase, XLR_I2C_BYTECNT, len - 2);
xlr_i2c_wreg(priv->iobase, XLR_I2C_DATAOUT, buf[1]);
xfer = XLR_I2C_STARTXFR_WR;
pos = 2;
}
priv->pos = pos;
retry:
xlr_i2c_wreg(priv->iobase, XLR_I2C_STARTXFR, xfer);
if (priv->irq > 0)
return xlr_i2c_wait(priv, XLR_I2C_TIMEOUT * len);
while (!timedout) {
checktime = jiffies;
i2c_status = xlr_i2c_rdreg(priv->iobase, XLR_I2C_STATUS);
if ((i2c_status & XLR_I2C_SDOEMPTY) && pos < len) {
xlr_i2c_wreg(priv->iobase, XLR_I2C_DATAOUT, buf[pos++]);
stoptime = jiffies + timeout;
}
timedout = time_after(checktime, stoptime);
if (i2c_status & XLR_I2C_ARB_STARTERR) {
if (timedout)
break;
goto retry;
}
if (i2c_status & XLR_I2C_ACK_ERR)
return -EIO;
if (!xlr_i2c_busy(priv, i2c_status) && pos >= len)
return 0;
}
dev_err(&adap->dev, "I2C transmit timeout\n");
return -ETIMEDOUT;
}
static int xlr_i2c_rx(struct xlr_i2c_private *priv, u16 len, u8 *buf, u16 addr)
{
struct i2c_adapter *adap = &priv->adap;
u32 i2c_status;
unsigned long timeout, stoptime, checktime;
int nbytes, timedout;
if (!len)
return -EOPNOTSUPP;
xlr_i2c_wreg(priv->iobase, XLR_I2C_CFG,
XLR_I2C_CFG_NOADDR | priv->cfg->cfg_extra);
xlr_i2c_wreg(priv->iobase, XLR_I2C_BYTECNT, len - 1);
xlr_i2c_wreg(priv->iobase, XLR_I2C_DEVADDR, addr);
priv->pos = 0;
timeout = msecs_to_jiffies(XLR_I2C_TIMEOUT);
stoptime = jiffies + timeout;
timedout = 0;
nbytes = 0;
retry:
xlr_i2c_wreg(priv->iobase, XLR_I2C_STARTXFR, XLR_I2C_STARTXFR_RD);
if (priv->irq > 0)
return xlr_i2c_wait(priv, XLR_I2C_TIMEOUT * len);
while (!timedout) {
checktime = jiffies;
i2c_status = xlr_i2c_rdreg(priv->iobase, XLR_I2C_STATUS);
if (i2c_status & XLR_I2C_RXRDY) {
if (nbytes >= len)
return -EIO;
buf[nbytes++] =
xlr_i2c_rdreg(priv->iobase, XLR_I2C_DATAIN);
stoptime = jiffies + timeout;
}
timedout = time_after(checktime, stoptime);
if (i2c_status & XLR_I2C_ARB_STARTERR) {
if (timedout)
break;
goto retry;
}
if (i2c_status & XLR_I2C_ACK_ERR)
return -EIO;
if (!xlr_i2c_busy(priv, i2c_status))
return 0;
}
dev_err(&adap->dev, "I2C receive timeout\n");
return -ETIMEDOUT;
}
static int xlr_i2c_xfer(struct i2c_adapter *adap,
struct i2c_msg *msgs, int num)
{
struct i2c_msg *msg;
int i;
int ret = 0;
struct xlr_i2c_private *priv = i2c_get_adapdata(adap);
ret = clk_enable(priv->clk);
if (ret)
return ret;
if (priv->irq)
xlr_i2c_wreg(priv->iobase, XLR_I2C_INT_EN, 0xf);
for (i = 0; ret == 0 && i < num; i++) {
msg = &msgs[i];
priv->msg = msg;
if (msg->flags & I2C_M_RD)
ret = xlr_i2c_rx(priv, msg->len, &msg->buf[0],
msg->addr);
else
ret = xlr_i2c_tx(priv, msg->len, &msg->buf[0],
msg->addr);
}
if (priv->irq)
xlr_i2c_wreg(priv->iobase, XLR_I2C_INT_EN, 0);
clk_disable(priv->clk);
priv->msg = NULL;
return (ret != 0) ? ret : num;
}
static u32 xlr_func(struct i2c_adapter *adap)
{
return (I2C_FUNC_SMBUS_EMUL & ~I2C_FUNC_SMBUS_QUICK) | I2C_FUNC_I2C;
}
static int xlr_i2c_probe(struct platform_device *pdev)
{
const struct of_device_id *match;
struct xlr_i2c_private *priv;
struct resource *res;
struct clk *clk;
unsigned long clk_rate;
unsigned long clk_div;
u32 busfreq;
int irq;
int ret;
priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
if (!priv)
return -ENOMEM;
match = of_match_device(xlr_i2c_dt_ids, &pdev->dev);
if (match)
priv->cfg = match->data;
else
priv->cfg = &xlr_i2c_config_default;
res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
priv->iobase = devm_ioremap_resource(&pdev->dev, res);
if (IS_ERR(priv->iobase))
return PTR_ERR(priv->iobase);
irq = platform_get_irq(pdev, 0);
if (irq > 0 && (priv->cfg->flags & XLR_I2C_FLAG_IRQ)) {
priv->irq = irq;
xlr_i2c_wreg(priv->iobase, XLR_I2C_INT_EN, 0);
xlr_i2c_wreg(priv->iobase, XLR_I2C_INT_STAT, 0xf);
ret = devm_request_irq(&pdev->dev, priv->irq, xlr_i2c_irq,
IRQF_SHARED, dev_name(&pdev->dev),
priv);
if (ret)
return ret;
init_waitqueue_head(&priv->wait);
}
if (of_property_read_u32(pdev->dev.of_node, "clock-frequency",
&busfreq))
busfreq = 100000;
clk = devm_clk_get(&pdev->dev, NULL);
if (!IS_ERR(clk)) {
ret = clk_prepare_enable(clk);
if (ret)
return ret;
clk_rate = clk_get_rate(clk);
clk_div = DIV_ROUND_UP(clk_rate, 2 * busfreq);
xlr_i2c_wreg(priv->iobase, XLR_I2C_CLKDIV, clk_div);
clk_disable(clk);
priv->clk = clk;
}
priv->adap.dev.parent = &pdev->dev;
priv->adap.dev.of_node = pdev->dev.of_node;
priv->adap.owner = THIS_MODULE;
priv->adap.algo_data = priv;
priv->adap.algo = &xlr_i2c_algo;
priv->adap.nr = pdev->id;
priv->adap.class = I2C_CLASS_HWMON;
snprintf(priv->adap.name, sizeof(priv->adap.name), "xlr-i2c");
i2c_set_adapdata(&priv->adap, priv);
ret = i2c_add_numbered_adapter(&priv->adap);
if (ret < 0) {
dev_err(&priv->adap.dev, "Failed to add i2c bus.\n");
return ret;
}
platform_set_drvdata(pdev, priv);
dev_info(&priv->adap.dev, "Added I2C Bus.\n");
return 0;
}
static int xlr_i2c_remove(struct platform_device *pdev)
{
struct xlr_i2c_private *priv;
priv = platform_get_drvdata(pdev);
i2c_del_adapter(&priv->adap);
clk_unprepare(priv->clk);
return 0;
}
