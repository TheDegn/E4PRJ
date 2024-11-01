#include "project.h"

CY_ISR_PROTO(sensor_isr_handler);
long count = 0;
int newCountFlag = 0;

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
    return count / 12000.0f;
}

CY_ISR(sensor_isr_handler)
{
    count = Counter_Sensor_ReadCounter();
    Counter_Sensor_Stop();
    newCountFlag = 1;
}