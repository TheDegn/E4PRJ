#include "project.h"
#include "motor.h"
#include "PIDcontroller.h"
#include "tfTest.h"
#include "sensor.h"
#include <stdio.h>

CY_ISR_PROTO(PID_HANDLER);


char outputBuffer[256];
extern float kp_;
extern float ki_;
extern float kd_;
extern float iMin_;
extern float iMax_;
extern float dt_;

float output = 0;
float measurement = 0;
float proportional = 0;
float integral = 0;
float derivative = 0;
float error = 0;
float setpoint = 0;

extern int newCountFlag;
extern long count;
double angle;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_Start();
    Motor_Init();
    CyDelay(5000);
    setspeed(1,200);
    setspeed(2,170);
    CyDelay(2000);
    
    isr_pid_StartEx(PID_HANDLER);
    
    //sensor
    /*char buff[256];
    
    startCounter();
    startBurst();
    initSensor();
    */
    for(;;)
    {
        /*if (newCountFlag == 1)
        {
            //time = calcTime(count);
            //avg = average(count);
            angle = calcAngle(count);
            sprintf(buff, "%.2f grader   \r\n", angle);
            UART_1_PutString(buff);
            newCountFlag = 0;
        }
        */
    }
}
 float counter = 0;
CY_ISR(PID_HANDLER)
{
    
    float test = sinFunc(0.1,counter++);
    sprintf(outputBuffer, "%f \r\n", test);
    tfTest(test,200,170);
    // output = PIDUpdate(setpoint, measurement, &proportional, &integral, &derivative);
    //to motor somehow
}