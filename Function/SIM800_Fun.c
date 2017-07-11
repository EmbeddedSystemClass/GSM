/***************************************************************************************************
*FileName:WifiFunction
*Description:һЩWIFIģ��Ĳ�������,����ֻ�����߳���ʹ��
*Author:xsx
*Data:2016��4��30��16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
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
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void gprsTest(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: ComWithWIFI��wifiģ��ͨ��
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��3��7��09:41:31
***************************************************************************************************/
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, 
    unsigned char errorCnt, unsigned short ms)
{
	MyRes statues = My_Pass;
	unsigned char i = 0;

	for(i = 0; i < errorCnt; i++)
	{
		printf("�� %d �γ���:", i+1);
		
		//��ն�������
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
*Description: SIM800cģ���ʼ��
*Input: 
*Output: 
*Return: My_Fail -- ��ʼ��ʧ��
        My_Pass -- ��ʼ���ɹ�
*Author: xsx
*Date: 2017��6��24��11:30:13
***************************************************************************************************/
MyRes Sim800cModuleInit(void)
{
    char * tempBuf = NULL;
    MyRes status = My_Fail;
	unsigned char i = 0;
    
    tempBuf = MyMalloc(1024);
    if(tempBuf)
    {
        printf("gprsģ���ʼ��...\r\r");
		
		//���ģ���Ƿ���������
		printf("����gprsģ������״̬...\r");
        if(My_Pass != ComWithSim800c("ATE0\r", "OK", tempBuf, 1024, 10, 500))
		{
			printf("ģ������ʧ��\r\r");
			goto END;
		}
		else
		{
			printf("ģ�����ӳɹ�\r\r");
		}

        //CREG?
		printf("���ģ������ע����Ϣ\r");
		if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", tempBuf, 1024, 10, 500))
		{
			printf("ģ������ע��ʧ��\r\r");
			goto END;
		}
		else
		{
			printf("ģ������ע��ɹ�\r\r");
		}


		printf("��ʼ��ȡsim��ICCID\r");
		if(My_Pass == readICCID())
		{
			printf("�ɹ���ȡICCID:%s\r\r", getGB_Iccid());
		}
		else
		{
			printf("��ȡICCIDʧ��\r\r");
			goto END;
		}
		
		printf("�ر��ƶ�����\r");
        if(My_Pass == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", tempBuf, 1024, 3, 1000))
			printf("�ر��ƶ������ɹ�\r\r");
		else
			printf("�ر��ƶ�����ʧ��\r\n");
		
		printf("����GPRS�ƶ�̨����ΪB\r");
        if(My_Pass == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");
		
		printf("����PDP�����ģ�1, IP, CMNET\r");
        if(My_Pass == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");
		
		printf("����GPRSҵ��\r");
        if(My_Pass == ComWithSim800c("AT+CGATT=1\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");
		
		printf("����GPRS����ģʽΪ: GPRS\r");
        if(My_Pass == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");
		
		printf("����Ϊ��IP����ģʽ\r");
        if(My_Pass == ComWithSim800c("AT+CIPMUX=0\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");
		
		printf("���ý���������ʾIPͷ\r");
        if(My_Pass == ComWithSim800c("AT+CIPHEAD=0\r", "OK", tempBuf, 1024, 1, 1000))
			printf("�����ɹ�\r\r");
		else
			printf("����ʧ��\r\r");

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
