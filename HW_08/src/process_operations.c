#include "defs.h"
#include "base64.h"
#include "process_operations.h"

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kernel.h>

// set mode encode or decode and reset buffers
#define SET_MODE _IOW(DEVICE_IOCTL_MAGIC, 1, int)

// return current mode (encode or decode)
#define GET_MODE _IOR(DEVICE_IOCTL_MAGIC, 2, int)

ssize_t clkd_read(struct file *file, char __user *ubuf, size_t size,
		  loff_t *offset);
ssize_t clkd_write(struct file *file, const char __user *ubuf, size_t size,
		   loff_t *off);
long clkd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

int clkd_open(struct inode *inode, struct file *file);
int clkd_release(struct inode *inode, struct file *filp);

struct DeviceBuffer {
	// data buffer
	char *buffer;

	// buffer size
	size_t size;

	// encode or decode
	// default value from module parameter
	processing_mode_t mode;

	// mutex to lock data
	struct mutex lock;
};

static struct file_operations clkd_per_process_fops = {
	.owner = THIS_MODULE,
	.read = clkd_read,
	.write = clkd_write,
	.unlocked_ioctl = clkd_ioctl,
	.open = clkd_open,
	.release = clkd_release,
};

const struct file_operations *per_process_fops(void)
{
	return &clkd_per_process_fops;
}

ssize_t clkd_read(struct file *file, char __user *ubuf, size_t size,
		  loff_t *offset)
{
	size_t bytes_to_read = 0;

	pr_info("Read %zu bytes, offset %lld\n", size, *offset);

	struct DeviceBuffer *pd = file->private_data;
	if (!pd || !pd->buffer) {
		pr_warn("[FAIL] cannot get device data to read\n");
		return -ENODEV;
	}

	mutex_lock(&pd->lock);

	if (*offset >= pd->size) {
		mutex_unlock(&pd->lock);
		pr_warn("Buffer more than data\n");
		return 0;
	}

	bytes_to_read = min(size, pd->size - *offset);

	if (copy_to_user(ubuf, pd->buffer + *offset, bytes_to_read)) {
		mutex_unlock(&pd->lock);
		pr_warn("[FAIL] cannot copy data to user for read\n");
		return -EFAULT;
	}

	*offset += bytes_to_read;

	mutex_unlock(&pd->lock);
	return bytes_to_read;
}

ssize_t clkd_write(struct file *file, const char __user *ubuf, size_t size,
		   loff_t *off)
{
	pr_info("Write %zu\n", size);

	struct DeviceBuffer *pd = file->private_data;
	if (!pd) {
		pr_err("[FAIL] cannot get device data to write\n");
		return -ENODEV;
	}

	if (size == 0) {
		pr_warn("[FAIL] no read read 0 bytes\n");
		return -EINVAL;
	}

	mutex_lock(&pd->lock);

	// Free previous buffers
	kfree(pd->buffer);
	pd->buffer = NULL;
	pd->size = 0;

	// Allocate input buffer
	char *input_buffer = kmalloc(size, GFP_KERNEL);
	if (!input_buffer) {
		mutex_unlock(&pd->lock);
		pr_err("[FAIL] cannot allocate buffer to copy user data to write\n");
		return -ENOMEM;
	}

	// Copy data from userspace
	if (copy_from_user(input_buffer, ubuf, size)) {
		kfree(input_buffer);
		mutex_unlock(&pd->lock);

		pr_err("[FAIL] cannot copy user data to write\n");
		return -EFAULT;
	}

	// Processing data
	switch (pd->mode) {
	case MODE_ENCODE: {
		// calculate data
		int output_size = base64_encode((unsigned char *)input_buffer,
						size, NULL, 0);

		pr_info("Calculated encoded %d bytes\n", output_size);

		if (output_size < 0) {
			kfree(input_buffer);
			mutex_unlock(&pd->lock);

			pr_err("[FAIL] cannot calculate encoded buffer size\n");
			return -EFAULT;
		}

		char *output_buffer = kmalloc(output_size, GFP_KERNEL);
		if (!output_buffer) {
			pr_err("[FAIL] cannot allocate memory for encoded buffer\n");
			kfree(input_buffer);
			mutex_unlock(&pd->lock);
			return -ENODEV;
		}

		int result_size = base64_encode((unsigned char *)input_buffer,
						size, output_buffer,
						output_size);

		if (result_size < 0) {
			kfree(input_buffer);
			kfree(output_buffer);
			mutex_unlock(&pd->lock);

			pr_err("[FAIL] cannot calculate encoded buffer size\n");
			return -EFAULT;
		}

		pd->buffer = output_buffer;
		pd->size = result_size;

		kfree(input_buffer);
		mutex_unlock(&pd->lock);
		pr_info("Encoded %zu bytes to %d bytes\n", size, result_size);

		return size;
	}

	break;

	case MODE_DECODE: {
		int output_size = base64_decode(input_buffer, size, NULL, 0);

		pr_info("Calculated decoded %d bytes\n", output_size);

		if (output_size < 0) {
			kfree(input_buffer);
			mutex_unlock(&pd->lock);

			pr_err("[FAIL] cannot calculate decoded buffer size\n");
			return -EFAULT;
		}

		char *output_buffer = kmalloc(output_size, GFP_KERNEL);
		if (!output_buffer) {
			pr_err("[FAIL] cannot allocate memory for decoded buffer\n");
			kfree(input_buffer);
			mutex_unlock(&pd->lock);
			return -ENODEV;
		}

		int result_size = base64_decode(input_buffer, size,
						(unsigned char *)output_buffer,
						output_size);

		if (result_size < 0) {
			kfree(input_buffer);
			kfree(output_buffer);
			mutex_unlock(&pd->lock);

			pr_err("[FAIL] cannot calculate decoded buffer size\n");
			return -EFAULT;
		}

		pd->buffer = output_buffer;
		pd->size = result_size;

		kfree(input_buffer);
		mutex_unlock(&pd->lock);
		pr_info("Decoded %zu bytes to %d bytes\n", size, result_size);

		return size;
	}

	break;

	default:
		return -EINVAL;
		break;
	};

	return -EINVAL;
}

long clkd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	pr_info("IOCTL command: %d\n", cmd);

	if (_IOC_TYPE(cmd) != DEVICE_IOCTL_MAGIC) {
		pr_err("[FAIL] ioctl command not matched\n");
		return -ENOTTY;
	}

	struct DeviceBuffer *pd = filp->private_data;
	if (!pd) {
		pr_err("[FAIL] no device data\n");
		return -ENODEV;
	}

	mutex_lock(&pd->lock);

	switch (cmd) {
	case SET_MODE: {
		int mode;
		if (copy_from_user(&mode, (int __user *)arg, sizeof(mode))) {
			pr_err("[FAIL] cannot get arguments for set mode\n");
			ret = -EFAULT;
			break;
		}

		if (mode != MODE_ENCODE && mode != MODE_DECODE) {
			pr_err("[FAIL] not matched arguments for set mode\n");
			ret = -EFAULT;
			break;
		}

		mutex_lock(&clkd_device_lock);
		processing_mode = mode;
		mutex_unlock(&clkd_device_lock);

		pd->mode = mode;
		pd->size = 0;
		kfree(pd->buffer);

	} break;
	case GET_MODE: {
		if (copy_to_user((int __user *)arg, &pd->mode,
				 sizeof(pd->mode))) {
			pr_err("[FAIL] cannot set arguments for get mode\n");
			ret = -EFAULT;
			break;
		}
	} break;
	default:
		ret = -ENOTTY;
		break;
	};

	mutex_unlock(&pd->lock);
	return ret;
}

int clkd_open(struct inode *inode, struct file *file)
{
	struct DeviceBuffer *pd;

	pd = kmalloc(sizeof(struct DeviceBuffer), GFP_KERNEL);
	if (!pd) {
		pr_err("[FAIL] cannot allocate memory for device data open\n");
		return -EINVAL;
	}

	mutex_lock(&clkd_device_lock);
	pd->size = 0;
	pd->buffer = NULL;
	pd->mode = processing_mode;
	mutex_init(&pd->lock);
	mutex_unlock(&clkd_device_lock);

	file->private_data = pd;

	pr_info("Device opened: %p - %p\n", inode, file);
	return 0;
}

int clkd_release(struct inode *inode, struct file *filp)
{
	struct DeviceBuffer *pd = filp->private_data;
	if (!pd) {
		pr_err("[FAIL] cannot get device data for release\n");
		return -EINVAL;
	}

	kfree(pd->buffer);
	mutex_destroy(&pd->lock);
	kfree(pd);

	pr_info("Device released: %p - %p\n", inode, filp);
	return 0;
}
