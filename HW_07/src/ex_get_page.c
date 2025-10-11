#define pr_fmt(fmt) "ex_get_pages: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/pagemap.h>

#define ALLOC_ORDER 0

static struct work_struct memory_alloc_work;

static void memory_alloc_handler(struct work_struct *work)
{
	for (size_t need_bytes = PAGE_SIZE;; need_bytes <<= 1) {
		const int need_allocate_pages = need_bytes / PAGE_SIZE;

		pr_info("get_pages: %zu byte by pages %d\n", need_bytes,
			need_allocate_pages);

		struct page **allocated_pages = kmalloc_array(
			need_allocate_pages, sizeof(struct page *), GFP_KERNEL);
		if (!allocated_pages) {
			pr_err("get_pages: Failes allocate memory for save pages\n");
			return;
		}

		ktime_t start_time = ktime_get();

		int allocated_count = 0;
		for (; allocated_count < need_allocate_pages;
		     allocated_count++) {
			allocated_pages[allocated_count] = alloc_pages(
				GFP_KERNEL | __GFP_ZERO, ALLOC_ORDER);
			if (!allocated_pages[allocated_count]) {
				pr_err("get_pages: failed at attempt  %d\n",
				       allocated_count);
				break;
			}

			get_page(allocated_pages[allocated_count]);

			// Debug
			// void *page_virt = page_address(allocated_pages[allocated_count]);
			// pr_info("get_pages: Allocated page %d, virtual %p\n", allocated_count, page_virt);
		}

		ktime_t stop_time = ktime_get();
		for (int free_count = 0; free_count < allocated_count;
		     free_count++) {
			if (allocated_pages[free_count]) {
				put_page(allocated_pages[free_count]);
				__free_pages(allocated_pages[free_count],
					     ALLOC_ORDER);
			}
		}

		kfree(allocated_pages);

		if (need_allocate_pages == allocated_count) {
			pr_info("get_pages: SUCCSESS\n");

			const int diff = ktime_to_ms(stop_time) -
					 ktime_to_ms(start_time);
			pr_info("get_pages: %zu byte, %d ms, type: %s\n",
				need_bytes, diff, "mempool");
		} else {
			pr_err("get_pages: FAIL to allocate memory\n");
			break;
		}
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
MODULE_DESCRIPTION("Module HW 07 get pages");
