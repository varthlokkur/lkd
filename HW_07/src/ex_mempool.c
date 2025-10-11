#define pr_fmt(fmt) "ex_mempool: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/mempool.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/errno.h>

#define DEFAULT_MIN_NR 128

static mempool_t *pool;
static struct work_struct memory_alloc_work;

static void *mempool_alloc_custom(gfp_t gfp_mask, void *pool_data)
{
	return kmalloc(PAGE_SIZE, gfp_mask);
}

static void mempool_free_custom(void *element, void *pool_data)
{
	kfree(element);
}

static void memory_alloc_handler(struct work_struct *work)
{
	for (size_t need_bytes = PAGE_SIZE;; need_bytes <<= 1) {
		const int need_allocation_count = need_bytes / PAGE_SIZE;

		pr_info("mempool: %zu byte by pages %d\n", need_bytes,
			need_allocation_count);

		void **allocated_elements = kmalloc_array(
			need_allocation_count, sizeof(void *), GFP_KERNEL);
		if (!allocated_elements) {
			pr_err("mempool: Failes allocate memory for save elements\n");
			return;
		}

		ktime_t start_time = ktime_get();

		int allocated_count = 0;
		for (; allocated_count < need_allocation_count;
		     allocated_count++) {
			allocated_elements[allocated_count] =
				mempool_alloc(pool, GFP_KERNEL);
			if (!allocated_elements[allocated_count]) {
				pr_err("mempool: failed at attempt  %d\n",
				       allocated_count);
				break;
			}
		}

		ktime_t stop_time = ktime_get();
		for (int free_count = 0; free_count < allocated_count;
		     free_count++) {
			if (allocated_elements[free_count]) {
				mempool_free(allocated_elements[free_count],
					     pool);
			}
		}

		kfree(allocated_elements);

		if (need_allocation_count == allocated_count) {
			pr_info("mempool: SUCCSESS\n");

			const int diff = ktime_to_ms(stop_time) -
					 ktime_to_ms(start_time);
			pr_info("mempool: %zu byte, %d ms, type: %s\n",
				need_bytes, diff, "mempool");
		} else {
			pr_err("mempool: FAIL to allocate memory\n");
			break;
		}
	}
}

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	pool = mempool_create(DEFAULT_MIN_NR, mempool_alloc_custom,
			      mempool_free_custom, NULL);

	if (!pool) {
		pr_err("Cannot create mempool\n");
		return -ENOMEM;
	}

	INIT_WORK(&memory_alloc_work, memory_alloc_handler);
	schedule_work(&memory_alloc_work);

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	mempool_destroy(pool);
	cancel_work_sync(&memory_alloc_work);

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 07 mempool");