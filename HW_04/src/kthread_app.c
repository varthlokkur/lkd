#define pr_fmt(fmt) "kthread_app: " fmt

#include "defs.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/errno.h>

static struct task_struct **philosophers;
static struct mutex *forks;

int init_forks(void)
{
	forks = kmalloc_array(PHILOSOPHER_COUNT, sizeof(struct mutex),
			      GFP_KERNEL);

	if (!forks) {
		return -ENOMEM;
	}

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		mutex_init(&forks[i]);
	}

	return 0;
}

void deinit_forks(void)
{
	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		mutex_destroy(&forks[i]);
	}

	kfree(forks);
}

static int philosopher_thread(void *data)
{
	struct Philosopher *pd = (struct Philosopher *)data;
	int id = pd->id;
	int left_fork = id;
	int right_fork = (id + 1) % PHILOSOPHER_COUNT;
	int eating_cycles = 0;

	pr_info("Philosopher %d: Starting\n", id);

	while (!kthread_should_stop() && eating_cycles <= EATING_CYCLES) {
		pr_info("Philosopher %d: Thinking\n", id);
		msleep_interruptible(TIME_TO_THINK * 1000);

		if (id % 2 == 0) {
			pr_info("Philosopher %d: Trying to take right fork %d\n",
				id, right_fork);
			mutex_lock(&forks[right_fork]);
			pr_info("Philosopher %d: Took right fork %d\n", id,
				right_fork);

			pr_info("Philosopher %d: Trying to take left fork %d\n",
				id, left_fork);
			mutex_lock(&forks[left_fork]);
			pr_info("Philosopher %d: Took left fork %d\n", id,
				left_fork);
		} else {
			pr_info("Philosopher %d: Trying to take left fork %d\n",
				id, left_fork);
			mutex_lock(&forks[left_fork]);
			pr_info("Philosopher %d: Took left fork %d\n", id,
				left_fork);

			pr_info("Philosopher %d: Trying to take right fork %d\n",
				id, right_fork);
			mutex_lock(&forks[right_fork]);
			pr_info("Philosopher %d: Took right fork %d\n", id,
				right_fork);
		}

		pr_info("Philosopher %d: Eating\n", id);
		msleep_interruptible(TIME_TO_EAT * 1000);

		if (id % 2 == 0) {
			mutex_unlock(&forks[right_fork]);
			printk(KERN_INFO
			       "Philosopher %d: Put down right fork %d\n",
			       id, right_fork);
			mutex_unlock(&forks[left_fork]);
			printk(KERN_INFO
			       "Philosopher %d: Put down left fork %d\n",
			       id, left_fork);
		} else {
			mutex_unlock(&forks[left_fork]);
			printk(KERN_INFO
			       "Philosopher %d: Put down left fork %d\n",
			       id, left_fork);
			mutex_unlock(&forks[right_fork]);
			printk(KERN_INFO
			       "Philosopher %d: Put down right fork %d\n",
			       id, right_fork);
		}

		printk(KERN_INFO
		       "Philosopher %d: Finished eating cycle %d/%d\n",
		       id, eating_cycles, EATING_CYCLES);

		eating_cycles++;
	}

	pr_info("Philosopher %d: Exiting\n", id);
	kfree(data);
	return 0;
}

// Initialization
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	int result = init_forks();
	if (result < 0) {
		pr_err("Cannot allocate memory for forks\n");
		return result;
	}

	philosophers = kmalloc_array(PHILOSOPHER_COUNT,
				     sizeof(struct task_struct *), GFP_KERNEL);

	if (!philosophers) {
		deinit_forks();
		pr_err("Cannot allocate memory for philosophers\n");
		return -ENOMEM;
	}

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		struct Philosopher *data =
			kmalloc(sizeof(struct Philosopher), GFP_KERNEL);
		if (!data) {
			pr_err("Cannot to allocate memory for philosopher data\n");
			continue;
		}
		data->id = i;

		philosophers[i] = kthread_run(philosopher_thread, data,
					      "philosopher_%d", i);
		if (IS_ERR(philosophers[i])) {
			pr_err("Cannot to create philosopher thread %d\n", i);
			kfree(data);
			continue;
		}
	}

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		if (!IS_ERR(philosophers[i])) {
			kthread_stop(philosophers[i]);
		}
	}

	deinit_forks();
	kfree(philosophers);

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 04");
