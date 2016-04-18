#include "controller.h"
#include "packet.h"
#include "dir.h"
#include "motor.h"
#include "debug.h"
#include <Arduino.h>

static Packet packet;
static void handleSayHi(const SayHi *req);

void controllerInit()
{
    dirInit();
    motorInit();
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
    }
}


static void handleSayHi(const SayHi *req)
{
    DEBUG_PRINT("OK");
    pinMode(13, OUTPUT);
    digitalWrite(13, req->rsv > 0 ? HIGH : LOW);
    motorRun(req->rsv);
    dirTurn(req->rsv);
}
