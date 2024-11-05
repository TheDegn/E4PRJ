#include "project.h"
#include "sensor.h"
#include <stdio.h>

extern int newCountFlag;
extern long count;
double time;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    UART_1_PutString("Started!\r\n");
    initSensor();
    char buff[256];
    
    startCounter();
    
    for(;;)
    {
        /*startBurst();
        CyDelay(1000);
        stopBurst();
        CyDelay(1000);*/
        if (newCountFlag == 1)
        {
            time = calcTime(count);
            sprintf(buff, "%.3f us\r\n", time);
            UART_1_PutString(buff);
            newCountFlag = 0;
        }
    }
}