#include "tfTest.h"
#include "motor.h"
#include <math.h>

void tfTest(float speed, int offset1,int offset2)
{
    setspeed(1, offset1 + (speed));
    setspeed(2, offset2 - (speed));   
}


float sinFunc(float freq, float time)
{
    return 10*sinf(2.0f *M_PI*freq*time);
}