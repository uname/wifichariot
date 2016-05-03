#include "controller.h"
#include "packet.h"
#include "dir.h"
#include "motor.h"
#include "packet.h"
#include "debug.h"
#include <Arduino.h>

static bool isInited = false;
static Packet packet;
static void handleSayHi(const SayHi *req);
static void handleTurn(const Turn *req);
static void handleRun(const Run *req);

void controllerInit()
{
    if(isInited)
    {
        return;
    }
    
    dirInit();
    motorInit();
    packetInit();
    isInited = true;
    DEBUG_PRINT("controllerInit OK");
}

void handlePacket()
{
    if(recvPakcet(&packet) != 0)
    {
        return;
    }
    
    DEBUG_PRINT("handle the packet...");
    switch(packet.cmd) {
    case SAY_HI:
        handleSayHi(&packet.sayHi);
        break;
    
    case TURN:
        handleTurn(&packet.turn);
        break;
    
    case RUN:
        handleRun(&packet.run);
        break;
    
    default:
        break;
    }
}

static void handleSayHi(const SayHi *req)
{
    for(int i = 0; i < 180; i++)
    {
        tone(TONE_PIN, 900 + sin(i * 3.1412 / 180) * 1800, 10);
        delay(2);
    }
}

static void handleTurn(const Turn *req)
{
    dirTurn(req->angle);
}

static void handleRun(const Run *req)
{
    motorRun(req->speed);
}

