#include "Nxt.h"
#include "LightSensor.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"
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

  //下記の処理でそれぞれのボタンを押したときに、変数の値が１になるようにする。
  bool enterBtn = (bool)(nxt.getButtons() & Nxt::ENTR_ON);
  bool  runBtn = (bool)(nxt.getButtons() & Nxt::RUN_ON);

  if(enterBtn == 1){
    color_max = light.getBrightness();//オレンジのボタンを押したときに白の値を取得する。
  }
  if(runBtn == 1){
    color_min = light.getBrightness();//ランボタンを押したときに黒の値を取得する。
  }
  //タッチセンサーが押されたら、ループ処理を実行する。
  if(touch.isPressed()==1){
    while(true){
      Linecheck();
    }
  }
}
