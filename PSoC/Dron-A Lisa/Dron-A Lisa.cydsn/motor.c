#include "motor.h"
#include "project.h"

// Initialize Motor to 50Hz
void Motor_Init()
{
    Clock_1_Start(); // Start the clock
    Motor_PWM_Start();   // Start the PWM
    stop();
}



// Stop both motors
void stop()
{
    Motor_PWM_WriteCompare1(1000); // Reset motor 1 speed
    Motor_PWM_WriteCompare2(1000); // Reset motor 2 speed
}

void setspeed(int Motor, int setpoint)
{
    int setpoint_ = (setpoint < 0) ? 0 : (setpoint > 1000) ? 1000 : setpoint;
    if (1 == Motor)
    {
        Motor_PWM_WriteCompare1(setpoint_ + 1000);
    }
    else if (2 == Motor)
    {
        Motor_PWM_WriteCompare2(setpoint_ + 1000);
    }
    else
    {
        UART_1_PutString("You have made a grave mistake\r\n");
    }
}