#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "servo.h"
#include "pindef.h"

#define MOTOR_MIN_PULSE_WIDTH        1000
#define MOTOR_MAX_PULSE_WIDTH        2000
#define MOTOR_DEFAULT_PULSE_WIDTH    1500

#define MOTOR_MAX_THROTTLE           100       // 油门量程

#define SPEED_PULSE_COEFFICIENT                   (((MOTOR_MAX_PULSE_WIDTH) - (MOTOR_MIN_PULSE_WIDTH)) / 2.0) / (MOTOR_MAX_THROTTLE)
#define GET_SPEED_WIDTH_FROM_THROTTLE(throttle)   ((int)(MOTOR_DEFAULT_PULSE_WIDTH) - (SPEED_PULSE_COEFFICIENT) * (throttle))

#ifdef __cplusplus
extern "C" {
#endif

void motorInit();
void motorRun(int speed);
void motorStop();
int motorGetThrottle();
    
#ifdef __cplusplus
}
#endif

#endif

