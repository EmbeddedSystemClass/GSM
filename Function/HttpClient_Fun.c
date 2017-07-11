/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"HttpClient_Fun.h"
#include	"SIM800_Fun.h"
#include 	"Usart1_Driver.h"

#include	"MyMem.h"
#include	"System_Data.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static char sendBuf[1024];
static char recvBuf[1024];
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static MyRes UpLoadFunction(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void UserMainFunction(void)
{
	unsigned int i=0;
	MyRes status = My_Fail;
	
	while(i < ErrorCnt && status == My_Fail)
	{
		status = UpLoadFunction();
		
		i++;
		
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

static MyRes UpLoadFunction(void)
{
	char * tempBuf2 = NULL;

    if(My_Fail == ComWithSim800c("ATE0\r", "OK", recvBuf, 100, 3, 100 / portTICK_RATE_MS))
		return My_Fail;

	//("检查模块网络注册信息\r");
	if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", recvBuf, 100, 5, 1000 / portTICK_RATE_MS))
		return My_Fail;

	//("开始读取sim卡ICCID\r");
	if(My_Pass == ComWithSim800c("AT+CCID\r", AT_OK, recvBuf, 500, 3, 1000 / portTICK_RATE_MS))
	{
		tempBuf2 = strtok(recvBuf, "\n");
		tempBuf2 = strtok(NULL, "\n");
		setGB_Iccid(tempBuf2);
	}
	else
		return My_Fail;

	//("关闭移动场景\r");
	if(My_Fail == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 3, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//("设置GPRS移动台类型为B\r");
	if(My_Fail == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//("定义PDP上下文：1, IP, CMNET\r");
	if(My_Fail == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000) / portTICK_RATE_MS)
		return My_Fail;
		
	//("附着GPRS业务\r");
	if(My_Fail == ComWithSim800c("AT+CGATT=1\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"设置GPRS连接模式为: GPRS\r");
	if(My_Fail == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"设置为单IP连接模式\r");
	if(My_Fail == ComWithSim800c("AT+CIPMUX=0\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"设置接收数据不显示IP头\r");
	if(My_Fail == ComWithSim800c("AT+CIPHEAD=0\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
	
	//"发起连接\r");
	if(My_Fail == ComWithSim800c("AT+CIPSTART=\"TCP\",\"iot.osapling.com\",80\r", "CONNECT", recvBuf, 100, 3, 3000 / portTICK_RATE_MS))
		return My_Fail;
	
	//"发送数据命令\r");
	if(My_Fail == ComWithSim800c("AT+CIPSEND\r", ">", recvBuf, 100, 3, 2000 / portTICK_RATE_MS))
		return My_Fail;
	
	//("发送数据\r");
	snprintf(sendBuf, 1000, "GET /iot/datacollection.do?iccid=%s&battery=%f HTTP/1.1\nHost: iot.osapling.com\nUser-Agent: xsx\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nConnection: keep-alive\n\n", getGB_Iccid(), getGB_BatteryV());
	ComWithSim800c(sendBuf, NULL, NULL, 1000, 1, 1000 / portTICK_RATE_MS);	

	//("\r\r发送并接收响应\r");
	sendBuf[0] = 0X1A;
	sendBuf[1] = 0;
	memset(recvBuf, 0, 1024);
	ComWithSim800c(sendBuf, NULL, recvBuf, 1000, 1, 3000 / portTICK_RATE_MS);	

	if(strstr(recvBuf, "1\r\n1\r\n0\r\n"))
		return My_Pass;
	else if(strstr(recvBuf, "1\r\n0\r\n0\r\n"))
		return My_Fail;
	else
		return My_Fail;
}
