#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define pr_fmt(fmt) "kthread_app: " fmt

static unsigned short num_ph = 5;
static unsigned short tt_die = 5;
static unsigned short tt_eat = 5;
static unsigned short tt_sleep = 5;
static unsigned short num_must_eat = 0;
static long int	begin_time = 0;

typedef struct s_dinner_data {
	ushort	num_ph;
	ushort	tt_die;
	ushort	tt_eat;
	ushort	tt_sleep;
	ushort	num_must_eat;
	long	begin_time;
}	t_dinner_data;

typedef struct s_philosopher{
	int	num;
	int	count_eat;
	int	*died;
	int	flag;
	int	last_eat;
	int eat_all;
	pthread_t *th;
	pthread_mutex_t	*fork;
	pthread_mutex_t	message;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	t_dinner_data	d_dinner;
}	t_philosopher;

static t_dinner_data d_dinner = {0}; 

static int philosopher_thread(void *unused)
{
    while (!kthread_should_stop()) {
        msleep(1000);
        pr_info("philosopher: %d eat\n", current->pid);
    }

	return 0;
}

// Initialization
static int __init mod_init(void)
{
    pr_info("init\n");

    d_dinner.num_ph = num_ph;
    d_dinner.tt_die = tt_die;
    d_dinner.tt_eat = tt_eat;
    d_dinner.tt_sleep = tt_sleep;
    d_dinner.num_must_eat = num_must_eat;
    d_dinner.begin_time = begin_time;


    /*for(int i = 0; i < num_ph; ++i)
    {
        philosophers[i] = kthread_create(philosopher_thread, NULL, "philosopher:");
        if (IS_ERR(philosophers[i])) {
            int err = PTR_ERR(philosophers[i]);

            pr_info("cannot create philosopher: %d - %d\n", i, err);
            continue;
        }
    }

    pr_info("philosophers created\n");
    
    for(int i = 0; i < num_ph; ++i)
    {
        wake_up_process(philosophers[i]);
    }

	pr_info("philosophers start eat\n");*/
	return 0;
}

static void __exit mod_exit(void)
{
    /*for(int i = 0; i < num_ph; ++i)
    {
        kthread_stop(philosophers[i]);
    }*/

	pr_info("exit\n");
}


	

module_param(num_ph, ushort, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(num_ph, "Philosophers count");

module_param(tt_die, ushort, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(tt_die, "Time to die");

module_param(tt_eat, ushort, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(tt_eat, "Time to eat");

module_param(tt_sleep, ushort, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(tt_sleep, "Time to sleep");

module_param(num_must_eat, ushort, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(num_must_eat, "Numbers to eat");

module_param(begin_time, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(begin_time, "Time to begin");

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 04");
