static inline uint16_t icu1_set(uint8_t offset, uint16_t set)
{
uint16_t data;
data = icu1_read(offset);
data |= set;
icu1_write(offset, data);
return data;
}
static inline uint16_t icu1_clear(uint8_t offset, uint16_t clear)
{
uint16_t data;
data = icu1_read(offset);
data &= ~clear;
icu1_write(offset, data);
return data;
}
static inline uint16_t icu2_set(uint8_t offset, uint16_t set)
{
uint16_t data;
data = icu2_read(offset);
data |= set;
icu2_write(offset, data);
return data;
}
static inline uint16_t icu2_clear(uint8_t offset, uint16_t clear)
{
uint16_t data;
data = icu2_read(offset);
data &= ~clear;
icu2_write(offset, data);
return data;
}
void vr41xx_enable_piuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(PIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_set(MPIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_piuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(PIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_clear(MPIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_aiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(AIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_set(MAIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_aiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(AIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_clear(MAIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_kiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(KIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_set(MKIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_kiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(KIU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4111 ||
current_cpu_type() == CPU_VR4121) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_clear(MKIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_macint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(ETHERNET_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_set(MMACINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_disable_macint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(ETHERNET_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_clear(MMACINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_enable_dsiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(DSIU_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_set(MDSIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_disable_dsiuint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(DSIU_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu1_clear(MDSIUINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_enable_firint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(FIR_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_set(MFIRINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_disable_firint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(FIR_IRQ);
unsigned long flags;
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_clear(MFIRINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
void vr41xx_enable_pciint(void)
{
struct irq_desc *desc = irq_to_desc(PCI_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MPCIINTREG, PCIINT0);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_pciint(void)
{
struct irq_desc *desc = irq_to_desc(PCI_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MPCIINTREG, 0);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_scuint(void)
{
struct irq_desc *desc = irq_to_desc(SCU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MSCUINTREG, SCUINT0);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_scuint(void)
{
struct irq_desc *desc = irq_to_desc(SCU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MSCUINTREG, 0);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_csiint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(CSI_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_set(MCSIINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_csiint(uint16_t mask)
{
struct irq_desc *desc = irq_to_desc(CSI_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_clear(MCSIINTREG, mask);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_enable_bcuint(void)
{
struct irq_desc *desc = irq_to_desc(BCU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MBCUINTREG, BCUINTR);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
void vr41xx_disable_bcuint(void)
{
struct irq_desc *desc = irq_to_desc(BCU_IRQ);
unsigned long flags;
if (current_cpu_type() == CPU_VR4122 ||
current_cpu_type() == CPU_VR4131 ||
current_cpu_type() == CPU_VR4133) {
raw_spin_lock_irqsave(&desc->lock, flags);
icu2_write(MBCUINTREG, 0);
raw_spin_unlock_irqrestore(&desc->lock, flags);
}
}
static void disable_sysint1_irq(struct irq_data *d)
{
icu1_clear(MSYSINT1REG, 1 << SYSINT1_IRQ_TO_PIN(d->irq));
}
static void enable_sysint1_irq(struct irq_data *d)
{
icu1_set(MSYSINT1REG, 1 << SYSINT1_IRQ_TO_PIN(d->irq));
}
static void disable_sysint2_irq(struct irq_data *d)
{
icu2_clear(MSYSINT2REG, 1 << SYSINT2_IRQ_TO_PIN(d->irq));
}
static void enable_sysint2_irq(struct irq_data *d)
{
icu2_set(MSYSINT2REG, 1 << SYSINT2_IRQ_TO_PIN(d->irq));
}
static inline int set_sysint1_assign(unsigned int irq, unsigned char assign)
{
struct irq_desc *desc = irq_to_desc(irq);
uint16_t intassign0, intassign1;
unsigned int pin;
pin = SYSINT1_IRQ_TO_PIN(irq);
raw_spin_lock_irq(&desc->lock);
intassign0 = icu1_read(INTASSIGN0);
intassign1 = icu1_read(INTASSIGN1);
switch (pin) {
case 0:
intassign0 &= ~INTASSIGN_MASK;
intassign0 |= (uint16_t)assign;
break;
case 1:
intassign0 &= ~(INTASSIGN_MASK << 3);
intassign0 |= (uint16_t)assign << 3;
break;
case 2:
intassign0 &= ~(INTASSIGN_MASK << 6);
intassign0 |= (uint16_t)assign << 6;
break;
case 3:
intassign0 &= ~(INTASSIGN_MASK << 9);
intassign0 |= (uint16_t)assign << 9;
break;
case 8:
intassign0 &= ~(INTASSIGN_MASK << 12);
intassign0 |= (uint16_t)assign << 12;
break;
case 9:
intassign1 &= ~INTASSIGN_MASK;
intassign1 |= (uint16_t)assign;
break;
case 11:
intassign1 &= ~(INTASSIGN_MASK << 6);
intassign1 |= (uint16_t)assign << 6;
break;
case 12:
intassign1 &= ~(INTASSIGN_MASK << 9);
intassign1 |= (uint16_t)assign << 9;
break;
default:
raw_spin_unlock_irq(&desc->lock);
return -EINVAL;
}
sysint1_assign[pin] = assign;
icu1_write(INTASSIGN0, intassign0);
icu1_write(INTASSIGN1, intassign1);
raw_spin_unlock_irq(&desc->lock);
return 0;
}
static inline int set_sysint2_assign(unsigned int irq, unsigned char assign)
{
struct irq_desc *desc = irq_to_desc(irq);
uint16_t intassign2, intassign3;
unsigned int pin;
pin = SYSINT2_IRQ_TO_PIN(irq);
raw_spin_lock_irq(&desc->lock);
intassign2 = icu1_read(INTASSIGN2);
intassign3 = icu1_read(INTASSIGN3);
switch (pin) {
case 0:
intassign2 &= ~INTASSIGN_MASK;
intassign2 |= (uint16_t)assign;
break;
case 1:
intassign2 &= ~(INTASSIGN_MASK << 3);
intassign2 |= (uint16_t)assign << 3;
break;
case 3:
intassign2 &= ~(INTASSIGN_MASK << 6);
intassign2 |= (uint16_t)assign << 6;
break;
case 4:
intassign2 &= ~(INTASSIGN_MASK << 9);
intassign2 |= (uint16_t)assign << 9;
break;
case 5:
intassign2 &= ~(INTASSIGN_MASK << 12);
intassign2 |= (uint16_t)assign << 12;
break;
case 6:
intassign3 &= ~INTASSIGN_MASK;
intassign3 |= (uint16_t)assign;
break;
case 7:
intassign3 &= ~(INTASSIGN_MASK << 3);
intassign3 |= (uint16_t)assign << 3;
break;
case 8:
intassign3 &= ~(INTASSIGN_MASK << 6);
intassign3 |= (uint16_t)assign << 6;
break;
case 9:
intassign3 &= ~(INTASSIGN_MASK << 9);
intassign3 |= (uint16_t)assign << 9;
break;
case 10:
intassign3 &= ~(INTASSIGN_MASK << 12);
intassign3 |= (uint16_t)assign << 12;
break;
default:
raw_spin_unlock_irq(&desc->lock);
return -EINVAL;
}
sysint2_assign[pin] = assign;
icu1_write(INTASSIGN2, intassign2);
icu1_write(INTASSIGN3, intassign3);
raw_spin_unlock_irq(&desc->lock);
return 0;
}
int vr41xx_set_intassign(unsigned int irq, unsigned char intassign)
{
int retval = -EINVAL;
if (current_cpu_type() != CPU_VR4133)
return -EINVAL;
if (intassign > INTASSIGN_MAX)
return -EINVAL;
if (irq >= SYSINT1_IRQ_BASE && irq <= SYSINT1_IRQ_LAST)
retval = set_sysint1_assign(irq, intassign);
else if (irq >= SYSINT2_IRQ_BASE && irq <= SYSINT2_IRQ_LAST)
retval = set_sysint2_assign(irq, intassign);
return retval;
}
static int icu_get_irq(unsigned int irq)
{
uint16_t pend1, pend2;
uint16_t mask1, mask2;
int i;
pend1 = icu1_read(SYSINT1REG);
mask1 = icu1_read(MSYSINT1REG);
pend2 = icu2_read(SYSINT2REG);
mask2 = icu2_read(MSYSINT2REG);
mask1 &= pend1;
mask2 &= pend2;
if (mask1) {
for (i = 0; i < 16; i++) {
if (irq == INT_TO_IRQ(sysint1_assign[i]) && (mask1 & (1 << i)))
return SYSINT1_IRQ(i);
}
}
if (mask2) {
for (i = 0; i < 16; i++) {
if (irq == INT_TO_IRQ(sysint2_assign[i]) && (mask2 & (1 << i)))
return SYSINT2_IRQ(i);
}
}
printk(KERN_ERR "spurious ICU interrupt: %04x,%04x\n", pend1, pend2);
atomic_inc(&irq_err_count);
return -1;
}
static int __init vr41xx_icu_init(void)
{
unsigned long icu1_start, icu2_start;
int i;
switch (current_cpu_type()) {
case CPU_VR4111:
case CPU_VR4121:
icu1_start = ICU1_TYPE1_BASE;
icu2_start = ICU2_TYPE1_BASE;
break;
case CPU_VR4122:
case CPU_VR4131:
case CPU_VR4133:
icu1_start = ICU1_TYPE2_BASE;
icu2_start = ICU2_TYPE2_BASE;
break;
default:
printk(KERN_ERR "ICU: Unexpected CPU of NEC VR4100 series\n");
return -ENODEV;
}
if (request_mem_region(icu1_start, ICU1_SIZE, "ICU") == NULL)
return -EBUSY;
if (request_mem_region(icu2_start, ICU2_SIZE, "ICU") == NULL) {
release_mem_region(icu1_start, ICU1_SIZE);
return -EBUSY;
}
icu1_base = ioremap(icu1_start, ICU1_SIZE);
if (icu1_base == NULL) {
release_mem_region(icu1_start, ICU1_SIZE);
release_mem_region(icu2_start, ICU2_SIZE);
return -ENOMEM;
}
icu2_base = ioremap(icu2_start, ICU2_SIZE);
if (icu2_base == NULL) {
iounmap(icu1_base);
release_mem_region(icu1_start, ICU1_SIZE);
release_mem_region(icu2_start, ICU2_SIZE);
return -ENOMEM;
}
icu1_write(MSYSINT1REG, 0);
icu1_write(MGIUINTLREG, 0xffff);
icu2_write(MSYSINT2REG, 0);
icu2_write(MGIUINTHREG, 0xffff);
for (i = SYSINT1_IRQ_BASE; i <= SYSINT1_IRQ_LAST; i++)
irq_set_chip_and_handler(i, &sysint1_irq_type,
handle_level_irq);
for (i = SYSINT2_IRQ_BASE; i <= SYSINT2_IRQ_LAST; i++)
irq_set_chip_and_handler(i, &sysint2_irq_type,
handle_level_irq);
cascade_irq(INT0_IRQ, icu_get_irq);
cascade_irq(INT1_IRQ, icu_get_irq);
cascade_irq(INT2_IRQ, icu_get_irq);
cascade_irq(INT3_IRQ, icu_get_irq);
cascade_irq(INT4_IRQ, icu_get_irq);
return 0;
}
