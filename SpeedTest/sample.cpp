//
// sample.cpp
//
// A sample program for Steve Hassenplug's speed benchmark test
// (http://www.teamhassenplug.org/NXT/NXTSoftwareSpeedTest.html)
//
/*
#include <stdlib.h>

// ECRobot++ API
#include "TouchSensor.h"
#include "LightSensor.h"
//#include "SonarSensor.h"
#include "Motor.h"
#include "Lcd.h"
#include "Clock.h"
using namespace ecrobot;


extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

TouchSensor touch(PORT_2);
LightSensor light(PORT_3);
//SonarSensor sonar(PORT_4);
Motor       motorA(PORT_A);
Motor       motorB(PORT_B, false);
Motor       motorC(PORT_C, false);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void)
{
  SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
  Lcd lcd;
  Clock clock;
//  int lightVal = light.getBrightness();
//  int sonarVal;
//  int rotateVal;
//  int touchVal = touch.isPressed();
//  int RN;
//  int loop = 0;

  // Set A to 0
//  int A = 0;

  // Start loop
//  nxtClock.reset();

while(1){
  if(light.getBrightness()<300){
    motorA.setPWM(0);
    motorB.setPWM(0);
    motorC.setPWM(0);
  }
  else{
    motorA.setPWM(10);
    motorB.setPWM(-40);
    motorC.setPWM(-40);
  }
  clock.wait(100);
}
}
}
*/

#include "LightSensor.h"
#include "SonarSensor.h"
#include "SoundSensor.h"
#include "TouchSensor.h"
#include "GyroSensor.h"
#include "Clock.h"
#include "Lcd.h"

#include "Motor.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

LightSensor  light(PORT_3/* lamp off */);
SonarSensor  sonar(PORT_4);
GyroSensor   gyro(PORT_1);
TouchSensor  touch(PORT_2);

Motor       motorA(PORT_A);
Motor       motorB(PORT_B);
Motor       motorC(PORT_C);

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
  SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
  Clock clock;
  Lcd lcd;
  int rotateA;
  int t = touch.isPressed();
  int color_num;
  int color_max=0;
  int color_min=1000;
  int color_ave;
  int count;
  while(t==0){
    count = count;
  }
  while(t==1){
    int color_num = light.getBrightness();
    if(color_num > color_max){
      color_max = color_num;
    }
    if(color_num < color_min){
      color_min = color_num;
    }

      lcd.clear();
      lcd.putf("sdn",   "white: ",color_max,0);
      lcd.putf("sd",  "black: ",color_min,0);
      lcd.disp();
}      //ボタンを離すと読み込み終了
      color_ave = (color_max + color_num) / 2;

  while(1)
  {
  rotateA = nxt_motor_get_count(PORT_A);
    //line_in
    if(light.getBrightness()<color_min+5){
    if(rotateA > -100){
      motorA.setPWM(-125);
      motorB.setPWM(-18);
      motorC.setPWM(-20);
    }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-18);
      motorC.setPWM(-20);
    }
        }
    //line_out 
    else{
    if(rotateA < 100){
      motorA.setPWM(125);
      motorB.setPWM(-20);
      motorC.setPWM(-18);
      }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-20);
      motorC.setPWM(-18);
    }
    }
    
    lcd.clear();
    //cd.putf("sddn", "1/2: ", gyro.getLevel(),0, touch.isPressed(),5);
    lcd.putf("sdd",  "Brightness: ", light.getBrightness(),0);
//  if(light.getBrightness()<310){
//
  lcd.disp();

    clock.wait(100);
  }
}
}
