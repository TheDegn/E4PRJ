#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//ISR Prototypes
CY_ISR_PROTO(ISR_toggle_handler);
CY_ISR_PROTO(ISR_print_handler);

//nyt
CY_ISR_PROTO(ISR_gradient_handler);

//Functions
int adcToDegrees(int adc_value);
int Gradient_SW();
int Gradient();
int AdjustDeg_SW();
void Init_ALL();
void Init_LCD();
void Init_ADC();
void Init_PWM();
void Init_ISR();

    



//Global Variels.
int16_t convertedResult = 0;
int toggle = 2;
int deg = 0;
char LCDbuffer[20];
int block = 0;
char tempbuffer[20];
int pass = 1;

//nyt
int gradientShift = 0;
int gradientCount = 0;


/* The LCD format in characters */
#define LCD_ROWS                (2u)
#define LCD_COLUMNS             (16u)



int main(void)
{

    CyDelay(1000); 
    Init_ALL();
    
    // test
    Clock_3_Start();
    PWM_2_Start();
    isr_gradient_StartEx(ISR_gradient_handler);
    
    for(;;)
    {
        if(toggle == 0)
        { 
            LCD_Position(0,0);
            LCD_PrintString("mode 1");
            LCD_Position(0,15);
            LCD_PutChar(1);
            
            AdjustDeg_SW();
            
            //nyt - reset mode 2
            gradientCount = 0;
        }
        else if (toggle == 1)
        {
            CyDelay(160);
            Gradient_SW();
            gradientCount = 0;
        }
        else
        {
            int adc = ADC_SAR_1_GetResult16();
            if (adc > 2052)
            {
                PWM_2_WritePeriod(10000/((adc-2050)/400));
                gradientShift = 1;
            }
            else if (adc < 2048)
            {
                PWM_2_WritePeriod(10000/((2050-adc)/400));
                gradientShift = -1;
            }
            else 
            {
                gradientShift = 0; 
            }
            
            CyDelay(160);
            //Gradient_SW();   
                
        }                  
    }
}



void Init_ALL(){
    Init_LCD();
    Init_ISR();
    Init_PWM();
    Init_ADC();
}


void Init_LCD(){
    LCD_ClearDisplay();
    LCD_Start();
    LCD_Position(0, 0);
    LCD_PrintString("Starting");
    LCD_Position(1, 0);
    LCD_PrintString("Application");
    CyDelay(1000);    
}

void Init_ADC(){
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert(); 
}

void Init_PWM(){
    PWM_1_Start();    
}

void Init_ISR(){
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_toggle_StartEx(ISR_toggle_handler);
    isr_print_StartEx(ISR_print_handler);   
}


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


CY_ISR(ISR_toggle_handler){
    deg = convertedResult;
    convertedResult = deg;
    if (0 == toggle)
    {
        toggle = 1;
    }
    else if (1 == toggle)
    {
        toggle = 2;
    }
    else if (2 == toggle)
    {
        toggle = 0;
    }
}


CY_ISR(ISR_print_handler){
    
    if(0 == toggle)
    {
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("mode 1");
            LCD_Position(0,15);
            LCD_PutChar(1);
            sprintf(LCDbuffer, "%i             ", convertedResult);    
            LCD_Position(1, 0);
            LCD_PrintString(LCDbuffer);
            LCD_Position(1,15);
            LCD_PutChar(0);
    }
    else if (1 == toggle)
    {
        LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("mode 2");
            LCD_Position(0,15);
            LCD_PutChar(3);
            sprintf(LCDbuffer, "%i             ", convertedResult);   
            LCD_Position(1, 0);
            LCD_PrintString(LCDbuffer);
            LCD_Position(1,15);
            LCD_PutChar(0);
    }
    else
    {
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("mode 3");
            LCD_Position(0,15);
            LCD_PutChar(3);
            sprintf(LCDbuffer, "%i             ", gradientCount);   
            LCD_Position(1, 0);
            LCD_PrintString(LCDbuffer);
            LCD_Position(1,15);
            LCD_PutChar(0);
    }
}


int Gradient_SW(){
    int jump = 1;
                           
        if(ADC_SAR_1_GetResult16()>2200&&ADC_SAR_1_GetResult16()<3400) //middle
        {
            jump = 1;
            deg = deg + jump;
        }
        else if(ADC_SAR_1_GetResult16()<1900&&ADC_SAR_1_GetResult16()>600) //middle
        {
           jump = 1;
           deg = deg - jump;
        }
        else if(ADC_SAR_1_GetResult16()>3401) //edge
        {
            jump = 2;
            deg = deg + jump;
            
        }
        else if(ADC_SAR_1_GetResult16()<599) //edge
        {
            jump = 2;
           deg = deg - jump;
        }
        
        //boundaries
        if(deg<-41)
            {
            deg = -41;
            }
           
        if(deg>41)
        {
        deg = 41;
        }
       
           
return convertedResult = deg;  
}

int AdjustDeg_SW(){
    
    //average
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



int adcToDegrees(int adc_value){
    // Define constants
    const int adc_min = 30;     // ADC value corresponding to -41 degrees
    const int adc_max = 4050;   // ADC value corresponding to +41 degrees
    const int deg_min = -41;    // Minimum degrees
    const int deg_max = 41;     // Maximum degrees
    
    // Check if adc_value is out of bounds
    if (adc_value < adc_min) {
        adc_value = adc_min;
    } else if (adc_value > adc_max) {
        adc_value = adc_max;
    }
    
    // Calculate degrees using linear interpolation formula
    int degrees = ((adc_value - adc_min) * (deg_max - deg_min)) / (adc_max - adc_min) + deg_min;

    return degrees;
}



