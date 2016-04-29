#ifndef __PACKET_H__
#define __PACKET_H__

#include <Arduino.h>

#define MAX_BUFF_SIZE                    32
#define RECV_START_FLAG                  0
#define RECV_SIZE                        1
#define RECV_BODY_DATA                   2
#define RECV_CRC                         3
#define MAX_RECV_BODY_TIMES_FOR_ONE_PKG  2

#pragma pack(1)

enum Commands
{
    SAY_HI              = 0, // For protocol testing
    TURN                = 1,
    RUN                 = 2,
};


typedef struct _SayHi
{
    int8_t rsv;
} SayHi;


typedef struct _Turn
{
    int8_t angle;
} Turn;

typedef struct _Run
{
    int16_t speed;
} Run;


typedef struct _Pakcet
{
    uint8_t  size;
    uint8_t  cmd;
    union
    {
        SayHi sayHi;
        Turn turn;
        Run run;
    };
    uint8_t  crc;
} Packet;

void packetInit();
int recvPakcet(Packet *packet);
int unpack(uint8_t *buff, uint8_t size, Packet *packet);

#endif
