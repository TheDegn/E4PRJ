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
    Sensor_Out_Start();
}

void stopBurst()
{
    Sensor_Out_Stop();
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
    return ((count * 0.00574795) - 64.70355264);
}

CY_ISR(sensor_isr_handler)
{
    count = Counter_Sensor_ReadCounter();
    Counter_Sensor_Stop();
    stopBurst();
    newCountFlag = 1;
    startCounter();
    startBurst();
}

/* could be
    count = Counter_Sensor_ReadCounter();
    
    newCountFlag = 1;
    Counter_Sensor_WriteCounter(0);
*/
