#ifndef __uart_H__
#define __uart_H__

typedef enum {
    CMD_REBOOT,
    CMD_SLEEP,
    CMD_SWITCH,
    CMD_BLENAME,
    CMD_RFINFO,
    CMD_REPORT,
    CMD_STATUS,
    CMD_RESTORE,
}MSG_CMD_TypeDef;

void processUart();
void sendProtocolMSG(unsigned char msgcmd, unsigned short length, unsigned char __xdata *msgbuffer);

#endif