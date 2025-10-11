#define pr_fmt(fmt) "ex_vmalloc: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

static struct work_struct memory_alloc_work;

static void memory_alloc_handler(struct work_struct *work)
{
	for (size_t need_bytes = PAGE_SIZE;; need_bytes <<= 1) {
		pr_info("vmalloc: %zu byte\n", need_bytes);

		ktime_t start_time = ktime_get();

		void *allocated_data = vmalloc(need_bytes);
		if (allocated_data != NULL) {
			ktime_t stop_time = ktime_get();

			pr_info("vmalloc: SUCCSESS\n");

			const int diff = ktime_to_ms(stop_time) -
					 ktime_to_ms(start_time);
			pr_info("vmalloc: %zu byte, %d ms, type: %s\n",
				need_bytes, diff, "vmalloc");
		} else {
			// strerr(errno)
			pr_info("vmalloc: FAIL , err_msg = %s\n", "error");
			break;
		}

		vfree(allocated_data);
	}
}

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	INIT_WORK(&memory_alloc_work, memory_alloc_handler);
	schedule_work(&memory_alloc_work);

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	cancel_work_sync(&memory_alloc_work);

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 07 vmalloc");