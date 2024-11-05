#include "project.h"

CY_ISR_PROTO(sensor_isr_handler);
long count = 0;
int newCountFlag = 0;
#define countFreq 12.f //MHz count input

void initSensor()
{
    sensor_input_isr_StartEx(sensor_isr_handler);
}

void startBurst()
{
    PWM_1_Start();
}

void stopBurst()
{
    PWM_1_Stop();
}

void startCounter()
{
    Counter_Sensor_WriteCounter(0);
    Counter_Sensor_Start();
}
double calcTime(double count)
{
    return count / countFreq;
}

double calcAngle(long count)
{
    return ((count * 0.0057) - 64.7036);
}

CY_ISR(sensor_isr_handler)
{
    count = Counter_Sensor_ReadCounter();
    Counter_Sensor_Stop();
    stopBurst();
    newCountFlag = 1;
    
}

/* could be
    count = Counter_Sensor_ReadCounter();
    
    newCountFlag = 1;
    Counter_Sensor_WriteCounter(0);
*/
