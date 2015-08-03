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
//宣言文1start
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
int color_max;
int color_min;
int ave;
static int diff[2];
static float integral;
#define DELTA_T 0.004
#define KP 0.34
#define KI 0.34
#define KD 0.2

//宣言文1end

class DriveMode
{
private:
	int a;
	float b,c;
public:
	void Forward(int);
};
void DriveMode::Forward(int fw){
	motorA.setPWM(fw);
	motorB.setPWM(-20);
	motorC.setPWM(-20);
}

class  LineChecker
{
private:
	float p,i,d;
	int pid;
public:
	int LineCheck(int);
	//int LineCheck(int sensor_val);
};
int LineChecker::LineCheck(int sensor_val){
	diff[0] = diff[1];
	diff[1] = sensor_val - ave;
	integral += (diff[1] + diff[0])/2*DELTA_T;
	p = KP*diff[1];
	 i = KI*integral;
	d = KD*(diff[1]-diff[0])/DELTA_T;
	pid = (int)(p+i+d);
	return pid;
}


extern "C"
{
	
//宣言に問題があったらここに宣言文1を書く

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
	bool button_flag = false;
	LineChecker lc;
	DriveMode dm;
	int light_num;
	int pid;
	while(true){
		//下記の処理でそれぞれのボタンを押したときに、変数の値が１になるようにする。
		bool enterBtn = (bool)(nxt.getButtons() & Nxt::ENTR_ON);
		bool  runBtn = (bool)(nxt.getButtons() & Nxt::RUN_ON);
		if(touch.isPressed()==1){
			ave = (color_max + color_min) / 2;
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
			lcd.clear();
			lcd.putf("sn","center : Get white");
			lcd.putf("sn","right : Get black");
			lcd.putf("sd",   "Brightness:",light.getBrightness(),0);
			lcd.disp();
			//タッチセンサーが押されたら、ループ処理を実行する。
		}else{
			light_num = light.getBrightness();
			pid = lc.LineCheck(light_num);
			dm.Forward(pid);
			lcd.clear();
			lcd.putf("sdn",   "color_max : ",color_max,0);
			lcd.putf("sd",   "color_min : ",color_min,0);
			lcd.disp();
		}
	}
}

}