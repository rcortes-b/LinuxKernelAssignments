#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Debugfs, a way of interaction between kernel and user!");
MODULE_VERSION("0.01");

#define DIR_NAME "fortytwo"
#define ID_FILE_NAME "id"
#define JIFFIES_FILE_NAME "jiffies"
#define FOO_FILE_NAME "foo"

#define LOGIN_LEN 8
static char id_data[LOGIN_LEN];
static char foo_data[PAGE_SIZE];
static size_t foo_data_len = 0;

static struct dentry *parent_dir;

static ssize_t id_read_operation(struct file *f, char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("id debugfs: read operation has been called.\n");
	size_t data_len = len < LOGIN_LEN - *ppos ? len : LOGIN_LEN - *ppos;
	int status = copy_to_user(buff, "rcortes-" + *ppos, data_len);
	if (status) {
		pr_err("id debugfs: Error copying data to user.\n");
		return -EFAULT;
	}
	*ppos += data_len;
	return data_len;
}

static ssize_t id_write_operation(struct file *f, const char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("id debugfs: write operation has been called.\n");
	if (len != LOGIN_LEN || strncmp(buff, "rcortes-", len) != 0) {
		pr_err("id debugfs: Invalid value\n");
		return -EINVAL;
	}
	int status = copy_from_user(id_data, buff, len);
	if (status) {
		pr_err("id debugfs: Error copying data from user.\n");
		return -EFAULT;
	}	
	return len;
}

static ssize_t jiffies_read_operation(struct file *f, char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("jiffies debugfs: read operation has been called.\n");
	char jiffies_buffer[23];
	size_t jiffies_len = snprintf(jiffies_buffer, sizeof(jiffies_buffer), "%lu\n", jiffies);
	jiffies_len = len < jiffies_len - *ppos ? len : jiffies_len - *ppos;
	int status = copy_to_user(buff, jiffies_buffer + *ppos, jiffies_len);
	if (status) {
		pr_err("jiffies debugfs: Error copying data to user.\n");
		return -EFAULT;
	}
	*ppos += jiffies_len;
	return jiffies_len;
}

static ssize_t foo_read_operation(struct file *f, char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("foo debugfs: read operation has been called.\n");
	size_t data_len = len < foo_data_len - *ppos ? len : foo_data_len - *ppos;
	int status = copy_to_user(buff, foo_data + *ppos, data_len);
	if (status) {
		pr_err("foo debugfs: Error copying data to user.\n");
		return -EFAULT;
	}
	*ppos += data_len;
	return data_len;
}

static ssize_t foo_write_operation(struct file *f, const char __user *buff, size_t len, loff_t *ppos)
{
	pr_info("foo debugfs: write operation has been called.\n");
	if (len > PAGE_SIZE)
		foo_data_len = PAGE_SIZE;
	else
		foo_data_len = len;
	int status = copy_from_user(foo_data, buff, foo_data_len);
	if (status) {
		pr_err("foo debugfs: Error copying data from user.\n");
		return -EFAULT;
	}
	return foo_data_len;
}

static struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read_operation,
	.write = id_write_operation,
};
static struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_read_operation,
};
static struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read_operation,
	.write = foo_write_operation,
};

static int __init hello_init(void)
{
	printk(KERN_INFO "Init function called!\n");
	parent_dir = debugfs_create_dir(DIR_NAME, NULL);
	if (!parent_dir) {
		pr_err("Error creating the \'fortytwo\' directory!\n");
		return -ENODEV;
	}
	if(!debugfs_create_file(ID_FILE_NAME, 0666, parent_dir, NULL, &id_fops)) {
		pr_err("Error creating the \'id\' file!\n");
		return -ENODEV;
	}
	if (!debugfs_create_file(JIFFIES_FILE_NAME, 0444, parent_dir, NULL, &jiffies_fops)) {
		pr_err("Error creating the \'jiffies\' file!\n");
		return -ENODEV;
	}
	if (!debugfs_create_file(FOO_FILE_NAME, 0644, parent_dir, NULL, &foo_fops)) {
		pr_err("Error creating the \'foo\' file!\n");
		return -ENODEV;
	}
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	debugfs_remove(parent_dir);
}

module_init(hello_init);
module_exit(hello_exit);
