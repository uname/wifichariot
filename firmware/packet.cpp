#define DEBUG
#include "packet.h"
#include "debug.h"

static const uint8_t START_PATTERN[] = {0xAA, 0xBB};
static uint8_t bodyBuff[MAX_BUFF_SIZE] = {0};
static int parttenPos = 0;
static int recvFlag = RECV_START_FLAG;
static int bodyLen;
static unsigned short bodyRecved = 0;
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

int unpack(uint8_t *buff, uint8_t bodyLen, Packet *packet)
{
    uint8_t *p = buff;
    int ret = 0;
    
    (void)bodyLen;
    
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
        
    default:
        ret = -1;
        break;
    }
        
    return ret;
}

int recvPakcet(Packet *packet)
{
    int ret = -1;
    int nbytes = Serial.available();
    
    if(nbytes < 1)
    {
        return ret;
    }
      
    switch(recvFlag)
    {
    case RECV_START_FLAG:
        {
            uint8_t val = (uint8_t)Serial.read();
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
            bodyLen = (uint8_t) Serial.read();
            if(bodyLen >= MAX_BUFF_SIZE)
            {
                recvFlag = RECV_START_FLAG;
                DEBUG_PRINT("error body size, clear serial");
                break;
            }
            recvFlag = RECV_BODY_DATA;
            recvBodyTimes = 0;
            bodyRecved = 0;
            DEBUG_PRINT("recvHead OK, bodyLen is %d", bodyLen);

            break;
        }
    
    case RECV_BODY_DATA:
        {
            bodyRecved +=  Serial.readBytes((char *)(bodyBuff + bodyRecved), bodyLen - bodyRecved);
            if(bodyRecved < bodyLen)
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
                if(bodyRecved > bodyLen)
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
            uint8_t crc = Serial.read() & 0xff;
            // 暂不校验CRC
            DEBUG_PRINT("crc done");
            ret = unpack(bodyBuff, bodyLen, packet);
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

