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
#include <string>
#include <cstdlib>

RawSerial pc(USBTX,USBRX);
uLCD_4DGL uLCD(p13,p14,p15);

I2C lightSensor(p28, p27);
DigitalOut LED(p23);
I2C tempHum(p9,p10);
AnalogIn moistureSensor(p20);
Mutex LCD;
Mutex SerialHold;
InterruptIn RPG_A(p16);//encoder A and B pins/bits use interrupts
InterruptIn RPG_B(p17);
DigitalIn RPG_PB(p18); //encode pushbutton switch "SW" on PCB
PwmOut speaker(p22);
BusOut mbedleds(LED1, LED2, LED3, LED4); // initialize LEDs
DigitalIn alarmSwitch(p30);

bool alarm = true;

double moistureAlarmThresh = 30.0;
char pcIn[2];
char* pEnd;

float LEDVal;
float ALS_Lux;
float temper;
float rh;
float lightPercent;
volatile bool menu;
volatile int saveSelection;

float tempData[10];
float humData[10];
float lightData[10];
float moistureData[10];

volatile int old_enc = 0;
volatile int enc_count = 2;
volatile int selLoc;

const int lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

void Enc_change_ISR(void){
    int new_enc = RPG_A<<1 | RPG_B;//current encoder bits
    //check truth table for -1,0 or +1 added to count
    enc_count = enc_count + (lookup_table[old_enc<<2 | new_enc]);
    old_enc = new_enc;
    if (enc_count>15) enc_count = 0;
    if (enc_count<0) enc_count = 15;
}
//debounced RPG pushbutton switch callback
void PB_callback(void){
    if (menu){
        saveSelection = enc_count >> 2;
    }
    menu = !menu;
    LCD.lock();
    uLCD.cls();
    LCD.unlock();
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
            // CH1_0 (IR Lower Bit)
            cmd[0] = 0x88;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[0] = tempLight[0];
            // CH1_1 (IR Upper Bit)
            cmd[0] = 0x89;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[1] = tempLight[0];
            // CH_0_0 (Light+IR Lower Bit)
            cmd[0] = 0x8A;
            lightSensor.write(0x52,cmd,1);
            lightSensor.read(0x52,tempLight,1);
            lightVal[2] = tempLight[0];
            // CH0_1 (Light+IR Upper Bit)
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
        light = ALS_Lux * 4;
        LEDVal = float(light) / 64000;
        lightPercent = LEDVal*100;
        if (lightPercent > 100){
            lightPercent = 100.0;
        }
        for(int i = 9; i > 0; i--){
            lightData[i] = lightData[i - 1];
        }
        lightData[0] = lightPercent;
        SerialHold.lock();
        pc.printf("Light = %.1f%% \n", lightPercent);
        SerialHold.unlock();
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
        for(int i = 9; i > 0; i--){
            tempData[i] = tempData[i - 1];
        }
        tempData[0] = temper;
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
        for(int i = 9; i > 0; i--){
            humData[i] = humData[i - 1];
        }
        humData[0] = rh;
        SerialHold.lock();
        pc.printf("Temp = %.1f F \n", temper);
        pc.printf("Relative Humidity = %.1f%% \n", rh);
        SerialHold.unlock();
        Thread::wait(5000);
    }
}

int main(){
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
    for (int i = 0; i < 8; i++){
        mbedleds = pow(2.0,double(i%4));
        wait(0.1);
    }
    for (int i = 0; i < 2; i++){
        mbedleds = 0;
        wait(0.1);
        mbedleds = 15;
        wait(0.1);
    }
    mbedleds = 0;
    RPG_A.mode(PullUp);
    RPG_B.mode(PullUp);
    RPG_PB.mode(PullUp);
    alarmSwitch.mode(PullUp);
    //RPG_PB.attach_deasserted(&PB_callback);
    //RPG_PB.setSampleFrequency();
    // generate an interrupt on any change in either encoder bit (A or B)
    RPG_A.rise(&Enc_change_ISR);
    RPG_A.fall(&Enc_change_ISR);
    RPG_B.rise(&Enc_change_ISR);
    RPG_B.fall(&Enc_change_ISR);
    // Starting Threads
    Thread lightThread(lightSensorThread);
    Thread temp_Hum_Thread(tempHumThread);
    //PB_callback();
    LCD.lock();
    uLCD.cls();
    LCD.unlock();
    int prevSel = 0;
    float moisture;
    float moisturePercent;
    while(1){
        alarm = alarmSwitch;
        LED = alarm;
        if (pc.readable()) {     // if they have pressed a key
            for (int i = 0; i < 2; i++){
                pcIn[i] = pc.getc();
            }
            moistureAlarmThresh = strtof(pcIn, &pEnd);
            SerialHold.lock();
            pc.printf("%f \n", moistureAlarmThresh);
            SerialHold.unlock();
        }
        //Main assigns LED and pulls analog moisture sensor value
        moisture = 1 - moistureSensor*1.1; //Output scales from 0-3v instead of 0-3.3v. Output is also inverted. 3.3v = 0 moisture, 0v = submerged.
        moisturePercent = (moisture*100 - 14) * 30/13;
        if (moisturePercent > 100){
            moisturePercent = 100.0;
        }
        SerialHold.lock();
        pc.printf("Moisture = %.1f%% \n", moisturePercent);
        SerialHold.unlock();
        for(int i = 9; i > 0; i--){
            moistureData[i] = moistureData[i - 1];
        }
        if (moisturePercent < moistureAlarmThresh){
            speaker = alarm*0.5;
        }
        else{
            speaker = 0;
        }
        moistureData[0] = moisturePercent;
        if (!RPG_PB){
            PB_callback();
        }
        if(menu){
            selLoc = (enc_count >> 2) << 4;
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
        else{
            switch (saveSelection){
                case 0:
                    LCD.lock();
                    uLCD.locate(7,0);
                    uLCD.text_height(1);
                    uLCD.color(0xEFFD5F);
                    uLCD.printf("LIGHT");
                    uLCD.color(WHITE);
                    for(int i = 0; i < 10; i++){
                        if (lightData[i] == 0){
                            break;
                        }
                        uLCD.locate(0,2+i);
                        uLCD.printf("%d: %.1f%%",i,lightData[i]);
                    }
                    break;
                case 1:
                    LCD.lock();
                    uLCD.locate(7,0);
                    uLCD.text_height(1);
                    uLCD.color(0xEFFD5F);
                    uLCD.printf("TEMP");
                    uLCD.color(WHITE);
                    for(int i = 0; i < 10; i++){
                        if (tempData[i] == 0){
                            break;
                        }
                        uLCD.locate(0,2+i);
                        uLCD.printf("%d: %.1f%%",i,tempData[i]);
                    }
                    break;
                case 2:
                    LCD.lock();
                    uLCD.locate(7,0);
                    uLCD.text_height(1);
                    uLCD.color(0xEFFD5F);
                    uLCD.printf("HUMIDITY");
                    uLCD.color(WHITE);
                    for(int i = 0; i < 10; i++){
                        if (humData[i] == 0){
                            break;
                        }
                        uLCD.locate(0,2+i);
                        uLCD.printf("%d: %.1f%%",i,humData[i]);
                    }
                    break;
                case 3:
                    LCD.lock();
                    uLCD.locate(7,0);
                    uLCD.text_height(1);
                    uLCD.color(0xEFFD5F);
                    uLCD.printf("MOISTURE");
                    uLCD.color(WHITE);
                    for(int i = 0; i < 10; i++){
                        if (moistureData[i] == 0){
                            break;
                        }
                        uLCD.locate(0,2+i);
                        uLCD.printf("%d: %.1f%%",i,moistureData[i]);
                    }
                    break;
                default:
                    break;
            }
        }
        Thread::wait(500);
        //pc.printf("enc_count: %d \n", enc_count);
    }
}