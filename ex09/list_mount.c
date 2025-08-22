#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Creating a custom /proc/mounts");
MODULE_VERSION("0.01");

#define PROC_FILE_NAME "mymounts"

static struct proc_dir_entry *proc_interface;

static ssize_t read_operation(struct file *f, char __user *buff, size_t len, loff_t *ppos)
{
	return 0;
}

static const struct proc_ops fops = {
	.proc_read = read_operation,
};

static int __init my_init(void)
{
	proc_interface = proc_create(PROC_FILE_NAME, 0444, NULL, &fops);
	if (!proc_interface) {
		pr_err("Error creating the proc interface.\n");
		return 1;
	}
	return 0;
}

static void __exit my_cleanup(void)
{
	remove_proc_entry(PROC_FILE_NAME, NULL);
}

module_init(my_init);
module_exit(my_cleanup);
