static int picolcd_debug_reset_show(struct seq_file *f, void *p)
{
if (picolcd_fbinfo((struct picolcd_data *)f->private))
seq_printf(f, "all fb\n");
else
seq_printf(f, "all\n");
return 0;
}
static int picolcd_debug_reset_open(struct inode *inode, struct file *f)
{
return single_open(f, picolcd_debug_reset_show, inode->i_private);
}
static ssize_t picolcd_debug_reset_write(struct file *f, const char __user *user_buf,
size_t count, loff_t *ppos)
{
struct picolcd_data *data = ((struct seq_file *)f->private_data)->private;
char buf[32];
size_t cnt = min(count, sizeof(buf)-1);
if (copy_from_user(buf, user_buf, cnt))
return -EFAULT;
while (cnt > 0 && (buf[cnt-1] == ' ' || buf[cnt-1] == '\n'))
cnt--;
buf[cnt] = '\0';
if (strcmp(buf, "all") == 0) {
picolcd_reset(data->hdev);
picolcd_fb_reset(data, 1);
} else if (strcmp(buf, "fb") == 0) {
picolcd_fb_reset(data, 1);
} else {
return -EINVAL;
}
return count;
}
static ssize_t picolcd_debug_eeprom_read(struct file *f, char __user *u,
size_t s, loff_t *off)
{
struct picolcd_data *data = f->private_data;
struct picolcd_pending *resp;
u8 raw_data[3];
ssize_t ret = -EIO;
if (s == 0)
return -EINVAL;
if (*off > 0x0ff)
return 0;
raw_data[0] = *off & 0xff;
raw_data[1] = (*off >> 8) & 0xff;
raw_data[2] = s < 20 ? s : 20;
if (*off + raw_data[2] > 0xff)
raw_data[2] = 0x100 - *off;
resp = picolcd_send_and_wait(data->hdev, REPORT_EE_READ, raw_data,
sizeof(raw_data));
if (!resp)
return -EIO;
if (resp->in_report && resp->in_report->id == REPORT_EE_DATA) {
ret = resp->raw_data[2];
if (ret > s)
ret = s;
if (copy_to_user(u, resp->raw_data+3, ret))
ret = -EFAULT;
else
*off += ret;
}
kfree(resp);
return ret;
}
static ssize_t picolcd_debug_eeprom_write(struct file *f, const char __user *u,
size_t s, loff_t *off)
{
struct picolcd_data *data = f->private_data;
struct picolcd_pending *resp;
ssize_t ret = -EIO;
u8 raw_data[23];
if (s == 0)
return -EINVAL;
if (*off > 0x0ff)
return -ENOSPC;
memset(raw_data, 0, sizeof(raw_data));
raw_data[0] = *off & 0xff;
raw_data[1] = (*off >> 8) & 0xff;
raw_data[2] = min_t(size_t, 20, s);
if (*off + raw_data[2] > 0xff)
raw_data[2] = 0x100 - *off;
if (copy_from_user(raw_data+3, u, min((u8)20, raw_data[2])))
return -EFAULT;
resp = picolcd_send_and_wait(data->hdev, REPORT_EE_WRITE, raw_data,
sizeof(raw_data));
if (!resp)
return -EIO;
if (resp->in_report && resp->in_report->id == REPORT_EE_DATA) {
if (memcmp(raw_data, resp->raw_data, 3+raw_data[2]) == 0) {
*off += raw_data[2];
ret = raw_data[2];
}
}
kfree(resp);
return ret;
}
static int _picolcd_flash_setaddr(struct picolcd_data *data, u8 *buf, long off)
{
buf[0] = off & 0xff;
buf[1] = (off >> 8) & 0xff;
if (data->addr_sz == 3)
buf[2] = (off >> 16) & 0xff;
return data->addr_sz == 2 ? 2 : 3;
}
static ssize_t _picolcd_flash_read(struct picolcd_data *data, int report_id,
char __user *u, size_t s, loff_t *off)
{
struct picolcd_pending *resp;
u8 raw_data[4];
ssize_t ret = 0;
int len_off, err = -EIO;
while (s > 0) {
err = -EIO;
len_off = _picolcd_flash_setaddr(data, raw_data, *off);
raw_data[len_off] = s > 32 ? 32 : s;
resp = picolcd_send_and_wait(data->hdev, report_id, raw_data, len_off+1);
if (!resp || !resp->in_report)
goto skip;
if (resp->in_report->id == REPORT_MEMORY ||
resp->in_report->id == REPORT_BL_READ_MEMORY) {
if (memcmp(raw_data, resp->raw_data, len_off+1) != 0)
goto skip;
if (copy_to_user(u+ret, resp->raw_data+len_off+1, raw_data[len_off])) {
err = -EFAULT;
goto skip;
}
*off += raw_data[len_off];
s -= raw_data[len_off];
ret += raw_data[len_off];
err = 0;
}
skip:
kfree(resp);
if (err)
return ret > 0 ? ret : err;
}
return ret;
}
static ssize_t picolcd_debug_flash_read(struct file *f, char __user *u,
size_t s, loff_t *off)
{
struct picolcd_data *data = f->private_data;
if (s == 0)
return -EINVAL;
if (*off > 0x05fff)
return 0;
if (*off + s > 0x05fff)
s = 0x06000 - *off;
if (data->status & PICOLCD_BOOTLOADER)
return _picolcd_flash_read(data, REPORT_BL_READ_MEMORY, u, s, off);
else
return _picolcd_flash_read(data, REPORT_READ_MEMORY, u, s, off);
}
static ssize_t _picolcd_flash_erase64(struct picolcd_data *data, int report_id,
loff_t *off)
{
struct picolcd_pending *resp;
u8 raw_data[3];
int len_off;
ssize_t ret = -EIO;
if (*off & 0x3f)
return -EINVAL;
len_off = _picolcd_flash_setaddr(data, raw_data, *off);
resp = picolcd_send_and_wait(data->hdev, report_id, raw_data, len_off);
if (!resp || !resp->in_report)
goto skip;
if (resp->in_report->id == REPORT_MEMORY ||
resp->in_report->id == REPORT_BL_ERASE_MEMORY) {
if (memcmp(raw_data, resp->raw_data, len_off) != 0)
goto skip;
ret = 0;
}
skip:
kfree(resp);
return ret;
}
static ssize_t _picolcd_flash_write(struct picolcd_data *data, int report_id,
const char __user *u, size_t s, loff_t *off)
{
struct picolcd_pending *resp;
u8 raw_data[36];
ssize_t ret = 0;
int len_off, err = -EIO;
while (s > 0) {
err = -EIO;
len_off = _picolcd_flash_setaddr(data, raw_data, *off);
raw_data[len_off] = s > 32 ? 32 : s;
if (copy_from_user(raw_data+len_off+1, u, raw_data[len_off])) {
err = -EFAULT;
break;
}
resp = picolcd_send_and_wait(data->hdev, report_id, raw_data,
len_off+1+raw_data[len_off]);
if (!resp || !resp->in_report)
goto skip;
if (resp->in_report->id == REPORT_MEMORY ||
resp->in_report->id == REPORT_BL_WRITE_MEMORY) {
if (memcmp(raw_data, resp->raw_data, len_off+1+raw_data[len_off]) != 0)
goto skip;
*off += raw_data[len_off];
s -= raw_data[len_off];
ret += raw_data[len_off];
err = 0;
}
skip:
kfree(resp);
if (err)
break;
}
return ret > 0 ? ret : err;
}
static ssize_t picolcd_debug_flash_write(struct file *f, const char __user *u,
size_t s, loff_t *off)
{
struct picolcd_data *data = f->private_data;
ssize_t err, ret = 0;
int report_erase, report_write;
if (s == 0)
return -EINVAL;
if (*off > 0x5fff)
return -ENOSPC;
if (s & 0x3f)
return -EINVAL;
if (*off & 0x3f)
return -EINVAL;
if (data->status & PICOLCD_BOOTLOADER) {
report_erase = REPORT_BL_ERASE_MEMORY;
report_write = REPORT_BL_WRITE_MEMORY;
} else {
report_erase = REPORT_ERASE_MEMORY;
report_write = REPORT_WRITE_MEMORY;
}
mutex_lock(&data->mutex_flash);
while (s > 0) {
err = _picolcd_flash_erase64(data, report_erase, off);
if (err)
break;
err = _picolcd_flash_write(data, report_write, u, 64, off);
if (err < 0)
break;
ret += err;
*off += err;
s -= err;
if (err != 64)
break;
}
mutex_unlock(&data->mutex_flash);
return ret > 0 ? ret : err;
}
static void dump_buff_as_hex(char *dst, size_t dst_sz, const u8 *data,
const size_t data_len)
{
int i, j;
for (i = j = 0; i < data_len && j + 4 < dst_sz; i++) {
dst[j++] = hex_asc[(data[i] >> 4) & 0x0f];
dst[j++] = hex_asc[data[i] & 0x0f];
dst[j++] = ' ';
}
dst[j] = '\0';
if (j > 0)
dst[j-1] = '\n';
if (i < data_len && j > 2)
dst[j-2] = dst[j-3] = '.';
}
void picolcd_debug_out_report(struct picolcd_data *data,
struct hid_device *hdev, struct hid_report *report)
{
u8 *raw_data;
int raw_size = (report->size >> 3) + 1;
char *buff;
#define BUFF_SZ 256
if (list_empty(&hdev->debug_list))
return;
buff = kmalloc(BUFF_SZ, GFP_ATOMIC);
if (!buff)
return;
raw_data = hid_alloc_report_buf(report, GFP_ATOMIC);
if (!raw_data) {
kfree(buff);
return;
}
snprintf(buff, BUFF_SZ, "\nout report %d (size %d) = ",
report->id, raw_size);
hid_debug_event(hdev, buff);
raw_data[0] = report->id;
hid_output_report(report, raw_data);
dump_buff_as_hex(buff, BUFF_SZ, raw_data, raw_size);
hid_debug_event(hdev, buff);
switch (report->id) {
case REPORT_LED_STATE:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_LED_STATE", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tGPO state: 0x%02x\n", raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_BRIGHTNESS:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_BRIGHTNESS", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tBrightness: 0x%02x\n", raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_CONTRAST:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_CONTRAST", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tContrast: 0x%02x\n", raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_RESET:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_RESET", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tDuration: 0x%02x%02x (%dms)\n",
raw_data[2], raw_data[1], raw_data[2] << 8 | raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_LCD_CMD:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_LCD_CMD", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_LCD_DATA:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_LCD_CMD", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_LCD_CMD_DATA:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_LCD_CMD", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_EE_READ:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_EE_READ", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
hid_debug_event(hdev, buff);
break;
case REPORT_EE_WRITE:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_EE_WRITE", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
hid_debug_event(hdev, buff);
if (raw_data[3] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
} else if (raw_data[3] + 4 <= raw_size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+4, raw_data[3]);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
}
hid_debug_event(hdev, buff);
break;
case REPORT_ERASE_MEMORY:
case REPORT_BL_ERASE_MEMORY:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_ERASE_MEMORY", report->id, raw_size-1);
hid_debug_event(hdev, buff);
switch (data->addr_sz) {
case 2:
snprintf(buff, BUFF_SZ, "\tAddress inside 64 byte block: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
break;
case 3:
snprintf(buff, BUFF_SZ, "\tAddress inside 64 byte block: 0x%02x%02x%02x\n",
raw_data[3], raw_data[2], raw_data[1]);
break;
default:
snprintf(buff, BUFF_SZ, "\tNot supported\n");
}
hid_debug_event(hdev, buff);
break;
case REPORT_READ_MEMORY:
case REPORT_BL_READ_MEMORY:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_READ_MEMORY", report->id, raw_size-1);
hid_debug_event(hdev, buff);
switch (data->addr_sz) {
case 2:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
break;
case 3:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x%02x\n",
raw_data[3], raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[4]);
break;
default:
snprintf(buff, BUFF_SZ, "\tNot supported\n");
}
hid_debug_event(hdev, buff);
break;
case REPORT_WRITE_MEMORY:
case REPORT_BL_WRITE_MEMORY:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_WRITE_MEMORY", report->id, raw_size-1);
hid_debug_event(hdev, buff);
switch (data->addr_sz) {
case 2:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
hid_debug_event(hdev, buff);
if (raw_data[3] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
} else if (raw_data[3] + 4 <= raw_size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+4, raw_data[3]);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
}
break;
case 3:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x%02x\n",
raw_data[3], raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[4]);
hid_debug_event(hdev, buff);
if (raw_data[4] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
} else if (raw_data[4] + 5 <= raw_size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+5, raw_data[4]);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
}
break;
default:
snprintf(buff, BUFF_SZ, "\tNot supported\n");
}
hid_debug_event(hdev, buff);
break;
case REPORT_SPLASH_RESTART:
break;
case REPORT_EXIT_KEYBOARD:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_EXIT_KEYBOARD", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tRestart delay: %dms (0x%02x%02x)\n",
raw_data[1] | (raw_data[2] << 8),
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_VERSION:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_VERSION", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_DEVID:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_DEVID", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_SPLASH_SIZE:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_SPLASH_SIZE", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_HOOK_VERSION:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_HOOK_VERSION", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_EXIT_FLASHER:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"REPORT_VERSION", report->id, raw_size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tRestart delay: %dms (0x%02x%02x)\n",
raw_data[1] | (raw_data[2] << 8),
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
break;
default:
snprintf(buff, BUFF_SZ, "out report %s (%d, size=%d)\n",
"<unknown>", report->id, raw_size-1);
hid_debug_event(hdev, buff);
break;
}
wake_up_interruptible(&hdev->debug_wait);
kfree(raw_data);
kfree(buff);
}
void picolcd_debug_raw_event(struct picolcd_data *data,
struct hid_device *hdev, struct hid_report *report,
u8 *raw_data, int size)
{
char *buff;
#define BUFF_SZ 256
if (list_empty(&hdev->debug_list))
return;
buff = kmalloc(BUFF_SZ, GFP_ATOMIC);
if (!buff)
return;
switch (report->id) {
case REPORT_ERROR_CODE:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_ERROR_CODE", report->id, size-1);
hid_debug_event(hdev, buff);
if (raw_data[2] < ARRAY_SIZE(error_codes))
snprintf(buff, BUFF_SZ, "\tError code 0x%02x (%s) in reply to report 0x%02x\n",
raw_data[2], error_codes[raw_data[2]], raw_data[1]);
else
snprintf(buff, BUFF_SZ, "\tError code 0x%02x in reply to report 0x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_KEY_STATE:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_KEY_STATE", report->id, size-1);
hid_debug_event(hdev, buff);
if (raw_data[1] == 0)
snprintf(buff, BUFF_SZ, "\tNo key pressed\n");
else if (raw_data[2] == 0)
snprintf(buff, BUFF_SZ, "\tOne key pressed: 0x%02x (%d)\n",
raw_data[1], raw_data[1]);
else
snprintf(buff, BUFF_SZ, "\tTwo keys pressed: 0x%02x (%d), 0x%02x (%d)\n",
raw_data[1], raw_data[1], raw_data[2], raw_data[2]);
hid_debug_event(hdev, buff);
break;
case REPORT_IR_DATA:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_IR_DATA", report->id, size-1);
hid_debug_event(hdev, buff);
if (raw_data[1] == 0) {
snprintf(buff, BUFF_SZ, "\tUnexpectedly 0 data length\n");
hid_debug_event(hdev, buff);
} else if (raw_data[1] + 1 <= size) {
snprintf(buff, BUFF_SZ, "\tData length: %d\n\tIR Data: ",
raw_data[1]);
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+2, raw_data[1]);
hid_debug_event(hdev, buff);
} else {
snprintf(buff, BUFF_SZ, "\tOverflowing data length: %d\n",
raw_data[1]-1);
hid_debug_event(hdev, buff);
}
break;
case REPORT_EE_DATA:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_EE_DATA", report->id, size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
hid_debug_event(hdev, buff);
if (raw_data[3] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
hid_debug_event(hdev, buff);
} else if (raw_data[3] + 4 <= size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+4, raw_data[3]);
hid_debug_event(hdev, buff);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
hid_debug_event(hdev, buff);
}
break;
case REPORT_MEMORY:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_MEMORY", report->id, size-1);
hid_debug_event(hdev, buff);
switch (data->addr_sz) {
case 2:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[3]);
hid_debug_event(hdev, buff);
if (raw_data[3] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
} else if (raw_data[3] + 4 <= size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+4, raw_data[3]);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
}
break;
case 3:
snprintf(buff, BUFF_SZ, "\tData address: 0x%02x%02x%02x\n",
raw_data[3], raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tData length: %d\n", raw_data[4]);
hid_debug_event(hdev, buff);
if (raw_data[4] == 0) {
snprintf(buff, BUFF_SZ, "\tNo data\n");
} else if (raw_data[4] + 5 <= size) {
snprintf(buff, BUFF_SZ, "\tData: ");
hid_debug_event(hdev, buff);
dump_buff_as_hex(buff, BUFF_SZ, raw_data+5, raw_data[4]);
} else {
snprintf(buff, BUFF_SZ, "\tData overflowed\n");
}
break;
default:
snprintf(buff, BUFF_SZ, "\tNot supported\n");
}
hid_debug_event(hdev, buff);
break;
case REPORT_VERSION:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_VERSION", report->id, size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tFirmware version: %d.%d\n",
raw_data[2], raw_data[1]);
hid_debug_event(hdev, buff);
break;
case REPORT_BL_ERASE_MEMORY:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_BL_ERASE_MEMORY", report->id, size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_BL_READ_MEMORY:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_BL_READ_MEMORY", report->id, size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_BL_WRITE_MEMORY:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_BL_WRITE_MEMORY", report->id, size-1);
hid_debug_event(hdev, buff);
break;
case REPORT_DEVID:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_DEVID", report->id, size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tSerial: 0x%02x%02x%02x%02x\n",
raw_data[1], raw_data[2], raw_data[3], raw_data[4]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tType: 0x%02x\n",
raw_data[5]);
hid_debug_event(hdev, buff);
break;
case REPORT_SPLASH_SIZE:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_SPLASH_SIZE", report->id, size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tTotal splash space: %d\n",
(raw_data[2] << 8) | raw_data[1]);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tUsed splash space: %d\n",
(raw_data[4] << 8) | raw_data[3]);
hid_debug_event(hdev, buff);
break;
case REPORT_HOOK_VERSION:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"REPORT_HOOK_VERSION", report->id, size-1);
hid_debug_event(hdev, buff);
snprintf(buff, BUFF_SZ, "\tFirmware version: %d.%d\n",
raw_data[1], raw_data[2]);
hid_debug_event(hdev, buff);
break;
default:
snprintf(buff, BUFF_SZ, "report %s (%d, size=%d)\n",
"<unknown>", report->id, size-1);
hid_debug_event(hdev, buff);
break;
}
wake_up_interruptible(&hdev->debug_wait);
kfree(buff);
}
void picolcd_init_devfs(struct picolcd_data *data,
struct hid_report *eeprom_r, struct hid_report *eeprom_w,
struct hid_report *flash_r, struct hid_report *flash_w,
struct hid_report *reset)
{
struct hid_device *hdev = data->hdev;
mutex_init(&data->mutex_flash);
if (reset)
data->debug_reset = debugfs_create_file("reset", 0600,
hdev->debug_dir, data, &picolcd_debug_reset_fops);
if (eeprom_r || eeprom_w)
data->debug_eeprom = debugfs_create_file("eeprom",
(eeprom_w ? S_IWUSR : 0) | (eeprom_r ? S_IRUSR : 0),
hdev->debug_dir, data, &picolcd_debug_eeprom_fops);
if (flash_r && flash_r->maxfield == 1 && flash_r->field[0]->report_size == 8)
data->addr_sz = flash_r->field[0]->report_count - 1;
else
data->addr_sz = -1;
if (data->addr_sz == 2 || data->addr_sz == 3) {
data->debug_flash = debugfs_create_file("flash",
(flash_w ? S_IWUSR : 0) | (flash_r ? S_IRUSR : 0),
hdev->debug_dir, data, &picolcd_debug_flash_fops);
} else if (flash_r || flash_w)
hid_warn(hdev, "Unexpected FLASH access reports, please submit rdesc for review\n");
}
void picolcd_exit_devfs(struct picolcd_data *data)
{
struct dentry *dent;
dent = data->debug_reset;
data->debug_reset = NULL;
debugfs_remove(dent);
dent = data->debug_eeprom;
data->debug_eeprom = NULL;
debugfs_remove(dent);
dent = data->debug_flash;
data->debug_flash = NULL;
debugfs_remove(dent);
mutex_destroy(&data->mutex_flash);
}
