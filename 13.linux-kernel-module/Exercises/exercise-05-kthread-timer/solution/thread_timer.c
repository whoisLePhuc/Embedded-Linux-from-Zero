#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>

struct my_data {
    struct task_struct *thread;
    struct timer_list timer;
    int counter;
};

static int thread_worker(void *data)
{
    struct my_data *priv = data;
    int count = 0;

    while (!kthread_should_stop()) {
        pr_info("Thread iteration %d\n", count++);
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(msecs_to_jiffies(1000));
    }

    pr_info("Thread stopping\n");
    return 0;
}

static void timer_callback(struct timer_list *t)
{
    struct my_data *priv = from_timer(priv, t, timer);

    priv->counter++;
    pr_info("Timer fired at jiffies=%lu, count=%d\n",
            jiffies, priv->counter);
    mod_timer(t, jiffies + msecs_to_jiffies(1000));
}

static struct my_data *priv;

static int __init my_init(void)
{
    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    timer_setup(&priv->timer, timer_callback, 0);
    mod_timer(&priv->timer, jiffies + msecs_to_jiffies(1000));

    priv->thread = kthread_run(thread_worker, priv, "my_kthread");
    if (IS_ERR(priv->thread)) {
        del_timer_sync(&priv->timer);
        kfree(priv);
        return PTR_ERR(priv->thread);
    }

    pr_info("Module loaded: thread and timer started\n");
    return 0;
}

static void __exit my_exit(void)
{
    kthread_stop(priv->thread);
    del_timer_sync(&priv->timer);
    kfree(priv);
    pr_info("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("Kernel thread and timer example");
