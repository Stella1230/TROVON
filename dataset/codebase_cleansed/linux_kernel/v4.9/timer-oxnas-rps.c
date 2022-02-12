static irqreturn_t oxnas_rps_timer_irq(int irq, void *dev_id)
{
struct oxnas_rps_timer *rps = dev_id;
writel_relaxed(0, rps->clkevt_base + TIMER_CLRINT_REG);
rps->clkevent.event_handler(&rps->clkevent);
return IRQ_HANDLED;
}
static void oxnas_rps_timer_config(struct oxnas_rps_timer *rps,
unsigned long period,
unsigned int periodic)
{
uint32_t cfg = rps->timer_prescaler;
if (period)
cfg |= TIMER_ENABLE;
if (periodic)
cfg |= TIMER_PERIODIC;
writel_relaxed(period, rps->clkevt_base + TIMER_LOAD_REG);
writel_relaxed(cfg, rps->clkevt_base + TIMER_CTRL_REG);
}
static int oxnas_rps_timer_shutdown(struct clock_event_device *evt)
{
struct oxnas_rps_timer *rps =
container_of(evt, struct oxnas_rps_timer, clkevent);
oxnas_rps_timer_config(rps, 0, 0);
return 0;
}
static int oxnas_rps_timer_set_periodic(struct clock_event_device *evt)
{
struct oxnas_rps_timer *rps =
container_of(evt, struct oxnas_rps_timer, clkevent);
oxnas_rps_timer_config(rps, rps->timer_period, 1);
return 0;
}
static int oxnas_rps_timer_set_oneshot(struct clock_event_device *evt)
{
struct oxnas_rps_timer *rps =
container_of(evt, struct oxnas_rps_timer, clkevent);
oxnas_rps_timer_config(rps, rps->timer_period, 0);
return 0;
}
static int oxnas_rps_timer_next_event(unsigned long delta,
struct clock_event_device *evt)
{
struct oxnas_rps_timer *rps =
container_of(evt, struct oxnas_rps_timer, clkevent);
oxnas_rps_timer_config(rps, delta, 0);
return 0;
}
static int __init oxnas_rps_clockevent_init(struct oxnas_rps_timer *rps)
{
ulong clk_rate = clk_get_rate(rps->clk);
ulong timer_rate;
rps->timer_prescaler = TIMER_DIV1;
rps->timer_period = DIV_ROUND_UP(clk_rate, HZ);
timer_rate = clk_rate;
if (rps->timer_period > TIMER_MAX_VAL) {
rps->timer_prescaler = TIMER_DIV16;
timer_rate = clk_rate / 16;
rps->timer_period = DIV_ROUND_UP(timer_rate, HZ);
}
if (rps->timer_period > TIMER_MAX_VAL) {
rps->timer_prescaler = TIMER_DIV256;
timer_rate = clk_rate / 256;
rps->timer_period = DIV_ROUND_UP(timer_rate, HZ);
}
rps->clkevent.name = "oxnas-rps";
rps->clkevent.features = CLOCK_EVT_FEAT_PERIODIC |
CLOCK_EVT_FEAT_ONESHOT |
CLOCK_EVT_FEAT_DYNIRQ;
rps->clkevent.tick_resume = oxnas_rps_timer_shutdown;
rps->clkevent.set_state_shutdown = oxnas_rps_timer_shutdown;
rps->clkevent.set_state_periodic = oxnas_rps_timer_set_periodic;
rps->clkevent.set_state_oneshot = oxnas_rps_timer_set_oneshot;
rps->clkevent.set_next_event = oxnas_rps_timer_next_event;
rps->clkevent.rating = 200;
rps->clkevent.cpumask = cpu_possible_mask;
rps->clkevent.irq = rps->irq;
clockevents_config_and_register(&rps->clkevent,
timer_rate,
1,
TIMER_MAX_VAL);
pr_info("Registered clock event rate %luHz prescaler %x period %lu\n",
clk_rate,
rps->timer_prescaler,
rps->timer_period);
return 0;
}
static u64 notrace oxnas_rps_read_sched_clock(void)
{
return ~readl_relaxed(timer_sched_base);
}
static int __init oxnas_rps_clocksource_init(struct oxnas_rps_timer *rps)
{
ulong clk_rate = clk_get_rate(rps->clk);
int ret;
clk_rate = clk_rate / 16;
writel_relaxed(TIMER_MAX_VAL, rps->clksrc_base + TIMER_LOAD_REG);
writel_relaxed(TIMER_PERIODIC | TIMER_ENABLE | TIMER_DIV16,
rps->clksrc_base + TIMER_CTRL_REG);
timer_sched_base = rps->clksrc_base + TIMER_CURR_REG;
sched_clock_register(oxnas_rps_read_sched_clock,
TIMER_BITS, clk_rate);
ret = clocksource_mmio_init(timer_sched_base,
"oxnas_rps_clocksource_timer",
clk_rate, 250, TIMER_BITS,
clocksource_mmio_readl_down);
if (WARN_ON(ret)) {
pr_err("can't register clocksource\n");
return ret;
}
pr_info("Registered clocksource rate %luHz\n", clk_rate);
return 0;
}
static int __init oxnas_rps_timer_init(struct device_node *np)
{
struct oxnas_rps_timer *rps;
void __iomem *base;
int ret;
rps = kzalloc(sizeof(*rps), GFP_KERNEL);
if (!rps)
return -ENOMEM;
rps->clk = of_clk_get(np, 0);
if (IS_ERR(rps->clk)) {
ret = PTR_ERR(rps->clk);
goto err_alloc;
}
ret = clk_prepare_enable(rps->clk);
if (ret)
goto err_clk;
base = of_iomap(np, 0);
if (!base) {
ret = -ENXIO;
goto err_clk_prepare;
}
rps->irq = irq_of_parse_and_map(np, 0);
if (rps->irq < 0) {
ret = -EINVAL;
goto err_iomap;
}
rps->clkevt_base = base + TIMER1_REG_OFFSET;
rps->clksrc_base = base + TIMER2_REG_OFFSET;
writel_relaxed(0, rps->clkevt_base + TIMER_CTRL_REG);
writel_relaxed(0, rps->clksrc_base + TIMER_CTRL_REG);
writel_relaxed(0, rps->clkevt_base + TIMER_LOAD_REG);
writel_relaxed(0, rps->clksrc_base + TIMER_LOAD_REG);
writel_relaxed(0, rps->clkevt_base + TIMER_CLRINT_REG);
writel_relaxed(0, rps->clksrc_base + TIMER_CLRINT_REG);
ret = request_irq(rps->irq, oxnas_rps_timer_irq,
IRQF_TIMER | IRQF_IRQPOLL,
"rps-timer", rps);
if (ret)
goto err_iomap;
ret = oxnas_rps_clocksource_init(rps);
if (ret)
goto err_irqreq;
ret = oxnas_rps_clockevent_init(rps);
if (ret)
goto err_irqreq;
return 0;
err_irqreq:
free_irq(rps->irq, rps);
err_iomap:
iounmap(base);
err_clk_prepare:
clk_disable_unprepare(rps->clk);
err_clk:
clk_put(rps->clk);
err_alloc:
kfree(rps);
return ret;
}
