
#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "USBHost.h"
#include "ys_protocol.h"

SBIT(POWER, 0xC0, 3);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main()
{
    Clock_init();
    SysClk_init();

    YS_DEBUG_INIT(115200);
    YS_LOG("Startup\n");

    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    Protocol_init();
    YS_LOG("Ready\n");

    Pin_mode(PORT4, PIN3, PIN_MODE_OUTPUT);
    POWER = 1;

    IRQ_enable();

    while(1)
    {
        if(!(P4_IN & (1 << 6)))
            runBootloader();
        processUart();
        checkRootHubConnections();
        pollHIDdevice();
    }
}

/*******************************************************************************
* Function Name  : TMR2_Interrupt
* Description    : Timer2中断服务程序
* Input          : None
* Return         : None
*******************************************************************************/	
static INTERRUPT(TMR2_Interrupt, INT_NO_TMR2)  
{
	TF2 = 0;
	sysTick++;
}

/**
 * stdio printf directed to UART0 using putchar and getchar
 */
int putchar(int c)
{
    UART1_send(c);
	return c;
}
