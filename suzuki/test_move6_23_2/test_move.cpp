/* test_move*/
/*
メモ
motorAは+で右回り-で左回り
motorB,Cは-で直進
motorB = 左
motorC = 右

6/23
右旋回はほぼ完璧だが左旋回に少し難あり
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

TASK(TaskMain)
{
  Clock clock;
  Lcd lcd;
  
  int Line_in(int);                 // line_inの際の動作
  int Line_out(int);              // line_outの際の動作
  int Revup_motor(void);   // モーターB.Cが停止した際の動作
  
  int threshold=380; // 色のしきい値
  int flag;                  // line_INorOUTの判別と角度調整の重み？
  int move_flag;       // モータの動作の有無の判定に使用
  
  flag =0;                 //line_outのときに値を増やしline_inのときに減らす
  move_flag = 1;     //初期動作のために初期値は"1"
  motorA.reset();
  
  while(touch.isPressed()==0)
  {
    
    //Revup_motor()の条件文：ロボットが静止したらモータを吹かす
    if(move_flag == 0){
        Revup_motor();
    }
    move_flag = motorC.getCount()+motorB.getCount();
    
    
    //line_in 光センサの値がしきい値未満
    if(light.getBrightness()<threshold){
        if(flag > -400){
            flag = flag-2;  //黒の方が認識する時間が短いので２倍にする
            }
        Line_in(flag);
        }
    //line_out 光センサの値がしきい値以上
    else{
        if(flag < 400){
            flag = flag++;  //line_outの時間分だけflagの値を増やす
            }
        Line_out(flag);
        }
        
    //Revup_motor()の条件文必要な変数の値を計算    
    move_flag = move_flag - ( motorC.getCount()+motorB.getCount() );
    
    lcd.clear();
    
    //各センサ・モータ類の値を常に画面に出力
    lcd.putf("sdn", "flag:", flag,0);
    lcd.putf("sdn", "Rotation_A:", motorA.getCount(),0);
    lcd.putf("sdn", "Rotation_B:", motorB.getCount(),0);
    lcd.putf("sdn", "Rotation_C:", motorC.getCount(),0);
    lcd.putf("sdn", "touch: ", touch.isPressed(),5);
    lcd.putf("sdn", "gyro: ", gyro.getAnglerVelocity(),0);
    lcd.putf("sdd",  "3/4: ", light.getBrightness(),0, sonar.getDistance(),5);
    lcd.disp();

    clock.wait(5);
  }
}

//line_inのとき左旋回
//カラーセンサーの差分の値とflagの値で旋回角度を決める
int Line_in(int f){
    int  angle = light.getBrightness() - f + 200;
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
//カラーセンサーの差分の値とflagの値で旋回角度を決める
int Line_out(int f){
    int  angle = light.getBrightness() + f - 200;
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

//モータBCに動きがないときに一瞬だけ吹かす
int Revup_motor(void){
    motorB.setPWM(-70);
    motorC.setPWM(-70);
    motorB.setPWM(-10);
    motorC.setPWM(-10);
}

}
//end