#ifndef _SIMPWRKEY_D_H_H
#define _SIMPWRKEY_D_H_H

#include	"stm32l1xx.h"


#define	PwrKey_Pin			GPIO_Pin_0
#define	PwrKey_GpioGroup	GPIOA
#define	PwrKey_Rcc			RCC_AHB1Periph_GPIOA

void PwrKey_GPIOInit(void);
void SimPwrON(void);
void SimPwrOff(void);
#endif


