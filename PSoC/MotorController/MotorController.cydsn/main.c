#include "project.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    PWM_1_Start();
    
    Motor_init();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

void Motor_init()
{

    PWM_1_WriteCompare(50);
}
