
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
	UART0_send(0xA5);
	UART0_send(length+1);
	UART0_send(msgcmd);
	sum = msgcmd;
	for (i = 0; i < length; i++)
	{
		UART0_send(msgbuffer[i]);
		sum += msgbuffer[i];
	}
	UART0_send(sum);
	WAKEUP = 1;
}

void sendHidPollMSG(unsigned char msgcmd, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
    UART0_send(0xFE);	
	UART0_send(length);
	UART0_send((unsigned char)(length>>8));
	UART0_send(msgcmd);
	UART0_send(type);
	UART0_send(device);
	UART0_send(endpoint);
	UART0_send(idVendorL);
	UART0_send(idVendorH);
	UART0_send(idProductL);
	UART0_send(idProductH);
	for (i = 0; i < length; i++)
	{
		UART0_send(msgbuffer[i]);
	}
	UART0_send('\n');
}