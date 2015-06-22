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
  while(1)
  {
  rotateA = nxt_motor_get_count(PORT_A);
    //line_in
    if(light.getBrightness()<300){
    motorA.setPWM(-10);
    motorB.setPWM(-16);
    motorC.setPWM(-16);
  }
    //line_out 
    else{
    motorA.setPWM(10);
    motorB.setPWM(-16);
    motorC.setPWM(-16);
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
