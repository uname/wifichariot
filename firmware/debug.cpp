#include "debug.h"

void printBin(unsigned char *buff, int len)
{
#ifdef DEBUG
    int i;
    int nlen = 0;
    static char buf[512];
    char tmp[128];
    
    if (len <= 0 || len > (int)sizeof(buf) || buff == NULL) {
        return;
    }
    
    buf[0] = 0;

    for (i = 0; i < len; i++) {
        if (!(i % 16)) {
            sprintf(tmp, "\n%04d> ", i / 16 + 1);
            nlen += sprintf(buf + nlen, tmp);
        }
        sprintf(tmp, "%02X ", (unsigned char)buff[i]);
        nlen += sprintf(buf + nlen, "%s", tmp);
    }
    
    Serial.println(buf);

#endif
}
