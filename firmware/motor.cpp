#include "motor.h"
#include "debug.h"
#include <Arduino.h>

static bool isInited = false;
static Servo servo;
static int throttle;

void motorInit()
{
    if(isInited)
    {
        return;
    }
    
    servo.setPulseWidth(MOTOR_MIN_PULSE_WIDTH, MOTOR_MAX_PULSE_WIDTH, MOTOR_DEFAULT_PULSE_WIDTH);
    servo.attach(MOTOR_PIN);
    
    throttle = 0;
    
    isInited = true;
}

int motorGetThrottle()
{
    return throttle;
}

void motorRun(int throttle_)
{   
    if(throttle_ > MOTOR_MAX_THROTTLE)
    {
        throttle_ = MOTOR_MAX_THROTTLE;
    }
    else if(throttle_ < -MOTOR_MAX_THROTTLE)
    {
        throttle_ = -MOTOR_MAX_THROTTLE;
    }
    
    throttle = throttle_;
    DEBUG_PRINT("**MOVE WITH %d", throttle_);
    servo.writeMicroseconds(GET_SPEED_WIDTH_FROM_THROTTLE(throttle));
}

void motorStop()
{
    motorRun(0);
}

