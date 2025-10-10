#define pr_fmt(fmt) "clkd: " fmt

#include "defs.h"
#include "process_operations.h"
#include "glob_operations.h"

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>

mode_t processing_mode = MODE_ENCODE;
int device_mode = MODE_PER_PROCESS_DATA;

DEFINE_MUTEX(clkd_device_lock);

// minor number
int clkd_minor = 0;

// major number
int clkd_major = 0;

// count of devices
int clkd_nr_devs = 1;

static dev_t clkd_dev = 0;
static struct cdev clkd_cdev;
static struct class *clkd_class;
static struct device *clkd_device;

// Params configuration
int mode_get(char *buffer, const struct kernel_param *kp)
{
	mutex_lock(&clkd_device_lock);
	int result = param_get_int(buffer, kp);
	mutex_unlock(&clkd_device_lock);

	return result;
}

int mode_set(const char *val, const struct kernel_param *kp)
{
	int res;
	int mode;

	res = kstrtoint(val, 10, &mode);
	if (res) {
		pr_err("[FAIL] invalid mode parameter value %d!\n", res);
		return -EINVAL;
	}

	if (mode != MODE_ENCODE && mode != MODE_DECODE) {
		pr_err("[FAIL] mode value must be between %d or %d!\n",
		       MODE_ENCODE, MODE_DECODE);
		return -EINVAL;
	}

	mutex_lock(&clkd_device_lock);
	processing_mode = mode;
	mutex_unlock(&clkd_device_lock);

	pr_info("mode changed to: %d\n", mode);
	return 0;
}

static const struct kernel_param_ops processing_mode_ops = {
	.set = mode_set,
	.get = mode_get,
};

module_param_cb(processing_mode, &processing_mode_ops, &processing_mode, 0644);
MODULE_PARM_DESC(processing_mode, "Processing data mode: 0=Encode, 1=Decode");

// Parameter declarations (read-only)
module_param(device_mode, int, 0444);
MODULE_PARM_DESC(device_mode,
		 "Global run mode: 0=global, 1=per_process (read-only)");

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing in mode: %d\n", device_mode);

	int result;

	result = alloc_chrdev_region(&clkd_dev, clkd_minor, clkd_nr_devs,
				     DEVICE_NAME);
	if (result < 0) {
		pr_err("Failed to allocate char dev region: %d\n", result);
		return result;
	}

	clkd_major = MAJOR(clkd_dev);
	pr_info("Allocated device maj: %d min: %d\n", clkd_major, clkd_minor);

	const struct file_operations *device_fops =
		(device_mode == MODE_PER_PROCESS_DATA) ? per_process_fops() :
							 global_fops();
	cdev_init(&clkd_cdev, device_fops);
	clkd_cdev.owner = THIS_MODULE;

	result = cdev_add(&clkd_cdev, clkd_dev, 1);
	if (result < 0) {
		pr_err("Failed to add device: %d\n", result);
		unregister_chrdev_region(clkd_dev, clkd_nr_devs);
		return result;
	}

	pr_info("Device added maj: %d min: %d\n", clkd_major, clkd_minor);

	clkd_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(clkd_class)) {
		pr_err("Failed to create device class\n");
		cdev_del(&clkd_cdev);
		unregister_chrdev_region(clkd_dev, 1);
		return PTR_ERR(clkd_class);
	}

	pr_info("Device class created maj: %d min: %d\n", clkd_major,
		clkd_minor);

	clkd_device =
		device_create(clkd_class, NULL, clkd_dev, NULL, DEVICE_NAME);
	if (IS_ERR(clkd_device)) {
		pr_err("Failed to create device\n");
		class_destroy(clkd_class);
		cdev_del(&clkd_cdev);
		unregister_chrdev_region(clkd_dev, 1);
		return PTR_ERR(clkd_device);
	}

	pr_info("Device created maj: %d min: %d\n", clkd_major, clkd_minor);

	// need register in sysfs/procfs

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	reset_global_buffers();

	device_destroy(clkd_class, clkd_dev);
	class_destroy(clkd_class);
	cdev_del(&clkd_cdev);
	unregister_chrdev_region(clkd_dev, clkd_nr_devs);

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 08 symbolic device");