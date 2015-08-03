/* test_move*/
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
#include"ecrobot_types.h"

LightSensor  light(PORT_3);
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

static float pid_sample(float sensor_val);
static float math_limit(float pid_val, float pid_min, float pid_max);

float DELTA_T=0.005;
float KP=0.10;
float KI=0.02;
float KD=0.03;
float target_val=380; //光センサの目標値

int threshold=400;

static float diff[2]; //"S32" 32bitのsinged(符号あり)整数型
static float integral; //"F32" 32bitの浮動小数点型

int Line_in(int);                 // line_inの際の動作
int Line_out(int);              // line_outの際の動作

TASK(TaskMain)
{
  Clock clock;
  Lcd lcd;

  int angle = 0;
  
  motorA.reset();
  
  while(touch.isPressed()==0)
  {    
    angle = pid_sample(light.getBrightness()); 
    
    //line_in 光センサの値がしきい値未満
    if(light.getBrightness()<threshold){
        Line_in(angle);
        }
    //line_out 光センサの値がしきい値以上
    else{
        Line_out(angle);
        }

    
    lcd.clear();
    //各センサ・モータ類の値を常に画面に出力
    lcd.putf("sdn", "angle:", angle,0);
    lcd.putf("sdn", "Rotation_A:", motorA.getCount(),0);
    lcd.putf("sdn", "Rotation_B:", motorB.getCount(),0);
    lcd.putf("sdn", "Rotation_C:", motorC.getCount(),0);
    lcd.putf("sdn", "touch: ", touch.isPressed(),5);
    lcd.putf("sdn", "gyro: ", gyro.getAnglerVelocity(),0);
    lcd.putf("sdd",  "3/4: ", light.getBrightness(),0, sonar.getDistance(),5);
    lcd.disp();

    clock.wait(5);//0.005_msec
  }
}

//line_inのとき左旋回
int Line_in(int angle){
    if(motorA.getCount() > -angle){
        motorA.setPWM(-100);
        motorC.setPWM(-18);
    }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-16);
      motorC.setPWM(-18);
    }
}

//line_outのとき右旋回
int Line_out(int angle){
    if(motorA.getCount() < angle){
        motorA.setPWM(100);
        motorB.setPWM(-18);
    }
    else{
      motorA.setPWM(0);
      motorB.setPWM(-18);
      motorC.setPWM(-16);
      }
}

float pid_sample(float sensor_val){
    float p,i,d;
    
    diff[0]=diff[1];
    diff[1] = sensor_val - target_val;
    integral += (diff[1] + diff[0] / 2.0 * DELTA_T);
    
    p = KP * diff[1];
    i = KI * integral;  
    d = KD * (diff[1] - diff[0]) / DELTA_T;
    
    return math_limit(p + i + d, -400.0, 400.0);
    }
    
float math_limit(float pid_val, float pid_min, float pid_max)
{
    if(pid_val < pid_min) {
        return pid_min;
    } else if(pid_val > pid_max) {
        return pid_max;
    }
    return pid_val;
}

}
//end