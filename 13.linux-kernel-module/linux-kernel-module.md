# Embedded Linux - Linux Kernel Module

> Tài liệu về Linux kernel module development: character drivers, platform drivers, device tree, procfs/sysfs, GPIO, kernel threads, timers (kernel 6.x APIs)

---

## Table of Contents

### [1. Kernel Module Basics](#1-kernel-module-basics)
- [1.1. What is a Kernel Module?](#11-what-is-a-kernel-module)
- [1.2. Module Lifecycle](#12-module-lifecycle)
- [1.3. Module Metadata and Licensing](#13-module-metadata-and-licensing)
- [1.4. Makefile for External Modules](#14-makefile-for-external-modules)
- [1.5. Building and Loading](#15-building-and-loading)

### [2. Kernel APIs and Conventions](#2-kernel-apis-and-conventions)
- [2.1. printk and Logging](#21-printk-and-logging)
- [2.2. Memory Allocation](#22-memory-allocation)
- [2.3. Kernel Data Types](#23-kernel-data-types)
- [2.4. Error Handling](#24-error-handling)

### [3. Character Device Drivers](#3-character-device-drivers)
- [3.1. Device Number Allocation](#31-device-number-allocation)
- [3.2. cdev_init and cdev_add](#32-cdev-init-and-cdev-add)
- [3.3. file_operations (Kernel 6.x)](#33-file-operations-kernel-6x)
- [3.4. Device Create with Class](#34-device-create-with-class)
- [3.5. Complete Character Driver Example](#35-complete-character-driver-example)

### [4. Platform Driver and Device Tree](#4-platform-driver-and-device-tree)
- [4.1. Platform Driver Concept](#41-platform-driver-concept)
- [4.2. Device Tree Binding](#42-device-tree-binding)
- [4.3. of_match_table](#43-of-match-table)
- [4.4. Resource Handling](#44-resource-handling)
- [4.5. Complete Platform Driver](#45-complete-platform-driver)

### [5. procfs and sysfs](#5-procfs-and-sysfs)
- [5.1. procfs with proc_ops](#51-procfs-with-proc-ops)
- [5.2. sysfs with kobject](#52-sysfs-with-kobject)
- [5.3. When to Use Which](#53-when-to-use-which)

### [6. GPIO (Descriptor-based API)](#6-gpio-descriptor-based-api)
- [6.1. gpiod_get / gpiod_put](#61-gpiod-get-gpiod-put)
- [6.2. Direction and Value](#62-direction-and-value)
- [6.3. Interrupt Handling](#63-interrupt-handling)

### [7. Kernel Threads and Timers](#7-kernel-threads-and-timers)
- [7.1. kthread_run / kthread_stop](#71-kthread-run-kthread-stop)
- [7.2. kernel_timer with timer_setup](#72-kernel-timer-with-timer-setup)
- [7.3. Workqueues](#73-workqueues)

### [8. Debugging Kernel Modules](#8-debugging-kernel-modules)
- [8.1. printk and dmesg](#81-printk-and-dmesg)
- [8.2. /proc and debugfs](#82-proc-and-debugfs)
- [8.3. Kernel Oops and Panic](#83-kernel-oops-and-panic)
- [8.4. Useful Tools](#84-useful-tools)

---

## 1. Kernel Module Basics

### 1.1. What is a Kernel Module?

A kernel module is code that can be loaded into the Linux kernel at runtime, extending its functionality without rebooting.

```
User Space:       Applications (processes, threads)
                        │
                        │ System calls
                        ▼
Kernel Space:     ┌──────────────┐
                  │   Kernel     │ ◄── Modules (drivers, filesystems)
                  │   Core       │
                  └──────────────┘
                        │
                        ▼
                  Hardware (CPU, Memory, Devices)
```

**Common use cases:**
- Device drivers (GPIO, I2C, SPI, USB, PCI)
- Filesystem implementations
- Network protocols and filters
- Security modules (SELinux)
- Hardware monitoring

### 1.2. Module Lifecycle

```
insmod mymodule.ko
    │
    ▼
module_init(my_init)  ← called
    │
    │ [Module runs, handles I/O]
    │
rmmod mymodule
    │
    ▼
module_exit(my_exit)  ← called
```

**Module entry and exit:**
```c
#include <linux/module.h>
#include <linux/kernel.h>

static int __init my_init(void) {
    pr_info("Module loaded\n");
    return 0;
}

static void __exit my_exit(void) {
    pr_info("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
```

### 1.3. Module Metadata and Licensing

License is **mandatory** — using GPL is required to access many kernel symbols:

```c
MODULE_LICENSE("GPL");                  // Required — "Proprietary" restricts API access
MODULE_AUTHOR("Your Name <email>");
MODULE_DESCRIPTION("My kernel module");
MODULE_VERSION("1.0");
MODULE_ALIAS("platform:my-device");
```

**License options:**
- `"GPL"` — GNU Public License v2
- `"GPL v2"` — GPL v2 specifically
- `"GPL and additional rights"` — Dual-licensed
- `"Dual BSD/GPL"` — Dual BSD/GPL
- `"Proprietary"` — No GPL symbols (very limited)

### 1.4. Makefile for External Modules

**Standard Makefile (kernel 6.x):**
```makefile
# Makefile — external kernel module
KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m := mymodule.o
mymodule-y := main.o helper.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
```

**For cross-compiled modules:**
```makefile
KDIR ?= /path/to/kernel/build/dir
CROSS_COMPILE ?= arm-linux-gnueabihf-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean
```

### 1.5. Building and Loading

```bash
# Build module
make

# Load module (requires root)
sudo insmod mymodule.ko

# Verify
lsmod | grep mymodule
cat /proc/modules | grep mymodule

# View kernel messages
dmesg | tail

# Remove module
sudo rmmod mymodule

# Module info
modinfo mymodule.ko
# filename:       mymodule.ko
# license:        GPL
# description:    My kernel module
# depends:
```

---

## 2. Kernel APIs and Conventions

### 2.1. printk and Logging

```c
printk(KERN_EMERG   "System is unusable\n");     // Level 0
printk(KERN_ALERT   "Action must be taken\n");     // Level 1
printk(KERN_CRIT    "Critical condition\n");       // Level 2
printk(KERN_ERR     "Error condition\n");          // Level 3
printk(KERN_WARNING "Warning\n");                  // Level 4
printk(KERN_NOTICE  "Normal but significant\n");   // Level 5
printk(KERN_INFO    "Informational\n");            // Level 6
printk(KERN_DEBUG   "Debug message\n");            // Level 7

// Convenience macros:
pr_emerg("...");
pr_alert("...");
pr_crit("...");
pr_err("...");
pr_warn("...");
pr_notice("...");
pr_info("...");
pr_debug("...");    // Only if DEBUG defined

// Dynamic dev_* variants include device info:
dev_info(dev, "Device initialized\n");
dev_err(dev, "Failed to read config: %d\n", ret);
```

### 2.2. Memory Allocation

```c
#include <linux/slab.h>

// GFP flags:
// GFP_KERNEL — Normal (can sleep, process context)
// GFP_ATOMIC — Atomic (cannot sleep, interrupt context)
// GFP_DMA   — DMA-able memory

struct my_data *data;
data = kmalloc(sizeof(*data), GFP_KERNEL);
if (!data)
    return -ENOMEM;

kfree(data);

// Zero-initialized
data = kzalloc(sizeof(*data), GFP_KERNEL);

// Array allocation
data = kmalloc_array(n, sizeof(*data), GFP_KERNEL);
```

### 2.3. Kernel Data Types

```c
// Fixed-size types (always use these, not plain C types)
u8   /* unsigned 8-bit  */          s8   /* signed 8-bit   */
u16  /* unsigned 16-bit */          s16  /* signed 16-bit  */
u32  /* unsigned 32-bit */          s32  /* signed 32-bit  */
u64  /* unsigned 64-bit */          s64  /* signed 64-bit  */

// Kernel-specific
pid_t          /* Process ID */
uid_t          /* User ID */
gid_t          /* Group ID */
dev_t          /* Device number (major+minor) */
loff_t         /* File offset (signed 64-bit) */
size_t         /* Size type */
ssize_t        /* Signed size (for return values) */

// Common variables
int ret;           // Return value (0 = success, negative = error)
unsigned long flags;  // For spinlock flags
size_t count;      // Byte count
loff_t f_pos;      // File position

// Boolean
bool flag = true;  /* or false */
```

### 2.4. Error Handling

Return negative errno values. The user space sees `errno`:

```c
return 0;             // Success
return -ENOMEM;       // Out of memory
return -EINVAL;       // Invalid argument
return -EFAULT;       // Bad address
return -EBUSY;        // Resource busy
return -EIO;          // I/O error
return -ENODEV;       // No such device
return -EPERM;        // Operation not permitted
return -ERESTARTSYS;  // Interrupted by signal (restart syscall)

// PTR_ERR for IS_ERR values
if (IS_ERR(ptr))
    return PTR_ERR(ptr);
```

---

## 3. Character Device Drivers

### 3.1. Device Number Allocation

```c
#include <linux/fs.h>

dev_t devno;
int major, minor;
int ret;

// Method 1: Dynamic allocation (preferred)
ret = alloc_chrdev_region(&devno, 0, 1, "mydriver");
if (ret < 0) {
    pr_err("Failed to allocate device number\n");
    return ret;
}
major = MAJOR(devno);
minor = MINOR(devno);
pr_info("Allocated dev %d:%d\n", major, minor);

// Method 2: Static (known major number)
// devno = MKDEV(major, minor);
// ret = register_chrdev_region(devno, count, "mydriver");

// Cleanup
unregister_chrdev_region(devno, 1);
```

### 3.2. cdev_init and cdev_add

```c
#include <linux/cdev.h>

static struct cdev my_cdev;

// Step 1: Initialize cdev with file_operations
cdev_init(&my_cdev, &my_fops);
my_cdev.owner = THIS_MODULE;

// Step 2: Add to system
ret = cdev_add(&my_cdev, devno, 1);
if (ret < 0) {
    unregister_chrdev_region(devno, 1);
    return ret;
}

// Cleanup
cdev_del(&my_cdev);
unregister_chrdev_region(devno, 1);
```

### 3.3. file_operations (Kernel 6.x)

```c
#include <linux/fs.h>

static int my_open(struct inode *inode, struct file *filp) {
    // Get per-device data from inode
    struct my_data *data = container_of(inode->i_cdev, struct my_data, cdev);
    filp->private_data = data;
    return 0;
}

static int my_release(struct inode *inode, struct file *filp) {
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf,
                       size_t len, loff_t *off) {
    struct my_data *data = filp->private_data;
    char kbuf[64];
    size_t available = sizeof(kbuf);

    if (*off >= available)
        return 0;
    if (len > available - *off)
        len = available - *off;

    snprintf(kbuf, sizeof(kbuf), "value=%d\n", data->value);

    if (copy_to_user(buf, kbuf + *off, len))
        return -EFAULT;

    *off += len;
    return len;
}

static ssize_t my_write(struct file *filp, const char __user *buf,
                        size_t len, loff_t *off) {
    struct my_data *data = filp->private_data;
    char kbuf[64];

    if (len > sizeof(kbuf) - 1)
        len = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';
    if (sscanf(kbuf, "%d", &data->value) == 1)
        pr_info("Set value to %d\n", data->value);

    return len;
}

static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct my_data *data = filp->private_data;
    int tmp;

    switch (cmd) {
    case MY_GET_VALUE:
        tmp = data->value;
        if (copy_to_user((void __user *)arg, &tmp, sizeof(tmp)))
            return -EFAULT;
        break;
    case MY_SET_VALUE:
        if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp)))
            return -EFAULT;
        data->value = tmp;
        break;
    default:
        return -EINVAL;
    }
    return 0;
}

// IMPORTANT: Use unlocked_ioctl in kernel 6.x (not ioctl)
static const struct file_operations my_fops = {
    .owner          = THIS_MODULE,
    .open           = my_open,
    .release        = my_release,
    .read           = my_read,
    .write          = my_write,
    .unlocked_ioctl = my_ioctl,
};
```

### 3.4. Device Create with Class

Automatically creates `/dev/mydriver` via devtmpfs:

```c
#include <linux/device.h>

static struct class *my_class;
static struct device *my_device;

static int __init my_init(void) {
    // ... allocate chrdev, cdev_add ...

    // Create class
    my_class = class_create(THIS_MODULE, "myclass");
    if (IS_ERR(my_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(my_class);
    }

    // Create device node
    my_device = device_create(my_class, NULL, devno,
                              NULL, "mydriver");
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(my_device);
    }

    return 0;
}

static void __exit my_exit(void) {
    device_destroy(my_class, devno);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(devno, 1);
}
```

### 3.5. Complete Character Driver Example

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"

static int major;
static struct class *chardev_class;
static struct cdev chardev_cdev;

struct chardev_data {
    unsigned long counter;
};

static struct chardev_data *chardev_data;

static int chardev_open(struct inode *inode, struct file *filp) {
    filp->private_data = chardev_data;
    pr_info("Device opened\n");
    return 0;
}

static int chardev_release(struct inode *inode, struct file *filp) {
    pr_info("Device closed\n");
    return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buf,
                            size_t len, loff_t *off) {
    struct chardev_data *data = filp->private_data;
    char tmp[32];
    int n;

    n = snprintf(tmp, sizeof(tmp), "%lu\n", data->counter);
    if (*off >= n)
        return 0;
    if (len > (size_t)(n - *off))
        len = n - *off;

    if (copy_to_user(buf, tmp + *off, len))
        return -EFAULT;

    *off += len;
    data->counter++;
    return len;
}

static const struct file_operations chardev_fops = {
    .owner   = THIS_MODULE,
    .open    = chardev_open,
    .release = chardev_release,
    .read    = chardev_read,
};

static int __init chardev_init(void) {
    dev_t dev;
    int ret;

    chardev_data = kzalloc(sizeof(*chardev_data), GFP_KERNEL);
    if (!chardev_data)
        return -ENOMEM;

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate dev\n");
        goto err_region;
    }
    major = MAJOR(dev);

    cdev_init(&chardev_cdev, &chardev_fops);
    chardev_cdev.owner = THIS_MODULE;
    ret = cdev_add(&chardev_cdev, dev, 1);
    if (ret < 0) {
        pr_err("Failed to add cdev\n");
        goto err_cdev;
    }

    chardev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        ret = PTR_ERR(chardev_class);
        goto err_class;
    }

    device_create(chardev_class, NULL, dev, NULL, DEVICE_NAME);
    pr_info("Module loaded: major=%d\n", major);
    return 0;

err_class:
    cdev_del(&chardev_cdev);
err_cdev:
    unregister_chrdev_region(dev, 1);
err_region:
    kfree(chardev_data);
    return ret;
}

static void __exit chardev_exit(void) {
    device_destroy(chardev_class, MKDEV(major, 0));
    class_destroy(chardev_class);
    cdev_del(&chardev_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
    kfree(chardev_data);
    pr_info("Module unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Character device driver example");
```

---

## 4. Platform Driver and Device Tree

### 4.1. Platform Driver Concept

Platform drivers handle devices that are discoverable without a hardware bus (enumerated by firmware/device tree).

```
Device Tree → platform_device → platform_driver → probe
```

### 4.2. Device Tree Binding

```dts
// my-device.dtsi
/ {
    my_device: my-device@1c00000 {
        compatible = "vendor,my-device";
        reg = <0x1c00000 0x1000>;
        interrupts = <0 42 4>;
        reset-gpios = <&gpio1 3 GPIO_ACTIVE_LOW>;
        status = "okay";
    };
};
```

### 4.3. of_match_table

```c
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>

static const struct of_device_id my_of_match[] = {
    { .compatible = "vendor,my-device", },
    { }  // Sentinel — must be empty
};
MODULE_DEVICE_TABLE(of, my_of_match);
```

### 4.4. Resource Handling

```c
static int my_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    struct resource *res;

    // Get memory region from reg property
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENXIO;

    // Get interrupts
    int irq = platform_get_irq(pdev, 0);
    if (irq < 0)
        return irq;

    // Request and map memory
    void __iomem *base = devm_ioremap_resource(dev, res);
    if (IS_ERR(base))
        return PTR_ERR(base);

    pr_info("Probed: phys=%pa, size=%zu, irq=%d\n",
            &res->start, resource_size(res), irq);
    return 0;
}
```

### 4.5. Complete Platform Driver

```c
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>

static int my_platform_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    struct resource *res;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENXIO;

    dev_info(dev, "Probed: phys=0x%llx, size=%zu\n",
             (unsigned long long)res->start, resource_size(res));
    return 0;
}

static void my_platform_remove(struct platform_device *pdev) {
    dev_info(&pdev->dev, "Removed\n");
}

static const struct of_device_id my_of_match[] = {
    { .compatible = "vendor,my-device" },
    { }
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_platform_driver = {
    .probe  = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "my-platform-driver",
        .of_match_table = my_of_match,
    },
};

module_platform_driver(my_platform_driver);
MODULE_LICENSE("GPL");
```

---

## 5. procfs and sysfs

### 5.1. procfs with proc_ops

Kernel 5.6+ uses `struct proc_ops` (not `file_operations`) for /proc files:

```c
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int my_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "Counter: %lu\n", counter);
    seq_printf(m, "Jiffies: %lu\n", jiffies);
    return 0;
}

static int my_proc_open(struct inode *inode, struct file *filp) {
    return single_open(filp, my_proc_show, NULL);
}

static const struct proc_ops my_proc_fops = {
    .proc_open    = my_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init my_init(void) {
    proc_create("mydriver", 0444, NULL, &my_proc_fops);
    return 0;
}

static void __exit my_exit(void) {
    proc_remove(THIS_MODULE);
}
```

### 5.2. sysfs with kobject

```c
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct kobject *my_kobj;
static int my_value;

static ssize_t my_value_show(struct kobject *kobj,
                             struct kobj_attribute *attr, char *buf) {
    return sysfs_emit(buf, "%d\n", my_value);
}

static ssize_t my_value_store(struct kobject *kobj,
                              struct kobj_attribute *attr,
                              const char *buf, size_t count) {
    if (kstrtoint(buf, 0, &my_value))
        return -EINVAL;
    return count;
}

static struct kobj_attribute my_value_attr =
    __ATTR(my_value, 0644, my_value_show, my_value_store);

static struct attribute *my_attrs[] = {
    &my_value_attr.attr,
    NULL,
};

static const struct attribute_group my_attr_group = {
    .attrs = my_attrs,
};

static int __init my_init(void) {
    int ret;

    my_kobj = kobject_create_and_add("mydriver", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;

    ret = sysfs_create_group(my_kobj, &my_attr_group);
    if (ret) {
        kobject_put(my_kobj);
        return ret;
    }
    return 0;
}

static void __exit my_exit(void) {
    sysfs_remove_group(my_kobj, &my_attr_group);
    kobject_put(my_kobj);
}
```

Using sysfs:
```bash
# Read
cat /sys/kernel/mydriver/my_value
# 42

# Write
echo 100 > /sys/kernel/mydriver/my_value
```

### 5.3. When to Use Which

| Interface | Visibility | Use Case |
|-----------|-----------|----------|
| **/proc** | Per-process or system-wide | Read-only status, debug info |
| **/sys** | Kernel objects hierarchy | Tunable parameters, device attributes |
| **/dev** | Device nodes | Data I/O (read/write/ioctl) |

---

## 6. GPIO (Descriptor-based API)

The modern GPIO API uses descriptors (`gpiod_*`), NOT the old integer-based (`gpio_*`) API.

### 6.1. gpiod_get / gpiod_put

```c
#include <linux/gpio/consumer.h>

struct gpio_desc *gpio;

// Get GPIO by con_id from device tree
gpio = gpiod_get(dev, "led", GPIOD_OUT_LOW);
if (IS_ERR(gpio))
    return PTR_ERR(gpio);

// Get optional GPIO (returns NULL if missing)
gpio = gpiod_get_optional(dev, "reset", GPIOD_OUT_HIGH);

// Indexed GPIO
gpio = gpiod_get_index(dev, "data", 0, GPIOD_IN);

// Release
gpiod_put(gpio);

// For raw GPIO number (when no DT)
// gpio = gpio_to_desc(42);  // Not recommended
```

### 6.2. Direction and Value

```c
// Set direction
gpiod_direction_input(gpio);
gpiod_direction_output(gpio, 0);  // Initial value 0

// Get value
int val = gpiod_get_value(gpio);

// Set value
gpiod_set_value(gpio, 1);  // High
gpiod_set_value(gpio, 0);  // Low

// Active-low handling (automatic)
// If DT has GPIO_ACTIVE_LOW, gpiod_get_value inverts automatically
```

### 6.3. Interrupt Handling

```c
#include <linux/interrupt.h>

static irqreturn_t my_gpio_irq_handler(int irq, void *data) {
    struct my_data *priv = data;
    priv->irq_count++;
    pr_info("GPIO interrupt #%d\n", priv->irq_count);
    return IRQ_HANDLED;      // or IRQ_WAKE_THREAD for threaded
}

static int my_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    struct gpio_desc *gpio;
    int irq, ret;

    gpio = gpiod_get(dev, "interrupt", GPIOD_IN);
    if (IS_ERR(gpio))
        return PTR_ERR(gpio);

    irq = gpiod_to_irq(gpio);
    if (irq < 0)
        return irq;

    ret = request_irq(irq, my_gpio_irq_handler,
                      IRQF_TRIGGER_RISING, "my_gpio", priv);
    if (ret)
        return ret;

    priv->irq = irq;
    return 0;
}
```

---

## 7. Kernel Threads and Timers

### 7.1. kthread_run / kthread_stop

```c
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

static struct task_struct *my_thread;

static int my_thread_worker(void *data) {
    struct my_data *priv = data;
    int count = 0;

    while (!kthread_should_stop()) {
        pr_info("Thread iteration %d, value=%d\n", count++, priv->value);

        // Sleep without busy-waiting
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(msecs_to_jiffies(1000));
    }

    pr_info("Thread stopping\n");
    return 0;
}

static int __init my_init(void) {
    my_thread = kthread_run(my_thread_worker, priv, "my_kthread");
    if (IS_ERR(my_thread))
        return PTR_ERR(my_thread);
    return 0;
}

static void __exit my_exit(void) {
    if (my_thread)
        kthread_stop(my_thread);  // Sets should_stop, waits for exit
}
```

### 7.2. kernel_timer with timer_setup

Modern timer API (kernel 4.15+). `timer_setup` replaces deprecated `setup_timer`:

```c
#include <linux/timer.h>

struct my_data {
    int value;
    struct timer_list timer;
};

static void my_timer_callback(struct timer_list *t) {
    struct my_data *data = from_timer(data, t, timer);
    pr_info("Timer fired, value=%d\n", data->value);

    // Periodic: re-arm timer
    mod_timer(t, jiffies + msecs_to_jiffies(500));
}

static int __init my_init(void) {
    // Initialize timer
    timer_setup(&data->timer, my_timer_callback, 0);

    // Set expiry: 500ms from now
    data->timer.expires = jiffies + msecs_to_jiffies(500);
    add_timer(&data->timer);

    // Or in one call:
    // mod_timer(&data->timer, jiffies + msecs_to_jiffies(500));
    return 0;
}

static void __exit my_exit(void) {
    del_timer_sync(&data->timer);  // Safe: waits for handler to finish
}
```

### 7.3. Workqueues

For deferred work that can sleep (unlike timers which run in atomic context):

```c
#include <linux/workqueue.h>

struct my_data {
    struct work_struct work;
    int value;
};

static void my_work_handler(struct work_struct *work) {
    struct my_data *data = container_of(work, struct my_data, work);
    pr_info("Work handler: value=%d\n", data->value);
    // Can sleep here (unlike timer callbacks)
    msleep(100);
}

// Schedule work
schedule_work(&data->work);   // In default system_wq

// Or dedicated workqueue
static struct workqueue_struct *my_wq;
my_wq = create_singlethread_workqueue("my_wq");
queue_work(my_wq, &data->work);
```

---

## 8. Debugging Kernel Modules

### 8.1. printk and dmesg

```bash
# View kernel messages
dmesg
dmesg -w          # Follow (like tail -f)
dmesg -l err      # Only error level
dmesg -n 7        # Set console log level (show all)

# Clear
dmesg -C

# Control log level
echo 8 > /proc/sys/kernel/printk
```

### 8.2. /proc and debugfs

```bash
# View loaded modules
cat /proc/modules

# View registered devices
cat /proc/devices

# DebugFS (mounted as /sys/kernel/debug)
mount -t debugfs none /sys/kernel/debug
cat /sys/kernel/debug/gpio
```

### 8.3. Kernel Oops and Panic

When a module crashes, the kernel produces an Oops message:

```
Unable to handle kernel NULL pointer dereference at virtual address ...
Internal error: Oops: 5 [#1] SMP ARM
CPU: 0 PID: 42 Comm: insmod Tainted: P
PC is at my_function+0x14/0x30 [mymodule]
...
[<bf000014>] (my_function [mymodule]) from [<bf00002c>] (my_init+0x8/0x20 [mymodule])
...
```

**Key information from Oops:**
- **PC** — Program Counter (where it crashed)
- **Call trace** — Function call stack
- **Tainted** — Module loaded (P: Proprietary, G: GPL)
- **Address in brackets** — `[mymodule]` shows which module

### 8.4. Useful Tools

```bash
# List loaded modules with memory addresses
cat /proc/modules

# Module info
modinfo mymodule.ko

# Kernel log monitor
dmesg -wH

# Memory pressure
cat /proc/meminfo

# Interrupt statistics
cat /proc/interrupts

# GPIO status
cat /sys/kernel/debug/gpio

# Kernel config check
zcat /proc/config.gz | grep CONFIG_MODULES

# Object dump for symbol check
arm-linux-gnueabihf-objdump -t mymodule.ko
arm-linux-gnueabihf-nm mymodule.ko
```

---

## 📚 References

- [Linux Kernel Documentation — Device Drivers](https://www.kernel.org/doc/html/latest/driver-api/)
- [Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)
- [gpiod API](https://www.kernel.org/doc/html/latest/driver-api/gpio/consumers.html)
- [Device Tree Usage](https://elinux.org/Device_Tree_Usage)
- [Linux Cross-Reference (Elixir)](https://elixir.bootlin.com/linux/latest/source)
