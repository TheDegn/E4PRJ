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
#include "PIDController.h"

int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    CY_ISR_PROTO(UART_HANDLER);
    void handleByteReceived(uint8_t byteReceived); //For testing
    UART_1_Start();
    UART_1_PutString("PID Controller started\n");

    for(;;)
    {
       
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

void handleByteReceived(uint8_t byteReceived)
{
  switch(byteReceived)
    {
        case 'i' :
        {
            
        }
        break;
        case 'd' :
        {
            
        }
        break;
        case 's' :
        {
            
        }
        break;
    }  
}

/* [] END OF FILE */
