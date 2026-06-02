#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>

static int my_platform_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct resource *res;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(dev, "No memory resource\n");
        return -ENXIO;
    }

    if (IS_ERR(devm_ioremap_resource(dev, res)))
        return PTR_ERR(devm_ioremap_resource(dev, res));

    dev_info(dev, "Probed: phys=0x%llx, size=%zu\n",
             (unsigned long long)res->start, resource_size(res));
    return 0;
}

static void my_platform_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Removed\n");
}

static const struct of_device_id my_of_match[] = {
    { .compatible = "vendor,my-device" },
    { }
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_platform_driver = {
    .probe  = my_platform_probe,
    .remove = my_platform_remove,
    .driver = {
        .name = "my-platform-driver",
        .of_match_table = my_of_match,
    },
};

module_platform_driver(my_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Team");
MODULE_DESCRIPTION("Platform driver example with Device Tree");
