#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#define NULL 0

typedef enum
{ 
	My_Pass = 1,		//操作成功
	My_Fail = 0			//操作失败
}MyRes;

typedef enum
{ 
	true = 1,
	false = 0
}bool;


//ADC平均次数
#define	ADC_Average_Cnt			10

#endif

