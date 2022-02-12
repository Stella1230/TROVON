static inline struct hibvt_pwm_chip *to_hibvt_pwm_chip(struct pwm_chip *chip)
{
return container_of(chip, struct hibvt_pwm_chip, chip);
}
static void hibvt_pwm_set_bits(void __iomem *base, u32 offset,
u32 mask, u32 data)
{
void __iomem *address = base + offset;
u32 value;
value = readl(address);
value &= ~mask;
value |= (data & mask);
writel(value, address);
}
static void hibvt_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
struct hibvt_pwm_chip *hi_pwm_chip = to_hibvt_pwm_chip(chip);
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CTRL_ADDR(pwm->hwpwm),
PWM_ENABLE_MASK, 0x1);
}
static void hibvt_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
struct hibvt_pwm_chip *hi_pwm_chip = to_hibvt_pwm_chip(chip);
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CTRL_ADDR(pwm->hwpwm),
PWM_ENABLE_MASK, 0x0);
}
static void hibvt_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
int duty_cycle_ns, int period_ns)
{
struct hibvt_pwm_chip *hi_pwm_chip = to_hibvt_pwm_chip(chip);
u32 freq, period, duty;
freq = div_u64(clk_get_rate(hi_pwm_chip->clk), 1000000);
period = div_u64(freq * period_ns, 1000);
duty = div_u64(period * duty_cycle_ns, period_ns);
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CFG0_ADDR(pwm->hwpwm),
PWM_PERIOD_MASK, period);
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CFG1_ADDR(pwm->hwpwm),
PWM_DUTY_MASK, duty);
}
static void hibvt_pwm_set_polarity(struct pwm_chip *chip,
struct pwm_device *pwm,
enum pwm_polarity polarity)
{
struct hibvt_pwm_chip *hi_pwm_chip = to_hibvt_pwm_chip(chip);
if (polarity == PWM_POLARITY_INVERSED)
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CTRL_ADDR(pwm->hwpwm),
PWM_POLARITY_MASK, (0x1 << PWM_POLARITY_SHIFT));
else
hibvt_pwm_set_bits(hi_pwm_chip->base, PWM_CTRL_ADDR(pwm->hwpwm),
PWM_POLARITY_MASK, (0x0 << PWM_POLARITY_SHIFT));
}
static void hibvt_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
struct pwm_state *state)
{
struct hibvt_pwm_chip *hi_pwm_chip = to_hibvt_pwm_chip(chip);
void __iomem *base;
u32 freq, value;
freq = div_u64(clk_get_rate(hi_pwm_chip->clk), 1000000);
base = hi_pwm_chip->base;
value = readl(base + PWM_CFG0_ADDR(pwm->hwpwm));
state->period = div_u64(value * 1000, freq);
value = readl(base + PWM_CFG1_ADDR(pwm->hwpwm));
state->duty_cycle = div_u64(value * 1000, freq);
value = readl(base + PWM_CTRL_ADDR(pwm->hwpwm));
state->enabled = (PWM_ENABLE_MASK & value);
}
static int hibvt_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
struct pwm_state *state)
{
if (state->polarity != pwm->state.polarity)
hibvt_pwm_set_polarity(chip, pwm, state->polarity);
if (state->period != pwm->state.period ||
state->duty_cycle != pwm->state.duty_cycle)
hibvt_pwm_config(chip, pwm, state->duty_cycle, state->period);
if (state->enabled != pwm->state.enabled) {
if (state->enabled)
hibvt_pwm_enable(chip, pwm);
else
hibvt_pwm_disable(chip, pwm);
}
return 0;
}
static int hibvt_pwm_probe(struct platform_device *pdev)
{
const struct hibvt_pwm_soc *soc =
of_device_get_match_data(&pdev->dev);
struct hibvt_pwm_chip *pwm_chip;
struct resource *res;
int ret;
int i;
pwm_chip = devm_kzalloc(&pdev->dev, sizeof(*pwm_chip), GFP_KERNEL);
if (pwm_chip == NULL)
return -ENOMEM;
pwm_chip->clk = devm_clk_get(&pdev->dev, NULL);
if (IS_ERR(pwm_chip->clk)) {
dev_err(&pdev->dev, "getting clock failed with %ld\n",
PTR_ERR(pwm_chip->clk));
return PTR_ERR(pwm_chip->clk);
}
pwm_chip->chip.ops = &hibvt_pwm_ops;
pwm_chip->chip.dev = &pdev->dev;
pwm_chip->chip.base = -1;
pwm_chip->chip.npwm = soc->num_pwms;
pwm_chip->chip.of_xlate = of_pwm_xlate_with_flags;
pwm_chip->chip.of_pwm_n_cells = 3;
res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
pwm_chip->base = devm_ioremap_resource(&pdev->dev, res);
if (IS_ERR(pwm_chip->base))
return PTR_ERR(pwm_chip->base);
ret = clk_prepare_enable(pwm_chip->clk);
if (ret < 0)
return ret;
pwm_chip->rstc = devm_reset_control_get(&pdev->dev, NULL);
if (IS_ERR(pwm_chip->rstc)) {
clk_disable_unprepare(pwm_chip->clk);
return PTR_ERR(pwm_chip->rstc);
}
reset_control_assert(pwm_chip->rstc);
msleep(30);
reset_control_deassert(pwm_chip->rstc);
ret = pwmchip_add(&pwm_chip->chip);
if (ret < 0) {
clk_disable_unprepare(pwm_chip->clk);
return ret;
}
for (i = 0; i < pwm_chip->chip.npwm; i++) {
hibvt_pwm_set_bits(pwm_chip->base, PWM_CTRL_ADDR(i),
PWM_KEEP_MASK, (0x1 << PWM_KEEP_SHIFT));
}
platform_set_drvdata(pdev, pwm_chip);
return 0;
}
static int hibvt_pwm_remove(struct platform_device *pdev)
{
struct hibvt_pwm_chip *pwm_chip;
pwm_chip = platform_get_drvdata(pdev);
reset_control_assert(pwm_chip->rstc);
msleep(30);
reset_control_deassert(pwm_chip->rstc);
clk_disable_unprepare(pwm_chip->clk);
return pwmchip_remove(&pwm_chip->chip);
}
