static int sensors_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_sensors_show, NULL);
}
static int poweron_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_poweron_show, NULL);
}
static int progress_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_progress_show, NULL);
}
static int clock_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_clock_show, NULL);
}
static int tone_freq_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_tone_freq_show, NULL);
}
static int tone_volume_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_tone_volume_show, NULL);
}
static int rmo_buf_open(struct inode *inode, struct file *file)
{
return single_open(file, ppc_rtas_rmo_buf_show, NULL);
}
static int __init proc_rtas_init(void)
{
if (!machine_is(pseries))
return -ENODEV;
rtas_node = of_find_node_by_name(NULL, "rtas");
if (rtas_node == NULL)
return -ENODEV;
proc_create("powerpc/rtas/progress", S_IRUGO|S_IWUSR, NULL,
&ppc_rtas_progress_operations);
proc_create("powerpc/rtas/clock", S_IRUGO|S_IWUSR, NULL,
&ppc_rtas_clock_operations);
proc_create("powerpc/rtas/poweron", S_IWUSR|S_IRUGO, NULL,
&ppc_rtas_poweron_operations);
proc_create("powerpc/rtas/sensors", S_IRUGO, NULL,
&ppc_rtas_sensors_operations);
proc_create("powerpc/rtas/frequency", S_IWUSR|S_IRUGO, NULL,
&ppc_rtas_tone_freq_operations);
proc_create("powerpc/rtas/volume", S_IWUSR|S_IRUGO, NULL,
&ppc_rtas_tone_volume_operations);
proc_create("powerpc/rtas/rmo_buffer", S_IRUSR, NULL,
&ppc_rtas_rmo_buf_ops);
return 0;
}
static int parse_number(const char __user *p, size_t count, unsigned long *val)
{
char buf[40];
char *end;
if (count > 39)
return -EINVAL;
if (copy_from_user(buf, p, count))
return -EFAULT;
buf[count] = 0;
*val = simple_strtoul(buf, &end, 10);
if (*end && *end != '\n')
return -EINVAL;
return 0;
}
static ssize_t ppc_rtas_poweron_write(struct file *file,
const char __user *buf, size_t count, loff_t *ppos)
{
struct rtc_time tm;
unsigned long nowtime;
int error = parse_number(buf, count, &nowtime);
if (error)
return error;
power_on_time = nowtime;
to_tm(nowtime, &tm);
error = rtas_call(rtas_token("set-time-for-power-on"), 7, 1, NULL,
tm.tm_year, tm.tm_mon, tm.tm_mday,
tm.tm_hour, tm.tm_min, tm.tm_sec, 0 );
if (error)
printk(KERN_WARNING "error: setting poweron time returned: %s\n",
ppc_rtas_process_error(error));
return count;
}
static int ppc_rtas_poweron_show(struct seq_file *m, void *v)
{
if (power_on_time == 0)
seq_printf(m, "Power on time not set\n");
else
seq_printf(m, "%lu\n",power_on_time);
return 0;
}
static ssize_t ppc_rtas_progress_write(struct file *file,
const char __user *buf, size_t count, loff_t *ppos)
{
unsigned long hex;
if (count >= MAX_LINELENGTH)
count = MAX_LINELENGTH -1;
if (copy_from_user(progress_led, buf, count)) {
return -EFAULT;
}
progress_led[count] = 0;
hex = simple_strtoul(progress_led, NULL, 10);
rtas_progress ((char *)progress_led, hex);
return count;
}
static int ppc_rtas_progress_show(struct seq_file *m, void *v)
{
if (progress_led[0])
seq_printf(m, "%s\n", progress_led);
return 0;
}
static ssize_t ppc_rtas_clock_write(struct file *file,
const char __user *buf, size_t count, loff_t *ppos)
{
struct rtc_time tm;
unsigned long nowtime;
int error = parse_number(buf, count, &nowtime);
if (error)
return error;
to_tm(nowtime, &tm);
error = rtas_call(rtas_token("set-time-of-day"), 7, 1, NULL,
tm.tm_year, tm.tm_mon, tm.tm_mday,
tm.tm_hour, tm.tm_min, tm.tm_sec, 0);
if (error)
printk(KERN_WARNING "error: setting the clock returned: %s\n",
ppc_rtas_process_error(error));
return count;
}
static int ppc_rtas_clock_show(struct seq_file *m, void *v)
{
int ret[8];
int error = rtas_call(rtas_token("get-time-of-day"), 0, 8, ret);
if (error) {
printk(KERN_WARNING "error: reading the clock returned: %s\n",
ppc_rtas_process_error(error));
seq_printf(m, "0");
} else {
unsigned int year, mon, day, hour, min, sec;
year = ret[0]; mon = ret[1]; day = ret[2];
hour = ret[3]; min = ret[4]; sec = ret[5];
seq_printf(m, "%lu\n",
mktime(year, mon, day, hour, min, sec));
}
return 0;
}
static int ppc_rtas_sensors_show(struct seq_file *m, void *v)
{
int i,j;
int state, error;
int get_sensor_state = rtas_token("get-sensor-state");
seq_printf(m, "RTAS (RunTime Abstraction Services) Sensor Information\n");
seq_printf(m, "Sensor\t\tValue\t\tCondition\tLocation\n");
seq_printf(m, "********************************************************\n");
if (ppc_rtas_find_all_sensors() != 0) {
seq_printf(m, "\nNo sensors are available\n");
return 0;
}
for (i=0; i<sensors.quant; i++) {
struct individual_sensor *p = &sensors.sensor[i];
char rstr[64];
const char *loc;
int llen, offs;
sprintf (rstr, SENSOR_PREFIX"%04d", p->token);
loc = of_get_property(rtas_node, rstr, &llen);
for (j = 0, offs = 0; j <= p->quant; j++) {
error = rtas_call(get_sensor_state, 2, 2, &state,
p->token, j);
ppc_rtas_process_sensor(m, p, state, error, loc);
seq_putc(m, '\n');
if (loc) {
offs += strlen(loc) + 1;
loc += strlen(loc) + 1;
if (offs >= llen)
loc = NULL;
}
}
}
return 0;
}
static int ppc_rtas_find_all_sensors(void)
{
const unsigned int *utmp;
int len, i;
utmp = of_get_property(rtas_node, "rtas-sensors", &len);
if (utmp == NULL) {
printk (KERN_ERR "error: could not get rtas-sensors\n");
return 1;
}
sensors.quant = len / 8;
for (i=0; i<sensors.quant; i++) {
sensors.sensor[i].token = *utmp++;
sensors.sensor[i].quant = *utmp++;
}
return 0;
}
static char *ppc_rtas_process_error(int error)
{
switch (error) {
case SENSOR_CRITICAL_HIGH:
return "(critical high)";
case SENSOR_WARNING_HIGH:
return "(warning high)";
case SENSOR_NORMAL:
return "(normal)";
case SENSOR_WARNING_LOW:
return "(warning low)";
case SENSOR_CRITICAL_LOW:
return "(critical low)";
case SENSOR_SUCCESS:
return "(read ok)";
case SENSOR_HW_ERROR:
return "(hardware error)";
case SENSOR_BUSY:
return "(busy)";
case SENSOR_NOT_EXIST:
return "(non existent)";
case SENSOR_DR_ENTITY:
return "(dr entity removed)";
default:
return "(UNKNOWN)";
}
}
static void ppc_rtas_process_sensor(struct seq_file *m,
struct individual_sensor *s, int state, int error, const char *loc)
{
const char * key_switch[] = { "Off\t", "Normal\t", "Secure\t",
"Maintenance" };
const char * enclosure_switch[] = { "Closed", "Open" };
const char * lid_status[] = { " ", "Open", "Closed" };
const char * power_source[] = { "AC\t", "Battery",
"AC & Battery" };
const char * battery_remaining[] = { "Very Low", "Low", "Mid", "High" };
const char * epow_sensor[] = {
"EPOW Reset", "Cooling warning", "Power warning",
"System shutdown", "System halt", "EPOW main enclosure",
"EPOW power off" };
const char * battery_cyclestate[] = { "None", "In progress",
"Requested" };
const char * battery_charging[] = { "Charging", "Discharching",
"No current flow" };
const char * ibm_drconnector[] = { "Empty", "Present", "Unusable",
"Exchange" };
int have_strings = 0;
int num_states = 0;
int temperature = 0;
int unknown = 0;
switch (s->token) {
case KEY_SWITCH:
seq_printf(m, "Key switch:\t");
num_states = sizeof(key_switch) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t", key_switch[state]);
have_strings = 1;
}
break;
case ENCLOSURE_SWITCH:
seq_printf(m, "Enclosure switch:\t");
num_states = sizeof(enclosure_switch) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t",
enclosure_switch[state]);
have_strings = 1;
}
break;
case THERMAL_SENSOR:
seq_printf(m, "Temp. (C/F):\t");
temperature = 1;
break;
case LID_STATUS:
seq_printf(m, "Lid status:\t");
num_states = sizeof(lid_status) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t", lid_status[state]);
have_strings = 1;
}
break;
case POWER_SOURCE:
seq_printf(m, "Power source:\t");
num_states = sizeof(power_source) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t",
power_source[state]);
have_strings = 1;
}
break;
case BATTERY_VOLTAGE:
seq_printf(m, "Battery voltage:\t");
break;
case BATTERY_REMAINING:
seq_printf(m, "Battery remaining:\t");
num_states = sizeof(battery_remaining) / sizeof(char *);
if (state < num_states)
{
seq_printf(m, "%s\t",
battery_remaining[state]);
have_strings = 1;
}
break;
case BATTERY_PERCENTAGE:
seq_printf(m, "Battery percentage:\t");
break;
case EPOW_SENSOR:
seq_printf(m, "EPOW Sensor:\t");
num_states = sizeof(epow_sensor) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t", epow_sensor[state]);
have_strings = 1;
}
break;
case BATTERY_CYCLESTATE:
seq_printf(m, "Battery cyclestate:\t");
num_states = sizeof(battery_cyclestate) /
sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t",
battery_cyclestate[state]);
have_strings = 1;
}
break;
case BATTERY_CHARGING:
seq_printf(m, "Battery Charging:\t");
num_states = sizeof(battery_charging) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t",
battery_charging[state]);
have_strings = 1;
}
break;
case IBM_SURVEILLANCE:
seq_printf(m, "Surveillance:\t");
break;
case IBM_FANRPM:
seq_printf(m, "Fan (rpm):\t");
break;
case IBM_VOLTAGE:
seq_printf(m, "Voltage (mv):\t");
break;
case IBM_DRCONNECTOR:
seq_printf(m, "DR connector:\t");
num_states = sizeof(ibm_drconnector) / sizeof(char *);
if (state < num_states) {
seq_printf(m, "%s\t",
ibm_drconnector[state]);
have_strings = 1;
}
break;
case IBM_POWERSUPPLY:
seq_printf(m, "Powersupply:\t");
break;
default:
seq_printf(m, "Unknown sensor (type %d), ignoring it\n",
s->token);
unknown = 1;
have_strings = 1;
break;
}
if (have_strings == 0) {
if (temperature) {
seq_printf(m, "%4d /%4d\t", state, cel_to_fahr(state));
} else
seq_printf(m, "%10d\t", state);
}
if (unknown == 0) {
seq_printf(m, "%s\t", ppc_rtas_process_error(error));
get_location_code(m, s, loc);
}
}
static void check_location(struct seq_file *m, const char *c)
{
switch (c[0]) {
case LOC_PLANAR:
seq_printf(m, "Planar #%c", c[1]);
break;
case LOC_CPU:
seq_printf(m, "CPU #%c", c[1]);
break;
case LOC_FAN:
seq_printf(m, "Fan #%c", c[1]);
break;
case LOC_RACKMOUNTED:
seq_printf(m, "Rack #%c", c[1]);
break;
case LOC_VOLTAGE:
seq_printf(m, "Voltage #%c", c[1]);
break;
case LOC_LCD:
seq_printf(m, "LCD #%c", c[1]);
break;
case '.':
seq_printf(m, "- %c", c[1]);
break;
default:
seq_printf(m, "Unknown location");
break;
}
}
static void check_location_string(struct seq_file *m, const char *c)
{
while (*c) {
if (isalpha(*c) || *c == '.')
check_location(m, c);
else if (*c == '/' || *c == '-')
seq_printf(m, " at ");
c++;
}
}
static void get_location_code(struct seq_file *m, struct individual_sensor *s,
const char *loc)
{
if (!loc || !*loc) {
seq_printf(m, "---");
} else {
check_location_string(m, loc);
}
seq_putc(m, ' ');
}
static ssize_t ppc_rtas_tone_freq_write(struct file *file,
const char __user *buf, size_t count, loff_t *ppos)
{
unsigned long freq;
int error = parse_number(buf, count, &freq);
if (error)
return error;
rtas_tone_frequency = freq;
error = rtas_call(rtas_token("set-indicator"), 3, 1, NULL,
TONE_FREQUENCY, 0, freq);
if (error)
printk(KERN_WARNING "error: setting tone frequency returned: %s\n",
ppc_rtas_process_error(error));
return count;
}
static int ppc_rtas_tone_freq_show(struct seq_file *m, void *v)
{
seq_printf(m, "%lu\n", rtas_tone_frequency);
return 0;
}
static ssize_t ppc_rtas_tone_volume_write(struct file *file,
const char __user *buf, size_t count, loff_t *ppos)
{
unsigned long volume;
int error = parse_number(buf, count, &volume);
if (error)
return error;
if (volume > 100)
volume = 100;
rtas_tone_volume = volume;
error = rtas_call(rtas_token("set-indicator"), 3, 1, NULL,
TONE_VOLUME, 0, volume);
if (error)
printk(KERN_WARNING "error: setting tone volume returned: %s\n",
ppc_rtas_process_error(error));
return count;
}
static int ppc_rtas_tone_volume_show(struct seq_file *m, void *v)
{
seq_printf(m, "%lu\n", rtas_tone_volume);
return 0;
}
static int ppc_rtas_rmo_buf_show(struct seq_file *m, void *v)
{
seq_printf(m, "%016lx %x\n", rtas_rmo_buf, RTAS_RMOBUF_MAX);
return 0;
}
