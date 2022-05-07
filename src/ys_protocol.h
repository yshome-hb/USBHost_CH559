#ifndef __YS_PROTOCOL_H__
#define __YS_PROTOCOL_H__

#define PRTL_MAGIC_BYTE		0xA5

enum PROTOCOL_CMD {
    CMD_REBOOT,
    CMD_SLEEP,
    CMD_SWITCH,
    CMD_RESTORE = 0x0F,

    CMD_DEVINFO = 0x10,
    CMD_BLENAME,
    CMD_RFCONFIG,

    CMD_KBSTATUS = 0x20,
    CMD_HIDREPORT,
};

enum PROTOCOL_RESP {
    RESP_SUCC = 0x00,
    RESP_FAIL = 0x80,
    ERROR_TIMEOUT = 0xA0,
    ERROR_CHECKSUM = 0xA1,
};

#define PRTL_PAYLOAD        uartRxBuff[3]
#define UART_BUFF_SIZE	    32
extern uint8_t __xdata uartRxBuff[UART_BUFF_SIZE];

void Protocol_init();
void Protocol_sendMsg(unsigned char cmd, unsigned char __xdata *msg, unsigned short len);
uint8_t Protocol_recvAck(unsigned char cmd);


#endif