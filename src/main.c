#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   2000


void main(void)
{
    const struct device *gpio_0_dev;

	bool gpio_is_on = true;
	int ret;

	gpio_0_dev = device_get_binding("GPIO_0");
	if (gpio_0_dev == NULL) {
		return;
	}    

	ret = gpio_pin_configure(gpio_0_dev, 18, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	while (1) {
        gpio_pin_set(gpio_0_dev, 18, (int)gpio_is_on);
		gpio_is_on = !gpio_is_on;
		k_msleep(SLEEP_TIME_MS);
	}
}
