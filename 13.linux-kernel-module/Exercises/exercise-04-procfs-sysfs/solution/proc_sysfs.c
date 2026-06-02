#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/jiffies.h>

#define PROC_NAME "mydriver"
#define SYS_NAME  "mydriver"

static struct kobject *my_kobj;
static int my_value;

/* --- procfs --- */

static int my_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "jiffies: %lu\n", jiffies);
    return 0;
}

static int my_proc_open(struct inode *inode, struct file *filp)
{
    return single_open(filp, my_proc_show, NULL);
}

static const struct proc_ops my_proc_fops = {
    .proc_open    = my_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* --- sysfs --- */

static ssize_t my_value_show(struct kobject *kobj,
                             struct kobj_attribute *attr, char *buf)
{
    return sysfs_emit(buf, "%d\n", my_value);
}

static ssize_t my_value_store(struct kobject *kobj,
                              struct kobj_attribute *attr,
                              const char *buf, size_t count)
{
    if (kstrtoint(buf, 0, &my_value))
        return -EINVAL;
    return count;
}

static struct kobj_attribute my_value_attr =
    __ATTR(value, 0644, my_value_show, my_value_store);

static struct attribute *my_attrs[] = {
    &my_value_attr.attr,
    NULL,
};

static const struct attribute_group my_attr_group = {
    .attrs = my_attrs,
};

static int __init my_init(void)
{
    int ret;

    proc_create(PROC_NAME, 0444, NULL, &my_proc_fops);

    my_kobj = kobject_create_and_add(SYS_NAME, kernel_kobj);
    if (!my_kobj) {
        proc_remove(THIS_MODULE);
        return -ENOMEM;
    }

    ret = sysfs_create_group(my_kobj, &my_attr_group);
    if (ret) {
        kobject_put(my_kobj);
        proc_remove(THIS_MODULE);
        return ret;
    }

    pr_info("procfs/sysfs module loaded\n");
    return 0;
}

static void __exit my_exit(void)
{
    sysfs_remove_group(my_kobj, &my_attr_group);
    kobject_put(my_kobj);
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("procfs/sysfs module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("procfs and sysfs example");
