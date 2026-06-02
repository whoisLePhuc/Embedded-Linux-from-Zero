#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>

static int my_platform_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct resource *res;

    // TODO: Lấy memory resource (IORESOURCE_MEM, index 0)
    // TODO: Nếu không có resource, trả về -ENXIO
    // TODO: Map resource với devm_ioremap_resource
    // TODO: In thông tin phys addr và size

    dev_info(dev, "Probed successfully\n");
    return 0;
}

static void my_platform_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Removed\n");
}

// TODO: Khai báo of_device_id với compatible "vendor,my-device"
// TODO: Dùng MODULE_DEVICE_TABLE(of, ...)

// TODO: Khai báo platform_driver với probe, remove, driver.name, of_match_table

// TODO: Dùng module_platform_driver() macro

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("Platform driver example with Device Tree");
