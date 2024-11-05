#include "motor.h"
#include "project.h"

// Initialize Motor to 50Hz
void Motor_Init()
{
    Clock_1_Start(); // Start the clock
    PWM_1_Start();   // Start the PWM
    PWM_1_WriteCompare1(1000);
    PWM_1_WriteCompare2(1000);
    // Send initialization messages via UART
    // UART_1_PutString("Motor control Application started\r\n");
    // UART_1_PutString("0: Stop\r\n");
    // UART_1_PutString("1: Decrease speed motor 0\r\n");
    // UART_1_PutString("2: Increase speed motor 0\r\n");
    // UART_1_PutString("q: Decrease speed motor 1\r\n");
    // UART_1_PutString("w: Increase speed motor 1\r\n");
}



// Stop both motors
void stop()
{
    PWM_1_WriteCompare1(1000); // Reset motor 1 speed
    PWM_1_WriteCompare2(1000); // Reset motor 0 speed
}

void setspeed(int Motor, int setpoint)
{
    int setpoint_ = (setpoint < 0) ? 0 : (setpoint > 1000) ? 1000 : setpoint;
    if (0 == Motor)
    {
        PWM_1_WriteCompare1(setpoint_ + 1000);
    }
    else if (1 == Motor)
    {
        PWM_1_WriteCompare2(setpoint_ + 1000);
    }
    else
    {
        UART_1_PutString("You have made a grave mistake\r\n");
    }
}