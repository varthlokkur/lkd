
#define pr_fmt(fmt) "ex_kmem_cache: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/mm.h>

static struct work_struct memory_alloc_work;
static struct kmem_cache *mem_cache;

static void memory_alloc_handler(struct work_struct *work)
{
	for (size_t need_bytes = PAGE_SIZE;; need_bytes <<= 1) {
		const int need_allocation_count = need_bytes / PAGE_SIZE;

		pr_info("kmem_cache: %zu byte by pages %d\n", need_bytes,
			need_allocation_count);

		void **allocated_elements = kmalloc_array(
			need_allocation_count, sizeof(void *), GFP_KERNEL);
		if (!allocated_elements) {
			pr_err("kmem_cache: Failes allocate memory for save elements\n");
			return;
		}

		ktime_t start_time = ktime_get();

		int allocated_count = 0;
		for (; allocated_count < need_allocation_count;
		     allocated_count++) {
			allocated_elements[allocated_count] =
				kmem_cache_alloc(mem_cache, GFP_KERNEL);
			if (!allocated_elements[allocated_count]) {
				pr_err("kmem_cache: failed at attempt  %d\n",
				       allocated_count);
				break;
			}
		}

		ktime_t stop_time = ktime_get();
		for (int free_count = 0; free_count < allocated_count;
		     free_count++) {
			if (allocated_elements[free_count]) {
				kmem_cache_free(mem_cache,
						allocated_elements[free_count]);
			}
		}

		kfree(allocated_elements);

		if (need_allocation_count == allocated_count) {
			pr_info("kmem_cache: SUCCSESS\n");

			const int diff = ktime_to_ms(stop_time) -
					 ktime_to_ms(start_time);
			pr_info("kmem_cache: %zu byte, %d ms, type: %s\n",
				need_bytes, diff, "mempool");
		} else {
			pr_err("kmem_cache: FAIL to allocate memory\n");
			break;
		}
	}
}

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	mem_cache = kmem_cache_create("kmem_cache_ex", 4096, 0,
				      SLAB_HWCACHE_ALIGN, NULL);
	if (mem_cache == NULL) {
		pr_err("Cannot create memcache\n");
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

	cancel_work_sync(&memory_alloc_work);
	kmem_cache_destroy(mem_cache);

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 07 kmem_cache");