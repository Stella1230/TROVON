static inline unsigned long tegra_uart_read(struct tegra_uart_port *tup,
unsigned long reg)
{
return readl(tup->uport.membase + (reg << tup->uport.regshift));
}
static inline void tegra_uart_write(struct tegra_uart_port *tup, unsigned val,
unsigned long reg)
{
writel(val, tup->uport.membase + (reg << tup->uport.regshift));
}
static inline struct tegra_uart_port *to_tegra_uport(struct uart_port *u)
{
return container_of(u, struct tegra_uart_port, uport);
}
static unsigned int tegra_uart_get_mctrl(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
if (tup->enable_modem_interrupt)
return TIOCM_RI | TIOCM_CD | TIOCM_DSR | TIOCM_CTS;
return TIOCM_CTS;
}
static void set_rts(struct tegra_uart_port *tup, bool active)
{
unsigned long mcr;
mcr = tup->mcr_shadow;
if (active)
mcr |= TEGRA_UART_MCR_RTS_EN;
else
mcr &= ~TEGRA_UART_MCR_RTS_EN;
if (mcr != tup->mcr_shadow) {
tegra_uart_write(tup, mcr, UART_MCR);
tup->mcr_shadow = mcr;
}
}
static void set_dtr(struct tegra_uart_port *tup, bool active)
{
unsigned long mcr;
mcr = tup->mcr_shadow;
if (active)
mcr |= UART_MCR_DTR;
else
mcr &= ~UART_MCR_DTR;
if (mcr != tup->mcr_shadow) {
tegra_uart_write(tup, mcr, UART_MCR);
tup->mcr_shadow = mcr;
}
}
static void tegra_uart_set_mctrl(struct uart_port *u, unsigned int mctrl)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
unsigned long mcr;
int dtr_enable;
mcr = tup->mcr_shadow;
tup->rts_active = !!(mctrl & TIOCM_RTS);
set_rts(tup, tup->rts_active);
dtr_enable = !!(mctrl & TIOCM_DTR);
set_dtr(tup, dtr_enable);
}
static void tegra_uart_break_ctl(struct uart_port *u, int break_ctl)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
unsigned long lcr;
lcr = tup->lcr_shadow;
if (break_ctl)
lcr |= UART_LCR_SBC;
else
lcr &= ~UART_LCR_SBC;
tegra_uart_write(tup, lcr, UART_LCR);
tup->lcr_shadow = lcr;
}
static void tegra_uart_wait_cycle_time(struct tegra_uart_port *tup,
unsigned int cycles)
{
if (tup->current_baud)
udelay(DIV_ROUND_UP(cycles * 1000000, tup->current_baud * 16));
}
static void tegra_uart_wait_sym_time(struct tegra_uart_port *tup,
unsigned int syms)
{
if (tup->current_baud)
udelay(DIV_ROUND_UP(syms * tup->symb_bit * 1000000,
tup->current_baud));
}
static void tegra_uart_fifo_reset(struct tegra_uart_port *tup, u8 fcr_bits)
{
unsigned long fcr = tup->fcr_shadow;
if (tup->cdata->allow_txfifo_reset_fifo_mode) {
fcr |= fcr_bits & (UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT);
tegra_uart_write(tup, fcr, UART_FCR);
} else {
fcr &= ~UART_FCR_ENABLE_FIFO;
tegra_uart_write(tup, fcr, UART_FCR);
udelay(60);
fcr |= fcr_bits & (UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT);
tegra_uart_write(tup, fcr, UART_FCR);
fcr |= UART_FCR_ENABLE_FIFO;
tegra_uart_write(tup, fcr, UART_FCR);
}
tegra_uart_read(tup, UART_SCR);
tegra_uart_wait_cycle_time(tup, 32);
}
static int tegra_set_baudrate(struct tegra_uart_port *tup, unsigned int baud)
{
unsigned long rate;
unsigned int divisor;
unsigned long lcr;
int ret;
if (tup->current_baud == baud)
return 0;
if (tup->cdata->support_clk_src_div) {
rate = baud * 16;
ret = clk_set_rate(tup->uart_clk, rate);
if (ret < 0) {
dev_err(tup->uport.dev,
"clk_set_rate() failed for rate %lu\n", rate);
return ret;
}
divisor = 1;
} else {
rate = clk_get_rate(tup->uart_clk);
divisor = DIV_ROUND_CLOSEST(rate, baud * 16);
}
lcr = tup->lcr_shadow;
lcr |= UART_LCR_DLAB;
tegra_uart_write(tup, lcr, UART_LCR);
tegra_uart_write(tup, divisor & 0xFF, UART_TX);
tegra_uart_write(tup, ((divisor >> 8) & 0xFF), UART_IER);
lcr &= ~UART_LCR_DLAB;
tegra_uart_write(tup, lcr, UART_LCR);
tegra_uart_read(tup, UART_SCR);
tup->current_baud = baud;
tegra_uart_wait_sym_time(tup, 2);
return 0;
}
static char tegra_uart_decode_rx_error(struct tegra_uart_port *tup,
unsigned long lsr)
{
char flag = TTY_NORMAL;
if (unlikely(lsr & TEGRA_UART_LSR_ANY)) {
if (lsr & UART_LSR_OE) {
flag = TTY_OVERRUN;
tup->uport.icount.overrun++;
dev_err(tup->uport.dev, "Got overrun errors\n");
} else if (lsr & UART_LSR_PE) {
flag = TTY_PARITY;
tup->uport.icount.parity++;
dev_err(tup->uport.dev, "Got Parity errors\n");
} else if (lsr & UART_LSR_FE) {
flag = TTY_FRAME;
tup->uport.icount.frame++;
dev_err(tup->uport.dev, "Got frame errors\n");
} else if (lsr & UART_LSR_BI) {
dev_err(tup->uport.dev, "Got Break\n");
tup->uport.icount.brk++;
if (!(lsr & UART_LSR_DR) && (lsr & UART_LSR_FIFOE))
tegra_uart_fifo_reset(tup, UART_FCR_CLEAR_RCVR);
}
}
return flag;
}
static int tegra_uart_request_port(struct uart_port *u)
{
return 0;
}
static void tegra_uart_release_port(struct uart_port *u)
{
}
static void tegra_uart_fill_tx_fifo(struct tegra_uart_port *tup, int max_bytes)
{
struct circ_buf *xmit = &tup->uport.state->xmit;
int i;
for (i = 0; i < max_bytes; i++) {
BUG_ON(uart_circ_empty(xmit));
if (tup->cdata->tx_fifo_full_status) {
unsigned long lsr = tegra_uart_read(tup, UART_LSR);
if ((lsr & TEGRA_UART_LSR_TXFIFO_FULL))
break;
}
tegra_uart_write(tup, xmit->buf[xmit->tail], UART_TX);
xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
tup->uport.icount.tx++;
}
}
static void tegra_uart_start_pio_tx(struct tegra_uart_port *tup,
unsigned int bytes)
{
if (bytes > TEGRA_UART_MIN_DMA)
bytes = TEGRA_UART_MIN_DMA;
tup->tx_in_progress = TEGRA_UART_TX_PIO;
tup->tx_bytes = bytes;
tup->ier_shadow |= UART_IER_THRI;
tegra_uart_write(tup, tup->ier_shadow, UART_IER);
}
static void tegra_uart_tx_dma_complete(void *args)
{
struct tegra_uart_port *tup = args;
struct circ_buf *xmit = &tup->uport.state->xmit;
struct dma_tx_state state;
unsigned long flags;
unsigned int count;
dmaengine_tx_status(tup->tx_dma_chan, tup->tx_cookie, &state);
count = tup->tx_bytes_requested - state.residue;
async_tx_ack(tup->tx_dma_desc);
spin_lock_irqsave(&tup->uport.lock, flags);
xmit->tail = (xmit->tail + count) & (UART_XMIT_SIZE - 1);
tup->tx_in_progress = 0;
if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
uart_write_wakeup(&tup->uport);
tegra_uart_start_next_tx(tup);
spin_unlock_irqrestore(&tup->uport.lock, flags);
}
static int tegra_uart_start_tx_dma(struct tegra_uart_port *tup,
unsigned long count)
{
struct circ_buf *xmit = &tup->uport.state->xmit;
dma_addr_t tx_phys_addr;
dma_sync_single_for_device(tup->uport.dev, tup->tx_dma_buf_phys,
UART_XMIT_SIZE, DMA_TO_DEVICE);
tup->tx_bytes = count & ~(0xF);
tx_phys_addr = tup->tx_dma_buf_phys + xmit->tail;
tup->tx_dma_desc = dmaengine_prep_slave_single(tup->tx_dma_chan,
tx_phys_addr, tup->tx_bytes, DMA_MEM_TO_DEV,
DMA_PREP_INTERRUPT);
if (!tup->tx_dma_desc) {
dev_err(tup->uport.dev, "Not able to get desc for Tx\n");
return -EIO;
}
tup->tx_dma_desc->callback = tegra_uart_tx_dma_complete;
tup->tx_dma_desc->callback_param = tup;
tup->tx_in_progress = TEGRA_UART_TX_DMA;
tup->tx_bytes_requested = tup->tx_bytes;
tup->tx_cookie = dmaengine_submit(tup->tx_dma_desc);
dma_async_issue_pending(tup->tx_dma_chan);
return 0;
}
static void tegra_uart_start_next_tx(struct tegra_uart_port *tup)
{
unsigned long tail;
unsigned long count;
struct circ_buf *xmit = &tup->uport.state->xmit;
tail = (unsigned long)&xmit->buf[xmit->tail];
count = CIRC_CNT_TO_END(xmit->head, xmit->tail, UART_XMIT_SIZE);
if (!count)
return;
if (count < TEGRA_UART_MIN_DMA)
tegra_uart_start_pio_tx(tup, count);
else if (BYTES_TO_ALIGN(tail) > 0)
tegra_uart_start_pio_tx(tup, BYTES_TO_ALIGN(tail));
else
tegra_uart_start_tx_dma(tup, count);
}
static void tegra_uart_start_tx(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
struct circ_buf *xmit = &u->state->xmit;
if (!uart_circ_empty(xmit) && !tup->tx_in_progress)
tegra_uart_start_next_tx(tup);
}
static unsigned int tegra_uart_tx_empty(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
unsigned int ret = 0;
unsigned long flags;
spin_lock_irqsave(&u->lock, flags);
if (!tup->tx_in_progress) {
unsigned long lsr = tegra_uart_read(tup, UART_LSR);
if ((lsr & TX_EMPTY_STATUS) == TX_EMPTY_STATUS)
ret = TIOCSER_TEMT;
}
spin_unlock_irqrestore(&u->lock, flags);
return ret;
}
static void tegra_uart_stop_tx(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
struct circ_buf *xmit = &tup->uport.state->xmit;
struct dma_tx_state state;
unsigned int count;
if (tup->tx_in_progress != TEGRA_UART_TX_DMA)
return;
dmaengine_terminate_all(tup->tx_dma_chan);
dmaengine_tx_status(tup->tx_dma_chan, tup->tx_cookie, &state);
count = tup->tx_bytes_requested - state.residue;
async_tx_ack(tup->tx_dma_desc);
xmit->tail = (xmit->tail + count) & (UART_XMIT_SIZE - 1);
tup->tx_in_progress = 0;
}
static void tegra_uart_handle_tx_pio(struct tegra_uart_port *tup)
{
struct circ_buf *xmit = &tup->uport.state->xmit;
tegra_uart_fill_tx_fifo(tup, tup->tx_bytes);
tup->tx_in_progress = 0;
if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
uart_write_wakeup(&tup->uport);
tegra_uart_start_next_tx(tup);
}
static void tegra_uart_handle_rx_pio(struct tegra_uart_port *tup,
struct tty_port *tty)
{
do {
char flag = TTY_NORMAL;
unsigned long lsr = 0;
unsigned char ch;
lsr = tegra_uart_read(tup, UART_LSR);
if (!(lsr & UART_LSR_DR))
break;
flag = tegra_uart_decode_rx_error(tup, lsr);
ch = (unsigned char) tegra_uart_read(tup, UART_RX);
tup->uport.icount.rx++;
if (!uart_handle_sysrq_char(&tup->uport, ch) && tty)
tty_insert_flip_char(tty, ch, flag);
} while (1);
}
static void tegra_uart_copy_rx_to_tty(struct tegra_uart_port *tup,
struct tty_port *tty,
unsigned int count)
{
int copied;
if (!count)
return;
tup->uport.icount.rx += count;
if (!tty) {
dev_err(tup->uport.dev, "No tty port\n");
return;
}
dma_sync_single_for_cpu(tup->uport.dev, tup->rx_dma_buf_phys,
TEGRA_UART_RX_DMA_BUFFER_SIZE, DMA_FROM_DEVICE);
copied = tty_insert_flip_string(tty,
((unsigned char *)(tup->rx_dma_buf_virt)), count);
if (copied != count) {
WARN_ON(1);
dev_err(tup->uport.dev, "RxData copy to tty layer failed\n");
}
dma_sync_single_for_device(tup->uport.dev, tup->rx_dma_buf_phys,
TEGRA_UART_RX_DMA_BUFFER_SIZE, DMA_TO_DEVICE);
}
static void tegra_uart_rx_buffer_push(struct tegra_uart_port *tup,
unsigned int residue)
{
struct tty_port *port = &tup->uport.state->port;
struct tty_struct *tty = tty_port_tty_get(port);
unsigned int count;
async_tx_ack(tup->rx_dma_desc);
count = tup->rx_bytes_requested - residue;
tegra_uart_copy_rx_to_tty(tup, port, count);
tegra_uart_handle_rx_pio(tup, port);
if (tty) {
tty_flip_buffer_push(port);
tty_kref_put(tty);
}
}
static void tegra_uart_rx_dma_complete(void *args)
{
struct tegra_uart_port *tup = args;
struct uart_port *u = &tup->uport;
unsigned long flags;
struct dma_tx_state state;
enum dma_status status;
spin_lock_irqsave(&u->lock, flags);
status = dmaengine_tx_status(tup->rx_dma_chan, tup->rx_cookie, &state);
if (status == DMA_IN_PROGRESS) {
dev_dbg(tup->uport.dev, "RX DMA is in progress\n");
goto done;
}
if (tup->rts_active)
set_rts(tup, false);
tegra_uart_rx_buffer_push(tup, 0);
tegra_uart_start_rx_dma(tup);
if (tup->rts_active)
set_rts(tup, true);
done:
spin_unlock_irqrestore(&u->lock, flags);
}
static void tegra_uart_handle_rx_dma(struct tegra_uart_port *tup)
{
struct dma_tx_state state;
if (tup->rts_active)
set_rts(tup, false);
dmaengine_terminate_all(tup->rx_dma_chan);
dmaengine_tx_status(tup->rx_dma_chan, tup->rx_cookie, &state);
tegra_uart_rx_buffer_push(tup, state.residue);
tegra_uart_start_rx_dma(tup);
if (tup->rts_active)
set_rts(tup, true);
}
static int tegra_uart_start_rx_dma(struct tegra_uart_port *tup)
{
unsigned int count = TEGRA_UART_RX_DMA_BUFFER_SIZE;
tup->rx_dma_desc = dmaengine_prep_slave_single(tup->rx_dma_chan,
tup->rx_dma_buf_phys, count, DMA_DEV_TO_MEM,
DMA_PREP_INTERRUPT);
if (!tup->rx_dma_desc) {
dev_err(tup->uport.dev, "Not able to get desc for Rx\n");
return -EIO;
}
tup->rx_dma_desc->callback = tegra_uart_rx_dma_complete;
tup->rx_dma_desc->callback_param = tup;
dma_sync_single_for_device(tup->uport.dev, tup->rx_dma_buf_phys,
count, DMA_TO_DEVICE);
tup->rx_bytes_requested = count;
tup->rx_cookie = dmaengine_submit(tup->rx_dma_desc);
dma_async_issue_pending(tup->rx_dma_chan);
return 0;
}
static void tegra_uart_handle_modem_signal_change(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
unsigned long msr;
msr = tegra_uart_read(tup, UART_MSR);
if (!(msr & UART_MSR_ANY_DELTA))
return;
if (msr & UART_MSR_TERI)
tup->uport.icount.rng++;
if (msr & UART_MSR_DDSR)
tup->uport.icount.dsr++;
if (msr & UART_MSR_DDCD)
uart_handle_dcd_change(&tup->uport, msr & UART_MSR_DCD);
if (msr & UART_MSR_DCTS)
uart_handle_cts_change(&tup->uport, msr & UART_MSR_CTS);
}
static irqreturn_t tegra_uart_isr(int irq, void *data)
{
struct tegra_uart_port *tup = data;
struct uart_port *u = &tup->uport;
unsigned long iir;
unsigned long ier;
bool is_rx_int = false;
unsigned long flags;
spin_lock_irqsave(&u->lock, flags);
while (1) {
iir = tegra_uart_read(tup, UART_IIR);
if (iir & UART_IIR_NO_INT) {
if (is_rx_int) {
tegra_uart_handle_rx_dma(tup);
if (tup->rx_in_progress) {
ier = tup->ier_shadow;
ier |= (UART_IER_RLSI | UART_IER_RTOIE |
TEGRA_UART_IER_EORD);
tup->ier_shadow = ier;
tegra_uart_write(tup, ier, UART_IER);
}
}
spin_unlock_irqrestore(&u->lock, flags);
return IRQ_HANDLED;
}
switch ((iir >> 1) & 0x7) {
case 0:
tegra_uart_handle_modem_signal_change(u);
break;
case 1:
tup->ier_shadow &= ~UART_IER_THRI;
tegra_uart_write(tup, tup->ier_shadow, UART_IER);
tegra_uart_handle_tx_pio(tup);
break;
case 4:
case 6:
case 2:
if (!is_rx_int) {
is_rx_int = true;
ier = tup->ier_shadow;
ier |= UART_IER_RDI;
tegra_uart_write(tup, ier, UART_IER);
ier &= ~(UART_IER_RDI | UART_IER_RLSI |
UART_IER_RTOIE | TEGRA_UART_IER_EORD);
tup->ier_shadow = ier;
tegra_uart_write(tup, ier, UART_IER);
}
break;
case 3:
tegra_uart_decode_rx_error(tup,
tegra_uart_read(tup, UART_LSR));
break;
case 5:
case 7:
break;
}
}
}
static void tegra_uart_stop_rx(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
struct dma_tx_state state;
unsigned long ier;
if (tup->rts_active)
set_rts(tup, false);
if (!tup->rx_in_progress)
return;
tegra_uart_wait_sym_time(tup, 1);
ier = tup->ier_shadow;
ier &= ~(UART_IER_RDI | UART_IER_RLSI | UART_IER_RTOIE |
TEGRA_UART_IER_EORD);
tup->ier_shadow = ier;
tegra_uart_write(tup, ier, UART_IER);
tup->rx_in_progress = 0;
dmaengine_terminate_all(tup->rx_dma_chan);
dmaengine_tx_status(tup->rx_dma_chan, tup->rx_cookie, &state);
tegra_uart_rx_buffer_push(tup, state.residue);
}
static void tegra_uart_hw_deinit(struct tegra_uart_port *tup)
{
unsigned long flags;
unsigned long char_time = DIV_ROUND_UP(10000000, tup->current_baud);
unsigned long fifo_empty_time = tup->uport.fifosize * char_time;
unsigned long wait_time;
unsigned long lsr;
unsigned long msr;
unsigned long mcr;
tegra_uart_write(tup, 0, UART_IER);
lsr = tegra_uart_read(tup, UART_LSR);
if ((lsr & UART_LSR_TEMT) != UART_LSR_TEMT) {
msr = tegra_uart_read(tup, UART_MSR);
mcr = tegra_uart_read(tup, UART_MCR);
if ((mcr & TEGRA_UART_MCR_CTS_EN) && (msr & UART_MSR_CTS))
dev_err(tup->uport.dev,
"Tx Fifo not empty, CTS disabled, waiting\n");
while ((lsr & UART_LSR_TEMT) != UART_LSR_TEMT) {
wait_time = min(fifo_empty_time, 100lu);
udelay(wait_time);
fifo_empty_time -= wait_time;
if (!fifo_empty_time) {
msr = tegra_uart_read(tup, UART_MSR);
mcr = tegra_uart_read(tup, UART_MCR);
if ((mcr & TEGRA_UART_MCR_CTS_EN) &&
(msr & UART_MSR_CTS))
dev_err(tup->uport.dev,
"Slave not ready\n");
break;
}
lsr = tegra_uart_read(tup, UART_LSR);
}
}
spin_lock_irqsave(&tup->uport.lock, flags);
tegra_uart_fifo_reset(tup, UART_FCR_CLEAR_XMIT | UART_FCR_CLEAR_RCVR);
tup->current_baud = 0;
spin_unlock_irqrestore(&tup->uport.lock, flags);
clk_disable_unprepare(tup->uart_clk);
}
static int tegra_uart_hw_init(struct tegra_uart_port *tup)
{
int ret;
tup->fcr_shadow = 0;
tup->mcr_shadow = 0;
tup->lcr_shadow = 0;
tup->ier_shadow = 0;
tup->current_baud = 0;
clk_prepare_enable(tup->uart_clk);
reset_control_assert(tup->rst);
udelay(10);
reset_control_deassert(tup->rst);
tup->rx_in_progress = 0;
tup->tx_in_progress = 0;
tup->fcr_shadow = UART_FCR_ENABLE_FIFO;
tup->fcr_shadow |= UART_FCR_R_TRIG_01;
tup->fcr_shadow |= TEGRA_UART_TX_TRIG_16B;
tegra_uart_write(tup, tup->fcr_shadow, UART_FCR);
tegra_uart_read(tup, UART_SCR);
tegra_uart_wait_cycle_time(tup, 3);
tup->lcr_shadow = TEGRA_UART_DEFAULT_LSR;
tegra_set_baudrate(tup, TEGRA_UART_DEFAULT_BAUD);
tup->fcr_shadow |= UART_FCR_DMA_SELECT;
tegra_uart_write(tup, tup->fcr_shadow, UART_FCR);
ret = tegra_uart_start_rx_dma(tup);
if (ret < 0) {
dev_err(tup->uport.dev, "Not able to start Rx DMA\n");
return ret;
}
tup->rx_in_progress = 1;
tup->ier_shadow = UART_IER_RLSI | UART_IER_RTOIE | TEGRA_UART_IER_EORD;
tegra_uart_write(tup, tup->ier_shadow, UART_IER);
return 0;
}
static void tegra_uart_dma_channel_free(struct tegra_uart_port *tup,
bool dma_to_memory)
{
if (dma_to_memory) {
dmaengine_terminate_all(tup->rx_dma_chan);
dma_release_channel(tup->rx_dma_chan);
dma_free_coherent(tup->uport.dev, TEGRA_UART_RX_DMA_BUFFER_SIZE,
tup->rx_dma_buf_virt, tup->rx_dma_buf_phys);
tup->rx_dma_chan = NULL;
tup->rx_dma_buf_phys = 0;
tup->rx_dma_buf_virt = NULL;
} else {
dmaengine_terminate_all(tup->tx_dma_chan);
dma_release_channel(tup->tx_dma_chan);
dma_unmap_single(tup->uport.dev, tup->tx_dma_buf_phys,
UART_XMIT_SIZE, DMA_TO_DEVICE);
tup->tx_dma_chan = NULL;
tup->tx_dma_buf_phys = 0;
tup->tx_dma_buf_virt = NULL;
}
}
static int tegra_uart_dma_channel_allocate(struct tegra_uart_port *tup,
bool dma_to_memory)
{
struct dma_chan *dma_chan;
unsigned char *dma_buf;
dma_addr_t dma_phys;
int ret;
struct dma_slave_config dma_sconfig;
dma_chan = dma_request_slave_channel_reason(tup->uport.dev,
dma_to_memory ? "rx" : "tx");
if (IS_ERR(dma_chan)) {
ret = PTR_ERR(dma_chan);
dev_err(tup->uport.dev,
"DMA channel alloc failed: %d\n", ret);
return ret;
}
if (dma_to_memory) {
dma_buf = dma_alloc_coherent(tup->uport.dev,
TEGRA_UART_RX_DMA_BUFFER_SIZE,
&dma_phys, GFP_KERNEL);
if (!dma_buf) {
dev_err(tup->uport.dev,
"Not able to allocate the dma buffer\n");
dma_release_channel(dma_chan);
return -ENOMEM;
}
dma_sconfig.src_addr = tup->uport.mapbase;
dma_sconfig.src_addr_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
dma_sconfig.src_maxburst = 4;
tup->rx_dma_chan = dma_chan;
tup->rx_dma_buf_virt = dma_buf;
tup->rx_dma_buf_phys = dma_phys;
} else {
dma_phys = dma_map_single(tup->uport.dev,
tup->uport.state->xmit.buf, UART_XMIT_SIZE,
DMA_TO_DEVICE);
if (dma_mapping_error(tup->uport.dev, dma_phys)) {
dev_err(tup->uport.dev, "dma_map_single tx failed\n");
dma_release_channel(dma_chan);
return -ENOMEM;
}
dma_buf = tup->uport.state->xmit.buf;
dma_sconfig.dst_addr = tup->uport.mapbase;
dma_sconfig.dst_addr_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
dma_sconfig.dst_maxburst = 16;
tup->tx_dma_chan = dma_chan;
tup->tx_dma_buf_virt = dma_buf;
tup->tx_dma_buf_phys = dma_phys;
}
ret = dmaengine_slave_config(dma_chan, &dma_sconfig);
if (ret < 0) {
dev_err(tup->uport.dev,
"Dma slave config failed, err = %d\n", ret);
tegra_uart_dma_channel_free(tup, dma_to_memory);
return ret;
}
return 0;
}
static int tegra_uart_startup(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
int ret;
ret = tegra_uart_dma_channel_allocate(tup, false);
if (ret < 0) {
dev_err(u->dev, "Tx Dma allocation failed, err = %d\n", ret);
return ret;
}
ret = tegra_uart_dma_channel_allocate(tup, true);
if (ret < 0) {
dev_err(u->dev, "Rx Dma allocation failed, err = %d\n", ret);
goto fail_rx_dma;
}
ret = tegra_uart_hw_init(tup);
if (ret < 0) {
dev_err(u->dev, "Uart HW init failed, err = %d\n", ret);
goto fail_hw_init;
}
ret = request_irq(u->irq, tegra_uart_isr, 0,
dev_name(u->dev), tup);
if (ret < 0) {
dev_err(u->dev, "Failed to register ISR for IRQ %d\n", u->irq);
goto fail_hw_init;
}
return 0;
fail_hw_init:
tegra_uart_dma_channel_free(tup, true);
fail_rx_dma:
tegra_uart_dma_channel_free(tup, false);
return ret;
}
static void tegra_uart_flush_buffer(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
tup->tx_bytes = 0;
if (tup->tx_dma_chan)
dmaengine_terminate_all(tup->tx_dma_chan);
}
static void tegra_uart_shutdown(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
tegra_uart_hw_deinit(tup);
tup->rx_in_progress = 0;
tup->tx_in_progress = 0;
tegra_uart_dma_channel_free(tup, true);
tegra_uart_dma_channel_free(tup, false);
free_irq(u->irq, tup);
}
static void tegra_uart_enable_ms(struct uart_port *u)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
if (tup->enable_modem_interrupt) {
tup->ier_shadow |= UART_IER_MSI;
tegra_uart_write(tup, tup->ier_shadow, UART_IER);
}
}
static void tegra_uart_set_termios(struct uart_port *u,
struct ktermios *termios, struct ktermios *oldtermios)
{
struct tegra_uart_port *tup = to_tegra_uport(u);
unsigned int baud;
unsigned long flags;
unsigned int lcr;
int symb_bit = 1;
struct clk *parent_clk = clk_get_parent(tup->uart_clk);
unsigned long parent_clk_rate = clk_get_rate(parent_clk);
int max_divider = (tup->cdata->support_clk_src_div) ? 0x7FFF : 0xFFFF;
max_divider *= 16;
spin_lock_irqsave(&u->lock, flags);
if (tup->rts_active)
set_rts(tup, false);
tegra_uart_write(tup, tup->ier_shadow | UART_IER_RDI, UART_IER);
tegra_uart_read(tup, UART_IER);
tegra_uart_write(tup, 0, UART_IER);
tegra_uart_read(tup, UART_IER);
lcr = tup->lcr_shadow;
lcr &= ~UART_LCR_PARITY;
termios->c_cflag &= ~CMSPAR;
if ((termios->c_cflag & PARENB) == PARENB) {
symb_bit++;
if (termios->c_cflag & PARODD) {
lcr |= UART_LCR_PARITY;
lcr &= ~UART_LCR_EPAR;
lcr &= ~UART_LCR_SPAR;
} else {
lcr |= UART_LCR_PARITY;
lcr |= UART_LCR_EPAR;
lcr &= ~UART_LCR_SPAR;
}
}
lcr &= ~UART_LCR_WLEN8;
switch (termios->c_cflag & CSIZE) {
case CS5:
lcr |= UART_LCR_WLEN5;
symb_bit += 5;
break;
case CS6:
lcr |= UART_LCR_WLEN6;
symb_bit += 6;
break;
case CS7:
lcr |= UART_LCR_WLEN7;
symb_bit += 7;
break;
default:
lcr |= UART_LCR_WLEN8;
symb_bit += 8;
break;
}
if (termios->c_cflag & CSTOPB) {
lcr |= UART_LCR_STOP;
symb_bit += 2;
} else {
lcr &= ~UART_LCR_STOP;
symb_bit++;
}
tegra_uart_write(tup, lcr, UART_LCR);
tup->lcr_shadow = lcr;
tup->symb_bit = symb_bit;
baud = uart_get_baud_rate(u, termios, oldtermios,
parent_clk_rate/max_divider,
parent_clk_rate/16);
spin_unlock_irqrestore(&u->lock, flags);
tegra_set_baudrate(tup, baud);
if (tty_termios_baud_rate(termios))
tty_termios_encode_baud_rate(termios, baud, baud);
spin_lock_irqsave(&u->lock, flags);
if (termios->c_cflag & CRTSCTS) {
tup->mcr_shadow |= TEGRA_UART_MCR_CTS_EN;
tup->mcr_shadow &= ~TEGRA_UART_MCR_RTS_EN;
tegra_uart_write(tup, tup->mcr_shadow, UART_MCR);
if (tup->rts_active)
set_rts(tup, true);
} else {
tup->mcr_shadow &= ~TEGRA_UART_MCR_CTS_EN;
tup->mcr_shadow &= ~TEGRA_UART_MCR_RTS_EN;
tegra_uart_write(tup, tup->mcr_shadow, UART_MCR);
}
uart_update_timeout(u, termios->c_cflag, baud);
tegra_uart_read(tup, UART_IER);
tegra_uart_write(tup, tup->ier_shadow, UART_IER);
tegra_uart_read(tup, UART_IER);
spin_unlock_irqrestore(&u->lock, flags);
}
static const char *tegra_uart_type(struct uart_port *u)
{
return TEGRA_UART_TYPE;
}
static int tegra_uart_parse_dt(struct platform_device *pdev,
struct tegra_uart_port *tup)
{
struct device_node *np = pdev->dev.of_node;
int port;
port = of_alias_get_id(np, "serial");
if (port < 0) {
dev_err(&pdev->dev, "failed to get alias id, errno %d\n", port);
return port;
}
tup->uport.line = port;
tup->enable_modem_interrupt = of_property_read_bool(np,
"nvidia,enable-modem-interrupt");
return 0;
}
static int tegra_uart_probe(struct platform_device *pdev)
{
struct tegra_uart_port *tup;
struct uart_port *u;
struct resource *resource;
int ret;
const struct tegra_uart_chip_data *cdata;
const struct of_device_id *match;
match = of_match_device(tegra_uart_of_match, &pdev->dev);
if (!match) {
dev_err(&pdev->dev, "Error: No device match found\n");
return -ENODEV;
}
cdata = match->data;
tup = devm_kzalloc(&pdev->dev, sizeof(*tup), GFP_KERNEL);
if (!tup) {
dev_err(&pdev->dev, "Failed to allocate memory for tup\n");
return -ENOMEM;
}
ret = tegra_uart_parse_dt(pdev, tup);
if (ret < 0)
return ret;
u = &tup->uport;
u->dev = &pdev->dev;
u->ops = &tegra_uart_ops;
u->type = PORT_TEGRA;
u->fifosize = 32;
tup->cdata = cdata;
platform_set_drvdata(pdev, tup);
resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
if (!resource) {
dev_err(&pdev->dev, "No IO memory resource\n");
return -ENODEV;
}
u->mapbase = resource->start;
u->membase = devm_ioremap_resource(&pdev->dev, resource);
if (IS_ERR(u->membase))
return PTR_ERR(u->membase);
tup->uart_clk = devm_clk_get(&pdev->dev, NULL);
if (IS_ERR(tup->uart_clk)) {
dev_err(&pdev->dev, "Couldn't get the clock\n");
return PTR_ERR(tup->uart_clk);
}
tup->rst = devm_reset_control_get(&pdev->dev, "serial");
if (IS_ERR(tup->rst)) {
dev_err(&pdev->dev, "Couldn't get the reset\n");
return PTR_ERR(tup->rst);
}
u->iotype = UPIO_MEM32;
u->irq = platform_get_irq(pdev, 0);
u->regshift = 2;
ret = uart_add_one_port(&tegra_uart_driver, u);
if (ret < 0) {
dev_err(&pdev->dev, "Failed to add uart port, err %d\n", ret);
return ret;
}
return ret;
}
static int tegra_uart_remove(struct platform_device *pdev)
{
struct tegra_uart_port *tup = platform_get_drvdata(pdev);
struct uart_port *u = &tup->uport;
uart_remove_one_port(&tegra_uart_driver, u);
return 0;
}
static int tegra_uart_suspend(struct device *dev)
{
struct tegra_uart_port *tup = dev_get_drvdata(dev);
struct uart_port *u = &tup->uport;
return uart_suspend_port(&tegra_uart_driver, u);
}
static int tegra_uart_resume(struct device *dev)
{
struct tegra_uart_port *tup = dev_get_drvdata(dev);
struct uart_port *u = &tup->uport;
return uart_resume_port(&tegra_uart_driver, u);
}
static int __init tegra_uart_init(void)
{
int ret;
ret = uart_register_driver(&tegra_uart_driver);
if (ret < 0) {
pr_err("Could not register %s driver\n",
tegra_uart_driver.driver_name);
return ret;
}
ret = platform_driver_register(&tegra_uart_platform_driver);
if (ret < 0) {
pr_err("Uart platform driver register failed, e = %d\n", ret);
uart_unregister_driver(&tegra_uart_driver);
return ret;
}
return 0;
}
static void __exit tegra_uart_exit(void)
{
pr_info("Unloading tegra uart driver\n");
platform_driver_unregister(&tegra_uart_platform_driver);
uart_unregister_driver(&tegra_uart_driver);
}
