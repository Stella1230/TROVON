static int tps6586x_gpio_get(struct gpio_chip *gc, unsigned offset)
{
struct tps6586x_gpio *tps6586x_gpio = gpiochip_get_data(gc);
uint8_t val;
int ret;
ret = tps6586x_read(tps6586x_gpio->parent, TPS6586X_GPIOSET2, &val);
if (ret)
return ret;
return !!(val & (1 << offset));
}
static void tps6586x_gpio_set(struct gpio_chip *gc, unsigned offset,
int value)
{
struct tps6586x_gpio *tps6586x_gpio = gpiochip_get_data(gc);
tps6586x_update(tps6586x_gpio->parent, TPS6586X_GPIOSET2,
value << offset, 1 << offset);
}
static int tps6586x_gpio_output(struct gpio_chip *gc, unsigned offset,
int value)
{
struct tps6586x_gpio *tps6586x_gpio = gpiochip_get_data(gc);
uint8_t val, mask;
tps6586x_gpio_set(gc, offset, value);
val = 0x1 << (offset * 2);
mask = 0x3 << (offset * 2);
return tps6586x_update(tps6586x_gpio->parent, TPS6586X_GPIOSET1,
val, mask);
}
static int tps6586x_gpio_to_irq(struct gpio_chip *gc, unsigned offset)
{
struct tps6586x_gpio *tps6586x_gpio = gpiochip_get_data(gc);
return tps6586x_irq_get_virq(tps6586x_gpio->parent,
TPS6586X_INT_PLDO_0 + offset);
}
static int tps6586x_gpio_probe(struct platform_device *pdev)
{
struct tps6586x_platform_data *pdata;
struct tps6586x_gpio *tps6586x_gpio;
int ret;
pdata = dev_get_platdata(pdev->dev.parent);
tps6586x_gpio = devm_kzalloc(&pdev->dev,
sizeof(*tps6586x_gpio), GFP_KERNEL);
if (!tps6586x_gpio)
return -ENOMEM;
tps6586x_gpio->parent = pdev->dev.parent;
tps6586x_gpio->gpio_chip.owner = THIS_MODULE;
tps6586x_gpio->gpio_chip.label = pdev->name;
tps6586x_gpio->gpio_chip.parent = &pdev->dev;
tps6586x_gpio->gpio_chip.ngpio = 4;
tps6586x_gpio->gpio_chip.can_sleep = true;
tps6586x_gpio->gpio_chip.direction_output = tps6586x_gpio_output;
tps6586x_gpio->gpio_chip.set = tps6586x_gpio_set;
tps6586x_gpio->gpio_chip.get = tps6586x_gpio_get;
tps6586x_gpio->gpio_chip.to_irq = tps6586x_gpio_to_irq;
#ifdef CONFIG_OF_GPIO
tps6586x_gpio->gpio_chip.of_node = pdev->dev.parent->of_node;
#endif
if (pdata && pdata->gpio_base)
tps6586x_gpio->gpio_chip.base = pdata->gpio_base;
else
tps6586x_gpio->gpio_chip.base = -1;
ret = devm_gpiochip_add_data(&pdev->dev, &tps6586x_gpio->gpio_chip,
tps6586x_gpio);
if (ret < 0) {
dev_err(&pdev->dev, "Could not register gpiochip, %d\n", ret);
return ret;
}
platform_set_drvdata(pdev, tps6586x_gpio);
return ret;
}
static int __init tps6586x_gpio_init(void)
{
return platform_driver_register(&tps6586x_gpio_driver);
}
static void __exit tps6586x_gpio_exit(void)
{
platform_driver_unregister(&tps6586x_gpio_driver);
}
