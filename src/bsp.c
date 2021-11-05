#include "CH559.h"
#include "bsp.h"

FunctionReference runBootloader = (FunctionReference)0xF400;

#ifndef FREQ_SYS
#define	FREQ_SYS	48000000
#endif 

void initClock()
{
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;
	CLOCK_CFG |= 6; 															  
	PLL_CFG = ((24 << 0) | (6 << 5)) & 255;

    SAFE_MOD = 0xFF;

	delay(7);
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
void pinMode(unsigned char port, unsigned char pin, unsigned char mode)
{
	volatile unsigned char *dir[] = {&P0_DIR, &P1_DIR, &P2_DIR, &P3_DIR};
	volatile unsigned char *pu[] = {&P0_PU, &P1_PU, &P2_PU, &P3_PU};
	switch (mode)
	{
	case PIN_MODE_INPUT: //Input only, no pull up
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] &= ~(1 << pin);
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_INPUT_PULLUP: //Input only, pull up
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] &= ~(1 << pin);
		*pu[port] |= 1 << pin;
		break;
	case PIN_MODE_OUTPUT: //Push-pull output, high and low level strong drive
		PORT_CFG &= ~(bP0_OC << port);
		*dir[port] |= ~(1 << pin);
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN: //Open drain output, no pull-up, support input
		PORT_CFG |= (bP0_OC << port);
		*dir[port] &= ~(1 << pin);
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK: //Open-drain output, no pull-up, only drives 2 clocks high when the transition output goes from low to high
		PORT_CFG |= (bP0_OC << port);
		*dir[port] |= 1 << pin;
		*pu[port] &= ~(1 << pin);
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP: //Weakly bidirectional (standard 51 mode), open drain output, with pull-up
		PORT_CFG |= (bP0_OC << port);
		*dir[port] &= ~(1 << pin);
		*pu[port] |= 1 << pin;
		break;
	case PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK: //Quasi-bidirectional (standard 51 mode), open-drain output, with pull-up, when the transition output is low to high, only drives 2 clocks high
		PORT_CFG |= (bP0_OC << port);
		*dir[port] |= 1 << pin;
		*pu[port] |= 1 << pin;
		break;
	default:
		break;
	}
}

/*******************************************************************************
* Function Name  : delayUs(UNIT16 n)
* Description    : us
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void	delayUs(unsigned short n)
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
* Function Name  : delay(UNIT16 n)
* Description    : ms
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void delay(unsigned short n)
{
	while (n) 
	{
		delayUs(1000);
		--n;
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
/**
 * stdio printf directed to UART0 using putchar and getchar
 */

int putchar(int c)
{
    while (!TI);
    TI = 0;
    SBUF = c & 0xFF;
    return c;
}

int getchar() 
{
    while(!RI);
    RI = 0;
    return SBUF;
}

/**
 * Initialize UART0 port with given boud rate
 * pins: tx = P3.1 rx = P3.0
 * alt != 0 pins: tx = P0.2 rx = P0.3
 */

void initUART0(unsigned long baud, int alt)
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

unsigned char UART0Receive()
{
    while(RI == 0);
    RI = 0;
    return SBUF;
}

void UART0Send(unsigned char b)
{
	SBUF = b;
	while(TI == 0);
	TI = 1;
}

/*******************************************************************************
* Function Name  : CH559UART1Init(UINT8 DIV,UINT8 mode,UINT8 pin)
* Description    : CH559 UART1初始化设置
* Input          : 
                   UINT8 DIV设置分频系数，时钟频率=Fsys/DIV,DIV不能为0
                   UINT8 mode，模式选择，1：普通串口模式；0:485模式
                   UINT8 pin，串口引脚选择；
                   当mode=1时
                   0：RXD1=P4.0,TXD1关闭；
                   1：RXD1&TXD1=P4.0&P4.4；
                   2：RXD1&TXD1=P2.6&P2.7；
                   3：RXD1&TXD1&TNOW=P2.6&P2.7&P2.5；
                   当mode=0时
                   0：无意义
                   1：P5.4&P5.5连接485,TNOW=P4.4；
                   2：P5.4&P5.5连接485；
                   3：P5.4&P5.5连接485,TNOW=P2.5；
* Output         : None
* Return         : None
*******************************************************************************/
void CH559UART1Init(UINT8 DIV,UINT8 mode,UINT8 pin)
{
    UINT32 x;
    UINT8 x2; 

    SER1_LCR |= bLCR_DLAB;                                                    // DLAB位置1，写DLL、DLM和DIV寄存器
    SER1_DIV = DIV;                                                           // 预分频
    x = 10 * FREQ_SYS *2 / DIV / 16 / CH559UART1_BPS;                             
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                      //四舍五入
    SER1_DLM = x>>8;
    SER1_DLL = x&0xff;
    SER1_LCR &= ~bLCR_DLAB;                                                   //DLAB位置0,防止修改UART1波特率和时钟
    if(mode == 1)                                                             //关闭RS485模式 RS485_EN = 0,不能省略
    {
	      XBUS_AUX |=  bALE_CLK_EN;                                     
    }
    else if(mode == 0)                                                        //开启RS485模式 RS485_EN = 1;
    {
        UHUB1_CTRL |= bUH1_DISABLE;                                   
        PIN_FUNC &= ~bXBUS_CS_OE;
        PIN_FUNC |= bXBUS_AL_OE;
        XBUS_AUX &= ~bALE_CLK_EN;	
        SER1_MCR |= bMCR_HALF;                                                //485模式只能使用半双工模式	    
    }
    SER1_LCR |= MASK_U1_WORD_SZ;                                              //线路控制
    SER1_LCR &= ~(bLCR_PAR_EN | bLCR_STOP_BIT);                               //无线路间隔，无校验，1位停止位，8位数据位

    SER1_IER |= ((pin << 4) & MASK_U1_PIN_MOD);                               //串口模式配置
    SER1_IER |= bIER_MODEM_CHG | bIER_LINE_STAT | bIER_THR_EMPTY | bIER_RECV_RDY;//中断使能配置
 
    SER1_FCR |= MASK_U1_FIFO_TRIG | bFCR_T_FIFO_CLR | bFCR_R_FIFO_CLR | bFCR_FIFO_EN;//FIFO控制器
                                                                               //清空接收、发送FIFO，7字节接收触发，FIFO使能
    SER1_MCR |= bMCR_OUT2;                                                     //MODEM控制寄存器
                                                                               //中断请求输出，不产生实际中断
    SER1_ADDR |= 0xff;                                                         //关闭多机通信
}

unsigned char UART1Receive()
{
    while((SER1_LSR & bLSR_DATA_RDY) == 0);                                   //等待数据准备好
    return SER1_RBR;
}

void UART1Send(unsigned char b)
{
	SER1_THR = b;
    while((SER1_LSR & bLSR_T_FIFO_EMP) == 0); 
}