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

void init_interface()
{
    Init_LCD();
    Init_ISR();
    Init_PWM();
    Init_ADC();
}

void Init_LCD()
{
    LCD_ClearDisplay();
    LCD_Start();
    LCD_Position(0, 0);
    LCD_PrintString("Starting");
    LCD_Position(1, 0);
    LCD_PrintString("Application");
    CyDelay(1000);
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
    // Define constants
    const int adc_min = 30;   // ADC value corresponding to -41 degrees
    const int adc_max = 4050; // ADC value corresponding to +41 degrees
    const int deg_min = -41;  // Minimum degrees
    const int deg_max = 41;   // Maximum degrees

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