#include <EEPROM.h>
#include "dir.h"
#include <Arduino.h>

static int isInited = 0;
static Servo servo;
static int angle = 0;
static int offsetAngle = 90; // 将小车车头调正

void dirInit()
{
    if(isInited)
    {
        return;
    }
    servo.setPulseWidth(DIR_MIN_PULSE_WIDTH, DIR_MAX_PULSE_WIDTH, DIR_DEFAULT_PULSE_WIDTH);
    servo.attach(DIR_SERVO_PIN);
    
    dirTurnToMiddle();
    
    isInited = 1;
}

void dirSetMiddleOffset(const int type, const int value)
{
    int offset = offsetAngle;
    switch(type)
    {
    case 0:
        offset++;
        break;
    case 1:
        offset--;
        break;
    case 2:
        offset = value;
        break;
        
    default:
        return;
    }
    
    if(offset < MIN_MIDDLE_ANGLE_OFFSET || offset > MAX_MIDDLE_ANGLE_OFFSET)
    {
        return;
    }
    
    offsetAngle = offset;
    dirTurnToMiddle();
}

int dirGetMiddleOffset()
{
    int offset = 0;//EEPROM.read(MIDDLEANGLEOFFSET_ADDR);
    if(offset < MIN_MIDDLE_ANGLE_OFFSET || offset > MAX_MIDDLE_ANGLE_OFFSET)
    {
        offset = MIDDLE_ANGLE_OFFSET;
    }
    //DEBUG_PRINT("middleAngleOffset=%d", offset);
    return offset;
}

void dirTurn(int angle)
{
    if(angle > DIR_MAX_TURN_ANGLE)
    {
        angle = DIR_MAX_TURN_ANGLE;
    }
    else if(angle < -DIR_MAX_TURN_ANGLE)
    {
        angle = -DIR_MAX_TURN_ANGLE;
    }
    
    angle = angle;
    servo.write(GET_REAL_TURN_ANGLE(angle));
}

void dirTurnToMiddle()
{
    servo.write(offsetAngle);
}

int dirGetCurAngle()
{
    return angle;
}
