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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
     
    
    
    for(;;)
    {
        PWM_1_Start();
        CyDelay(10000);
        PWM_1_Stop();
        CyDelay(10000);
    }
}

/* [] END OF FILE */
