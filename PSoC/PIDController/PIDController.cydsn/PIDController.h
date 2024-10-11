#pragma once
#include "project.h"

// The PIDStartUp function initializes the PID controller with the given parameters.
// kp is the proportional gain factor, ki is the integral gain factor, and kd is the derivative gain factor.
// Additionally, iMax and iMin define the maximum and minimum limits for the integral term, respectively.
// The time interval between each sample is specified by the parameter dt, in seconds.
void PIDStartUp(float kp, float ki, float kd, float iMax, float iMin, float dt);

//The PIDSetpointChange function updates the setpoint, which is the target value the system 
//should reach. This target value is provided through the setpoint parameter.
void PIDSetpointChange(float setpoint);


//The PIDUpdate function performs a single step of the PID control algorithm. 
//The parameters proportional, integral, and derivative are useful for analysis or debugging, 
//as they hold the values of the proportional, integral, and derivative terms respectively. 
//The current system output is provided via the currentOutput parameter, and the PID controller’s output result is returned as a float.
float PIDUpdate(float currentOutput, float* proportional, float* integral, float* derivative);