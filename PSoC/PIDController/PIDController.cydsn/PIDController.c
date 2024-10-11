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


// The PIDStartUp function initializes the PID controller with the given parameters.
// kp is the proportional gain factor, ki is the integral gain factor, and kd is the derivative gain factor.
// Additionally, iMax and iMin define the maximum and minimum limits for the integral term, respectively.
// The time interval between each sample is specified by the parameter dt, in seconds.
void PIDStartUp(float kp, float ki, float kd, float iMax, float iMin, float dt)
{
    if (dt_ <= 0 || iMax_<= iMin_)
    {
        //Error handling
        return;
    }
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    iMin_ = iMin;
    iMax_ = iMax;
    dt_ = dt;
}

//The PIDSetpointChange function updates the setpoint, which is the target value the system 
//should reach. This target value is provided through the setpoint parameter.
void PIDSetpointChange(float setpoint)
{
    setpoint_ = setpoint;
}


//The PIDUpdate function performs a single step of the PID control algorithm. 
//The parameters proportional, integral, and derivative are useful for analysis or debugging, 
//as they hold the values of the proportional, integral, and derivative terms respectively. 
//The current system output is provided via the currentOutput parameter, and the PID controller’s output result is returned as a float.
float PIDUpdate(float currentOutput, float* proportional, float* integral, float* derivative)
{
    float output = 0;
    float p = 0;
    float i = 0;
    float d = 0;
    float currentError = 0;
    
    //Calculating the current error
    currentError = setpoint_ - currentOutput;
    
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