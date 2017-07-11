#ifndef _PWR_D_H_H
#define _PWR_D_H_H

#include	"stm32l1xx.h"
#include 	"stm32l1xx_rcc.h"
#include	"stm32l1xx_gpio.h"

#define	Pwr_Pin				GPIO_Pin_3
#define	Pwr_GpioGroup		GPIOA
#define	Pwr_GpioRcc			RCC_AHBPeriph_GPIOA

void Pwr_GPIOInit(void);
void SystemPwrOff(void);

#endif


