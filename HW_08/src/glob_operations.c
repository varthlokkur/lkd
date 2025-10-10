#include "defs.h"
#include "base64.h"
#include "glob_operations.h"

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kernel.h>

ssize_t clkd_glob_read(struct file *file, char __user *ubuf, size_t size,
		       loff_t *offset);
ssize_t clkd_glob_write(struct file *file, const char __user *ubuf, size_t size,
			loff_t *off);

int clkd_glob_open(struct inode *inode, struct file *file);
int clkd_glob_release(struct inode *inode, struct file *filp);

struct GlobalDeviceBuffer {
	// data buffer
	char *buffer;

	// buffer size
	size_t size;
};

static struct file_operations clkd_global_fops = {
	.owner = THIS_MODULE,
	.read = clkd_glob_read,
	.write = clkd_glob_write,
	.open = clkd_glob_open,
	.release = clkd_glob_release,
};

static struct GlobalDeviceBuffer global_buffer = { .buffer = NULL, .size = 0 };

const struct file_operations *global_fops(void)
{
	return &clkd_global_fops;
}

void reset_global_buffers(void)
{
	pr_info("Global buffers reset command\n");

	mutex_lock(&clkd_device_lock);
	kfree(global_buffer.buffer);
	global_buffer.buffer = NULL;
	global_buffer.size = 0;
	mutex_unlock(&clkd_device_lock);

	pr_info("Global buffers reseted\n");
}

ssize_t clkd_glob_read(struct file *file, char __user *ubuf, size_t size,
		       loff_t *offset)
{
	size_t bytes_to_read = 0;

	pr_info("Read %zu bytes, offset %lld\n", size, *offset);

	mutex_lock(&clkd_device_lock);

	if (!global_buffer.buffer) {
		mutex_unlock(&clkd_device_lock);
		pr_warn("[FAIL] cannot get device data to read\n");
		return 0;
	}

	if (*offset >= global_buffer.size) {
		mutex_unlock(&clkd_device_lock);
		pr_warn("Buffer more than data\n");
		return 0;
	}

	bytes_to_read = min(size, global_buffer.size - *offset);

	if (copy_to_user(ubuf, global_buffer.buffer + *offset, bytes_to_read)) {
		mutex_unlock(&clkd_device_lock);
		pr_warn("[FAIL] cannot copy data to user for read\n");
		return -EFAULT;
	}

	*offset += bytes_to_read;

	mutex_unlock(&clkd_device_lock);
	return bytes_to_read;
}

ssize_t clkd_glob_write(struct file *file, const char __user *ubuf, size_t size,
			loff_t *off)
{
	pr_info("Write %zu\n", size);

	if (size == 0) {
		pr_warn("[FAIL] no read read 0 bytes\n");
		return -EINVAL;
	}

	// Allocate input buffer
	char *input_buffer = kmalloc(size, GFP_KERNEL);
	if (!input_buffer) {
		pr_err("[FAIL] cannot allocate buffer to copy user data to write\n");
		return -ENOMEM;
	}

	// Copy data from userspace
	if (copy_from_user(input_buffer, ubuf, size)) {
		kfree(input_buffer);
		pr_err("[FAIL] cannot copy user data to write\n");
		return -EFAULT;
	}

	mutex_lock(&clkd_device_lock);

	// Processing data
	switch (processing_mode) {
	case MODE_ENCODE: {
		// calculate data
		int output_size = base64_encode((unsigned char *)input_buffer,
						size, NULL, 0);

		pr_info("Calculated encoded %d bytes\n", output_size);

		if (output_size < 0) {
			kfree(input_buffer);
			mutex_unlock(&clkd_device_lock);

			pr_err("[FAIL] cannot calculate encoded buffer size\n");
			return -EFAULT;
		}

		char *output_buffer = kmalloc(output_size, GFP_KERNEL);
		if (!output_buffer) {
			pr_err("[FAIL] cannot allocate memory for encoded buffer\n");
			kfree(input_buffer);
			mutex_unlock(&clkd_device_lock);
			return -ENODEV;
		}

		int result_size = base64_encode((unsigned char *)input_buffer,
						size, output_buffer,
						output_size);

		if (result_size < 0) {
			kfree(input_buffer);
			kfree(output_buffer);
			mutex_unlock(&clkd_device_lock);

			pr_err("[FAIL] cannot calculate encoded buffer size\n");
			return -EFAULT;
		}

		// Free previous buffers
		kfree(global_buffer.buffer);
		global_buffer.buffer = NULL;
		global_buffer.size = 0;

		// Write new message
		global_buffer.buffer = output_buffer;
		global_buffer.size = result_size;

		kfree(input_buffer);
		mutex_unlock(&clkd_device_lock);
		pr_info("Encoded %zu bytes to %d bytes\n", size, result_size);

		return size;
	}

	break;

	case MODE_DECODE: {
		int output_size = base64_decode(input_buffer, size, NULL, 0);

		pr_info("Calculated decoded %d bytes\n", output_size);

		if (output_size < 0) {
			kfree(input_buffer);
			mutex_unlock(&clkd_device_lock);
			pr_err("[FAIL] cannot calculate decoded buffer size\n");
			return -EFAULT;
		}

		char *output_buffer = kmalloc(output_size, GFP_KERNEL);
		if (!output_buffer) {
			pr_err("[FAIL] cannot allocate memory for decoded buffer\n");
			kfree(input_buffer);
			mutex_unlock(&clkd_device_lock);
			return -ENODEV;
		}

		int result_size = base64_decode(input_buffer, size,
						(unsigned char *)output_buffer,
						output_size);

		if (result_size < 0) {
			kfree(input_buffer);
			kfree(output_buffer);
			mutex_unlock(&clkd_device_lock);

			pr_err("[FAIL] cannot calculate decoded buffer size\n");
			return -EFAULT;
		}

		// Free previous buffers
		kfree(global_buffer.buffer);
		global_buffer.buffer = NULL;
		global_buffer.size = 0;

		global_buffer.buffer = output_buffer;
		global_buffer.size = result_size;

		kfree(input_buffer);
		mutex_unlock(&clkd_device_lock);
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

int clkd_glob_open(struct inode *inode, struct file *file)
{
	pr_info("Device opened in global mode: %p - %p\n", inode, file);
	return 0;
}

int clkd_glob_release(struct inode *inode, struct file *filp)
{
	pr_info("Device released in global mode: %p - %p\n", inode, filp);
	return 0;
}