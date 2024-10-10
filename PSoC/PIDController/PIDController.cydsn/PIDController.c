#include "PIDController.h"

// The PIDControl_StartUp function initializes the PID controller with the given parameters. 
// Kp is the proportional gain factor, Ki is the integral gain factor, and Kd is the derivative gain factor. 
// Additionally, integralMax and integralMin define the maximum and minimum limits for the integral term, respectively. 
// The time interval between each sample is specified by the parameter dt, in seconds.
void PIDControl_StartUp(float Kp, float Ki, float Kd, float integralMax, float integralMin, float dt)
{

}

/**
 * The PIDControl_changeSetPoint function updates the setpoint, which is the target value the system 
 * should reach. This target value is provided through the setPoint parameter.
 */
void PIDControl_changeSetPoint(float setPoint)
{

}

/**
 * The PIDControl_OneStep function performs a single step of the PID control algorithm. 
 * The parameters proportionalPart, integralPart, and derivativePart are useful for analysis or debugging, 
 * as they hold the values of the proportional, integral, and derivative terms respectively. 
 * The current system output is provided via the systemOutput parameter, and the PID controller’s result is 
 * returned as a float.
 */
float PIDControl_OneStep(float systemOutput, float* proportionalPart, float* integralPart, float* derivativePart)
{

}