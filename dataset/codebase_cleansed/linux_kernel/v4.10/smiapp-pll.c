static inline uint32_t clk_div_even(uint32_t a)
{
return max_t(uint32_t, 1, a & ~1);
}
static inline uint32_t clk_div_even_up(uint32_t a)
{
if (a == 1)
return 1;
return (a + 1) & ~1;
}
static inline uint32_t is_one_or_even(uint32_t a)
{
if (a == 1)
return 1;
if (a & 1)
return 0;
return 1;
}
static int bounds_check(struct device *dev, uint32_t val,
uint32_t min, uint32_t max, char *str)
{
if (val >= min && val <= max)
return 0;
dev_dbg(dev, "%s out of bounds: %d (%d--%d)\n", str, val, min, max);
return -EINVAL;
}
static void print_pll(struct device *dev, struct smiapp_pll *pll)
{
dev_dbg(dev, "pre_pll_clk_div\t%u\n", pll->pre_pll_clk_div);
dev_dbg(dev, "pll_multiplier \t%u\n", pll->pll_multiplier);
if (!(pll->flags & SMIAPP_PLL_FLAG_NO_OP_CLOCKS)) {
dev_dbg(dev, "op_sys_clk_div \t%u\n", pll->op.sys_clk_div);
dev_dbg(dev, "op_pix_clk_div \t%u\n", pll->op.pix_clk_div);
}
dev_dbg(dev, "vt_sys_clk_div \t%u\n", pll->vt.sys_clk_div);
dev_dbg(dev, "vt_pix_clk_div \t%u\n", pll->vt.pix_clk_div);
dev_dbg(dev, "ext_clk_freq_hz \t%u\n", pll->ext_clk_freq_hz);
dev_dbg(dev, "pll_ip_clk_freq_hz \t%u\n", pll->pll_ip_clk_freq_hz);
dev_dbg(dev, "pll_op_clk_freq_hz \t%u\n", pll->pll_op_clk_freq_hz);
if (!(pll->flags & SMIAPP_PLL_FLAG_NO_OP_CLOCKS)) {
dev_dbg(dev, "op_sys_clk_freq_hz \t%u\n",
pll->op.sys_clk_freq_hz);
dev_dbg(dev, "op_pix_clk_freq_hz \t%u\n",
pll->op.pix_clk_freq_hz);
}
dev_dbg(dev, "vt_sys_clk_freq_hz \t%u\n", pll->vt.sys_clk_freq_hz);
dev_dbg(dev, "vt_pix_clk_freq_hz \t%u\n", pll->vt.pix_clk_freq_hz);
}
static int check_all_bounds(struct device *dev,
const struct smiapp_pll_limits *limits,
const struct smiapp_pll_branch_limits *op_limits,
struct smiapp_pll *pll,
struct smiapp_pll_branch *op_pll)
{
int rval;
rval = bounds_check(dev, pll->pll_ip_clk_freq_hz,
limits->min_pll_ip_freq_hz,
limits->max_pll_ip_freq_hz,
"pll_ip_clk_freq_hz");
if (!rval)
rval = bounds_check(
dev, pll->pll_multiplier,
limits->min_pll_multiplier, limits->max_pll_multiplier,
"pll_multiplier");
if (!rval)
rval = bounds_check(
dev, pll->pll_op_clk_freq_hz,
limits->min_pll_op_freq_hz, limits->max_pll_op_freq_hz,
"pll_op_clk_freq_hz");
if (!rval)
rval = bounds_check(
dev, op_pll->sys_clk_div,
op_limits->min_sys_clk_div, op_limits->max_sys_clk_div,
"op_sys_clk_div");
if (!rval)
rval = bounds_check(
dev, op_pll->sys_clk_freq_hz,
op_limits->min_sys_clk_freq_hz,
op_limits->max_sys_clk_freq_hz,
"op_sys_clk_freq_hz");
if (!rval)
rval = bounds_check(
dev, op_pll->pix_clk_freq_hz,
op_limits->min_pix_clk_freq_hz,
op_limits->max_pix_clk_freq_hz,
"op_pix_clk_freq_hz");
if (pll->flags & SMIAPP_PLL_FLAG_NO_OP_CLOCKS)
return rval;
if (!rval)
rval = bounds_check(
dev, pll->vt.sys_clk_freq_hz,
limits->vt.min_sys_clk_freq_hz,
limits->vt.max_sys_clk_freq_hz,
"vt_sys_clk_freq_hz");
if (!rval)
rval = bounds_check(
dev, pll->vt.pix_clk_freq_hz,
limits->vt.min_pix_clk_freq_hz,
limits->vt.max_pix_clk_freq_hz,
"vt_pix_clk_freq_hz");
return rval;
}
static int __smiapp_pll_calculate(
struct device *dev, const struct smiapp_pll_limits *limits,
const struct smiapp_pll_branch_limits *op_limits,
struct smiapp_pll *pll, struct smiapp_pll_branch *op_pll, uint32_t mul,
uint32_t div, uint32_t lane_op_clock_ratio)
{
uint32_t sys_div;
uint32_t best_pix_div = INT_MAX >> 1;
uint32_t vt_op_binning_div;
uint32_t more_mul_min, more_mul_max;
uint32_t more_mul_factor;
uint32_t min_vt_div, max_vt_div, vt_div;
uint32_t min_sys_div, max_sys_div;
unsigned int i;
dev_dbg(dev, "pre_pll_clk_div %u\n", pll->pre_pll_clk_div);
more_mul_max = limits->max_pll_multiplier / mul;
dev_dbg(dev, "more_mul_max: max_pll_multiplier check: %u\n",
more_mul_max);
more_mul_max =
min_t(uint32_t,
more_mul_max,
limits->max_pll_op_freq_hz
/ (pll->ext_clk_freq_hz / pll->pre_pll_clk_div * mul));
dev_dbg(dev, "more_mul_max: max_pll_op_freq_hz check: %u\n",
more_mul_max);
more_mul_max = min(more_mul_max,
op_limits->max_sys_clk_div * pll->pre_pll_clk_div
/ div);
dev_dbg(dev, "more_mul_max: max_op_sys_clk_div check: %u\n",
more_mul_max);
more_mul_max = min(more_mul_max,
DIV_ROUND_UP(limits->max_pll_multiplier, mul));
dev_dbg(dev, "more_mul_max: min_pll_multiplier check: %u\n",
more_mul_max);
more_mul_min = DIV_ROUND_UP(limits->min_pll_op_freq_hz,
pll->ext_clk_freq_hz / pll->pre_pll_clk_div
* mul);
dev_dbg(dev, "more_mul_min: min_pll_op_freq_hz check: %u\n",
more_mul_min);
more_mul_min = max(more_mul_min,
DIV_ROUND_UP(limits->min_pll_multiplier, mul));
dev_dbg(dev, "more_mul_min: min_pll_multiplier check: %u\n",
more_mul_min);
if (more_mul_min > more_mul_max) {
dev_dbg(dev,
"unable to compute more_mul_min and more_mul_max\n");
return -EINVAL;
}
more_mul_factor = lcm(div, pll->pre_pll_clk_div) / div;
dev_dbg(dev, "more_mul_factor: %u\n", more_mul_factor);
more_mul_factor = lcm(more_mul_factor, op_limits->min_sys_clk_div);
dev_dbg(dev, "more_mul_factor: min_op_sys_clk_div: %d\n",
more_mul_factor);
i = roundup(more_mul_min, more_mul_factor);
if (!is_one_or_even(i))
i <<= 1;
dev_dbg(dev, "final more_mul: %u\n", i);
if (i > more_mul_max) {
dev_dbg(dev, "final more_mul is bad, max %u\n", more_mul_max);
return -EINVAL;
}
pll->pll_multiplier = mul * i;
op_pll->sys_clk_div = div * i / pll->pre_pll_clk_div;
dev_dbg(dev, "op_sys_clk_div: %u\n", op_pll->sys_clk_div);
pll->pll_ip_clk_freq_hz = pll->ext_clk_freq_hz
/ pll->pre_pll_clk_div;
pll->pll_op_clk_freq_hz = pll->pll_ip_clk_freq_hz
* pll->pll_multiplier;
op_pll->sys_clk_freq_hz =
pll->pll_op_clk_freq_hz / op_pll->sys_clk_div;
op_pll->pix_clk_div = pll->bits_per_pixel;
dev_dbg(dev, "op_pix_clk_div: %u\n", op_pll->pix_clk_div);
op_pll->pix_clk_freq_hz =
op_pll->sys_clk_freq_hz / op_pll->pix_clk_div;
if (pll->flags & SMIAPP_PLL_FLAG_NO_OP_CLOCKS) {
goto out_skip_vt_calc;
}
if (limits->min_line_length_pck_bin > limits->min_line_length_pck
/ pll->binning_horizontal)
vt_op_binning_div = pll->binning_horizontal;
else
vt_op_binning_div = 1;
dev_dbg(dev, "vt_op_binning_div: %u\n", vt_op_binning_div);
dev_dbg(dev, "scale_m: %u\n", pll->scale_m);
min_vt_div = DIV_ROUND_UP(op_pll->pix_clk_div * op_pll->sys_clk_div
* pll->scale_n,
lane_op_clock_ratio * vt_op_binning_div
* pll->scale_m);
dev_dbg(dev, "min_vt_div: %u\n", min_vt_div);
min_vt_div = max(min_vt_div,
DIV_ROUND_UP(pll->pll_op_clk_freq_hz,
limits->vt.max_pix_clk_freq_hz));
dev_dbg(dev, "min_vt_div: max_vt_pix_clk_freq_hz: %u\n",
min_vt_div);
min_vt_div = max_t(uint32_t, min_vt_div,
limits->vt.min_pix_clk_div
* limits->vt.min_sys_clk_div);
dev_dbg(dev, "min_vt_div: min_vt_clk_div: %u\n", min_vt_div);
max_vt_div = limits->vt.max_sys_clk_div * limits->vt.max_pix_clk_div;
dev_dbg(dev, "max_vt_div: %u\n", max_vt_div);
max_vt_div = min(max_vt_div,
DIV_ROUND_UP(pll->pll_op_clk_freq_hz,
limits->vt.min_pix_clk_freq_hz));
dev_dbg(dev, "max_vt_div: min_vt_pix_clk_freq_hz: %u\n",
max_vt_div);
min_sys_div = limits->vt.min_sys_clk_div;
dev_dbg(dev, "min_sys_div: %u\n", min_sys_div);
min_sys_div = max(min_sys_div,
DIV_ROUND_UP(min_vt_div,
limits->vt.max_pix_clk_div));
dev_dbg(dev, "min_sys_div: max_vt_pix_clk_div: %u\n", min_sys_div);
min_sys_div = max(min_sys_div,
pll->pll_op_clk_freq_hz
/ limits->vt.max_sys_clk_freq_hz);
dev_dbg(dev, "min_sys_div: max_pll_op_clk_freq_hz: %u\n", min_sys_div);
min_sys_div = clk_div_even_up(min_sys_div);
dev_dbg(dev, "min_sys_div: one or even: %u\n", min_sys_div);
max_sys_div = limits->vt.max_sys_clk_div;
dev_dbg(dev, "max_sys_div: %u\n", max_sys_div);
max_sys_div = min(max_sys_div,
DIV_ROUND_UP(max_vt_div,
limits->vt.min_pix_clk_div));
dev_dbg(dev, "max_sys_div: min_vt_pix_clk_div: %u\n", max_sys_div);
max_sys_div = min(max_sys_div,
DIV_ROUND_UP(pll->pll_op_clk_freq_hz,
limits->vt.min_pix_clk_freq_hz));
dev_dbg(dev, "max_sys_div: min_vt_pix_clk_freq_hz: %u\n", max_sys_div);
for (vt_div = min_vt_div; vt_div <= max_vt_div;
vt_div += 2 - (vt_div & 1)) {
for (sys_div = min_sys_div;
sys_div <= max_sys_div;
sys_div += 2 - (sys_div & 1)) {
uint16_t pix_div = DIV_ROUND_UP(vt_div, sys_div);
if (pix_div < limits->vt.min_pix_clk_div
|| pix_div > limits->vt.max_pix_clk_div) {
dev_dbg(dev,
"pix_div %u too small or too big (%u--%u)\n",
pix_div,
limits->vt.min_pix_clk_div,
limits->vt.max_pix_clk_div);
continue;
}
if (pix_div * sys_div
<= roundup(min_vt_div, best_pix_div))
best_pix_div = pix_div;
}
if (best_pix_div < INT_MAX >> 1)
break;
}
pll->vt.sys_clk_div = DIV_ROUND_UP(min_vt_div, best_pix_div);
pll->vt.pix_clk_div = best_pix_div;
pll->vt.sys_clk_freq_hz =
pll->pll_op_clk_freq_hz / pll->vt.sys_clk_div;
pll->vt.pix_clk_freq_hz =
pll->vt.sys_clk_freq_hz / pll->vt.pix_clk_div;
out_skip_vt_calc:
pll->pixel_rate_csi =
op_pll->pix_clk_freq_hz * lane_op_clock_ratio;
pll->pixel_rate_pixel_array = pll->vt.pix_clk_freq_hz;
return check_all_bounds(dev, limits, op_limits, pll, op_pll);
}
int smiapp_pll_calculate(struct device *dev,
const struct smiapp_pll_limits *limits,
struct smiapp_pll *pll)
{
const struct smiapp_pll_branch_limits *op_limits = &limits->op;
struct smiapp_pll_branch *op_pll = &pll->op;
uint16_t min_pre_pll_clk_div;
uint16_t max_pre_pll_clk_div;
uint32_t lane_op_clock_ratio;
uint32_t mul, div;
unsigned int i;
int rval = -EINVAL;
if (pll->flags & SMIAPP_PLL_FLAG_NO_OP_CLOCKS) {
op_limits = &limits->vt;
op_pll = &pll->vt;
}
if (pll->flags & SMIAPP_PLL_FLAG_OP_PIX_CLOCK_PER_LANE)
lane_op_clock_ratio = pll->csi2.lanes;
else
lane_op_clock_ratio = 1;
dev_dbg(dev, "lane_op_clock_ratio: %u\n", lane_op_clock_ratio);
dev_dbg(dev, "binning: %ux%u\n", pll->binning_horizontal,
pll->binning_vertical);
switch (pll->bus_type) {
case SMIAPP_PLL_BUS_TYPE_CSI2:
pll->pll_op_clk_freq_hz = pll->link_freq * 2
* (pll->csi2.lanes / lane_op_clock_ratio);
break;
case SMIAPP_PLL_BUS_TYPE_PARALLEL:
pll->pll_op_clk_freq_hz = pll->link_freq * pll->bits_per_pixel
/ DIV_ROUND_UP(pll->bits_per_pixel,
pll->parallel.bus_width);
break;
default:
return -EINVAL;
}
dev_dbg(dev, "min / max pre_pll_clk_div: %u / %u\n",
limits->min_pre_pll_clk_div, limits->max_pre_pll_clk_div);
max_pre_pll_clk_div =
min_t(uint16_t, limits->max_pre_pll_clk_div,
clk_div_even(pll->ext_clk_freq_hz /
limits->min_pll_ip_freq_hz));
min_pre_pll_clk_div =
max_t(uint16_t, limits->min_pre_pll_clk_div,
clk_div_even_up(
DIV_ROUND_UP(pll->ext_clk_freq_hz,
limits->max_pll_ip_freq_hz)));
dev_dbg(dev, "pre-pll check: min / max pre_pll_clk_div: %u / %u\n",
min_pre_pll_clk_div, max_pre_pll_clk_div);
i = gcd(pll->pll_op_clk_freq_hz, pll->ext_clk_freq_hz);
mul = div_u64(pll->pll_op_clk_freq_hz, i);
div = pll->ext_clk_freq_hz / i;
dev_dbg(dev, "mul %u / div %u\n", mul, div);
min_pre_pll_clk_div =
max_t(uint16_t, min_pre_pll_clk_div,
clk_div_even_up(
DIV_ROUND_UP(mul * pll->ext_clk_freq_hz,
limits->max_pll_op_freq_hz)));
dev_dbg(dev, "pll_op check: min / max pre_pll_clk_div: %u / %u\n",
min_pre_pll_clk_div, max_pre_pll_clk_div);
for (pll->pre_pll_clk_div = min_pre_pll_clk_div;
pll->pre_pll_clk_div <= max_pre_pll_clk_div;
pll->pre_pll_clk_div += 2 - (pll->pre_pll_clk_div & 1)) {
rval = __smiapp_pll_calculate(dev, limits, op_limits, pll,
op_pll, mul, div,
lane_op_clock_ratio);
if (rval)
continue;
print_pll(dev, pll);
return 0;
}
dev_dbg(dev, "unable to compute pre_pll divisor\n");
return rval;
}
