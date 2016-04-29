#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG
#include <Arduino.h>

static char dbgBuff[128] = {0};

#define DEFAULT_LOG_LEVEL LOG_LEVEL_DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    do \
    { \
        snprintf(dbgBuff, sizeof(dbgBuff) - 1, "[%15s] ", __FUNCTION__); \
        snprintf(dbgBuff + strlen(dbgBuff), sizeof(dbgBuff) - strlen(dbgBuff) - 1, fmt, ##__VA_ARGS__ ); \
        Serial.println(dbgBuff); \
    } while(0)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void printBin(unsigned char *buff, int len);

#endif
