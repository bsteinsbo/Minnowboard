/*
 * Minnowboard Max pps gpio board file.
 * Copyright (c) 2015, Bjarne Steinsbo
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * Based on low-speed-spidev.c by California Sullivan <california.l.sullivan@intel.com>
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/pps-gpio.h>

#if (!defined(CONFIG_PPS_CLIENT_GPIO_MODULE) && !defined(CONFIG_PPS_CLIENT_GPIO))
        #error PPS_CLIENT_GPIO is required.
#endif

/* Change this values to what your board uses. */
#define PPS_GPIO 483

static struct pps_gpio_platform_data pps_gpio_info = {
	.assert_falling_edge	= false,
	.capture_clear		= false,
	.gpio_pin		= PPS_GPIO,
	.gpio_label		= "PPS",
};

static struct platform_device pps_gpio_device = {
 	.name	= "pps-gpio",
	.id	= -1,
	.dev	= {
        	.platform_data	= &pps_gpio_info,
 	},
};

static struct platform_device *dev;

static int __init minnow_pps_gpio_module_init(void)
{
	int err;

	pr_info("minnow-pps-gpio module init\n");

	err = request_module("pps-gpio");
	if (err) {
		pr_err("pps-gpio module is unavailable.\n");
		goto out;
	}

	err = platform_device_register(&pps_gpio_device);
	if (err)
		goto out;

	pr_info("minnow-pps-gpio device registered\n");
	dev = &pps_gpio_device;
	err = 0;

 out:
	if (err)
		pr_err("Failed to register minnow-pps-gpio device\n");
	return err;
}

static void __exit minnow_pps_gpio_module_exit(void)
{
	pr_info("minnow-pps-gpio module exit");
	if (dev)
		platform_device_unregister(&pps_gpio_device);
}

module_init(minnow_pps_gpio_module_init);
module_exit(minnow_pps_gpio_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bjarne Steinsbo <bsteinsbo@gmail.com>");
MODULE_DESCRIPTION("Minnowboard PPS GPIO Board File");
