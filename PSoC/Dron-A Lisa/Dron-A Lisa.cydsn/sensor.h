void initSensor();
void startBurst();
void stopBurst();
void startCounter();
double calcTime(double count);
double calcAngle(long count);
float FIRMovingAverage(float new_value);
float IIRFilter(float new_value);