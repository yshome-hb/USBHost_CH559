
#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "USBHost.h"
#include "uart_protocol.h"

SBIT(SLEEP_PIN, 0xB0, 3);

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
    initUsbHost();
    
    UartProtocol_init();
    YS_LOG("Ready\n");

    //Pin_mode(PORT3, PIN3, PIN_MODE_INPUT_OUTPUT_PULLUP);

    IRQ_enable();

    while(1)
    {
        if(!(P4_IN & (1 << 6)))
            runBootloader();
        
        if(SLEEP_PIN)
            Power_sleep(WAKE_SRC_P3_2E_3L);
        
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
