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
    RESP_SUCC = 0xA5,
    RESP_FAIL = 0xAA,
};

#define PRTL_PAYLOAD        uartRxBuff[1]
#define UART_BUFF_SIZE	    32
extern uint8_t __xdata uartRxBuff[UART_BUFF_SIZE];

void Protocol_init();
void Protocol_sendMsg(unsigned char cmd, unsigned char __xdata *msg, unsigned short len);
uint8_t Protocol_recvAck();


#endif