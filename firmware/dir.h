#ifndef __DIR_H__
#define __DIR_H__

#include "servo.h"
#include "pindef.h"

//1000
#define DIR_MIN_PULSE_WIDTH        544
//2000
#define DIR_MAX_PULSE_WIDTH        2400
//1500
#define DIR_DEFAULT_PULSE_WIDTH    1000
#define DIR_MAX_TURN_ANGLE         35
#define MIN_MIDDLE_ANGLE_OFFSET    ((MIDDLE_ANGLE_OFFSET) - (DIR_MAX_TURN_ANGLE))
#define MIDDLE_ANGLE_OFFSET        90
#define MAX_MIDDLE_ANGLE_OFFSET    ((MIDDLE_ANGLE_OFFSET) + (DIR_MAX_TURN_ANGLE))

#define MIDDLE_ANGLE               (0 + (offsetAngle))
#define GET_REAL_TURN_ANGLE(angle) ((offsetAngle) - (angle))

#ifdef __cplusplus
extern "C" {
#endif

void dirInit();
void dirTurn(int angle);
void dirTurnToMiddle();
void dirSetMiddleOffset(const int type, const int value);
int  dirGetMiddleOffset();
int  dirGetCurAngle();
    
#ifdef __cplusplus
}
#endif

#endif
