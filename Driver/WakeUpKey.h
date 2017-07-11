#ifndef _WAKEUPKEY_D_H_H
#define _WAKEUPKEY_D_H_H

#include	"stm32l1xx.h"
#include	"Define.h"

#define	WakeUp_Pin			GPIO_Pin_0
#define	WakeUp_GpioGroup	GPIOA
#define	WakeUp_Rcc			RCC_AHB1Periph_GPIOA

void WakeUpKey_Init(void);	//IO≥ı ºªØ
bool CheckoutKeyIsPressed(void);	

#endif
