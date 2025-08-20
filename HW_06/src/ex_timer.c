#define pr_fmt(fmt) "ex_timer: " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

#define TIMER_PERIOD 30 // 30 sec
#define STOP_INTERVAL 5 * 60 // 5 min
#define SECONDS_IN_MINUTE 60

struct timer_private {
	unsigned long stop_timepoint;
	unsigned long start_timepoint;
	struct timer_list timer_handle;
	struct hrtimer hrtimer_handle;
} timer_singleton;

static void timer_handler(struct timer_list *t)
{
	struct timer_private *data = from_timer(data, t, timer_handle);
	if (time_before_eq(jiffies, data->stop_timepoint)) {
		const int elapsed_seconds =
			(jiffies - data->start_timepoint) / HZ;
		pr_info("min=%d: Hello, timer!\n",
			elapsed_seconds / SECONDS_IN_MINUTE);

		mod_timer(&timer_singleton.timer_handle,
			  jiffies + msecs_to_jiffies(TIMER_PERIOD * 1000));
	} else {
		pr_info("timer completed work!\n");
	}
}

static enum hrtimer_restart hrtimer_handler(struct hrtimer *hr)
{
	struct timer_private *data =
		container_of(hr, struct timer_private, hrtimer_handle);

	if (time_before_eq(jiffies, data->stop_timepoint)) {
		const int elapsed_seconds =
			(jiffies - data->start_timepoint) / HZ;
		pr_info("min=%d: Hello, hrtimer!\n",
			elapsed_seconds / SECONDS_IN_MINUTE);

		hrtimer_forward_now(hr, ktime_set(TIMER_PERIOD, 0));
		return HRTIMER_RESTART;
	} else {
		pr_info("hrtimer completed work!\n");
	}

	return HRTIMER_NORESTART;
}

// Module interface
static int __init mod_init(void)
{
	pr_info("Initializing\n");

	timer_setup(&timer_singleton.timer_handle, timer_handler, 0);
	hrtimer_init(&timer_singleton.hrtimer_handle, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL_HARD);
	timer_singleton.hrtimer_handle.function = hrtimer_handler;
	pr_info("Initialized timer\n");

	timer_singleton.start_timepoint = jiffies;
	timer_singleton.stop_timepoint = jiffies + STOP_INTERVAL * HZ;

	mod_timer(&timer_singleton.timer_handle, jiffies + TIMER_PERIOD * HZ);
	hrtimer_forward_now(&timer_singleton.hrtimer_handle,
			    ktime_set(TIMER_PERIOD, 0));
	hrtimer_start(&timer_singleton.hrtimer_handle,
		      ktime_set(TIMER_PERIOD, 0), HRTIMER_MODE_REL_HARD);

	pr_info("Initialization complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Exiting\n");

	timer_delete_sync(&timer_singleton.timer_handle);
	hrtimer_cancel(&timer_singleton.hrtimer_handle);
	pr_info("Timer killed\n");

	pr_info("Exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 06 timer");
