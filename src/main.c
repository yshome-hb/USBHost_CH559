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

SBIT(LED, 0x90, 6);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main()
{
    unsigned char s;
    initClock();
    debug_printf_init(115200);
    initUART0(115200, 0);
    YS_LOG("Startup\n");
    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    YS_LOG("Ready\n");
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
    UART1Send(c);
	return c;
}

int getchar() 
{
    return UART1Receive();
}
