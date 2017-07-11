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
#include 	"Usart3_Driver.h"
#include	"SimPwrKey_Driver.h"

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
static char recvBuf[4096];
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

unsigned char UpLoadFunction(unsigned short cnt)
{
	char * tempBuf2 = NULL;

	printf("gprs模块初始化...\r\r");
		
	//检查模块是否连接正常
	printf("测试gprs模块连接状态...\r");
    if(My_Pass != ComWithSim800c("ATE0\r", "OK", recvBuf, 100, 10, 500))
	{
		printf("模块连接失败\r\r");
		return 0;
	}
	else
	{
		printf("模块连接成功\r\r");
	}

	//CREG?
	printf("检查模块网络注册信息\r");
	if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", recvBuf, 100, 10, 500))
	{
		printf("模块网络注册失败\r\r");
		return 0;
	}
	else
	{
		printf("模块网络注册成功\r\r");
	}


	printf("开始读取sim卡ICCID\r");
	if(My_Pass == ComWithSim800c("AT+CCID\r", AT_OK, recvBuf, 500, 3, 1000))
	{
		tempBuf2 = strtok(recvBuf, "\n");
		tempBuf2 = strtok(NULL, "\n");
		setGB_Iccid(tempBuf2);
		
		printf("成功读取ICCID:%s\r\r", getGB_Iccid());
	}
	else
	{
		printf("读取ICCID失败\r\r");
		return 0;
	}
		
	printf("关闭移动场景\r");
	if(My_Pass == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 3, 1000))
		printf("关闭移动场景成功\r\r");
	else
	{
		printf("关闭移动场景失败\r\n");
		return 0;
	}
		
	printf("设置GPRS移动台类型为B\r");
	if(My_Pass == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
		
	printf("定义PDP上下文：1, IP, CMNET\r");
	if(My_Pass == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
		
	printf("附着GPRS业务\r");
	if(My_Pass == ComWithSim800c("AT+CGATT=1\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
		
	printf("设置GPRS连接模式为: GPRS\r");
	if(My_Pass == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
		
	printf("设置为单IP连接模式\r");
	if(My_Pass == ComWithSim800c("AT+CIPMUX=0\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
		
	printf("设置接收数据不显示IP头\r");
	if(My_Pass == ComWithSim800c("AT+CIPHEAD=0\r", "OK", recvBuf, 100, 1, 1000))
		printf("操作成功\r\r");
	else
	{
		printf("操作失败\r\r");
		return 0;
	}
	
	printf("发起连接\r");
	if(My_Pass == ComWithSim800c("AT+CIPSTART=\"TCP\",\"iot.osapling.com\",80\r", "CONNECT", recvBuf, 100, 3, 2000))
	{
		printf("连接成功\r\r");
	}
	else
	{
		printf("连接失败\r\r");
		return 0;
	}
	
	printf("发送数据命令\r");
	if(My_Pass == ComWithSim800c("AT+CIPSEND\r", ">", recvBuf, 100, 3, 2000))
	{
		printf("连接成功\r\r");
	}
	else
	{
		printf("连接失败\r\r");
		return 0;
	}
	
	printf("发送数据\r");
	snprintf(sendBuf, 1000, "GET /iot/datacollection.do?iccid=%s&battery=%d HTTP/1.1\nHost: iot.osapling.com\nUser-Agent: xsx\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nConnection: keep-alive\n\n", getGB_Iccid(), cnt);
	ComWithSim800c(sendBuf, NULL, NULL, 1000, 1, 1000);	

	printf("\r\r发送并接收响应\r");
	sendBuf[0] = 0X1A;
	sendBuf[1] = 0;
	memset(recvBuf, 0, 4096);
	ComWithSim800c(sendBuf, NULL, recvBuf, 1000, 1, 3000);	

	printf(recvBuf, strlen(recvBuf));
	
	if(strstr(recvBuf, "1\r\n1\r\n0\r\n"))
		return 1;
	else if(strstr(recvBuf, "1\r\n0\r\n0\r\n"))
		return 2;
	else
	{
		printf("数据提交成功\r");
		return 0;
	}
}
