#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Miscellaneous device assignment!");
MODULE_VERSION("0.01");

#define LOGIN_LEN 8
static char data[LOGIN_LEN];


static ssize_t read_operation(struct file *f, char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("misc device: read operation has been called.\n");
	size_t data_len = len < LOGIN_LEN - *ppos ? len : LOGIN_LEN - *ppos;
	int status = copy_to_user(buff, data + *ppos, data_len);
	if (status) {
		pr_err("misc device: Error copying data to user.\n");
		return -EFAULT;
	}
	*ppos += data_len;
	return data_len;
}

static ssize_t write_operation(struct file *f, const char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("misc device: write operation has been called.\n");
	if (len != LOGIN_LEN || strncmp(buff, "rcortes-", len) != 0) {
		pr_err("misc device: Invalid value\n");
		return -EINVAL;
	}
	int status = copy_from_user(data, buff, len);
	if (status) {
		pr_err("misc device: Error copying data from user.\n");
		return -EFAULT;
	}	
	return len;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = read_operation,
	.write = write_operation,
};

static struct miscdevice misc = {
	.name = "fortytwo",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

static int __init hello_init(void)
{
	printk(KERN_INFO "misc device: Registering the miscellaneous device.\n");
	int status = misc_register(&misc);
	if (status != 0) {
		pr_err("misc device: Error registering the miscellaneous device.\n");
		return 1;
	}
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "misc device: Deregistering the miscellaneous device.\n");
	misc_deregister(&misc);
}

module_init(hello_init);
module_exit(hello_exit);
