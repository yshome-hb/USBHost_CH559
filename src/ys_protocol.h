#ifndef __YS_PROTOCOL_H__
#define __YS_PROTOCOL_H__

#define PRTL_MAGIC_BYTE		0xA5

enum PROTOCOL_CMD {
    CMD_REBOOT,
    CMD_SLEEP,
    CMD_SWITCH,
    CMD_BLENAME,
    CMD_RFINFO,
    CMD_REPORT,
    CMD_STATUS,
    CMD_RESTORE,
};

enum PROTOCOL_RESP {
    RESP_ACK,
    RESP_FAILED = 0xFF,
};

void Protocol_init();
void Protocol_sendMsg(unsigned char cmd, unsigned char __xdata *msg, unsigned short len);

void processUart();


#endif