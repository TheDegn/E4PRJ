#include "project.h"
#include "Motor.h"
#include "tfInput.h"

// Function prototypes
CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);

void Motor_increaseSpeed(int Motor, int *Motor_speed);
void Motor_decreaseSpeed(int Motor, int *Motor_speed);


// Global variables for motor speeds
int Motor0_speed = 1000;
int Motor1_speed = 1000;
int offset = 5;
int speed;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    Motor_Init();
    for (;;)
    {
    }
}

// UART interrupt service routine
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

// Handle received byte and perform corresponding action
void handleByteReceived(uint8_t byteReceived)
{
    switch (byteReceived)
    {
    case 'q':
        setspeed(1, 700);
        break;
    case 'w':
        setspeed(1, 2000);
        break;
    case '1':
        setspeed(1, -1);
        break;
    case '2':
        setspeed(1, 500);
        break;
    case '0':
        stop(); // Stop both motors
        break;
    case 'z':
        setspeed(0, 700);
        break;
    case 'x':
        setspeed(0, 2000);
        break;
    case 'c':
        setspeed(0, -1000);
        break;
    case 'v':
        setspeed(0, 500);
        break;
    default:
        // Do nothing for unrecognized commands
        break;
    }
}

// Increase motor speed
void Motor_increaseSpeed(int Motor, int *Motor_speed)
{
    if (*Motor_speed < 2000 && Motor == 0)
    {
        UART_1_PutString("Increasing speed\r\n");
        (*Motor_speed)++;
        PWM_1_WriteCompare1(*Motor_speed); // Update PWM compare value for motor 0
    }
    else if (*Motor_speed < 2000 && Motor == 1)
    {
        UART_1_PutString("Increasing speed\r\n");
        (*Motor_speed)++;
        PWM_1_WriteCompare2(*Motor_speed); // Update PWM compare value for motor 1
    }
    else
    {
        UART_1_PutString("TOO FAST\r\n"); // Speed limit reached
    }
}

// Decrease motor speed
void Motor_decreaseSpeed(int Motor, int *Motor_speed)
{
    if (*Motor_speed > 1000 && Motor == 0)
    {
        UART_1_PutString("Decreasing speed\r\n");
        (*Motor_speed)--;
        PWM_1_WriteCompare1(*Motor_speed); // Update PWM compare value for motor 0
    }
    else if (*Motor_speed > 1000 && Motor == 1)
    {
        UART_1_PutString("Decreasing speed\r\n");
        (*Motor_speed)--;
        PWM_1_WriteCompare2(*Motor_speed); // Update PWM compare value for motor 1
    }
    else
    {
        UART_1_PutString("TOO SLOW\r\n"); // Speed limit reached
    }
}

void tfTest(speed,offset);
