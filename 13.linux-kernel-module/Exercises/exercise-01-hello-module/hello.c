#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
    // TODO: In "Hello, World!" ra kernel log
    return 0;
}

static void __exit hello_exit(void)
{
    // TODO: In "Goodbye!" ra kernel log
}

module_init(hello_init);
module_exit(hello_exit);

// TODO: Thay "Your Name" và "Description" phù hợp
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple hello world kernel module");
