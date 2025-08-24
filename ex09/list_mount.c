#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/mnt_namespace.h>
#include <linux/nsproxy.h>
#include <linux/sched.h>
#include <../fs/mount.h>
#include <linux/list.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rcortes-");
MODULE_DESCRIPTION("Creating a custom /proc/mounts");
MODULE_VERSION("0.01");

#define PROC_FILE_NAME "mymounts"

static struct proc_dir_entry *proc_interface;

static int mount_info_display(struct seq_file *m, void *v)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *mnt;
	struct mount *child;

	if (ns->root)
		mnt = ns->root;
	else
		return 1;
	int counter = 0;
	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
		seq_printf(m, "%s\t%s\n", child->mnt_mountpoint->d_name.name, child->mnt.mnt_sb->s_type->name);
		mnt = child;
		printk("Counter: %d\n", counter++);
	};
	return 0;
}

static int mount_info_open(struct inode *inode, struct file *f)
{
	return single_open(f, mount_info_display, NULL);
}

static const struct proc_ops fops = {
	.proc_open = mount_info_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
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
