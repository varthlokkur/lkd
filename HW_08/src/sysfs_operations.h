#ifndef __CLKD_MODULE_SYMBOLIC_SYSFS_OPERATIONS_H__
#define __CLKD_MODULE_SYMBOLIC_SYSFS_OPERATIONS_H__

#include <linux/device.h>

int register_sysfs(struct device* dev);
void unregister_sysfs(struct device* dev);

#endif // __CLKD_MODULE_SYMBOLIC_SYSFS_OPERATIONS_H__