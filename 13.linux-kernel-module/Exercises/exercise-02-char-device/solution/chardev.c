#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"

static int major;
static struct class *chardev_class;
static struct cdev chardev_cdev;
static int my_value;

static int chardev_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &my_value;
    pr_info("chardev: opened\n");
    return 0;
}

static int chardev_release(struct inode *inode, struct file *filp)
{
    pr_info("chardev: closed\n");
    return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buf,
                            size_t len, loff_t *off)
{
    int *val = filp->private_data;
    char tmp[32];
    int n;

    n = snprintf(tmp, sizeof(tmp), "%d\n", *val);
    if (*off >= n)
        return 0;
    if (len > (size_t)(n - *off))
        len = n - *off;

    if (copy_to_user(buf, tmp + *off, len))
        return -EFAULT;

    *off += len;
    return len;
}

static ssize_t chardev_write(struct file *filp, const char __user *buf,
                             size_t len, loff_t *off)
{
    int *val = filp->private_data;
    char tmp[32];
    int newval;

    if (len > sizeof(tmp) - 1)
        len = sizeof(tmp) - 1;

    if (copy_from_user(tmp, buf, len))
        return -EFAULT;

    tmp[len] = '\0';
    if (kstrtoint(tmp, 0, &newval) == 0) {
        *val = newval;
        pr_info("chardev: set value to %d\n", *val);
    }

    return len;
}

static const struct file_operations chardev_fops = {
    .owner   = THIS_MODULE,
    .open    = chardev_open,
    .release = chardev_release,
    .read    = chardev_read,
    .write   = chardev_write,
};

static int __init chardev_init(void)
{
    dev_t dev;
    int ret;

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("chardev: failed to allocate device number\n");
        return ret;
    }
    major = MAJOR(dev);

    cdev_init(&chardev_cdev, &chardev_fops);
    chardev_cdev.owner = THIS_MODULE;
    ret = cdev_add(&chardev_cdev, dev, 1);
    if (ret < 0) {
        pr_err("chardev: failed to add cdev\n");
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    chardev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        cdev_del(&chardev_cdev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(chardev_class);
    }

    device_create(chardev_class, NULL, dev, NULL, DEVICE_NAME);
    pr_info("chardev: loaded, major=%d\n", major);
    return 0;
}

static void __exit chardev_exit(void)
{
    device_destroy(chardev_class, MKDEV(major, 0));
    class_destroy(chardev_class);
    cdev_del(&chardev_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
    pr_info("chardev: unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("A simple character device driver");
