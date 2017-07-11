#ifndef LED_D_H_H
#define LED_D_H_H

#include	"stm32l1xx.h"
#include 	"stm32l1xx_rcc.h"
#include	"stm32l1xx_gpio.h"

#define	Led_Pin				GPIO_Pin_4
#define	Led_GpioGroup		GPIOA
#define	Led_GpioRcc			RCC_AHBPeriph_GPIOA

typedef enum
{
	ON = Bit_SET,			//LED POWER ON
	OFF = Bit_RESET			//LED POWER OFF
}LED_State;


void Led_GPIOInit(void);
void SetLedState(LED_State state);
void LedToggle(void);

#endif


