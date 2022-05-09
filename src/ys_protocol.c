
#include "bsp.h"
#include "ys_protocol.h"

SBIT(WAKEUP_PIN, 0xB0, 2);

static uint16_t __data uartRxTick;
uint8_t __xdata uartRxBuff[UART_BUFF_SIZE];

void Protocol_init()
{
	uartRxTick = 0;
	UART0_init(115200, 0);
    //Pin_mode(PORT3, PIN2, PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK);
	P3_DIR |= PIN2;
    WAKEUP_PIN = 1;
}

void Protocol_sendMsg(unsigned char cmd, unsigned char __xdata *msg, unsigned short len)
{
    unsigned short i;
	unsigned char sum;

	RI = 0;
	WAKEUP_PIN = 0;
	delayMs(1);
	UART0_send(PRTL_MAGIC_BYTE);
	UART0_send(cmd);
	UART0_send(len);
	sum = PRTL_MAGIC_BYTE ^ cmd ^ len;
	for (i = 0; i < len; i++)
	{
		UART0_send(msg[i]);
		sum ^= msg[i];
	}
	UART0_send(sum);
	WAKEUP_PIN = 1;
}

uint8_t Protocol_recvAck(unsigned char cmd)
{
	uint8_t rxIndex = 0;
	uint8_t checkSum = 0;

	uartRxTick = clock_time();
	while(TIMER_DIFF(uartRxTick) < 100)
	{
		if(RI)
		{
			RI = 0;
			uartRxBuff[rxIndex] = SBUF;
			if(rxIndex == 0 && uartRxBuff[0] != PRTL_MAGIC_BYTE)
				continue;

			checkSum ^= uartRxBuff[rxIndex++];
			if(rxIndex > 2 && rxIndex > (uartRxBuff[2] + 3))
			{
				if(checkSum == 0)
				{
					return uartRxBuff[1] ^ cmd;
				}
				else
				{
					return ERROR_CHECKSUM;
				}
			}
		}
	}

	return ERROR_TIMEOUT;
}
