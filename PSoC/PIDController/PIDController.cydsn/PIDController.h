#pragma once
#include "project.h"

//The PIDUpdate function performs a single step of the PID control algorithm. 
//The parameters proportional, integral, and derivative are useful for analysis or debugging, 
//as they hold the values of the proportional, integral, and derivative terms respectively. 
//The physical systems current output is provided via the measurement parameter.
//The parameter setpoint sets the wanted setpoint for the PID system.
//PID controller’s output result is returned as a float.
float PIDUpdate(float setpoint, float measurement, float* proportional, float* integral, float* derivative);