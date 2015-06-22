/* sample.cpp for TOPPERS/ATK(OSEK) */
//NxtSensor2
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

Motor motorA(PORT_A); // brake by defalut前輪
Motor motorB(PORT_B); // brake by defalut左後輪
Motor motorC(PORT_C);// 右後輪

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
  SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
  Nxt nxt;
  Clock clock;
  Lcd lcd;
  int color_num;
  int color_max=0;
  int color_min=1000;
  int color_ave;
  int turn_rotate;
  while(touch.isPressed()==0){} //start
  
  while(touch.isPressed()==1) //ボタンを押した時光センサーで読み込む
  {
  	color_num = light.getBrightness();
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
  } //ボタンを離すと読み込み終了
  color_ave = (color_max + color_min) / 2;
  while(1)
  {
		if (nxt.getButtons() == Nxt::ENTR_ON)
		{
			motorA.setPWM(0);   // pwm=0 and count=0
			motorA.setCount(0);
			motorB.setPWM(0);   // pwm=0 and count=0
			motorB.setCount(0);
			motorC.setPWM(0); // count = 0
			motorC.setCount(0); //消すかも
		}
		else
		{
			turn_rotate = ( color_ave - light.getBrightness() );
			//turn_rotate = ( color_ave - light.getBrightness() );
				motorA.setPWM( turn_rotate * turn_rotate * turn_rotate); //前輪
				motorB.setPWM(-12); //左の後輪
				motorC.setPWM(-12); //右の後輪
			//nxt_motor_set_speed(NXT_PORT_B, -30, 1);
			//nxt_motor_set_speed(NXT_PORT_C, -30, 1);
		}
    lcd.clear();
    lcd.putf("sdn",   "color_ave: ",color_ave,0);
    lcd.putf("sdn",   "turn_rotate: ",turn_rotate,0);
    lcd.putf("sd",   "roteta: ",nxt_motor_get_count(NXT_PORT_A),0);
    lcd.disp();

    clock.wait(100);
  }
  }
}