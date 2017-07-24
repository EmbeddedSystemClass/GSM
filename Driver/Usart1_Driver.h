#ifndef __USART1_H__
#define __USART1_H__

#include 	"stm32l1xx_rcc.h"
#include	"stm32l1xx_gpio.h"
#include 	"stm32l1xx_usart.h"
#include 	"misc.h"
#include	"Define.h"

#define Usart1TxQueueLen	1024
#define Usart1RxQueueLen	1024

void Usart1_Init(void);
void EnableUsart1TXInterrupt(void);
MyRes SendDataByUsart1(void * data, unsigned short len);
unsigned short RecvDataByUsart1(void * data, unsigned short len);

#endif

