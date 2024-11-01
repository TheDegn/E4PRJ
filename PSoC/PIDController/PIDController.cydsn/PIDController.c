#include "PIDController.h"

float kp_ = 0;
float ki_ = 0;
float kd_ = 0;
float iMin_ = 0;
float iMax_ = 0;
float dt_ = 0;
float setpoint_ = 0;
float previousError_ = 0;
float i_ = 0;

//The PIDUpdate function performs a single step of the PID control algorithm. 
//The parameters proportional, integral, and derivative are useful for analysis or debugging, 
//as they hold the values of the proportional, integral, and derivative terms respectively. 
//The physical systems current output is provided via the measurement parameter.
//The parameter setpoint sets the wanted setpoint for the PID system.
//PID controller’s output result is returned as a float.
float PIDUpdate(float setpoint, float measurement, float* proportional, float* integral, float* derivative)
{
    float output = 0;
    float p = 0;
    float i = 0;
    float d = 0;
    float currentError = 0;

    //Calculating the current error
    currentError = setpoint - measurement;
    
    //Calculating the proportional part
    p = kp_* currentError;
    
    //Calculating the integral part
    i_ += ki_ * (currentError * dt_);
    
    //Limits for the integral part
    i_ = (i_ < iMin_) ? iMin_ : (i_ > iMax_) ? iMax_ : i_;
    
    //Assign to the i variable for possible debugging
    i = i_;
    
    //Calculating the derivative part, and avoiding division by 0
    d = (dt_ <= 0) ? 0: kd_ * (currentError - previousError_) / dt_;
    
    //Calculating the output
    output = p + i + d;
    
    //Saving the p, i and d for analysis or debugging
    *proportional = p;
    *integral = i;
    *derivative = d;
    
    //Saving the current error to later use
    previousError_ = currentError;
    
    return output;
}