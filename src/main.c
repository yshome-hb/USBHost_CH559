typedef unsigned char *PUINT8;
typedef unsigned char __xdata *PUINT8X;
typedef const unsigned char __code *PUINT8C;
typedef unsigned char __xdata UINT8X;
typedef unsigned char __data UINT8D;

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "bsp.h"
#include "USBHost.h"
#include "uart.h"

SBIT(POWER, 0xC0, 3);
SBIT(WAKEUP, 0xB0, 3);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main()
{
    unsigned char s;
    Clock_init();
    YS_DEBUG_INIT(115200);
    UART0_init(115200, 0);
    YS_LOG("Startup\n");
    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    YS_LOG("Ready\n");

    Pin_mode(PORT3, PIN3, PIN_MODE_OUTPUT_OPEN_DRAIN);
    Pin_mode(PORT4, PIN3, PIN_MODE_OUTPUT);
    WAKEUP = 1;
    POWER = 1;

    while(1)
    {
        if(!(P4_IN & (1 << 6)))
            runBootloader();
        processUart();
        s = checkRootHubConnections();
        pollHIDdevice();
    }
}

/**
 * stdio printf directed to UART0 using putchar and getchar
 */
int putchar(int c)
{
    UART1_send(c);
	return c;
}

int getchar() 
{
    return UART1_receive();
}
