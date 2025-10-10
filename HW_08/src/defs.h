#ifndef __CLKD_MODULE_DEFS_H__
#define __CLKD_MODULE_DEFS_H__

#include <linux/types.h>
#include <linux/mutex.h>

#define DEVICE_NAME "clkd"
#define CLASS_NAME "clkd"
#define DEVICE_IOCTL_MAGIC 'v'


// Processing data modes
typedef enum {
    MODE_ENCODE = 0,
    MODE_DECODE = 1
} processing_mode_t;

// Device run modes
typedef enum {
    MODE_GLOBAL_DATA = 0,
    MODE_PER_PROCESS_DATA = 1
} run_mode_t;

extern struct mutex clkd_device_lock;
extern processing_mode_t processing_mode;

#endif // __CLKD_MODULE_DEFS_H__