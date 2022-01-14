#ifndef __BSP_H__
#define __BSP_H__

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define YS_DEBUG_INIT(baud)    UART1_init(baud, 1)
#define YS_LOG(...) printf(__VA_ARGS__);
#else
#define YS_DEBUG_INIT(baud)
#define YS_LOG(...) (void)0;
#endif

#define IRQ_enable()   { EA = 1; }
#define IRQ_disable()  { EA = 0; }

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

void Pin_mode(unsigned char port, unsigned char pin, unsigned char mode);

void UART0_init(unsigned long baud, int alt);
unsigned char UART0_receive();
void UART0_send(unsigned char b);

void UART1_init(unsigned long baud, int alt);
unsigned char UART1_receive();
void UART1_send(unsigned char b);

typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif