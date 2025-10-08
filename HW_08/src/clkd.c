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

#define DEVICE_IOCTL_MAGIC 'v'

#define GET_VERSION _IOR(DEVICE_IOCTL_MAGIC, 0, int)
/*#define SET_THRESHOLD _IOW(MYDEV_MAGIC, 1, int)
#define CALIBRATE _IOWR(MYDEV_MAGIC, 2, struct
calibration)
#define RESET_DEVICE _IO(MYDEV_MAGIC, 3)*/

// minor number
int clkd_minor = 0;

// major number
int clkd_major = 0;

// count of devices
int clkd_nr_devs = 1;

// radix 
int radix = 16;

static dev_t clkd_dev = 0;
static struct cdev clkd_cdev;
static struct class *clkd_class;
static struct device *clkd_device;
static DEFINE_MUTEX(device_mutex);

struct DeviceBuffer {
    char *buf;

    size_t size;
};

static struct DeviceBuffer *alloc_buf(size_t buf_size, int pages)
{
	struct DeviceBuffer *device_buf;
	device_buf = kmalloc(sizeof(struct DeviceBuffer), GFP_KERNEL);
	if (!device_buf)
		return NULL;

	device_buf->buf = kmalloc(buf_size, GFP_KERNEL);
	if (!device_buf->buf)
	{
		kfree(device_buf);
		return NULL;
	}

	device_buf->size = buf_size;
	return device_buf;
}

ssize_t clkd_read(struct file *file, char __user *ubuf, size_t size, loff_t *offset)
{
	pr_info("Read %d\n", size);

	int bytes_read = 0;
    struct DeviceBuffer* device_buf = file->private_data;

    if (!device_buf)
	{
		pr_err("Failed read no device data\n");
		return -EFAULT;
	}

	if (!device_buf->buf)
	{
		pr_err("Failed read no device buffer\n");
		return -EFAULT;
	}

	if (*offset >= device_buf->size)
		return 0; // EOF

	size_t to_read = min(size, device_buf->size - (size_t)*offset);
	if (copy_to_user(ubuf, device_buf->buf + *offset, to_read))
	{
		pr_err("Failed copy to user\n");
		return -EFAULT;
	}

	*offset += to_read;

	pr_info(KERN_INFO "Read %zu bytes\n", to_read);
	return to_read;
}

ssize_t clkd_write(struct file *file, const char __user *ubuf, size_t size, loff_t *off)
{
	pr_info("Write %d\n", size);

	struct DeviceBuffer* device_buf = file->private_data;
	if (!device_buf)
	{
		pr_err("Failed write no device data\n");
		return -EFAULT;
	}

	if (!size)
	{
		pr_err("Failed try to read 0 bytes\n");
		return 0;
	}

	size_t buf_size = min((size_t)(32 * 1024), size);

	//cpu = strtol(arg, &endptr, radix);

	device_buf->buf = kmalloc(buf_size, GFP_KERNEL);
	if (!device_buf->buf)
	{
		pr_err("Failed try allocate write buffer\n");
		return -ENOMEM;
	}

	device_buf->size = buf_size;

	if (copy_from_user(device_buf->buf, ubuf, buf_size)) 
	{
		kfree(device_buf->buf);
		pr_err("Failed to copy user data\n");
		return -EFAULT;
	}

	pr_info(KERN_INFO "Write %zu bytes\n", buf_size);
    return buf_size; 
}

long clkd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_info("IOCTL %d\n", cmd);

	if (_IOC_TYPE(cmd) != DEVICE_IOCTL_MAGIC)
		return -ENOTTY;

	if (_IOC_NR(cmd) > 3)
		return -ENOTTY;

	switch (cmd) 
	{
	case GET_VERSION:
		int version = 1;
		if (copy_to_user((int __user *)arg, &version, sizeof(version)))
			return -EFAULT;
		break;
	};

	return 0;
}

static int clkd_open(struct inode * inode, struct file * file)
{
	struct DeviceBuffer* device_buff;

	if (!mutex_trylock(&device_mutex)) 
	{
		pr_err("Device is busy\n");
		return -EBUSY;
	}

    if(!try_module_get(THIS_MODULE)) 
    {
        pr_err("Failed increment module refcount\n");
        return -EINVAL;
    }

    device_buff = kmalloc(sizeof(struct DeviceBuffer), GFP_KERNEL);
    if(!device_buff) 
    {
        pr_err("Failed allocate memory for private data\n");
        module_put(THIS_MODULE);
        return -EINVAL;
    }

	device_buff->size = 0;
    file->private_data = device_buff;

    pr_info("Device opened: %p - %p\n", inode, file);
    return 0;
}

static int clkd_release(struct inode * inode, struct file * filp)
{
	struct DeviceBuffer* device_buff = filp->private_data;
	if(!device_buff) 
    {
        pr_err("Failed allocate memory for private data\n");
        module_put(THIS_MODULE);
        return -EINVAL;
    }

    kfree(device_buff->buf);
	kfree(device_buff);
    module_put(THIS_MODULE);

    mutex_unlock(&device_mutex);
    pr_info("Device released: %p - %p\n", inode, filp);
    return 0;
}

struct file_operations clkd_fops = {
	.owner = THIS_MODULE,
	.read = clkd_read,
    .write = clkd_write,
    .unlocked_ioctl = clkd_ioctl,
    .open = clkd_open,
    .release = clkd_release,
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

	// need register in sysfs/procfs

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