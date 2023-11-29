# Plant Health Monitor

3 Sensors that track plant health, a uLCD screen to display data, a rotary pulse knob to select sensors and view previously recorded measurements
Bluetooth for remote selection, and Wi-Fi for data recording


Sensors:

Temp/Humidity: HTU21D-F (https://www.adafruit.com/product/1899)
--I2C Operates on 0x40 7-bit address (0x80 8-bit)
--Data measured and recorded in tempHumThread

Lux (Light): LTR-303ALS-01 (https://www.adafruit.com/product/5610)
--I2C Operates on 0x29 7-bit address (0x52 8-bit)
--Data measured and recorded in lightSensorThread

Moisture: DFRobot SEN0193 (https://www.dfrobot.com/product-1385.html)
--Analog sensor. Reports data vales from 0 - 3.0V


Devices:

uLCD: 144-G2 (https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/)

Bluetooth: Bluefruit LE UART (https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/)

Wi-Fi: ESP8266 HUZZAH (https://os.mbed.com/teams/Ece-4180-team-who/wiki/Using-Adafruit-ESP8266-Huzzah)

Rotary Encoder: KY-040 (https://www.rcscomponents.kiev.ua/datasheets/ky-040-datasheet.pdf)
