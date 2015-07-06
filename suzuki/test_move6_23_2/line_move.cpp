/* line_move*/
/*
メモ
motorAは+で右回り-で左回り
motorB,Cは-で直進
motorB = 左
motorC = 右
*/

// ECRobot++ API
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
  
  int Line_in(void);
  int Line_out(void);
  
  int motor_power;
  
  motorA.reset();
  
  while(touch.isPressed()==0)
  {
    motor_power = motorA.getCount();

    //line_in 光センサの値が370未満
    if(light.getBrightness()<370){
        Line_in();
        }
    //line_out 光センサの値が370以上
    else{
        Line_out();
        }
    
    lcd.clear();
    
    //各センサ・モータ類の値を常に画面に出力
    lcd.putf("sn",   "NXT Sensors");
    lcd.putf("sdn", "Rotation_A:", motorA.getCount(),0);
    lcd.putf("sdn", "Rotation_B:", motorB.getCount(),0);
    lcd.putf("sdn", "Rotation_C:", motorC.getCount(),0);
    lcd.putf("sdn", "touch: ", touch.isPressed(),5);
    lcd.putf("sdn", "gyro: ", gyro.getAnglerVelocity(),0);
    lcd.putf("sdd",  "3/4: ", light.getBrightness(),0, sonar.getDistance(),5);
    lcd.disp();

    clock.wait(50);
  }
}

//line_inのとき左旋回
int Line_in(void){
    if(motorA.getCount() > -250){
        motorA.setPWM(-100);
        motorC.setPWM(-13);
    }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-12);
      motorC.setPWM(-12);
    }
}

//line_outのとき右旋回
int Line_out(void){
    if(motorA.getCount() < 250){
      motorA.setPWM(100);
      motorB.setPWM(-13);
      }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-12);
      motorC.setPWM(-12);
      
    }
}
}