#include "defs.h"
#include "procfs_operations.h"
#include "glob_operations.h"

#include <linux/proc_fs.h>

int proc_open(struct inode *inode, struct file *file);
int proc_release(struct inode *inode, struct file *file);
ssize_t proc_write(struct file *file, const char __user *buf, size_t count,
		   loff_t *ppos);

static struct proc_dir_entry *proc_dir = NULL;
static struct proc_dir_entry *proc_file = NULL;

static const struct proc_ops proc_fops = {
	.proc_open = proc_open,
	.proc_write = proc_write,
	.proc_release = proc_release,
};

int proc_open(struct inode *inode, struct file *file)
{
	pr_info("Procfs file opened\n");
	return 0;
}

int proc_release(struct inode *inode, struct file *file)
{
	pr_info("Procfs file released\n");
	return 0;
}

ssize_t proc_write(struct file *file, const char __user *ubuf, size_t size,
		   loff_t *ppos)
{
	pr_info("Procfs file write data\n");

	char *input_buffer = kmalloc(size, GFP_KERNEL);
	if (!input_buffer) {
		pr_err("[FAIL] cannot allocate buffer to copy user data to procfs write\n");
		return -ENOMEM;
	}

	if (copy_from_user(input_buffer, ubuf, size)) {
		kfree(input_buffer);
		pr_err("[FAIL] cannot copy user data to procfs write\n");
		return -EFAULT;
	}

	input_buffer[size] = '\0';

	int reset = 0;
	int res = kstrtoint(input_buffer, 10, &reset);
	if (res) {
		pr_err("[FAIL] cannot convert reset parameter value %d!\n",
		       res);
		return -EINVAL;
	}

	kfree(input_buffer);

	if (reset != 1) {
		pr_err("[FAIL] Invalid parameters to reset data\n");
		return -EINVAL;
	}
	reset_global_buffers();
	return size;
}

int register_procfs(void)
{
	pr_info("Registering procfs file\n");
	proc_dir = proc_mkdir(PROC_DEVICE_DIR_NAME, NULL);
	if (!proc_dir) {
		pr_err("[FAIL] Failed to create proc directory\n");
		return -ENOMEM;
	}

	proc_file =
		proc_create(PROC_RESET_FILE_NAME, 0666, proc_dir, &proc_fops);
	if (!proc_file) {
		remove_proc_entry(PROC_DEVICE_DIR_NAME, NULL);
		pr_err("[FAIL] Failed to create proc file\n");
		return -ENOMEM;
	}

	pr_info("Proc file created: /proc/%s/%s\n", PROC_DEVICE_DIR_NAME,
		PROC_RESET_FILE_NAME);
	return 0;
}

void unregister_procfs(void)
{
	pr_info("Unregister procfs file\n");
	if (proc_file) {
		remove_proc_entry(PROC_RESET_FILE_NAME, proc_dir);
	}

	if (proc_dir) {
		remove_proc_entry(PROC_DEVICE_DIR_NAME, NULL);
	}

	pr_info("Unregistered procfs file\n");
}