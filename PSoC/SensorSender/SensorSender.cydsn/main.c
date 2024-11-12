#include "project.h"
#include "sensor.h"
#include <stdio.h>

extern int newCountFlag;
extern long count;
double time;
double angle;
//long avg = 0;
//long average(long new_value);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    UART_1_PutString("Started!\r\n");
    initSensor();
    char buff[256];
    
    startCounter();
    startBurst();
    for(;;)
    {
        //startBurst();
        //CyDelay(100);
        //stopBurst();
        //CyDelay(100);

        if (newCountFlag == 1)
        {
            //time = calcTime(count);
            //avg = average(count);
            
            newCountFlag = 0;
            //angle = calcAngle(count);
            
            sprintf(buff, "%li    \r\n", count);
            UART_1_PutString(buff);
            startCounter();
            startBurst();
        }
    }
}
/*
long average(long new_value) {
    static long values[100] = {0};  // Array til at gemme de seneste 20 værdier
    static int index = 0;          // Index for den aktuelle position i arrayet
    static long sum = 0;           // Summen af de 20 værdier
    static int count = 0;          // Antal værdier til at håndtere de første 20 kald

    // Opdater summen ved at trække den gamle værdi fra og tilføje den nye
    sum -= values[index];
    sum += new_value;

    // Opdater arrayet med den nye værdi
    values[index] = new_value;

    // Flyt til næste position i arrayet, og wrap rundt efter 20
    index = (index + 1) % 100;

    // Tæl op til 20 for at sikre korrekt gennemsnit på de første kald
    if (count < 100) {
        count++;
    }

    // Returner gennemsnittet som en long
    return sum / count;
}*/