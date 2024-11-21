#include "project.h"
#include "sensor.h"
#include <stdio.h>

extern int newCountFlag;
extern long count;
double time;
double angle;
//long avg = 0;
float FIRMovingAverage(float new_value);
float IIRFilter(float new_value);
#define WINDOW_SIZE 5    // Antallet af værdier i glidende gennemsnit

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    UART_1_PutString("Started!\r\n");
    initSensor();
    char buff[256];
    
    /*startCounter();
    startBurst();*/
    for(;;)
    {
        startBurst();
        //startCounter();
        CyDelay(3);
        stopBurst();
        //sprintf(buff, "%li    \r\n", count);
        //UART_1_PutString(buff);
        CyDelay(3);
    

        if (newCountFlag == 1)
        {
            //time = calcTime(count);
            
            newCountFlag = 0;
            
            //count = FIRMovingAverage(count);
            count = IIRFilter(count);
            angle = calcAngle(count);
            sprintf(buff, " %.2f  \r\n", angle);
            UART_1_PutString(buff);
            startCounter();
            //startBurst();
        }
        
        /*startCounter();
        startBurst();
        while (!Sensor_Input_Read())
        {
            Sensor_Input_Read();
        }
        Counter_Sensor_Stop();
        count = Counter_Sensor_ReadCounter(); 
        stopBurst();
        //angle = average(calcAngle(count));
        //count = average(count);
        sprintf(buff, "%li    \r\n", count);
        UART_1_PutString(buff);
        while (Sensor_Input_Read())
        {
            Sensor_Input_Read();
        }
        CyDelay(50);*/
    }
}

float FIRMovingAverage(float new_value) {
    
    static float values[WINDOW_SIZE] = {0};  // Array til at gemme de seneste 20 værdier
    static int index = 0;                  // Index for den aktuelle position i arrayet
    static float sum = 0;                  // Summen af de 20 værdier
    static int count = 0;                  // Antal værdier, der er blevet tilføjet

    // Opdater summen ved at trække den gamle værdi fra og tilføje den nye
    sum -= values[index];
    sum += new_value;

    // Opdater arrayet med den nye værdi
    values[index] = new_value;

    // Flyt til næste position i arrayet, og wrap rundt efter WINDOW_SIZE
    index = (index + 1) % WINDOW_SIZE;

    // Tæl op til WINDOW_SIZE for at sikre korrekt gennemsnit på de første kald
    if (count < WINDOW_SIZE) {
        count++;
    }

    // Returner gennemsnittet
    return sum / count;
}

float IIRFilter(float new_value) {
    static float previous_output = 0;  // Gemmer det forrige output
    const float alpha = 0.2;          // Justerbar vægtning, mellem 0 og 1

    // Beregn det nye output baseret på nuværende input og tidligere output
    float output = alpha * new_value + (1 - alpha) * previous_output;

    // Opdater det forrige output
    previous_output = output;

    return output;
}