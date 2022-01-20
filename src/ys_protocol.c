
#include "bsp.h"
#include "ys_protocol.h"

SBIT(WAKEUP, 0xB0, 3);

uint8_t __xdata uartRxBuff;


void Protocol_init()
{
	UART0_init(115200, 0);
    Pin_mode(PORT3, PIN3, PIN_MODE_OUTPUT_OPEN_DRAIN);
    WAKEUP = 1;
}


void processUart(){
    while(RI){
        RI=0;
        uartRxBuff = SBUF;
    }
}

void Protocol_sendMsg(unsigned char cmd, unsigned char __xdata *msg, unsigned short len)
{
    unsigned short i;
	unsigned char sum;

	WAKEUP = 0;
	delayMs(1);
	UART0_send(PRTL_MAGIC_BYTE);
	UART0_send(len+1);
	UART0_send(cmd);
	sum = cmd;
	for (i = 0; i < len; i++)
	{
		UART0_send(msg[i]);
		sum += msg[i];
	}
	UART0_send(sum);
	WAKEUP = 1;
}
