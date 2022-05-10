
#include "bsp.h"
#include "uart_protocol.h"

SBIT(WAKEUP_PIN, 0xB0, 2);
uint8_t __xdata uartRxBuff[UART_BUFF_SIZE];

static void UartProtocol_sendMsg(uint8_t cmd, uint8_t __xdata *msg, uint8_t len)
{
    uint8_t i, sum;

	RI = 0;
	WAKEUP_PIN = 0;
	delayMs(1);
	UART0_send(PRTL_MAGIC_BYTE);
	sum = PRTL_MAGIC_BYTE;
	UART0_send(cmd);
	sum ^= cmd;
	UART0_send(len);
	sum ^= len;
	for (i = 0; i < len; i++)
	{
		UART0_send(msg[i]);
		sum ^= msg[i];
	}
	UART0_send(sum);
	WAKEUP_PIN = 1;
}

static uint8_t UartProtocol_waitAck(uint8_t cmd)
{
	static uint16_t __data rxTick = 0;
	uint8_t rxIndex = 0;
	uint8_t checkSum = 0;

	rxTick = clock_time();
	while(TICK_DIFF(rxTick) < 100)
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

void UartProtocol_init()
{
    //Pin_mode(PORT3, PIN2, PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK);
	P3_DIR |= PIN2;
    WAKEUP_PIN = 1;
	UART0_init(115200, 0);
}

void UartProtocol_writeHID(uint8_t report, uint8_t *buf, uint8_t len)
{
	uint8_t i, sum;
	
	RI = 0;
	WAKEUP_PIN = 0;
	delayMs(1);
	UART0_send(PRTL_MAGIC_BYTE);
	sum = PRTL_MAGIC_BYTE;
	UART0_send(CMD_HIDREPORT);
	sum ^= CMD_HIDREPORT;
	UART0_send(len+1);
	sum ^= (len+1);
	UART0_send(report);
	sum ^= report;
	for(i = 0; i < len; i++)
	{
		UART0_send(buf[i]);
		sum ^=buf[i];
	}
	UART0_send(sum);
	WAKEUP_PIN = 1;
}

uint8_t UartProtocol_readStatus()
{
	UartProtocol_sendMsg(CMD_KBSTATUS, NULL, 0);
	return UartProtocol_waitAck(CMD_KBSTATUS);
}