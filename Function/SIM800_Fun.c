/***************************************************************************************************
*FileName:WifiFunction
*Description:一些WIFI模块的操作函数,函数只能在线程中使用
*Author:xsx
*Data:2016年4月30日16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SIM800_Fun.h"

#include 	"Usart1_Driver.h"
#include	"QueueUnits.h"

#include	"MyMem.h"
#include	"System_Data.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: ComWithWIFI与wifi模块通信
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年3月7日09:41:31
***************************************************************************************************/
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, unsigned char errorCnt, portTickType queueBlockTime)
{
	MyRes statues = My_Fail;
	unsigned char i = 0;

	while(statues == My_Fail && i < errorCnt)
	{
		//清空队列数据
		if(rxBuf)
			while(ReceiveDataFromQueue(GetUsart1RXQueue(), rxBuf, maxRxLen, NULL, 1, 0) == My_Pass);

		SendDataToQueue(GetUsart1TXQueue(), txBuf, strlen(txBuf), 1, 100 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
				
		if(rxBuf)
		{
			ReceiveDataFromQueue(GetUsart1RXQueue(), rxBuf, maxRxLen, NULL, 1, queueBlockTime);

			if(strcmp)
			{
				if(strstr(rxBuf, strcmp))
					statues = My_Pass;
				else
					statues = My_Fail;
			}
			else
				statues = My_Pass;
		}
		else
			statues = My_Pass;
		
		i++;
	}

	return statues;
}

