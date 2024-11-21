#include "project.h"

CY_ISR_PROTO(sensor_isr_handler);
long count = 0;
int newCountFlag = 0;
#define countFreq 12.f //MHz count input
#define WINDOW_SIZE 5    // Antallet af værdier i glidende gennemsnit

void initSensor()
{
    sensor_input_isr_StartEx(sensor_isr_handler);
}

void startBurst()
{
    Sensor_Out_Start();
}

void stopBurst()
{
    Sensor_Out_Stop();
}

void startCounter()
{
    Counter_Sensor_WriteCounter(0);
    Counter_Sensor_Start();
}

double calcTime(double count)
{
    return count / countFreq;
}

double calcAngle(long count)
{
    return ((count * 0.00581473) - 69.19523235);
    
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

CY_ISR(sensor_isr_handler)
{
    count = Counter_Sensor_ReadCounter();
    Counter_Sensor_Stop();
    newCountFlag = 1;
}

/* could be
    count = Counter_Sensor_ReadCounter();
    
    newCountFlag = 1;
    Counter_Sensor_WriteCounter(0);
*/

