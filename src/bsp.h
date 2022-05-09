#ifndef __BSP_H__
#define __BSP_H__

#include <stdio.h>
#include "CH559.h"

#define DEBUG

#ifdef DEBUG
#define YS_DEBUG_INIT(baud)    UART1_init(baud, 0)
#define YS_LOG(...) printf(__VA_ARGS__);
#else
#define YS_DEBUG_INIT(baud)
#define YS_LOG(...) (void)0;
#endif

#define IRQ_enable()   { EA = 1; }
#define IRQ_disable()  { EA = 0; }

extern __data unsigned short sysTick;
#define clock_time()   (sysTick)
#define TIMER_DIFF(t)  ((unsigned short)((sysTick)-(t)))
#define SysClk_init() { \
    TMR2_init(TMR_DIV_12); \
    TMR2_setCount(4000); \
    TMR2_enable(); \
    TMR2_INTenable(); \
}

void Clock_init();
void delayUs(unsigned short n);
void delayMs(unsigned short n);

typedef enum
{
    PORT0 = 0,
    PORT1 = 1,
    PORT2 = 2,
    PORT3 = 3,
    PORT4 = 4,
}PIN_PORT_TypeDef;

typedef enum
{
    PIN0 = 0x01,
    PIN1 = 0x02,
    PIN2 = 0x04,
    PIN3 = 0x08,
    PIN4 = 0x10,
    PIN5 = 0x20,
    PIN6 = 0x40,
    PIN7 = 0x80,
}PIN_NUM_TypeDef;

typedef enum
{
    PIN_MODE_INPUT = 0,
    PIN_MODE_INPUT_PULLUP = 1,
    PIN_MODE_OUTPUT = 2,
    PIN_MODE_OUTPUT_OPEN_DRAIN = 3,
    PIN_MODE_OUTPUT_OPEN_DRAIN_2CLK = 4,
    PIN_MODE_INPUT_OUTPUT_PULLUP = 5,
    PIN_MODE_INPUT_OUTPUT_PULLUP_2CLK = 6,
}PIN_MODE_TypeDef;

void Pin_mode(PIN_PORT_TypeDef port, PIN_NUM_TypeDef pin, PIN_MODE_TypeDef mode);

typedef enum
{
    WAKE_SRC_RXD0 = 0x01,
    WAKE_SRC_P3_2E_3L = 0x02,
    WAKE_SRC_CAP3 = 0x04,
    WAKE_SRC_P0_3 = 0x08,
    WAKE_SRC_P1_4 = 0x10,
    WAKE_SRC_P1_5 = 0x20,
    WAKE_SRC_RXD1 = 0x40,
    WAKE_SRC_USB = 0x80,
}WAKEUP_SRC_TypeDef;

void Power_sleep(WAKEUP_SRC_TypeDef src);
void SW_reset(void);
void WDT_enable(unsigned char en);
void WDT_feed(unsigned char cnt); 

#define UART0_INTenable()   { ES = 1; }
void UART0_init(unsigned long baud, int alt);
unsigned char UART0_receive();
void UART0_send(unsigned char b);

#define UART1_INTenable()   { ET1 = 1; }
void UART1_init(unsigned long baud, int alt);
unsigned char UART1_receive();
void UART1_send(unsigned char b);

typedef enum
{
    TMR_MODE_13BIT = 0,
    TMR_MODE_16BIT = 1,
    TMR_MODE_AUTO8BIT = 2,
}TMR_MODE_TypeDef;

typedef enum
{
    TMR_DIV_1 = 0,
    TMR_DIV_4 = 1,
    TMR_DIV_12 = 2,
}TMR_DIV_TypeDef;

#define TMR0_enable()   { TR0 = 1; }
#define TMR0_disable()  { TR0 = 0; }
#define TMR0_INTenable()   { ET0 = 1; }
#define TMR0_INTdisable()  { ET0 = 0; }
#define TMR1_enable()   { TR1 = 1; }
#define TMR1_disable()  { TR1 = 0; }
#define TMR1_INTenable()   { ET1 = 1; }
#define TMR1_INTdisable()  { ET1 = 0; }
#define TMR2_enable()   { TR2 = 1; }
#define TMR2_disable()  { TR2 = 0; }
#define TMR2_INTenable()   { ET2 = 1; }
#define TMR2_INTdisable()  { ET2 = 0; }
void TMR0_init(TMR_MODE_TypeDef mode, TMR_DIV_TypeDef div);
void TMR0_setCount(unsigned short cnt);
void TMR1_init(TMR_MODE_TypeDef mode, TMR_DIV_TypeDef div);
void TMR1_setCount(unsigned short cnt);
void TMR2_init(TMR_DIV_TypeDef div);
void TMR2_setCount(unsigned short cnt);

typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif