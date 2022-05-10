#ifndef __UART_PROTOCOL_H__
#define __UART_PROTOCOL_H__

#define PRTL_MAGIC_BYTE		0xA5

enum PROTOCOL_CMD {
/*---------- system command ----------*/
    CMD_REBOOT,
    CMD_SLEEP,
    CMD_SWITCH,
    CMD_RESTORE = 0x0F,

/*---------- get info ----------*/
    CMD_GET_DEVINFO = 0x10,
    CMD_GET_DEVMODE,

/*---------- set config ----------*/
    CMD_SET_RFCONFIG,
    CMD_SET_DEVNAME = 0x30,

/*---------- hid status ----------*/
    CMD_HIDSTATUS_KB = 0x60,

/*---------- hid report ----------*/
    CMD_HIDREPORT_MIN = 0x70,
    CMD_HIDREPORT_MAX = 0x7F,

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

void UartProtocol_init();
void UartProtocol_writeHID(uint8_t report, uint8_t *buf, uint8_t len);
uint8_t UartProtocol_readStatus();


#endif