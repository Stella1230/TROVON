int stk1160_read_reg(struct stk1160 *dev, u16 reg, u8 *value)
{
int ret;
int pipe = usb_rcvctrlpipe(dev->udev, 0);
u8 *buf;
*value = 0;
buf = kmalloc(sizeof(u8), GFP_KERNEL);
if (!buf)
return -ENOMEM;
ret = usb_control_msg(dev->udev, pipe, 0x00,
USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
0x00, reg, buf, sizeof(u8), HZ);
if (ret < 0) {
stk1160_err("read failed on reg 0x%x (%d)\n",
reg, ret);
kfree(buf);
return ret;
}
*value = *buf;
kfree(buf);
return 0;
}
int stk1160_write_reg(struct stk1160 *dev, u16 reg, u16 value)
{
int ret;
int pipe = usb_sndctrlpipe(dev->udev, 0);
ret = usb_control_msg(dev->udev, pipe, 0x01,
USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
value, reg, NULL, 0, HZ);
if (ret < 0) {
stk1160_err("write failed on reg 0x%x (%d)\n",
reg, ret);
return ret;
}
return 0;
}
void stk1160_select_input(struct stk1160 *dev)
{
int route;
static const u8 gctrl[] = {
0x98, 0x90, 0x88, 0x80, 0x98
};
if (dev->ctl_input == STK1160_SVIDEO_INPUT)
route = SAA7115_SVIDEO3;
else
route = SAA7115_COMPOSITE0;
if (dev->ctl_input < ARRAY_SIZE(gctrl)) {
v4l2_device_call_all(&dev->v4l2_dev, 0, video, s_routing,
route, 0, 0);
stk1160_write_reg(dev, STK1160_GCTRL, gctrl[dev->ctl_input]);
}
}
static void stk1160_reg_reset(struct stk1160 *dev)
{
int i;
static const struct regval ctl[] = {
{STK1160_GCTRL+2, 0x0078},
{STK1160_RMCTL+1, 0x0000},
{STK1160_RMCTL+3, 0x0002},
{STK1160_PLLSO, 0x0010},
{STK1160_PLLSO+1, 0x0000},
{STK1160_PLLSO+2, 0x0014},
{STK1160_PLLSO+3, 0x000E},
{STK1160_PLLFD, 0x0046},
{STK1160_TIGEN, 0x0012},
{STK1160_TICTL, 0x002D},
{STK1160_TICTL+1, 0x0001},
{STK1160_TICTL+2, 0x0000},
{STK1160_TICTL+3, 0x0000},
{STK1160_TIGEN, 0x0080},
{0xffff, 0xffff}
};
for (i = 0; ctl[i].reg != 0xffff; i++)
stk1160_write_reg(dev, ctl[i].reg, ctl[i].val);
}
static void stk1160_release(struct v4l2_device *v4l2_dev)
{
struct stk1160 *dev = container_of(v4l2_dev, struct stk1160, v4l2_dev);
stk1160_info("releasing all resources\n");
stk1160_i2c_unregister(dev);
v4l2_ctrl_handler_free(&dev->ctrl_handler);
v4l2_device_unregister(&dev->v4l2_dev);
kfree(dev->alt_max_pkt_size);
kfree(dev);
}
static int stk1160_scan_usb(struct usb_interface *intf, struct usb_device *udev,
unsigned int *max_pkt_size)
{
int i, e, sizedescr, size, ifnum;
const struct usb_endpoint_descriptor *desc;
bool has_video = false, has_audio = false;
const char *speed;
ifnum = intf->altsetting[0].desc.bInterfaceNumber;
for (i = 0; i < intf->num_altsetting; i++) {
for (e = 0; e < intf->altsetting[i].desc.bNumEndpoints; e++) {
desc = &intf->altsetting[i].endpoint[e].desc;
sizedescr = le16_to_cpu(desc->wMaxPacketSize);
size = sizedescr & 0x7ff;
if (udev->speed == USB_SPEED_HIGH)
size = size * hb_mult(sizedescr);
if (usb_endpoint_xfer_isoc(desc) &&
usb_endpoint_dir_in(desc)) {
switch (desc->bEndpointAddress) {
case STK1160_EP_AUDIO:
has_audio = true;
break;
case STK1160_EP_VIDEO:
has_video = true;
max_pkt_size[i] = size;
break;
}
}
}
}
if (!(has_audio || has_video)) {
dev_err(&udev->dev, "no audio or video endpoints found\n");
return -ENODEV;
}
switch (udev->speed) {
case USB_SPEED_LOW:
speed = "1.5";
break;
case USB_SPEED_FULL:
speed = "12";
break;
case USB_SPEED_HIGH:
speed = "480";
break;
default:
speed = "unknown";
}
dev_info(&udev->dev, "New device %s %s @ %s Mbps (%04x:%04x, interface %d, class %d)\n",
udev->manufacturer ? udev->manufacturer : "",
udev->product ? udev->product : "",
speed,
le16_to_cpu(udev->descriptor.idVendor),
le16_to_cpu(udev->descriptor.idProduct),
ifnum,
intf->altsetting->desc.bInterfaceNumber);
if (has_audio)
dev_warn(&udev->dev, "audio interface %d found.\n\
This is not implemented by this driver,\
you should use snd-usb-audio instead\n", ifnum);
if (has_video)
dev_info(&udev->dev, "video interface %d found\n",
ifnum);
if (udev->speed != USB_SPEED_HIGH)
dev_warn(&udev->dev, "must be connected to a high-speed USB 2.0 port\n\
You may not be able to stream video smoothly\n");
return 0;
}
static int stk1160_probe(struct usb_interface *interface,
const struct usb_device_id *id)
{
int rc = 0;
unsigned int *alt_max_pkt_size;
struct usb_device *udev;
struct stk1160 *dev;
udev = interface_to_usbdev(interface);
if (interface->altsetting[0].desc.bInterfaceClass == USB_CLASS_AUDIO)
return -ENODEV;
alt_max_pkt_size = kmalloc(sizeof(alt_max_pkt_size[0]) *
interface->num_altsetting, GFP_KERNEL);
if (alt_max_pkt_size == NULL)
return -ENOMEM;
rc = stk1160_scan_usb(interface, udev, alt_max_pkt_size);
if (rc < 0) {
kfree(alt_max_pkt_size);
return rc;
}
dev = kzalloc(sizeof(struct stk1160), GFP_KERNEL);
if (dev == NULL) {
kfree(alt_max_pkt_size);
return -ENOMEM;
}
dev->alt_max_pkt_size = alt_max_pkt_size;
dev->udev = udev;
dev->num_alt = interface->num_altsetting;
dev->ctl_input = input;
dev->dev = &interface->dev;
usb_set_intfdata(interface, dev);
rc = stk1160_vb2_setup(dev);
if (rc < 0)
goto free_err;
spin_lock_init(&dev->buf_lock);
mutex_init(&dev->v4l_lock);
mutex_init(&dev->vb_queue_lock);
rc = v4l2_ctrl_handler_init(&dev->ctrl_handler, 0);
if (rc) {
stk1160_err("v4l2_ctrl_handler_init failed (%d)\n", rc);
goto free_err;
}
dev->v4l2_dev.release = stk1160_release;
dev->v4l2_dev.ctrl_handler = &dev->ctrl_handler;
rc = v4l2_device_register(dev->dev, &dev->v4l2_dev);
if (rc) {
stk1160_err("v4l2_device_register failed (%d)\n", rc);
goto free_ctrl;
}
rc = stk1160_i2c_register(dev);
if (rc < 0)
goto unreg_v4l2;
dev->sd_saa7115 = v4l2_i2c_new_subdev(&dev->v4l2_dev, &dev->i2c_adap,
"saa7115_auto", 0, saa7113_addrs);
stk1160_info("driver ver %s successfully loaded\n",
STK1160_VERSION);
v4l2_device_call_all(&dev->v4l2_dev, 0, core, reset, 0);
v4l2_device_call_all(&dev->v4l2_dev, 0, video, s_stream, 0);
stk1160_reg_reset(dev);
stk1160_select_input(dev);
stk1160_ac97_register(dev);
rc = stk1160_video_register(dev);
if (rc < 0)
goto unreg_i2c;
return 0;
unreg_i2c:
stk1160_i2c_unregister(dev);
unreg_v4l2:
v4l2_device_unregister(&dev->v4l2_dev);
free_ctrl:
v4l2_ctrl_handler_free(&dev->ctrl_handler);
free_err:
kfree(alt_max_pkt_size);
kfree(dev);
return rc;
}
static void stk1160_disconnect(struct usb_interface *interface)
{
struct stk1160 *dev;
dev = usb_get_intfdata(interface);
usb_set_intfdata(interface, NULL);
mutex_lock(&dev->vb_queue_lock);
mutex_lock(&dev->v4l_lock);
stk1160_uninit_isoc(dev);
stk1160_ac97_unregister(dev);
stk1160_clear_queue(dev);
video_unregister_device(&dev->vdev);
v4l2_device_disconnect(&dev->v4l2_dev);
dev->udev = NULL;
mutex_unlock(&dev->v4l_lock);
mutex_unlock(&dev->vb_queue_lock);
v4l2_device_put(&dev->v4l2_dev);
}
