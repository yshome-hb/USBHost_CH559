
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "bsp.h"
#include "uart.h"

SBIT(LED, 0x90, 6);

uint8_t __xdata uartRxBuff;

void processUart(){
    while(RI){
        RI=0;
        uartRxBuff = SBUF;
		LED = uartRxBuff & 0x01;
    }
}

void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char __xdata *msgbuffer){
    unsigned short i;
	UART0Send(msgtype);
	for (i = 0; i < length; i++)
	{
		UART0Send(msgbuffer[i]);
	}
}

void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
    UART0Send(0xFE);	
	UART0Send(length);
	UART0Send((unsigned char)(length>>8));
	UART0Send(msgtype);
	UART0Send(type);
	UART0Send(device);
	UART0Send(endpoint);
	UART0Send(idVendorL);
	UART0Send(idVendorH);
	UART0Send(idProductL);
	UART0Send(idProductH);
	for (i = 0; i < length; i++)
	{
		UART0Send(msgbuffer[i]);
	}
	UART0Send('\n');
}