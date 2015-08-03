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
  float KP=0.01;
  float KI=0.01;
  float KD=0.01;
  float target_val;
  static float diff[2]; //"S32" 32bitのsinged(符号あり)整数型
  static float integral; //"F32" 32bitの浮動小数点型
  int Line_in(int);
  int Line_out(int);

  TASK(TaskMain)
  {
    Nxt nxt;
      Clock clock;
        Lcd lcd;
    int color_max;
    int color_min;
    int Revup_motor(void);   // モーターB.Cが停止した際の動作
    int move_flag;       // モータの動作の有無の判定に使用
  int angle = 0;
    move_flag = 1;     //初期動作のために初期値は"1"
  bool button_flag = false;
    motorA.reset();
    while(true)
    {
    //下記の処理でそれぞれのボタンを押したときに、変数の値が１になるようにする。
    bool enterBtn = (bool)(nxt.getButtons() & Nxt::ENTR_ON);
    bool  runBtn = (bool)(nxt.getButtons() & Nxt::RUN_ON);
    if(touch.isPressed()==1){
      button_flag = true;
      }
      if(button_flag == false){
        if(enterBtn == 1){
          color_max = light.getBrightness();//オレンジのボタンを押したときに白の値を取得する。
          ecrobot_sound_tone(440, 100, 100);
          systick_wait_ms(1000);
          }
        if(runBtn == 1){
          color_min = light.getBrightness();//ランボタンを押したときに黒の値を取得する。
          ecrobot_sound_tone(294, 100, 100);
          systick_wait_ms(1000);
          }
      target_val = (color_max + color_min)/2;
           lcd.clear();
           lcd.putf("sn","center : Get max");
           lcd.putf("sn","right : Get min");
           lcd.putf("sdn",   "Brightness:",light.getBrightness(),0);
       lcd.putf("sdn", "Rotation_A:", motorA.getCount(),0);
       lcd.putf("sdn", "Rotation_B:", motorB.getCount(),0);
       lcd.putf("sdn", "Rotation_C:", motorC.getCount(),0);
           lcd.disp();
          //タッチセンサーが押されたら、ループ処理を実行する。
          }else{
        int threshold = (color_max + color_min)/2;
    angle = pid_sample(light.getBrightness());
//Revup_motor()の条件文：ロボットが静止したらモータを吹かす
      if(move_flag == 0){
        Revup_motor();
        }
        move_flag = motorC.getCount()+motorB.getCount();
//line_in 光センサの値がしきい値未満
        if(light.getBrightness()<threshold){
            Line_in(angle);
            }
            //line_out 光センサの値がしきい値以上
            else{
                Line_out(angle);
              }
//Revup_motor()の条件文必要な変数の値を計算
            move_flag = move_flag - ( motorC.getCount()+motorB.getCount() );
      lcd.clear();
//各センサ・モータ類の値を常に画面に出力
           lcd.clear();
       lcd.putf("sn","center : Get max");
       lcd.putf("sn","right : Get min");
       lcd.putf("sdn",   "Brightness:",light.getBrightness(),0);
       lcd.putf("sdn", "Rotation_A:", motorA.getCount(),0);
       lcd.putf("sdn", "Rotation_B:", motorB.getCount(),0);
       lcd.putf("sdn", "Rotation_C:", motorC.getCount(),0);
       lcd.disp();
            clock.wait(5);
            }
  }
}
  //line_inのとき左旋回
  //カラーセンサーの差分の値とflagの値で旋回角度を決める
int Line_in(int angle){
  if(motorA.getCount() > -angle){
    motorA.setPWM(-100);
    motorC.setPWM(-21);
    }
  else{
    motorA.setPWM(0);
    motorB.setPWM(-19);
    motorC.setPWM(-21);
    }
  }

  //line_outのとき右旋回
  //カラーセンサーの差分の値とflagの値で旋回角度を決める
int Line_out(int angle){
    if(motorA.getCount() < angle){
      motorA.setPWM(100);
      motorB.setPWM(-21);
      }
    else{
    motorA.setPWM(0);
        motorB.setPWM(-21);
        motorC.setPWM(-19);
        }
  }

  //モータBCに動きがないときに一瞬だけ吹かす
  int Revup_motor(void){
    motorB.setPWM(-70);
    motorC.setPWM(-70);
    motorB.setPWM(-10);
    motorC.setPWM(-10);
  }

float pid_sample(float sensor_val){
  float p,i,d;

  diff[0]=diff[1];
  diff[1] = sensor_val - target_val;
  integral += (diff[1] + diff[0] / 2.0 * DELTA_T);
  p = KP * diff[1];
  i = KI * integral;
  d = KD * (diff[1] - diff[0]) / DELTA_T;
  return math_limit(p + i + d, -100.0, 100.0);
  }
float math_limit(float pid_val, float pid_min, float pid_max){
  if(pid_val < pid_min) {
    return pid_min;
    } else if(pid_val > pid_max) {
      return pid_max;
      }
    return pid_val;
    }

}
