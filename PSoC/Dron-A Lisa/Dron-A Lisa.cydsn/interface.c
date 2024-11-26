#include "project.h"
#include "interface.h"

// Global Variels.
char LCDbuffer[20];
int block = 0;

// Define constants
#define adc_min 30   // ADC value corresponding to -41 degrees
#define adc_max 4050 // ADC value corresponding to +41 degrees
#define deg_min -35  // Minimum degrees
#define deg_max 35   // Maximum degrees




void init_interface()
{
    Init_LCD();
    Init_ADC();
    isr_toggle_StartEx(ISR_toggle_handler);
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


void writeLCD(int setpoint, float sensor)
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

int interfaceSetpoint()
{
    //return adcToDegrees( FIRMovingAverage( ADC_SAR_1_GetResult16() ) );
    return adcToDegrees( ADC_SAR_1_GetResult16() );
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

CY_ISR(ISR_toggle_handler)
{
    block = !block;
}