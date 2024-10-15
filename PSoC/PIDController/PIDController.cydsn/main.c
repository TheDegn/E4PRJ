#include "project.h"
#include "PIDController.h"
#include <stdio.h>

char outputBuffer[256];
float kp = 0;
float ki = 0;
float kd = 0;
float iMin = 0;
float iMax = 0;
float output = 0;
float proportional = 0;
float integral = 0;
float derivative = 0;
float error = 0;
float setpoint = 0;

//For testing with a UART interface
void handleByteReceived(uint8_t byteReceived);

CY_ISR_PROTO(UART_HANDLER);
CY_ISR_PROTO(PID_HANDLER);

int main(void)
{
    
    
    
    CyGlobalIntEnable; //Enable global interrupts.
    
    //Initializing all the modules - Mostly for testing
    PWM_1_Start();
    UART_1_Start();
    isr_uart_rx_StartEx(UART_HANDLER);
    isr_pid_StartEx(PID_HANDLER);
    //PIDStartUp
    UART_1_PutString("PID Controller started - Please choose the following parameters:\r\n");
    UART_1_PutString("Kp: ");
    scanf("%f",&kp);
    UART_1_PutString("\r\nKi: ");
    scanf("%f",&ki);
    UART_1_PutString("\r\nKd: ");
    scanf("%f",&kd);
    UART_1_PutString("\r\nintegral minimum: ");
    scanf("%f",&iMin);
    UART_1_PutString("\r\nintegral Maximum: ");
    scanf("%f",&iMax);
    UART_1_PutString("\n\rAll parameters recieved!\r\n");
    for(;;)
    {
       
    
    }
}
//Handler for UART
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
//Handler for samplingtime of the PID
CY_ISR(PID_HANDLER)
{
    output = PIDUpdate(output,&proportional,&integral,&derivative);
    snprintf(outputBuffer, sizeof(outputBuffer), "Setpoint: %f, output: %f, error: %f, kp: %f, ki: %f, kd: %f, P: %f, I: %f, D: %f \r\n", setpoint, output, error, kp, ki, kd, proportional, integral, derivative);
    PWM_1_WriteCompare(output);
    UART_1_PutString(outputBuffer);
    error = setpoint - output;
}

//How to change the setpoint
void handleByteReceived(uint8_t byteReceived)
{
  switch(byteReceived)
    {
        case 's' :
        {
           UART_1_PutString("Please choose a new setpoint:");
           scanf("%f",&setpoint);
            //PIDSetpointChange
           UART_1_PutString("\r\nSetpoint is changed!\r\n"); 
            
           return;
        }
        break;
    }  
}

/*
variabler kan implementeres som *extern*
setpoint kan opdateres i PIDUpdate
scanf virker kun i terminal ikke UART
switch case i handle byte received med forskelle prædifineret cases
Setpoint change og startup ikke indskevet i main
*/