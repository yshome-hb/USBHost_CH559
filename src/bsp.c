#include "config.h"
#include "bsp.h"

FunctionReference runBootloader = (FunctionReference)BOOT_LOAD_ADDR;

__data unsigned short sysTick;

#ifndef FREQ_SYS
#define	FREQ_SYS	48000000
#endif 

/*******************************************************************************
* Function Name  : Clock_init()
* Description    : Initialize system clock
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void Clock_init()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;
	CLOCK_CFG |= 6; 															  
	PLL_CFG = ((24 << 0) | (6 << 5)) & 255;

    SAFE_MOD = 0xFF;

	delayMs(7);
}

/*******************************************************************************
* Function Name  : delayUs(UNIT16 n)
* Description    : us
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void delayUs(unsigned short n)
{
	while (n) 
	{  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 34000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 36000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 38000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 40000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 42000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 44000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 46000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 48000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 50000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 52000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 54000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 56000000
		++ SAFE_MOD;
#endif
#endif
		--n;
	}
}

/*******************************************************************************
* Function Name  : delayMs(UNIT16 n)
* Description    : ms
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void delayMs(unsigned short n)
{
	while (n) 
	{
		delayUs(1000);
		--n;
	}
}

/**
* #define PIN_MODE_INPUT 0
* #define PIN_MODE_INPUT_PULLUP 1
* #define PIN_MODE_OUTPUT 2
* #define PIN_MODE_OUTPUT_OPEN_DRAIN 3
* #define PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK 4
* #define PIN_MODE_INPUT_OUTPUT_PULLUP 5
* #define PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK 6
 */
void Pin_mode(unsigned char port, unsigned char pin, unsigned char mode)
{
	volatile unsigned char *dir[] = {&P0_DIR, &P1_DIR, &P2_DIR, &P3_DIR};
	volatile unsigned char *pu[] = {&P0_PU, &P1_PU, &P2_PU, &P3_PU};

	if(port == PORT4)
	{
		switch (mode)
		{
		case PIN_MODE_INPUT: //Input only, no pull up
			P4_DIR &= ~pin;
			P4_PU &= ~pin;
			break;
		case PIN_MODE_INPUT_PULLUP: //Input only, pull up
			P4_DIR &= ~pin;
			P4_PU |= pin;
			break;
		case PIN_MODE_OUTPUT: //Push-pull output, high and low level strong drive
			P4_DIR |= pin;
			break;
		default:
			break;
		}
		return;
	}

	switch(mode)
	{
	case PIN_MODE_INPUT: //Input only, no pull up
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] &= ~pin;
		*pu[port] &= ~pin;
		break;
	case PIN_MODE_INPUT_PULLUP: //Input only, pull up
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] &= ~pin;
		*pu[port] |= pin;
		break;
	case PIN_MODE_OUTPUT: //Push-pull output, high and low level strong drive
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] |= pin;
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN:  //Open drain output, no pull-up, support input
		PORT_CFG |= (bP0_OC << port);
		*dir[port] &= ~pin;
		*pu[port] &= ~pin;
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK: //Open-drain output, no pull-up, only drives 2 clocks high when the transition output goes from low to high
		PORT_CFG |= (bP0_OC << port);
		*dir[port] |= pin;
		*pu[port] &= ~pin;
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP: //Weakly bidirectional (standard 51 mode), open drain output, with pull-up
		PORT_CFG |= (bP0_OC << port);
		*dir[port] &= pin;
		*pu[port] |= ~pin;
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK: //Quasi-bidirectional (standard 51 mode), open-drain output, with pull-up, when the transition output is low to high, only drives 2 clocks high
		PORT_CFG |= (bP0_OC << port);
		*dir[port] |= pin;
		*pu[port] |= ~pin;
		break;
	default:
		break;
	}
}

/*
unsigned char getPortAddress(unsigned char port)
{
	const unsigned char portAddr[] = {0x80, 0x90, 0xA0, 0xB0, 0xC0};
	return portAddr[port];
}

unsigned char digitalRead(unsigned char port, unsigned char pin)
{

}
*/

/*******************************************************************************
* Function Name  : SW_reset()
* Description    : system soft reset
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SW_reset()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;
    GLOBAL_CFG |= bSW_RESET;
}

/*******************************************************************************
* Function Name  : WDT_enable
* Description    : Watchdog enable
* Input          : en：
*                   0：disable
*                   1：enable
* Return         : None
*******************************************************************************/
void WDT_enable(unsigned char en)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	if(en)
		GLOBAL_CFG |= bWDOG_EN;
	else 
		GLOBAL_CFG &= ~bWDOG_EN;
	SAFE_MOD = 0xFF;
	WDOG_COUNT = 0;
}

/*******************************************************************************
* Function Name  : WDT_feed
* Description    : Watchdog feed
*                  看门狗复位时间(s) = (256-tim)/(Fsys/131072)
*                  00H(Fsys=12MHz)=2.8s
*                  80H(Fsys=12MHz)=1.4s
* Input          : cnt：看门狗计数赋值
* Return         : None
*******************************************************************************/
void WDT_feed(unsigned char cnt)
{
	WDOG_COUNT = cnt;
}

/**
 * Initialize UART0 port with given boud rate
 * pins: tx = P3.1 rx = P3.0
 * alt != 0 pins: tx = P0.2 rx = P0.3
 */
void UART0_init(unsigned long baud, int alt)
{
	unsigned long x;
	if(alt)
	{
		PORT_CFG |= bP0_OC;
		P0_DIR |= bTXD_;
		P0_PU |= bTXD_ | bRXD_;
		PIN_FUNC |= bUART0_PIN_X;
	}

 	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;
    //RCLK = 0;
    //TCLK = 0;
    PCON |= SMOD;
    x = (((unsigned long)FREQ_SYS / 8) / baud + 1) / 2;

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;
    TH1 = (256 - x) & 255;
    TR1 = 1;
	TI = 1;
}

unsigned char UART0_receive()
{
    while(RI == 0);
    RI = 0;
    return SBUF;
}

void UART0_send(unsigned char b)
{
    SBUF = b;
    while(TI == 0);
    TI = 0;
}

/**
 * Initialize UART1 port with given boud rate
 * pins: tx = P4.4 rx = P4.0
 * alt != 0 pins: tx = P2.7 rx = P2.6
 */
void UART1_init(unsigned long baud, int alt)
{
    unsigned long x = FREQ_SYS / 16 / baud;

    SER1_LCR |= bLCR_DLAB;                          // DLAB位置1，写DLL、DLM和DIV寄存器
    SER1_DIV = 2;                      				// 预分频
    SER1_DLM = x>>8;
    SER1_DLL = x&0xFF;

	XBUS_AUX |=  bALE_CLK_EN;
    SER1_LCR &= ~bLCR_DLAB;                         //DLAB位置0,防止修改UART1波特率和时钟
    SER1_LCR |= MASK_U1_WORD_SZ;                    //线路控制
    SER1_LCR &= ~(bLCR_PAR_EN | bLCR_STOP_BIT);     //无线路间隔，无校验，1位停止位，8位数据位

	if(alt)
    	SER1_IER |= ((2 << 4) & MASK_U1_PIN_MOD);   //串口模式配置
    else
		SER1_IER |= ((1 << 4) & MASK_U1_PIN_MOD);   //串口模式配置	

    SER1_IER |= bIER_MODEM_CHG | bIER_LINE_STAT | bIER_THR_EMPTY | bIER_RECV_RDY;	//中断使能配置
 
    SER1_FCR |= MASK_U1_FIFO_TRIG | bFCR_T_FIFO_CLR | bFCR_R_FIFO_CLR | bFCR_FIFO_EN;//FIFO控制器
                                                                               //清空接收、发送FIFO，7字节接收触发，FIFO使能
    SER1_MCR |= bMCR_OUT2;                          //MODEM控制寄存器
                                                    //中断请求输出，不产生实际中断
    SER1_ADDR |= 0xFF;                              //关闭多机通信
}

unsigned char UART1_receive()
{
    while((SER1_LSR & bLSR_DATA_RDY) == 0);
    return SER1_RBR;
}

void UART1_send(unsigned char b)
{
	SER1_THR = b;
    while((SER1_LSR & bLSR_T_FIFO_EMP) == 0);
}

/*******************************************************************************
* Function Name  : TMR0_init
* Description    : Initialize timer0
* Input          : mode：Timer模式选择
*                   0-模式0，13位定时器，TLn的高3位无效
*                   1-模式1，16位定时器
*                   2-模式2，8位自动重装定时器
*                   3-模式3，Timer0分成两个8位定时器，Timer1停止
*                  div：时钟分频
*                   0- Fsys
*                   1- Fsys/4
*                   2- Fsys/12
* Return         : None
*******************************************************************************/
void TMR0_init(unsigned char mode, unsigned char div)
{
	TMOD = TMOD & 0xf0 | mode;
	
	if(div == TMR_DIV_1)
	{
		T2MOD |= bTMR_CLK | bT0_CLK;
	}
	else if(div == TMR_DIV_4)
	{
		T2MOD &= ~bTMR_CLK;
		T2MOD |= bT0_CLK;
	}
	else if(div == TMR_DIV_12)
	{
		T2MOD &= ~(bTMR_CLK | bT0_CLK);
	}
}

/*******************************************************************************
* Function Name  : TMR0_setCount
* Description    : Set timer0 counter value
* Input          : cnt：计数器赋值
* Return         : None
*******************************************************************************/
void TMR0_setCount(unsigned short cnt)
{
	unsigned short tmp = 65536 - cnt;
	TL0 = tmp & 0xff;
	TH0 = (tmp>>8) & 0xff;
}

/*******************************************************************************
* Function Name  : TMR1_init
* Description    : Initialize timer1
* Input          : mode：Timer模式选择
*                   0-模式0，13位定时器，TLn的高3位无效
*                   1-模式1，16位定时器
*                   2-模式2，8位自动重装定时器
*                  div：时钟分频
*                   0- Fsys
*                   1- Fsys/4
*                   2- Fsys/12
* Return         : None
*******************************************************************************/
void TMR1_init(unsigned char mode, unsigned char div)
{
	TMOD = TMOD & 0x0f | (mode<<4);
	
	if(div == TMR_DIV_1)
	{
		T2MOD |= bTMR_CLK | bT1_CLK;
	}
	else if(div == TMR_DIV_4)
	{
		T2MOD &= ~bTMR_CLK;
		T2MOD |= bT1_CLK;
	}
	else if(div == TMR_DIV_12)
	{
		T2MOD &= ~(bTMR_CLK | bT1_CLK);
	}
}

/*******************************************************************************
* Function Name  : TMR1_setCount
* Description    : Set timer1 counter value
* Input          : cnt：计数器赋值
* Return         : None
*******************************************************************************/
void TMR1_setCount(unsigned short cnt)
{
	unsigned short tmp = 65536 - cnt;
	TL1 = tmp & 0xff;
	TH1 = (tmp>>8) & 0xff;
}

/*******************************************************************************
* Function Name  : TMR2_init
* Description    : Initialize timer2
* Input          : div：时钟分频
*                   0- Fsys
*                   1- Fsys/4
*                   2- Fsys/12
* Return         : None
*******************************************************************************/
void TMR2_init(unsigned char div)
{
	RCLK = 0;
	TCLK = 0;
	CP_RL2 = 0;
	
	if(div == TMR_DIV_1)
	{
		T2MOD |= bTMR_CLK | bT2_CLK;
	}
	else if(div == TMR_DIV_4)
	{
		T2MOD &= ~bTMR_CLK;
		T2MOD |= bT2_CLK;
	}
	else if(div == TMR_DIV_12)
	{
		T2MOD &= ~(bTMR_CLK | bT2_CLK);
	}
}

/*******************************************************************************
* Function Name  : TMR2_setCount
* Description    : Set timer2 counter value
* Input          : cnt：计数器赋值
* Return         : None
*******************************************************************************/
void TMR2_setCount(unsigned short cnt)
{
	unsigned short tmp = 65536 - cnt;
	RCAP2L = TL2 = tmp & 0xff;
	RCAP2H = TH2 = (tmp>>8) & 0xff;
}