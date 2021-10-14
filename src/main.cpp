#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include <logging/log.h>
#include <sys/printk.h>

#include "ADS131M08_zephyr.hpp"

#define DATA_READY_GPIO     ((uint8_t)21)

LOG_MODULE_REGISTER(main);

/* Static Functions */
static int gpio_init(void);
static void ads131m08_drdy_cb(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins);

/* Global variables */
const struct device *gpio_0_dev;
struct gpio_callback callback;

ADS131M08 adc;

void main(void)
{
    LOG_ERR("This is a error message!");
    LOG_WRN("This is a warning message!");
    LOG_INF("This is a information message!");
    LOG_DBG("This is a debugging message!");
    int ret = 0;
    uint16_t reg_value = 0;

    ret = gpio_init();
    if (ret == 0){
        LOG_INF("GPIOs Int'd!");        
    }
    
    adc.init(5, 21, 7, 2000000); // cs_pin, drdy_pin, sync_rst_pin, 2MHz SPI bus

    reg_value = adc.readReg(ADS131_ID);
    LOG_INF("ADS131_ID: 0x%X", reg_value);
    reg_value = adc.readReg(ADS131_STATUS);
    LOG_INF("ADS131_STATUS: 0x%X", reg_value);
    reg_value = adc.readReg(ADS131_MODE);
    LOG_INF("ADS131_MODE: 0x%X", reg_value);

    while(1){
        LOG_INF("Hello...");
        k_msleep(3000);
    }
}

static int gpio_init(void){
	int ret = 0;

    gpio_0_dev = device_get_binding("GPIO_0");
	if (gpio_0_dev == NULL) {
		LOG_ERR("***ERROR: GPIO device binding!");
        return -1;
	}    

    ret += gpio_pin_configure(gpio_0_dev, DATA_READY_GPIO, GPIO_INPUT | GPIO_PULL_UP);
    ret += gpio_pin_interrupt_configure(gpio_0_dev, DATA_READY_GPIO, GPIO_INT_EDGE_FALLING);
    gpio_init_callback(&callback, ads131m08_drdy_cb, BIT(DATA_READY_GPIO));    
    ret += gpio_add_callback(gpio_0_dev, &callback);
    if (ret != 0){
        LOG_ERR("***ERROR: GPIO initialization\n");
    }

    return ret;
}

static void ads131m08_drdy_cb(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins){
    LOG_INF("DATA READY!");
}
