
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "bsp.h"
#include "uart.h"

SBIT(WAKEUP, 0xB0, 3);

uint8_t __xdata uartRxBuff;

void processUart(){
    while(RI){
        RI=0;
        uartRxBuff = SBUF;
    }
}

void sendProtocolMSG(unsigned char msgcmd, unsigned short length, unsigned char __xdata *msgbuffer){
    unsigned short i;
	unsigned char sum;
	WAKEUP = 0;
	delayMs(1);
	UART0Send(0xA5);
	UART0Send(length+1);
	UART0Send(msgcmd);
	sum = msgcmd;
	for (i = 0; i < length; i++)
	{
		UART0Send(msgbuffer[i]);
		sum += msgbuffer[i];
	}
	UART0Send(sum);
	WAKEUP = 1;
}

void sendHidPollMSG(unsigned char msgcmd, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
    UART0Send(0xFE);	
	UART0Send(length);
	UART0Send((unsigned char)(length>>8));
	UART0Send(msgcmd);
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