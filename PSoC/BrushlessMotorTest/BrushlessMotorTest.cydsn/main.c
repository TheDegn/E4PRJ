/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byte1);
uint8_t speed1 = 0;
uint8_t speed2 = 0;
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Clock_1_Start();
    PWM_1_Start();
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    UART_1_PutString("Drone Startet!\r\n");
    UART_1_PutString("Styr motor 1, send hastighed i procent! (eg 10)\r\n");
    for(;;)
    {
        /* Place your application code here. */
    }
}

CY_ISR(ISR_UART_rx_handler)
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

void handleByteReceived(uint8_t byte)
{
    switch (byte)
    {
    case '1':
        if (speed2 != 0)
            speed2 = speed2 - 50;
        PWM_1_WriteCompare2(1000 + speed2);
        break;
    case '2':
        if (speed2 < 200)
            speed2 = speed2 + 50;
        PWM_1_WriteCompare2(1000 + speed2);
        break;
    case '9':
        if (speed1 != 0)
            speed1 = speed1 - 50;
        PWM_1_WriteCompare1(1000 + speed1);
        break;
    case '0':
        if (speed1 < 200)
            speed1 = speed1 + 50;
        PWM_1_WriteCompare1(1000 + speed1);
        break;
    default:
        break;
    }
}
/* [] END OF FILE */
