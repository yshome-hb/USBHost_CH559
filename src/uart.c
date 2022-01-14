
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
