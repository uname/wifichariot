#include "packet.h"
#include "pindef.h"
#include "debug.h"
#include <SoftwareSerial.h>

static SoftwareSerial softSerial(SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN); // RX, TX
static const uint8_t START_PATTERN[] = {0xAA, 0xBB};
static uint8_t bodyBuff[MAX_BUFF_SIZE] = {0};
static int parttenPos = 0;
static int recvFlag = RECV_START_FLAG;
static int size;
static unsigned short recvSize = 0;
static int recvBodyTimes = 0;

static unsigned long int unpacku32(unsigned char *buf)
{
	return ((unsigned long int)*buf++ << 24) |
	       ((unsigned long int)*buf++ << 16) |
	       ((unsigned long int)*buf++ << 8)  |
	        (unsigned long int)*buf++;
}

static void unpackSayHi(Packet *packet, uint8_t *buf)
{
    packet->sayHi.rsv = (uint8_t)*buf;
    DEBUG_PRINT("-> rsv is %d", packet->sayHi.rsv);
}

static void unpackTurn(Packet *packet, uint8_t *buf)
{
    packet->turn.angle = (int8_t)*buf;
    DEBUG_PRINT("-> angle is %d", packet->turn.angle);
}

static void unpackRun(Packet *packet, uint8_t *buf)
{
    packet->run.speed = (int16_t)*buf;
    DEBUG_PRINT("-> speed is %d", packet->run.speed);
}

void packetInit()
{
    softSerial.begin(115200);
}

int unpack(uint8_t *buff, uint8_t size, Packet *packet)
{
    uint8_t *p = buff;
    int ret = 0;
    
    (void)size;
    
    if(buff == NULL || packet == NULL)
    {
        return -1;
    }
    memset(packet, 0, sizeof(*packet));
    packet->cmd = (uint8_t)*p++;
    switch(packet->cmd)
    {
    case SAY_HI:
        unpackSayHi(packet, p);
        break;
    
    case TURN:
        unpackTurn(packet, p);
        break;
    
    case RUN:
        unpackRun(packet, p);
        break;
        
    default:
        ret = -1;
        break;
    }
        
    return ret;
}

int recvPakcet(Packet *packet)
{
    int ret = -1;
    int nbytes = softSerial.available();
    
    if(nbytes < 1)
    {
        return ret;
    }
      
    switch(recvFlag)
    {
    case RECV_START_FLAG:
        {
            uint8_t val = (uint8_t)softSerial.read();
            if(val == START_PATTERN[parttenPos])
            {
                if(++parttenPos == sizeof(START_PATTERN))
                {
                    recvFlag = RECV_SIZE;
                    parttenPos = 0;
                    DEBUG_PRINT("start flag ok");
                    break;
                }
                
            }
            else
            {
                DEBUG_PRINT("flag error");
                parttenPos = 0;
            }
            
            break;
        }
    
    case RECV_SIZE:
        {
            size = (uint8_t) softSerial.read();
            if(size >= MAX_BUFF_SIZE)
            {
                recvFlag = RECV_START_FLAG;
                DEBUG_PRINT("error body size, clear serial");
                break;
            }
            recvFlag = RECV_BODY_DATA;
            recvBodyTimes = 0;
            recvSize = 0;
            DEBUG_PRINT("recvHead OK, size is %d", size);

            break;
        }
    
    case RECV_BODY_DATA:
        {
            recvSize +=  softSerial.readBytes((char *)(bodyBuff + recvSize), size - recvSize);
            if(recvSize < size)
            {
                if(++recvBodyTimes > MAX_RECV_BODY_TIMES_FOR_ONE_PKG)
                {
                    recvFlag = RECV_START_FLAG;
                    DEBUG_PRINT("RECV BODY TIMEOUT, RESET TO START FLAG");
                }
                break;
                
            }
            else
            {
                if(recvSize > size)
                {
                    DEBUG_PRINT("ERROR!!!!!");
                }
                else
                {
                    DEBUG_PRINT("recv buff done");
                }
                recvFlag = RECV_CRC;
            }
            
            break;
        }
        
    case RECV_CRC:
        {
            DEBUG_PRINT("recv crc %d", nbytes);
            uint8_t crc = softSerial.read() & 0xff;
            // 暂不校验CRC
            DEBUG_PRINT("crc done");
            ret = unpack(bodyBuff, size, packet);
            recvFlag = RECV_START_FLAG;
            break;
        }
        
    default:
        {
            DEBUG_PRINT("ERROR");
            break;
        }
    }
    
    return ret;
}

