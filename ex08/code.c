#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = myfd_read,
	.write = myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops,
};

static char str[PAGE_SIZE];

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t i, j;
	char *tmp;

	if (!strlen(str))
		return 0;
	tmp = kmalloc(sizeof(char) * (strlen(str) + 1), GFP_KERNEL);
	if (!tmp) {
		pr_info("Error allocating memory with malloc.\n");
		return -EFAULT;
	}
	for (j = strlen(str) - 1, i = 0; j > 0; j--, i++)
		tmp[i] = str[j];
	tmp[i++] = str[j++];
	tmp[i] = 0x0;
	int res = simple_read_from_buffer((void*)user, size, offs, tmp, i);
	kfree(tmp);
	return res;
}

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res = 0;
	res = simple_write_to_buffer(str, size, offs, user, size);
	/*
	 * 0x0 = '\0'
	 */
	if (res >= 0)
		str[res++] = 0x0;
	return res;
}

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	return retval;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);
