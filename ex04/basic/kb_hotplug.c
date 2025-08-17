#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Load when a Keyboard is Plugged In");
MODULE_VERSION("0.01");

static int __init kb_init(void)
{
	printk(KERN_INFO "Keyboard has been connecteddddd!\n");
	return 0;
}

static void __exit kb_exit(void)
{
	printk(KERN_INFO "Module clean uppppppp!\n");
}

module_init(kb_init);
module_exit(kb_exit);
