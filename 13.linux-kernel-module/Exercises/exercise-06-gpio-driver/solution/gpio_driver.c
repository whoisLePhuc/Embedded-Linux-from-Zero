#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

struct gpio_priv {
    struct gpio_desc *gpio;
    struct timer_list timer;
    int irq;
    unsigned long irq_count;
    struct kobject *kobj;
};

static struct gpio_priv *priv;

static irqreturn_t my_gpio_irq_handler(int irq, void *data)
{
    struct gpio_priv *p = data;
    p->irq_count++;
    return IRQ_HANDLED;
}

static void my_timer_callback(struct timer_list *t)
{
    struct gpio_priv *p = from_timer(p, t, timer);
    int val;

    val = gpiod_get_value(p->gpio);
    gpiod_set_value(p->gpio, !val);

    mod_timer(t, jiffies + msecs_to_jiffies(2000));
}

static ssize_t value_show(struct kobject *kobj,
                          struct kobj_attribute *attr, char *buf)
{
    return sysfs_emit(buf, "%d\n", gpiod_get_value(priv->gpio));
}

static ssize_t irq_count_show(struct kobject *kobj,
                              struct kobj_attribute *attr, char *buf)
{
    return sysfs_emit(buf, "%lu\n", priv->irq_count);
}

static struct kobj_attribute value_attr =
    __ATTR(value, 0444, value_show, NULL);
static struct kobj_attribute irq_count_attr =
    __ATTR(irq_count, 0444, irq_count_show, NULL);

static struct attribute *gpio_attrs[] = {
    &value_attr.attr,
    &irq_count_attr.attr,
    NULL,
};

static const struct attribute_group gpio_attr_group = {
    .attrs = gpio_attrs,
};

static int __init my_init(void)
{
    int irq, ret;

    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->gpio = gpiod_get(NULL, "mygpio", GPIOD_OUT_LOW);
    if (IS_ERR(priv->gpio)) {
        pr_err("Failed to get GPIO descriptor\n");
        ret = PTR_ERR(priv->gpio);
        goto err_free;
    }

    gpiod_direction_output(priv->gpio, 0);

    irq = gpiod_to_irq(priv->gpio);
    if (irq < 0) {
        pr_err("Failed to get IRQ from GPIO\n");
        ret = irq;
        goto err_put;
    }
    priv->irq = irq;

    ret = request_irq(irq, my_gpio_irq_handler,
                      IRQF_TRIGGER_RISING, "my_gpio", priv);
    if (ret) {
        pr_err("Failed to request IRQ\n");
        goto err_put;
    }

    timer_setup(&priv->timer, my_timer_callback, 0);
    mod_timer(&priv->timer, jiffies + msecs_to_jiffies(2000));

    priv->kobj = kobject_create_and_add("gpio_driver", kernel_kobj);
    if (!priv->kobj) {
        ret = -ENOMEM;
        goto err_timer;
    }

    ret = sysfs_create_group(priv->kobj, &gpio_attr_group);
    if (ret)
        goto err_kobj;

    pr_info("GPIO driver loaded\n");
    return 0;

err_kobj:
    kobject_put(priv->kobj);
err_timer:
    del_timer_sync(&priv->timer);
    free_irq(priv->irq, priv);
err_put:
    gpiod_put(priv->gpio);
err_free:
    kfree(priv);
    return ret;
}

static void __exit my_exit(void)
{
    sysfs_remove_group(priv->kobj, &gpio_attr_group);
    kobject_put(priv->kobj);
    del_timer_sync(&priv->timer);
    free_irq(priv->irq, priv);
    gpiod_put(priv->gpio);
    kfree(priv);
    pr_info("GPIO driver unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("GPIO driver using descriptor-based API");
