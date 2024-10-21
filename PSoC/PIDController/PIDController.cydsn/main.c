#include "project.h"
#include "PIDController.h"
#include <stdio.h>

char outputBuffer[256];
extern float kp_;
extern float ki_;
extern float kd_;
extern float iMin_;
extern float iMax_;
extern float dt_;

float output = 0;
float proportional = 0;
float integral = 0;
float derivative = 0;
float error = 0;
float setpoint = 0;

// For testing with a UART interface
void handleByteReceived(uint8_t byteReceived);

CY_ISR_PROTO(UART_HANDLER);
CY_ISR_PROTO(PID_HANDLER);

int main(void)
{

    CyGlobalIntEnable; // Enable global interrupts.

    // Initializing all the modules - Mostly for testing
    PWM_1_Start();
    UART_1_Start();
    isr_uart_rx_StartEx(UART_HANDLER);
    isr_pid_StartEx(PID_HANDLER);
    for (;;)
    {
    }
}
// Handler for UART
CY_ISR(UART_HANDLER)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        UART_1_WriteTxData(byteReceived); // echo back

        handleByteReceived(byteReceived);

        bytesToRead--;
    }
}
// Handler for samplingtime of the PID
CY_ISR(PID_HANDLER)
{
    output = PIDUpdate(setpoint, output, &proportional, &integral, &derivative);
    PWM_1_WriteCompare(output);
    UART_1_PutString(outputBuffer);
    error = setpoint - output;
    snprintf(outputBuffer, sizeof(outputBuffer), "Setpoint: %f, output: %f, error: %f, kp: %f, ki: %f, kd: %f, P: %f, I: %f, D: %f \r\n", setpoint, output, error, kp_, ki_, kd_, proportional, integral, derivative);
}

// How to change the setpoint
void handleByteReceived(uint8_t byteReceived)
{
    switch (byteReceived)
    {
    case '1':
    {
        setpoint = 10;
        // PIDSetpointChange
        UART_1_PutString("Setpoint is changed to 10!\r\n");

        return;
    }
    break;

    case '2':
    {
        setpoint = 20;
        // PIDSetpointChange
        UART_1_PutString("Setpoint is changed to 20!\r\n");

        return;
    }
    break;

    case '3':
    {
        setpoint = 30;
        // PIDSetpointChange
        UART_1_PutString("Setpoint is changed to 30!\r\n");

        return;
    }
    break;
    }
}