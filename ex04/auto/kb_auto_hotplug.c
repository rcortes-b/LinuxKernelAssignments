#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Load when a Keyboard is Plugged In");
MODULE_VERSION("0.01");

static struct usb_device_id usb_keyboard_id[] = {
	{ USB_INTERFACE_INFO(
		USB_INTERFACE_CLASS_HID,
		USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }	
};

MODULE_DEVICE_TABLE(usb, usb_keyboard_id);

static int kb_connect(struct usb_interface *intf, const struct usb_device_id *id)
{
	pr_info("Keyboard connected\n");
	printk(KERN_ALERT "Keyboard connected\n");
	return 0;
}

static void kb_disconnect(struct usb_interface *intf)
{
	pr_info("Keyboard disconnected\n");
}

static struct usb_driver kb_driver = {
	.name = "keyboard_driver",
	.probe = kb_connect,
	.disconnect = kb_disconnect,
	.id_table = usb_keyboard_id,
};

static int __init kb_init(void)
{
	printk(KERN_ALERT "HOLA, IS THIS WORKING?\n");
	int result = usb_register(&kb_driver);
	if (result < 0) {
		printk(KERN_ALERT "usb reg failed\n");
		pr_err("usb_register failed\n");
		return -1;
	}
	return 0;
}

static void __exit kb_exit(void)
{
	usb_deregister(&kb_driver);
}

module_init(kb_init);
module_exit(kb_exit);
