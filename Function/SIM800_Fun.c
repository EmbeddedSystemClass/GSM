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

#include 	"Usart3_Driver.h"

#include	"CRC16.h"
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
static void gprsTest(void);
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
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, 
    unsigned char errorCnt, unsigned short ms)
{
	MyRes statues = My_Pass;
	unsigned char i = 0;

	for(i = 0; i < errorCnt; i++)
	{
		printf("第 %d 次尝试:", i+1);
		
		//清空队列数据
		if(rxBuf)
			while( RecvDataByUsart3(rxBuf, maxRxLen) > 0);
		
		SendDataByUsart3(txBuf, strlen(txBuf));
		delay_ms(ms);
			
		if(rxBuf)
		{
			memset(rxBuf, 0, maxRxLen);

			RecvDataByUsart3(rxBuf, maxRxLen);
				
			//printf(rxBuf, strlen(rxBuf));
				
			if(strcmp)
			{
				if(strstr(rxBuf, strcmp) == NULL)
					statues = My_Fail;
				else
				{
					statues = My_Pass;
					break;
				}
				
				delay_ms(1000);
			}
			else
			{
				statues = My_Pass;
				break;
			}
		}
		else
		{
			statues = My_Pass;
			break;
		}
	}

	return statues;
}

/***************************************************************************************************
*FunctionName: Sim800cModuleInit
*Description: SIM800c模块初始化
*Input: 
*Output: 
*Return: My_Fail -- 初始化失败
        My_Pass -- 初始化成功
*Author: xsx
*Date: 2017年6月24日11:30:13
***************************************************************************************************/
MyRes Sim800cModuleInit(void)
{
    char * tempBuf = NULL;
    MyRes status = My_Fail;
	unsigned char i = 0;
    
    tempBuf = MyMalloc(1024);
    if(tempBuf)
    {
        printf("gprs模块初始化...\r\r");
		
		//检查模块是否连接正常
		printf("测试gprs模块连接状态...\r");
        if(My_Pass != ComWithSim800c("ATE0\r", "OK", tempBuf, 1024, 10, 500))
		{
			printf("模块连接失败\r\r");
			goto END;
		}
		else
		{
			printf("模块连接成功\r\r");
		}

        //CREG?
		printf("检查模块网络注册信息\r");
		if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", tempBuf, 1024, 10, 500))
		{
			printf("模块网络注册失败\r\r");
			goto END;
		}
		else
		{
			printf("模块网络注册成功\r\r");
		}


		printf("开始读取sim卡ICCID\r");
		if(My_Pass == readICCID())
		{
			printf("成功读取ICCID:%s\r\r", getGB_Iccid());
		}
		else
		{
			printf("读取ICCID失败\r\r");
			goto END;
		}
		
		printf("关闭移动场景\r");
        if(My_Pass == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", tempBuf, 1024, 3, 1000))
			printf("关闭移动场景成功\r\r");
		else
			printf("关闭移动场景失败\r\n");
		
		printf("设置GPRS移动台类型为B\r");
        if(My_Pass == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");
		
		printf("定义PDP上下文：1, IP, CMNET\r");
        if(My_Pass == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");
		
		printf("附着GPRS业务\r");
        if(My_Pass == ComWithSim800c("AT+CGATT=1\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");
		
		printf("设置GPRS连接模式为: GPRS\r");
        if(My_Pass == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");
		
		printf("设置为单IP连接模式\r");
        if(My_Pass == ComWithSim800c("AT+CIPMUX=0\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");
		
		printf("设置接收数据显示IP头\r");
        if(My_Pass == ComWithSim800c("AT+CIPHEAD=0\r", "OK", tempBuf, 1024, 1, 1000))
			printf("操作成功\r\r");
		else
			printf("操作失败\r\r");

		status = My_Pass;
    }
    
    END:
        MyFree(tempBuf);
    
        return status;
}

MyRes readICCID(void)
{
	char * tempBuf = NULL;
	char * tempBuf2 = NULL;
    MyRes status = My_Fail;
    
    tempBuf = MyMalloc(1024);
    if(tempBuf)
    {
		if(My_Pass == ComWithSim800c("AT+CCID\r", AT_OK, tempBuf, 1024, 3, 1000))
        {
			tempBuf2 = strtok(tempBuf, "\n");
			tempBuf2 = strtok(NULL, "\n");
			setGB_Iccid(tempBuf2);

			status = My_Pass;
		}
    }
    
    END:
        MyFree(tempBuf);
    
        return status;
}

static void gprsTest(void)
{
    char * tempBuf = NULL;
    char * sendBuf = NULL;
    MyRes status = My_Fail;
    
    tempBuf = MyMalloc(1024);
    sendBuf = MyMalloc(1024);
    if(tempBuf && sendBuf)
    {
        
        snprintf(sendBuf, 100, "AT+CIPSTART=\"TCP\",\"iot.osapling.com\",80\r");
        ComWithSim800c(sendBuf, "OK", tempBuf, 1024, 3, 1000);

        for(char i=0; i<10; i++)
        {
            snprintf(sendBuf, 1000, "GET /iot/datacollection.do?iccid=%s&battery=%d HTTP/1.1\nHost: iot.osapling.com\nUser-Agent: xsx\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nConnection: keep-alive\n\n", "89860114871025185511", i);
            //SendDataToQueue(GetUsart2TXQueue(), NULL, sendBuf, strlen(sendBuf), 1, 1000 * portTICK_RATE_MS, 0, EnableUsart2TXInterrupt);
            
            //ReceiveDataFromQueue(GetUsart2RXQueue(), NULL, tempBuf, 1024, NULL, 1, 5000 * portTICK_RATE_MS, 0);
        }
    }
    
    MyFree(tempBuf);
    MyFree(sendBuf);
}
