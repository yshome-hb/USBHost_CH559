
#include "bsp.h"
#include "ys_protocol.h"

SBIT(WAKEUP, 0xB0, 3);

#define UART_BUFF_SIZE		32

static uint16_t __data uartRxTick;
static uint8_t __xdata uartRxBuff[UART_BUFF_SIZE];

void Protocol_init()
{
	uartRxTick = 0;
	UART0_init(115200, 0);
    Pin_mode(PORT3, PIN3, PIN_MODE_OUTPUT_OPEN_DRAIN);
    WAKEUP = 1;
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

uint8_t Protocol_recvAck()
{
	uint8_t rxIndex = 0;

	uartRxTick = clock_time();
	while(TIMER_DIFF(uartRxTick) < 100)
	{
		if(RI)
		{
			RI = 0;
			uartRxBuff[rxIndex++] = SBUF;
			if((rxIndex == 3) && 
			   (uartRxBuff[2] = (uartRxBuff[0] + uartRxBuff[1])))
			{
				return uartRxBuff[0];
			}
		}
	}

	return 0;
}
