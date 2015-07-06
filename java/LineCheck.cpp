#include "Nxt.h"
#include "LightSensor.h"

LightSensor  light(PORT_3;

class LineCheck
{
        bool check = false;
        static int limit_out_count;
        drivemode = DriveMode();
        colorchecker = ColorChecker();
    public:
        string char* states = "inline";
        static int ava = 0;
        
        frwh = Mathmatical(0.34,0.34,0.34);
        riwh = Mathmatical(0.34,0.34,0.34);
        lewh = Mathmatical(0.34,0.34,0.34);
        
         double cur;
         double cur2 = 0;
         
        void Check() {
             if (ava == 0) {
            ava = colorchecker.Extract();
            }
            cur = readNormalizedValue();
            cur = pid(ava,cur);
            Forward(int cur, int cur2);
        }
}