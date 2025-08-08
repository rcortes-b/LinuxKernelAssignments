#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Hello word assignment!");
MODULE_VERSION("0.01");

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
