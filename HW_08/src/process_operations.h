#ifndef __CLKD_MODULE_SYMBOLIC_PER_PROCESS_OPERATIONS_H__
#define __CLKD_MODULE_SYMBOLIC_PER_PROCESS_OPERATIONS_H__

#include <linux/fs.h>

const struct file_operations* per_process_fops(void);

#endif // __CLKD_MODULE_SYMBOLIC_PER_PROCESS_OPERATIONS_H__
