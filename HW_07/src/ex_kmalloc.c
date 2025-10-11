#define pr_fmt(fmt) "ex_kmalloc: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/errno.h>

static struct work_struct memory_alloc_work;

static void memory_alloc_handler(struct work_struct *work)
{
	for (size_t need_bytes = PAGE_SIZE;; need_bytes <<= 1) {
		pr_info("kmalloc: %zu byte\n", need_bytes);

		ktime_t start_time = ktime_get();

		void *allocated_data = kmalloc(need_bytes, GFP_KERNEL);
		if (allocated_data != NULL) {
			ktime_t stop_time = ktime_get();

			pr_info("kmalloc: SUCCSESS\n");

			const int diff = ktime_to_ms(stop_time) -
					 ktime_to_ms(start_time);
			pr_info("kmalloc: %zu byte, %d ms, type: %s\n",
				need_bytes, diff, "kmalloc");
		} else {
			// strerr(errno)
			pr_info("kmalloc: FAIL , err_msg = %s\n", "error");
			break;
		}

		kfree(allocated_data);
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
MODULE_DESCRIPTION("Module HW 07 kmalloc");