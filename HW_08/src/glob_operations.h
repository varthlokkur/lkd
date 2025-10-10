#ifndef __CLKD_MODULE_SYMBOLIC_GLOBAL_OPERATIONS_H__
#define __CLKD_MODULE_SYMBOLIC_GLOBAL_OPERATIONS_H__

#include <linux/fs.h>

// Get global file operations
const struct file_operations* global_fops(void);

// Reset global data buffers
void reset_global_buffers(void);

#endif // __CLKD_MODULE_SYMBOLIC_GLOBAL_OPERATIONS_H__