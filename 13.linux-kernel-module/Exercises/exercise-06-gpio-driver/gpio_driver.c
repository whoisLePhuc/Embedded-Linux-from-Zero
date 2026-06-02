#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

// TODO: Khai báo cấu trúc chứa gpio_desc, timer, irq, irq_count, kobject
// TODO: Sử dụng descriptor-based API (gpiod_*), KHÔNG legacy gpio_*

static irqreturn_t my_gpio_irq_handler(int irq, void *data)
{
    // TODO: Tăng irq_count
    // TODO: Trả về IRQ_HANDLED
    return IRQ_HANDLED;
}

static void my_timer_callback(struct timer_list *t)
{
    // TODO: Dùng from_timer lấy data
    // TODO: Toggle GPIO: gpiod_set_value(gpio, !gpiod_get_value(gpio))
    // TODO: Re-arm timer
}

// TODO: Implement sysfs show cho value và irq_count
// TODO: Khai báo kobj_attribute, attribute group

static int __init my_init(void)
{
    // TODO:
    // 1. Cấp phát data (kzalloc)
    // 2. Lấy GPIO với gpiod_get (cần device context - tạm thời dùng NULL)
    //    Trên host PC có thể mock, trên board dùng devm_gpiod_get
    // 3. Set direction output
    // 4. gpiod_to_irq + request_irq
    // 5. Timer toggle: timer_setup + mod_timer
    // 6. sysfs: kobject_create_and_add + sysfs_create_group
    return 0;
}

static void __exit my_exit(void)
{
    // TODO: Dọn dẹp theo thứ tự: timer, irq, gpio, sysfs, kfree
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("GPIO driver using descriptor-based API");
