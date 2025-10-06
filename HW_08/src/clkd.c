#define pr_fmt(fmt) "clkd: " fmt

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


#define DEVICE_NAME "clkd"
#define CLASS_NAME "clkd"

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

ssize_t clkd_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
	pr_info("Read %d\n", size);
    return 0; 
}

ssize_t clkd_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
	pr_info("Write %d\n", size);
    return 0; 
}

long clkd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_info("IOCTL %d\n", cmd);
	return 0;
}

struct file_operations clkd_fops = {
	.owner = THIS_MODULE,
	.read = clkd_read,
    .write = clkd_write,
    .unlocked_ioctl = clkd_ioctl,
};

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	int result;
	
	result = alloc_chrdev_region(&clkd_dev, clkd_minor, clkd_nr_devs, DEVICE_NAME);
	if(result < 0)
	{
		pr_err("Failed to allocate char dev region: %d\n", result);
		return result;
	}

	clkd_major = MAJOR(clkd_dev);
	pr_info("Allocated device maj: %d min: %d\n", clkd_major, clkd_minor);

	cdev_init(&clkd_cdev, &clkd_fops);
	clkd_cdev.owner = THIS_MODULE;

	result = cdev_add(&clkd_cdev, clkd_dev, 1);
	if (result < 0) 
	{
		pr_err("Failed to add device: %d\n", result);
		unregister_chrdev_region(clkd_dev, clkd_nr_devs);
		return result;
	}

	pr_info("Device added maj: %d min: %d\n", clkd_major, clkd_minor);

	clkd_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(clkd_class)) 
	{
		pr_err("Failed to create device class\n");
		cdev_del(&clkd_cdev);
		unregister_chrdev_region(clkd_dev, 1);
		return PTR_ERR(clkd_class);
	}

	pr_info("Device class created maj: %d min: %d\n", clkd_major, clkd_minor);

	clkd_device = device_create(clkd_class, NULL, clkd_dev, NULL, DEVICE_NAME);
	if (IS_ERR(clkd_device)) 
	{
		pr_err("Failed to create device\n");
		class_destroy(clkd_class);
		cdev_del(&clkd_cdev);
		unregister_chrdev_region(clkd_dev, 1);
		return PTR_ERR(clkd_device);
	} 

	pr_info("Device created maj: %d min: %d\n", clkd_major, clkd_minor);

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

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