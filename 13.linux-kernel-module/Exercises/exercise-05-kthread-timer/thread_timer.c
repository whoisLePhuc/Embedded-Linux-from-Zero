#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/timer.h>

// TODO: Khai báo cấu trúc data chứa thread_struct, timer, counter

static int thread_worker(void *data)
{
    // TODO: Ép kiểu data về struct my_data
    // TODO: Vòng lặp kiểm tra kthread_should_stop
    // TODO: In iteration counter, sleep 1 giây với schedule_timeout
    return 0;
}

static void timer_callback(struct timer_list *t)
{
    // TODO: Dùng from_timer để lấy data
    // TODO: In "Timer fired at jiffies = ..."
    // TODO: Re-arm timer với mod_timer
}

static int __init my_init(void)
{
    // TODO:
    // 1. Cấp phát data (kzalloc)
    // 2. Khởi tạo timer với timer_setup
    // 3. Add timer (mod_timer sau 1 giây)
    // 4. Chạy thread với kthread_run
    return 0;
}

static void __exit my_exit(void)
{
    // TODO:
    // 1. kthread_stop (dừng thread)
    // 2. del_timer_sync (hủy timer)
    // 3. kfree(data)
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("Kernel thread and timer example");
