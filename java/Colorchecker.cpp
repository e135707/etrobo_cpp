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

static int min_lightValue = 0;
static int max_lightValue = 0;

boolean Valuecheck = false;
/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
  SleeperMonitor(); // needed for I2C device and Clock classes
}

int Extract()
{
  Lcd lcd;
  int lightAverage;
  while(Valuecheck != 1){
  lightAverage = (min_lightValue + max_lightValue) / 2;
  lcd.clear();
  lcd.putf("sdd",  "White: ", color_max,0);
  lcd.putf("sdd",  "Black: ", color_min,0);
  lcd.putf("sdd",  "Average: ", color_ave,0);
  lcd.disp();
    }
}
void kuro(){
  max_lightValue = light.getBrightness();
}
void shiro(){
  min_lightValue = light.getBrightness();
}
}
