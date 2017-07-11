#ifndef LED_D_H_H
#define LED_D_H_H

#include	"stm32l1xx.h"


#define	Led_Pin			GPIO_Pin_9
#define	Led_GpioGroup	GPIOF
#define	Led_Rcc			RCC_AHB1Periph_GPIOF

typedef enum
{ 
	ON = Bit_RESET,			//LED POWER ON
	OFF = Bit_SET			//LED POWER OFF
}LED_State;


void Led_GPIOInit(void);
void SetLedState(LED_State state);
void LedToggle(void);

#endif


