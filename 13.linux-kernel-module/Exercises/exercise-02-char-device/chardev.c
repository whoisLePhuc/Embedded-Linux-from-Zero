#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"

// TODO: Khai báo biến global để lưu device number, class, cdev, device, value

static int chardev_open(struct inode *inode, struct file *filp)
{
    // TODO: Gán filp->private_data trỏ tới biến global value
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
    // TODO: Đọc giá trị từ private_data, format, copy_to_user
    return 0;
}

static ssize_t chardev_write(struct file *filp, const char __user *buf,
                             size_t len, loff_t *off)
{
    // TODO: copy_from_user, parse số nguyên, lưu vào private_data
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
    // TODO:
    // 1. alloc_chrdev_region
    // 2. cdev_init + cdev_add
    // 3. class_create + device_create
    return 0;
}

static void __exit chardev_exit(void)
{
    // TODO: Dọn dẹp theo thứ tự ngược lại
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("A simple character device driver");
