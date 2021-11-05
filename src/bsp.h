#ifndef __BSP_H__
#define __BSP_H__
#include <stdio.h>

#if 1
#define DEBUG_OUT(...) printf(__VA_ARGS__);
#else
#define DEBUG_OUT(...) (void)0;
#endif

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

void initClock();
void pinMode(unsigned char port, unsigned char pin, unsigned char mode);
void delayUs(unsigned short n);
void delay(unsigned short n);
void initUART0(unsigned long baud, int alt);
unsigned char UART0Receive();
void UART0Send(unsigned char b);

typedef void(* __data FunctionReference)();
extern FunctionReference runBootloader;

#endif