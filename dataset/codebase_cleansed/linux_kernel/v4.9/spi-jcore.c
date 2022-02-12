static int jcore_spi_wait(void __iomem *ctrl_reg)
{
unsigned timeout = JCORE_SPI_WAIT_RDY_MAX_LOOP;
do {
if (!(readl(ctrl_reg) & JCORE_SPI_STAT_BUSY))
return 0;
cpu_relax();
} while (--timeout);
return -EBUSY;
}
static void jcore_spi_program(struct jcore_spi *hw)
{
void __iomem *ctrl_reg = hw->base + CTRL_REG;
if (jcore_spi_wait(ctrl_reg))
dev_err(hw->master->dev.parent,
"timeout waiting to program ctrl reg.\n");
writel(hw->cs_reg | hw->speed_reg, ctrl_reg);
}
static void jcore_spi_chipsel(struct spi_device *spi, bool value)
{
struct jcore_spi *hw = spi_master_get_devdata(spi->master);
u32 csbit = 1U << (2 * spi->chip_select);
dev_dbg(hw->master->dev.parent, "chipselect %d\n", spi->chip_select);
if (value)
hw->cs_reg |= csbit;
else
hw->cs_reg &= ~csbit;
jcore_spi_program(hw);
}
static void jcore_spi_baudrate(struct jcore_spi *hw, int speed)
{
if (speed == hw->speed_hz) return;
hw->speed_hz = speed;
if (speed >= hw->clock_freq / 2)
hw->speed_reg = 0;
else
hw->speed_reg = ((hw->clock_freq / 2 / speed) - 1) << 27;
jcore_spi_program(hw);
dev_dbg(hw->master->dev.parent, "speed=%d reg=0x%x\n",
speed, hw->speed_reg);
}
static int jcore_spi_txrx(struct spi_master *master, struct spi_device *spi,
struct spi_transfer *t)
{
struct jcore_spi *hw = spi_master_get_devdata(master);
void __iomem *ctrl_reg = hw->base + CTRL_REG;
void __iomem *data_reg = hw->base + DATA_REG;
u32 xmit;
const unsigned char *tx;
unsigned char *rx;
unsigned int len;
unsigned int count;
jcore_spi_baudrate(hw, t->speed_hz);
xmit = hw->cs_reg | hw->speed_reg | JCORE_SPI_CTRL_XMIT;
tx = t->tx_buf;
rx = t->rx_buf;
len = t->len;
for (count = 0; count < len; count++) {
if (jcore_spi_wait(ctrl_reg))
break;
writel(tx ? *tx++ : 0, data_reg);
writel(xmit, ctrl_reg);
if (jcore_spi_wait(ctrl_reg))
break;
if (rx)
*rx++ = readl(data_reg);
}
spi_finalize_current_transfer(master);
if (count < len)
return -EREMOTEIO;
return 0;
}
static int jcore_spi_probe(struct platform_device *pdev)
{
struct device_node *node = pdev->dev.of_node;
struct jcore_spi *hw;
struct spi_master *master;
struct resource *res;
u32 clock_freq;
struct clk *clk;
int err = -ENODEV;
master = spi_alloc_master(&pdev->dev, sizeof(struct jcore_spi));
if (!master)
return err;
master->num_chipselect = 3;
master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
master->transfer_one = jcore_spi_txrx;
master->set_cs = jcore_spi_chipsel;
master->dev.of_node = node;
master->bus_num = pdev->id;
hw = spi_master_get_devdata(master);
hw->master = master;
platform_set_drvdata(pdev, hw);
res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
if (!res)
goto exit_busy;
if (!devm_request_mem_region(&pdev->dev, res->start,
resource_size(res), pdev->name))
goto exit_busy;
hw->base = devm_ioremap_nocache(&pdev->dev, res->start,
resource_size(res));
if (!hw->base)
goto exit_busy;
clock_freq = 50000000;
clk = devm_clk_get(&pdev->dev, "ref_clk");
if (!IS_ERR_OR_NULL(clk)) {
if (clk_enable(clk) == 0)
clock_freq = clk_get_rate(clk);
else
dev_warn(&pdev->dev, "could not enable ref_clk\n");
}
hw->clock_freq = clock_freq;
hw->cs_reg = JCORE_SPI_CTRL_CS_BITS;
jcore_spi_baudrate(hw, 400000);
err = devm_spi_register_master(&pdev->dev, master);
if (err)
goto exit;
return 0;
exit_busy:
err = -EBUSY;
exit:
spi_master_put(master);
return err;
}
