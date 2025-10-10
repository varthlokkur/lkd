#ifndef __CLKD_MODULE_SYMBOLIC_PROCFS_OPERATIONS_H__
#define __CLKD_MODULE_SYMBOLIC_PROCFS_OPERATIONS_H__

#include <linux/fs.h>

int register_procfs(void);
void unregister_procfs(void);

#endif // __CLKD_MODULE_SYMBOLIC_PROCFS_OPERATIONS_H__