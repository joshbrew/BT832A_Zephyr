# BT832A_Zephyr
Working repository for the BT832A test design. 

With PlatformIO installed and the Nordic nRF52 platform installed from the main menu:

Compile the project with `pio run` command from the terminal (`View -> Terminal`)

More details about `pio` commands can be found here [here](https://docs.platformio.org/en/latest/core/quickstart.html#quick-start)

    # Build our environment
    > pio run -e bt832a_ads131m08

    # Upload firmware for our specific environment 
    > pio run -e bt832a_ads131m08 --target upload

There appears to be an MPU-6050 example for Zephyr here https://github.com/intel/zephyr/tree/master/drivers/sensor/mpu6050

For MAX30101 (we are using the 30102 which has identical commands): https://github.com/intel/zephyr/tree/master/drivers/sensor/max30101

Otherwise we need to convert the ADS131M08 drivers and add the MAX30001 drivers.
