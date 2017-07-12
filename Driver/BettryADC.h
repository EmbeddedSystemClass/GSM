#ifndef _BETTRYADC_D_H_H
#define _BETTRYADC_D_H_H

#include	"stm32l1xx_gpio.h"
#include	"stm32l1xx_adc.h"
#include 	"stm32l1xx_rcc.h"
#include 	"stm32l1xx_dma.h"
#include	"Define.h"

#define	ADC_Pin			GPIO_Pin_2
#define	ADC_GpioGroup	GPIOA
#define	ADC_Rcc			RCC_AHBPeriph_GPIOA
#define	ADC_Channel		ADC_Channel_2b

void BettryADC_Init(void);
double getBettryV(void) ;

#endif 
