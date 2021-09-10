# BT832A_Zephyr
Working repository for the BT832A test design

Place the bt832a_ads131m08.json in `C:/Users/[you]/.platformio/platforms/nordicnrf52/boards`

Open the folder in vscode with platformIO extension installed and then click the platformIO start button (tiny home icon on bottom bar)

There appears to be an MPU-6050 example for Zephyr here https://github.com/intel/zephyr/tree/master/drivers/sensor/mpu6050

For MAX30101 (we are using the 30102 which has identical commands): https://github.com/intel/zephyr/tree/master/drivers/sensor/max30101

Otherwise we need to convert the ADS131M08 drivers and add the MAX30001 drivers.