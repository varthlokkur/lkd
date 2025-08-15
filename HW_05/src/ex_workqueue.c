#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#define pr_fmt(fmt) "ex_workqueue: " fmt

static struct work_struct system_work;
static struct delayed_work delayed_work;

static struct workqueue_struct *dynamic_wq;
static struct work_struct dedicated_work;

static void system_work_handler(struct work_struct *work)
{
	pr_info("System Workqueue: Executing on CPU %d\n", smp_processor_id());
	msleep(10);
	pr_info("System Workqueue: Work completed\n");
}

static void delayed_work_handler(struct work_struct *work)
{
	pr_info("Delayed Work: Executing after delay on CPU %d\n",
		smp_processor_id());
	msleep(10);
	pr_info("Delayed Work: Work completed\n");
}

static void dedicated_work_handler(struct work_struct *work)
{
	pr_info("Dedicated Workqueue: Executing on CPU %d\n",
		smp_processor_id());
	for (int i = 0; i < 1000000; i++)
		;
	pr_info("Dedicated Workqueue: CPU-intensive work completed\n");
}

// Initialization
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	INIT_WORK(&system_work, system_work_handler);
	schedule_work(&system_work);
	pr_info("Scheduled work on system workqueue\n");

	INIT_DELAYED_WORK(&delayed_work, delayed_work_handler);
	schedule_delayed_work(&delayed_work, msecs_to_jiffies(2000));
	pr_info("Scheduled delayed work (2 seconds)\n");

	dynamic_wq = alloc_workqueue("dynamic_workqueue",
				     WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!dynamic_wq) {
		pr_err("Failed to create workqueue\n");
		return -ENOMEM;
	}
	pr_info("Created dedicated workqueue\n");

	INIT_WORK(&dedicated_work, dedicated_work_handler);
	queue_work(dynamic_wq, &dedicated_work);
	pr_info("Queued work on dedicated workqueue\n");

	struct work_struct *another_work =
		kmalloc(sizeof(struct work_struct), GFP_KERNEL);
	if (!another_work) {
		pr_err("Failed to allocate work structure\n");
	} else {
		INIT_WORK(another_work, dedicated_work_handler);
		queue_work(dynamic_wq, another_work);
		pr_info("Queued another work item on dedicated workqueue\n");
	}

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	cancel_work_sync(&system_work);
	cancel_delayed_work_sync(&delayed_work);
	pr_info("Cancelled system workqueue items\n");

	if (dynamic_wq) {
		flush_workqueue(dynamic_wq);
		destroy_workqueue(dynamic_wq);
		pr_info("Destroyed dedicated workqueue\n");
	}

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 05 workqueue");