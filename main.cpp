/*
RESOURCES FOR I2C AND SENSORS
LTR-303 (Light Sensor) Datasheet: https://cdn-shop.adafruit.com/product-files/5610/LTR-303ALS-01-Lite-On-datasheet-140480318.pdf
DFRobot SEN0193 (Moisture Sensor) Datasheet: https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/2107/SEN0193_Web.pdf
HTU21D (Temp/Humidity Sensor) Datasheet: https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf
I2C API Reference: https://os.mbed.com/docs/mbed-os/v6.16/apis/i2c.html
I2C example: https://os.mbed.com/media/uploads/phill/mbed_course_notes_-_serial_communications_with_i2c.pdf
*/


#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX,USBRX);

I2C lightSensor(p28, p27);
PwmOut LED(p21);
I2C tempHum(p9,p10);
AnalogIn moisture(p20);


float LEDVal;
float ALS_Lux;
float temper;
float rh;

const int lightSensAddr = 0x52;      // 8bit I2C address
const int tempHumAddress = 0x40 << 1;

void lightSensorThread(void const *args){
    char cmd[2];
    char intMode[1];
    int light = 0;
    int lightAndIR;
    int IR;
    char lightVal[4];
    char tempLight[1];
    // Starting ALS
    cmd[0] = 0x80;
    cmd[1] = 0x01;
    lightSensor.write(lightSensAddr,cmd,2);
    wait(0.1);
    while (1) {
        cmd[0] = 0x8C;
        lightSensor.write(lightSensAddr,cmd,1);
        lightSensor.read(lightSensAddr,cmd,1);

        //Checking for new data value
        if ((cmd[0] & 0x04)){
            // Each has to be read 1 byte at a time. All 4 must be read to reset 0x8C new value bit
            // IR Lower Bit
            cmd[0] = 0x88;
            lightSensor.write(lightSensAddr,cmd,1);
            lightSensor.read(lightSensAddr,tempLight,1);
            lightVal[0] = tempLight[0];
            // IR Upper Bit
            cmd[0] = 0x89;
            lightSensor.write(lightSensAddr,cmd,1);
            lightSensor.read(lightSensAddr,tempLight,1);
            lightVal[1] = tempLight[0];
            // Light+IR Lower Bit
            cmd[0] = 0x8A;
            lightSensor.write(lightSensAddr,cmd,1);
            lightSensor.read(lightSensAddr,tempLight,1);
            lightVal[2] = tempLight[0];
            // Light+IR Upper Bit
            cmd[0] = 0x8B;
            lightSensor.write(lightSensAddr,cmd,1);
            lightSensor.read(lightSensAddr,tempLight,1);
            lightVal[3] = tempLight[0];
        }

        IR = ((lightVal[1] << 8) | lightVal[0]);
        lightAndIR = ((lightVal[3] << 8) | lightVal[2]);


        // Formula for Lux: LTR-303ALS-01 Appendix A

        float ratio = float(IR) / (IR + lightAndIR);

        if (ratio < 0.45){
            ALS_Lux = (1.7743 * lightAndIR + 1.1059 * IR);
        }
        else if (ratio < 0.64){
            ALS_Lux = (4.2785 * lightAndIR - 1.9548 * IR);
        }
        else if (ratio < 0.85){
            ALS_Lux = (0.5926 * lightAndIR + 0.1185 * IR);
        }
        else{
            ALS_Lux = 0;
        }
        light = ALS_Lux * 2;
        LEDVal = float(light) / 65535;
        float lightPercent = LEDVal*100;
        pc.printf("Light = %.3f%% \n", lightPercent);
        // pc.printf("Light = %d\n", light);
        // pc.printf("LED = %.3f \n \n \n", LEDVal);
        Thread::wait(500);
    }
}

void tempHumThread(void const *args){
    tempHum.frequency(400000);
    while(1){
        char cmd[1];
        char temperature[2];
        char humidity[2];
        // Preparing to read Temp (HOLD ON)
        cmd[0] = 0xE3;
        tempHum.write(tempHumAddress,cmd,1);
        wait(0.05);
        tempHum.read(tempHumAddress,temperature,2);
        wait(0.001);
        unsigned int t = ((temperature[0] << 8) | temperature[1]);
        t &= 0xFFFC;
        temper = t;
        temper /= 65536.0f;
        temper *= 175.72f;
        temper -= 46.85f;
        temper = temper*1.8 + 32;
        // Preparing to read Humidity (HOLD ON)
        cmd[0] = 0xE5;
        tempHum.write(tempHumAddress, cmd, 1);
        wait(0.05);
        tempHum.read(tempHumAddress,humidity,2);
        wait(0.001);
        // Humidity formula from datasheet
        unsigned int rawHumidity = (humidity[0] << 8) | humidity[1];
        rawHumidity &= 0xFFFC;
        float tempRH = rawHumidity / (float)65536; //2^16 = 65536
        rh = -6 + (125 * tempRH);
        pc.printf("Temp = %.3f F \n", temper);
        pc.printf("Relative Humidity = %.3f \n", rh);
        Thread::wait(500);
    }
}

int main()
{
    temper = 0;
    LED = 0;
    rh = 0;
    pc.printf("Starting... \n");
    wait(1);
    Thread lightThread(lightSensorThread);
    Thread temp_Hum_Thread(tempHumThread);
    Thread::wait(500);
    while(1){
        //Main assigns LED and pulls analog moisture sensor value
        //LED used to display light levels
        
        LED = LEDVal;
        float val = 1 - moisture*1.1; //Output scales from 0-3v instead of 0-3.3v. Output is also inverted. 3.3v = 0 moisture, 0v = submerged.
        pc.printf("Moisture = %3.1f%% \n",val*100);
        pc.printf("\n");
        Thread::wait(500);
    }
}