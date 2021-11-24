#ifndef __uart_H__
#define __uart_H__

typedef enum
{
    MSG_TYPE_DISCONNECTED = 0x35,
    MSG_TYPE_CMD = 0x5A,
    MSG_TYPE_DATA = 0xA5,
    MSG_TYPE_ERROR = 0xFF,
}MSG_TYPE_TypeDef;

void processUart();
void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH);
void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char __xdata *msgbuffer);

#endif