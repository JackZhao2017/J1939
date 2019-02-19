#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "apps_cfg.h"
#include "Jz_Peripheral_Led.h"

extern int JzAppMainInit(void);
extern void Jz_Periphral_Init(void);

#define START_TASK_PRIO		17 
#define START_STK_SIZE		128  

OS_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *pdata);		


int main()
{
	INT16U   id =START_TASK_PRIO;
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	   
	Jz_Periphral_Init();
	delay_ms(2000);
	OSInit();
	JzAppMainInit();
	OSTaskCreateExt(start_task,
					(void *)0,
					(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
					START_TASK_PRIO,
					id,
					(OS_STK *)&START_TASK_STK[0],
					START_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK);  
	OSStart();
	return 0;
}

void start_task(void *pdata)
{
	OS_STK_DATA stk_data;
	pdata = pdata; 
	OSTimeDlyHMSM(0,0,2,5);
	//OSTaskSuspend(START_TASK_PRIO);
	while(1)
	{
			u8 err = OSTaskStkChk(UART_READ_PRIO,&stk_data);
			printf("UART_READ_PRIO err %d  used/free:%d/%d usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));

			err = OSTaskStkChk(UART_PARSE_PRIO,&stk_data);
			printf("UART_PARSE_PRIO err %d  used/free:%d/%d usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));		

			err = OSTaskStkChk(CAN_READ_PRIO,&stk_data);
			printf("CAN_READ_PRIO err %d  used/free:%d/%d usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));


			err = OSTaskStkChk(CAN_PARSE_PRIO,&stk_data);
			printf("CAN_PARSE_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));		

			err = OSTaskStkChk(UART_SEND_PRIO,&stk_data);
			printf("UART_SEND_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));
			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
			OSTimeDlyHMSM(0,0,3,0);
	}						
}


