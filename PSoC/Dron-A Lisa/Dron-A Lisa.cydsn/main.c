#include "project.h"
#include "motor.h"
#include "PIDcontroller.h"
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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_Start();
    Motor_Init();
    isr_pid_StartEx(PID_HANDLER);
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

CY_ISR(PID_HANDLER)
{
    output = PIDUpdate(setpoint, measurement, &proportional, &integral, &derivative);
    //to motor somehow
}