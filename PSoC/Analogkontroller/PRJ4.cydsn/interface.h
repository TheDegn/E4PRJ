#ifndef INTERFACE_H
#define INTERFACE_H

#include "project.h"
#include <stdio.h>
#include <stdlib.h>

//Intializering
void init_interface();
void Init_LCD();
void Init_ADC();
void Init_PWM();
void Init_ISR();

//Functions
void writeLCD(char *top, int endTopChar, char *buttom, int endButtomChar);

int adcToDegrees(int adc_value);

int mode_1();
int mode_2();
void mode_3();

int motorSwitch();

//ISR Prototypes
CY_ISR_PROTO(ISR_toggle_handler);
CY_ISR_PROTO(ISR_print_handler);
CY_ISR_PROTO(ISR_gradient_handler);

/* The LCD format in characters */
#define LCD_ROWS                (2u)
#define LCD_COLUMNS             (16u)

#endif /* INTERFACE_H */