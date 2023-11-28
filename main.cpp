/*
RESOURCES FOR I2C AND LIGHT SENSOR
LTR-303 Datasheet: https://cdn-shop.adafruit.com/product-files/5610/LTR-303ALS-01-Lite-On-datasheet-140480318.pdf
HTU21D (Temp/Humidity) Datasheet: https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf
DFRobot SEN0193 (Moisture) Datasheet: https://www.digikey.com/en/products/detail/dfrobot/SEN0193/6588605
I2C API Reference: https://os.mbed.com/docs/mbed-os/v6.16/apis/i2c.html
I2C example: https://os.mbed.com/media/uploads/phill/mbed_course_notes_-_serial_communications_with_i2c.pdf
*/

#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"

//Serial pc(USBTX,USBRX);
uLCD_4DGL uLCD(p13,p14,p15);

I2C lightSensor(p28, p27);
//PwmOut LED(p21);
I2C tempHum(p9,p10);
AnalogIn moistureSensor(p20);
Mutex LCD;
InterruptIn RPG_A(p25);//encoder A and B pins/bits use interrupts
InterruptIn RPG_B(p24);
PinDetect RPG_PB(p23); //encode pushbutton switch "SW" on PCB

float LEDVal;
float ALS_Lux;
float temper;
float rh;
float lightPercent;
volatile bool menu;

volatile int old_enc = 0;
volatile int enc_count = 2;

const int lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

void Enc_change_ISR(void)
{
    int new_enc = RPG_A<<1 | RPG_B;//current encoder bits
    //check truth table for -1,0 or +1 added to count
    enc_count = enc_count + (lookup_table[old_enc<<2 | new_enc]);
    old_enc = new_enc;
    if (enc_count>15) enc_count = 0;
    if (enc_count<0) enc_count = 15;
}
//debounced RPG pushbutton switch callback
void PB_callback(void)
{
    //pc.printf("Callback");
    menu = !menu;
    LCD.lock();
    uLCD.cls();
    LCD.unlock();
    // menu = !menu;
    // pc.printf("Bool Changed \n");
    // wait(.1);
    // pc.printf("Wait Over \n");
    // LCD.lock();
    // uLCD.cls();
    // enc_count = 0;
    // LCD.unlock();
}


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
    lightSensor.write(0x52,cmd,2);
    Thread::wait(100);
    while (1) {
        cmd[0] = 0x8C;
        lightSensor.write(0x52,cmd,1);
        lightSensor.read(0x52,cmd,1);

        //Checking for new data value
        if ((cmd[0] & 0x04)){
            // Each has to be read 1 byte at a time. All 4 must be read to reset 0x8C new value bit
            // IR Lower Bit
            cmd[0] = 0x88;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[0] = tempLight[0];
            // IR Upper Bit
            cmd[0] = 0x89;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[1] = tempLight[0];
            // Light+IR Lower Bit
            cmd[0] = 0x8A;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[2] = tempLight[0];
            // Light+IR Upper Bit
            cmd[0] = 0x8B;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
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
        lightPercent = LEDVal*100;
        //pc.printf("Light = %.1f%% \n", lightPercent);
        // pc.printf("Light = %d\n", light);
        // pc.printf("LED = %.3f \n \n \n", LEDVal);
        Thread::wait(1000);
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
        tempHum.write(0x80,cmd,1);
        Thread::wait(50);
        tempHum.read(0x80,temperature,2);
        Thread::wait(1);
        unsigned int t = ((temperature[0] << 8) | temperature[1]);
        t &= 0xFFFC;
        //t = t >> 2;
        temper = t;
        temper /= 65536.0f;
        temper *= 175.72f;
        temper -= 46.85f;
        temper = temper*1.8 + 32;
        // Preparing to read Humidity (HOLD ON)
        cmd[0] = 0xE5;
        tempHum.write(0x80, cmd, 1);
        Thread::wait(50);
        tempHum.read(0x80,humidity,2);
        Thread::wait(1);
        // Humidity formula from datasheet
        unsigned int rawHumidity = (humidity[0] << 8) | humidity[1];
        rawHumidity &= 0xFFFC;
        float tempRH = rawHumidity / (float)65536; //2^16 = 65536
        rh = -6 + (125 * tempRH);
        //pc.printf("Temp = %.1f F \n", temper);
        //pc.printf("Relative Humidity = %.1f \n", rh);
        Thread::wait(5000);
    }
}

int main()
{
    // Initialize variables
    temper = 0;
    //LED = 0;
    rh = 0;
    menu = true;
    // Starting Screen
    LCD.lock();
    uLCD.locate(5,7);
    uLCD.text_height(2);
    uLCD.printf("Starting...");
    LCD.unlock();
    //pc.printf("Starting... \n");
    wait(1);
    RPG_A.mode(PullUp);
    RPG_B.mode(PullUp);
    RPG_PB.mode(PullDown);
    RPG_PB.attach_deasserted(&PB_callback);
    RPG_PB.setSampleFrequency(20000);
    // generate an interrupt on any change in either encoder bit (A or B)
    RPG_A.rise(&Enc_change_ISR);
    RPG_A.fall(&Enc_change_ISR);
    RPG_B.rise(&Enc_change_ISR);
    RPG_B.fall(&Enc_change_ISR);
    // Starting Threads
    Thread lightThread(lightSensorThread);
    Thread temp_Hum_Thread(tempHumThread);
    LCD.lock();
    uLCD.cls();
    LCD.unlock();
    int prevSel = 0;
    while(1){
        //Main assigns LED and pulls analog moisture sensor value
        //LED used to display light levels
        //LED = LEDVal;
        float moisture = 1 - moistureSensor*1.1; //Output scales from 0-3v instead of 0-3.3v. Output is also inverted. 3.3v = 0 moisture, 0v = submerged.
        float moisturePercent = moisture*100;
        if(menu){
            int selLoc = (int(enc_count / 4)) << 4;
            LCD.lock();
            uLCD.locate(7,0);
            uLCD.text_height(1);
            uLCD.color(0xEFFD5F);
            uLCD.printf("MENU");
            uLCD.color(WHITE);
            // Printing Data values
            uLCD.locate(1,4);
            uLCD.printf("Light: %.1f%%  ", lightPercent);
            uLCD.locate(1,6);
            uLCD.printf("Temp: %.1f F  ", temper);
            uLCD.locate(1,8);
            uLCD.printf("Humidity: %.1f%% ", rh);
            uLCD.locate(1,10);
            uLCD.printf("Moisture: %.1f%%  ",moisturePercent);
            if(prevSel != selLoc){
                uLCD.filled_rectangle(0, 30+prevSel, 5, 40+prevSel, 0x000000);
                prevSel = selLoc;
            }
            // Printing selection square
            uLCD.filled_rectangle(0,30+selLoc,5, 40+selLoc, 0xFFFF20);
            LCD.unlock();
        }
        Thread::wait(500);
        //pc.printf("enc_count: %d \n", enc_count);
    }
}