#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Training");
MODULE_DESCRIPTION("Hello World kernel module");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello from Yocto kernel module!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye from kernel module!\n");
}

module_init(hello_init);
module_exit(hello_exit);
