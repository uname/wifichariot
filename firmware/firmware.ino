#include "controller.h"
#include "pindef.h"
#include "debug.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>

void setup()
{
    Serial.begin(115200);
    pinMode(SIG_LED_PIN, OUTPUT);
    
    controllerInit();
    DEBUG_PRINT("system running...");
    
    while(1)
    {
        flashSigLED();
        handlePacket();
    }
}

void loop()
{
}

void flashSigLED()
{
    static unsigned char lightStatus = 0;
    static long lightCount = 0;
    if( ++lightCount < 10000)
    {
        return;
    }
    lightCount = 0;
    lightStatus = !lightStatus;
    digitalWrite(SIG_LED_PIN, lightStatus);
}

