/***************************************************************************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/**************************************Header File List*********************************************/
/***************************************************************************************************/
#include	"BettryADC.h"
/***************************************************************************************************/
/**************************************Static Value*************************************************/
/***************************************************************************************************/
static unsigned int temp = 0;
/***************************************************************************************************/
/**************************************Static Method*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/**************************************File Start**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void BettryADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// Enable The HSI (16Mhz) 
	RCC_HSICmd(ENABLE);
	// Check that HSI oscillator is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	RCC_AHBPeriphClockCmd(ADC_Rcc, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	GPIO_InitStructure.GPIO_Pin = ADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_2MHz;
	GPIO_Init(ADC_GpioGroup, &GPIO_InitStructure);

	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// ADC1 regular channel5 or channel1 configuration
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_192Cycles);

	// Define delay between ADC1 conversions
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
  
	// Enable ADC1 Power Down during Delay
//	ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器
	
	// Wait until ADC1 ON status
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
		;
	}
}

double getBettryV(void)   
{
	unsigned short i=0;
	
	double value = 0.0;
	
	temp = 0;
	for(i=0; i<ADC_Average_Cnt; i++)
	{
		 ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
		temp += ADC_GetConversionValue(ADC1);
	}
	value = temp;
	value /= ADC_Average_Cnt;
	value /= 4095;
	value *= 3.3;
	
	//电阻分压
	value *= 1.2;
	
	return value;
}

/****************************************end of file************************************************/
