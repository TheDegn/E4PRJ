#include "project.h"

#include "interface.h"

// Global Variels.
int16_t convertedResult = 0;
int toggle = 3;
int deg = 0;
char LCDbuffer[20];
int block = 0;
char tempbuffer[20];
int pass = 1;
int gradientShift = 0;
int gradientCount = 0;

// Define constants
#define adc_min 30   // ADC value corresponding to -41 degrees
#define adc_max 4050 // ADC value corresponding to +41 degrees
#define deg_min -35  // Minimum degrees
#define deg_max 35   // Maximum degrees

enum symbol
{
    degree = 0,
    lock,
    undefined,
    unlock
};

CY_ISR(ISR_gradient_handler)
{
    if (0 == gradientShift)
    {
        return;
    }
    else if (-1 == gradientShift)
    {
        if (-40 <= gradientCount)
        {
            gradientCount = gradientCount - 1;
        }
    }
    else if (1 == gradientShift)
    {
        if (40 >= gradientCount)
        {
            gradientCount = gradientCount + 1;
        }
    }
}

CY_ISR(ISR_toggle_handler)
{
    deg = convertedResult;
    convertedResult = deg;
    if (1 == toggle)
    {
        toggle = 2;
    }
    else if (2 == toggle)
    {
        toggle = 3;
    }
    else if (3 == toggle)
    {
        toggle = 1;
    }
}

CY_ISR(ISR_print_handler)
{
    if (motorSwitch())
    {
        if (1 == toggle)
        {
            sprintf(LCDbuffer, "%i             ", convertedResult);
            writeLCD("mode 1", unlock, LCDbuffer, degree);
        }
        else if (2 == toggle)
        {
            sprintf(LCDbuffer, "%i             ", convertedResult);
            writeLCD("mode 2", lock, LCDbuffer, degree);
        }
        else
        {
            sprintf(LCDbuffer, "%i             ", gradientCount);
            writeLCD("mode 3", lock, LCDbuffer, degree);
        }  
    }
}

CY_ISR(ISR_motorSwitch_On_handler)
{
    LCD_ClearDisplay();
    LCD_Position(0, 0);
    LCD_PrintString("On");
}
CY_ISR(ISR_motorSwitch_Off_handler)
{
    LCD_ClearDisplay();
    LCD_Position(0, 0);
    LCD_PrintString("Off");
}

void init_interface()
{
    Init_LCD();
    //Init_ISR();
    //Init_PWM();
    Init_ADC();
    isr_motorSwitch_On_StartEx(ISR_motorSwitch_On_handler);
    isr_motorSwitch_Off_StartEx(ISR_motorSwitch_Off_handler);
}

void Init_LCD()
{
    LCD_ClearDisplay();
    LCD_Start();
    LCD_Position(0, 0);
    LCD_PrintString("Starting");
    LCD_Position(1, 0);
    LCD_PrintString("Dron-A Lisa");
}

void Init_ADC()
{
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
}

void Init_PWM()
{
    PWM_1_Start();
    PWM_2_Start();
}

void Init_ISR()
{
    isr_toggle_StartEx(ISR_toggle_handler);
    isr_print_StartEx(ISR_print_handler);
    isr_gradient_StartEx(ISR_gradient_handler);
}

void writeLCD(char *top, int endTopChar, char *buttom, int endButtomChar)
{
    LCD_ClearDisplay();
    LCD_Position(0, 0);
    LCD_PrintString(top);
    LCD_Position(0, 15);
    LCD_PutChar(endTopChar);
    LCD_Position(1, 0);
    LCD_PrintString(buttom);
    LCD_Position(1, 15);
    LCD_PutChar(endButtomChar);
}

void writeLCD2(int setpoint, float sensor)
{
    char buff[16];
    LCD_ClearDisplay();
    sprintf(buff,"Setpoint: %i", setpoint);
    LCD_Position(0, 0);
    LCD_PrintString(buff);
    sprintf(buff,"Sensor: %.2f", sensor);
    LCD_Position(1, 0);
    LCD_PrintString(buff);
}
#define WINDOW_SIZE 5    // Antallet af værdier i glidende gennemsnit
static int values[WINDOW_SIZE] = {0};  // Array til at gemme de seneste 20 værdier
static int i = 0;                  // Index for den aktuelle position i arrayet
static float sum = 0;                  // Summen af de 20 værdier
static int c = 0;                  // Antal værdier, der er blevet tilføjet

int FIRMovingAverage(int new_value) 
{
    // Opdater summen ved at trække den gamle værdi fra og tilføje den nye
    sum -= values[i];
    sum += new_value;

    // Opdater arrayet med den nye værdi
    values[i] = new_value;

    // Flyt til næste position i arrayet, og wrap rundt efter WINDOW_SIZE
    i = (i + 1) % WINDOW_SIZE;

    // Tæl op til WINDOW_SIZE for at sikre korrekt gennemsnit på de første kald
    if (c < WINDOW_SIZE) {
        c++;
    }

    // Returner gennemsnittet
    return sum / c;
}

int setpoint()
{
    //return adcToDegrees( FIRMovingAverage( ADC_SAR_1_GetResult16() ) );
    return adcToDegrees( ADC_SAR_1_GetResult16() );
}

int mode_1()
{
    // average
    int loops = 10;
    long sum = 0;
    for (int i = 0; i < loops; i++)
    {
        sum = sum + ADC_SAR_1_GetResult16();
    }
    sum = sum / loops;
    convertedResult = adcToDegrees(sum);
    return convertedResult;
}

int mode_2()
{
    int jump = 1;
    if (ADC_SAR_1_GetResult16() > 2200 && ADC_SAR_1_GetResult16() < 3400) // middle
    {
        jump = 1;
        deg = deg + jump;
    }
    else if (ADC_SAR_1_GetResult16() < 1900 && ADC_SAR_1_GetResult16() > 600) // middle
    {
        jump = 1;
        deg = deg - jump;
    }
    else if (ADC_SAR_1_GetResult16() > 3401) // edge
    {
        jump = 2;
        deg = deg + jump;
    }
    else if (ADC_SAR_1_GetResult16() < 599) // edge
    {
        jump = 2;
        deg = deg - jump;
    }
    // boundaries
    if (deg < -41)
    {
        deg = -41;
    }
    if (deg > 41)
    {
        deg = 41;
    }
    return convertedResult = deg;
}

void mode_3()
{
    int adc = ADC_SAR_1_GetResult16();
    if (adc > 2052)
    {
        PWM_2_WritePeriod(10000/((adc-2050)/300));
        gradientShift = 1;
    }
    else if (adc < 2048)
    {
        PWM_2_WritePeriod(10000/((2050-adc)/300));
        gradientShift = -1;
    }
    else 
    {
        gradientShift = 0; 
    }
}

int motorSwitch()
{
    return MotorSwitchPin_Read();
}

int adcToDegrees(int adc_value)
{

    // Check if adc_value is out of bounds
    if (adc_value < adc_min)
    {
        adc_value = adc_min;
    }
    else if (adc_value > adc_max)
    {
        adc_value = adc_max;
    }

    // Calculate degrees using linear interpolation formula
    int degrees = ((adc_value - adc_min) * (deg_max - deg_min)) / (adc_max - adc_min) + deg_min;

    return degrees;
}