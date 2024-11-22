#ifndef INTERFACE_H
#define INTERFACE_H

#include "project.h"
#include <stdio.h>
#include <stdlib.h>

//Intializering
void init_interface();
void Init_LCD();
void Init_ADC();

//Functions
void writeLCD(int setpoint, float sensor);

int adcToDegrees(int adc_value);

int interfaceSetpoint();

//ISR Prototypes
CY_ISR_PROTO(ISR_motorSwitch_On_handler);
CY_ISR_PROTO(ISR_motorSwitch_Off_handler);

/* The LCD format in characters */
#define LCD_ROWS                (2u)
#define LCD_COLUMNS             (16u)

#endif /* INTERFACE_H */