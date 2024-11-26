#include "project.h"
#include "motor.h"
#include "PIDcontroller.h"
#include "tfTest.h"
#include "sensor.h"
#include "interface.h"
#include <stdio.h>

CY_ISR_PROTO(PID_HANDLER);


//values from PID
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

//values from interface
//extern int newCountFlag;
extern long count;
extern int block;
double angle;
int step = 0;

// Handle received byte and perform corresponding action
void handleByteReceived(uint8_t byteReceived)
{
    switch (byteReceived)
    {
    case '1':
        setpoint = 10;
        break;
    case '2':
        setpoint = 20;
        break;
    case '3':
        setpoint = -10;
        break;
    case '4':
        setpoint = -20;
        break;
    default:
        // Do nothing for unrecognized commands
        break;
    }
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); // Get the number of bytes to read
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData(); // Read a byte from UART
        UART_1_WriteTxData(byteReceived);           // Echo back the received byte

        handleByteReceived(byteReceived); // Handle the received byte

        bytesToRead--; // Decrement the byte counter
    }
}



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    
    //interface
    init_interface();
    
    //UART
    UART_1_Start();
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    
    //motor
    Motor_Init();
    // CyDelay(5000);
    // setspeed(1,200);
    // setspeed(2,175);
    // CyDelay(2000);
    
    //PID
    if (MotorSwitchPin_Read()) // to avoid motor turning on if switch is off at startup
    {
        isr_pid_StartEx(PID_HANDLER);
    }
    
    //sensor
    initSensor();
    
    UART_1_PutString("started!");
    
    for(int i = 0;;++i)
    {
        startBurst();
        CyDelay(3);
        stopBurst();
        CyDelay(3);

            
        count = IIRFilter(count);
        angle = calcAngle(count);
            
        // read interface and update setpoint
        if (block)
        {
            setpoint = interfaceSetpoint();
        }
            
        //sprintf(buff, "%i %.2f  \r\n", step,  angle);
        //UART_1_PutString(buff);
        sprintf(outputBuffer, "%.2f %.2f \r\n", output,angle);
        UART_1_PutString(outputBuffer);
        
        if (i >= 50)
        {
            writeLCD(setpoint, angle);
            i = 0;
        }
        startCounter();
    }
}
 //float counter = 0;
CY_ISR(PID_HANDLER)
{
    
    //float test = sinFunc(0.1,(counter++ * (1/333)));
    //sprintf(outputBuffer, "%f \r\n", test);
    //tfTest(test,200,170);
    output = PIDUpdate(setpoint, angle, &proportional, &integral, &derivative);
    //to motor somehow
    /*
    if(output<0)
    {
        setspeed(1,200-(output/50));
        setspeed(2,170);
    }
    else if(output>0)
    {
        setspeed(2,170+(output/50));
        setspeed(1,200);
    }
    else
    {
        setspeed(1,200);
        setspeed(2,170);
    }*/
    
    setspeed(1,200-(output));
    setspeed(2,170+(output));
    
    
}

CY_ISR(ISR_motorSwitch_On_handler)
{
    isr_pid_StartEx(PID_HANDLER);
}
CY_ISR(ISR_motorSwitch_Off_handler)
{
    isr_pid_Stop();
    stop();
}