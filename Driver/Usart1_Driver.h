#ifndef __USART1_H__
#define __USART1_H__

#include 	"stm32l1xx_rcc.h"
#include	"stm32l1xx_gpio.h"
#include 	"stm32l1xx_usart.h"
#include 	"misc.h"
#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue1_Len				500
#define xTxQueue1_Len				500

void Usart1_Init(void);
void EnableUsart1TXInterrupt(void);
xQueueHandle GetUsart1RXQueue(void);
xQueueHandle GetUsart1TXQueue(void);

#endif
