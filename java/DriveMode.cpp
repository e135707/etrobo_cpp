#include <cmath> 
#include "Lcd.h"
#include "Motor.h"

using namespace ecrobot;

public class DriveMode{
	String states;
	int a;
	float b,c;
	puvlic void Forward(int fw){
		a = abs(nxt_motor_get_count(NXT_PORT_A));
		if ( signbit(fw) ) {
			b = 1;
		} else {
			b = -1;
		}
		lcd.putf("sdn",   "a=: ",a,0);
		lcd.putf("sd",   "fw=: ",fw,0);
		if(a < 500 || (0 < fw * nxt_motor_get_count(NXT_PORT_A))){
			//nxt_motor_set_speed(NXT_PORT_A,500,0);//0はフロー、1はブレーキ
			motorA.setPWM(fw);
		}
		//nxt_motor_set_speed(NXT_PORT_B,50 + 20*b,0);
		//nxt_motor_set_speed(NXT_PORT_C,50 + 20*b,0);
		motorB.setPWM(-50 - 20*b);
		motorC.setPWM(-50 - 20*b);
	}
}
