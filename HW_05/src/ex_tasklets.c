#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>

#define pr_fmt(fmt) "ex_tasklets: " fmt

static struct tasklet_struct work_tasklet;
static struct tasklet_struct work_hi_tasklet;

static struct task_struct *worker_thread;

static void tasklet_handler(unsigned long data)
{
	pr_info("Tasklet: Running on CPU %d (data=%lu)\n", smp_processor_id(),
		data);
	mdelay(5);
	pr_info("Tasklet: Work completed\n");
}

static void hi_tasklet_handler(unsigned long data)
{
	pr_info("HI-Tasklet: Running on CPU %d (data=%lu)\n",
		smp_processor_id(), data);
	mdelay(5);
	pr_info("HI-Tasklet: High priority work done\n");
}

static int worker_thread_handler(void *data)
{
	int count = 0;

	pr_info("Worker Thread: Started\n");

	while (!kthread_should_stop() && count < 5) {
		count++;

		pr_info("Worker Thread: Simulating interrupt (count=%d)\n",
			count);

		tasklet_schedule(&work_tasklet);
		pr_info("Worker Thread: Scheduled normal tasklet\n");

		if (count % 2 == 0) {
			tasklet_hi_schedule(&work_hi_tasklet);
			pr_info("Worker Thread: Scheduled HI tasklet\n");
		}

		msleep_interruptible(1000);
	}

	pr_info("Worker Thread: Exit\n");

	worker_thread = NULL;
	return 0;
}

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	tasklet_init(&work_tasklet, tasklet_handler, 12345);
	pr_info("Initialized normal tasklet\n");

	tasklet_init(&work_hi_tasklet, hi_tasklet_handler, 54321);
	pr_info("Initialized HI tasklet\n");

	worker_thread =
		kthread_run(worker_thread_handler, NULL, "tasklet_worker");
	if (IS_ERR(worker_thread)) {
		pr_err("Failed to create worker thread\n");
		tasklet_kill(&work_tasklet);
		tasklet_kill(&work_hi_tasklet);
		return PTR_ERR(worker_thread);
	}

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	if (worker_thread && !IS_ERR(worker_thread)) {
		kthread_stop(worker_thread);
		pr_info("Worker thread stopped\n");
	}

	tasklet_kill(&work_tasklet);
	tasklet_kill(&work_hi_tasklet);
	pr_info("Tasklets killed\n");

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 05 tasklets");
