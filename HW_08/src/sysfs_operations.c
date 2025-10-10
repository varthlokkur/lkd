#include "defs.h"
#include "sysfs_operations.h"
#include "glob_operations.h"

ssize_t data_store(struct device *dev, struct device_attribute *attr,
		   const char *ubuf, size_t count);

ssize_t data_show(struct device *dev, struct device_attribute *attr, char *buf);

struct device_attribute dev_attr_data = {
	.attr = { .name = SYSFS_RESET_ATTR_NAME,
		  .mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH |
			  S_IWOTH },
	.store = data_store,
	.show = data_show
};

ssize_t data_show(struct device *dev, struct device_attribute *, char *)
{
	pr_err("[FAIL] attribute show not implemented\n");
	return -EINVAL;
}

ssize_t data_store(struct device *dev, struct device_attribute *attr,
		   const char *ubuf, size_t size)
{
	pr_info("Store sysfs attr\n");
	size_t actual_count = size;

	if (size > 0 && ubuf[size - 1] == '\n')
		actual_count = size - 1;

	if (actual_count > 0) {
		char *input_buffer = kmalloc(actual_count + 1, GFP_KERNEL);
		if (!input_buffer) {
			pr_err("[FAIL] cannot allocate buffer to copy user data to sysfs store\n");
			return -ENOMEM;
		}

		memcpy(input_buffer, ubuf, actual_count);
		input_buffer[actual_count] = '\0';

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
	}

	return size;
}

int register_sysfs(struct device *dev)
{
	pr_info("Registering sysfs attr\n");
	int ret = device_create_file(dev, &dev_attr_data);
	if (ret) {
		pr_err("[FAIL] cannot create device file: %d\n", ret);
		return ret;
	}

	pr_info("Sysfs device file created: /sys/class/%s/%s/%s\n", DEVICE_NAME,
		DEVICE_NAME, SYSFS_RESET_ATTR_NAME);
	return 0;
}

void unregister_sysfs(struct device *dev)
{
	pr_info("Unregister sysfs file\n");
	if (dev) {
		device_remove_file(dev, &dev_attr_data);
	}

	pr_info("Unregistered sysfs file\n");
}