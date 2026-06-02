#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/jiffies.h>

#define PROC_NAME "mydriver"
#define SYS_NAME  "mydriver"

// TODO: Khai báo biến global: kobject pointer, giá trị value

// --- procfs ---
static int my_proc_show(struct seq_file *m, void *v)
{
    // TODO: In jiffies ra seq_file
    return 0;
}

static int my_proc_open(struct inode *inode, struct file *filp)
{
    return single_open(filp, my_proc_show, NULL);
}

// TODO: Khai báo struct proc_ops

// --- sysfs ---
// TODO: Implement show function (dùng sysfs_emit)
// TODO: Implement store function (dùng kstrtoint)
// TODO: Khai báo struct kobj_attribute với __ATTR
// TODO: Khai báo attribute group

static int __init my_init(void)
{
    // TODO:
    // 1. proc_create("/proc/mydriver"...)
    // 2. kobject_create_and_add("mydriver", kernel_kobj)
    // 3. sysfs_create_group
    return 0;
}

static void __exit my_exit(void)
{
    // TODO: Dọn dẹp proc, sysfs, kobject
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("procfs and sysfs example");
