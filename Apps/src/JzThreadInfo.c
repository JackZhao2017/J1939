
#include "JzParseUser.h"
#include <string.h>
#include <stdlib.h>
#include "apps_cfg.h"

#define UARTREAD  "uartread"
#define UARTPARSE "uartparse"
#define CANREAD   "canread"
#define CANPARSE  "canparse"
#define UARTSEND  "uartsend"
#define UARTCAN	  "uartcan"


static JZ_STACK      UART_PARSE_TASK_STK[UART_PARSE_STK_SIZE];
static JZ_STACK      UART_READ_TASK_STK[UART_READ_STK_SIZE];
static JZ_STACK 	 UART_SEND_TASK_STK[UART_SEND_STK_SIZE];
static JZ_STACK 	 CAN_READ_TASK_STK[CAN_READ_STK_SIZE];
static JZ_STACK 	 CAN_PARSE_TASK_STK[CAN_PARSE_STK_SIZE];




JZ_S32 Jz_SystermGetMutexAttrByName(const char *name ,JZ_MutexAttr *attr)
{
	if(name==NULL || attr==NULL){
		Jz_printf(" %s faild\r\n",__func__);
		return -1;
	}
	
	if(!strcmp(name,CANPARSE))
	{
		attr->prio = QUEUE_MUTEX_CANPARSE_PRIO;
	}
	else if(!strcmp(name,UARTPARSE))
	{
		attr->prio = RINGBUF_MUTEX_PRIO;
	}
	else if(!strcmp(name,UARTSEND))
	{
		attr->prio = QUEUE_MUTEX_UARTSEND_PRIO;
	}
	else if(!strcmp(name,UARTCAN))
	{
		attr->prio = UARTCAN_MUTEX_PRIO;
	}
	else{
		return JZ_FAILD;
	}
	//Jz_printf("mutex name %s prio %d \r\n",name ,attr->prio);
	return JZ_SUCCESS;
}

JZ_S32 Jz_SystermGetThreadInfoByName(const char *name ,Jz_ThreadAttr *attr)
{
	if(name==NULL || attr==NULL){
		Jz_printf(" %s faild\r\n",__func__);
		return -1;
	}

	if(!strcmp(name,UARTREAD))
	{
		attr->stktop = (JZ_STACK *)&UART_READ_TASK_STK[UART_READ_STK_SIZE-1];
		attr->prio   = UART_READ_PRIO;
		attr->ID     =UART_READ_PRIO;
		attr->stkbot =(JZ_STACK *)&UART_READ_TASK_STK[0];
		attr->stksize= UART_READ_STK_SIZE;
	}
	else if(!strcmp(name,UARTPARSE))
	{
		attr->stktop = (JZ_STACK *)&UART_PARSE_TASK_STK[UART_PARSE_STK_SIZE-1];
		attr->prio   = UART_PARSE_PRIO;
		attr->ID     =UART_PARSE_PRIO;
		attr->stkbot =(JZ_STACK *)&UART_PARSE_TASK_STK[0];
		attr->stksize= UART_PARSE_STK_SIZE;
	}
	else if(!strcmp(name,UARTSEND))
	{
		attr->stktop = (JZ_STACK *)&UART_SEND_TASK_STK[UART_SEND_STK_SIZE-1];
		attr->prio   = UART_SEND_PRIO;
		attr->ID     =UART_SEND_PRIO;
		attr->stkbot =(JZ_STACK *)&UART_SEND_TASK_STK[0];
		attr->stksize= UART_SEND_STK_SIZE;		
	}
	else if(!strcmp(name,CANREAD))
	{
		attr->stktop = (JZ_STACK *)&CAN_READ_TASK_STK[CAN_READ_STK_SIZE-1];
		attr->prio   = CAN_READ_PRIO;
		attr->ID     =CAN_READ_PRIO;
		attr->stkbot =(JZ_STACK *)&CAN_READ_TASK_STK[0];
		attr->stksize= CAN_READ_STK_SIZE;		
	}
	else if(!strcmp(name,CANPARSE))
	{
		attr->stktop = (JZ_STACK *)&CAN_PARSE_TASK_STK[CAN_PARSE_STK_SIZE-1];
		attr->prio   = CAN_PARSE_PRIO;
		attr->ID     =CAN_PARSE_PRIO;
		attr->stkbot =(JZ_STACK *)&CAN_PARSE_TASK_STK[0];
		attr->stksize= CAN_PARSE_STK_SIZE;		
	}
	else{
		return JZ_FAILD;
	}
	//Jz_printf("thread name %s prio %d \r\n",name ,attr->prio);
	return JZ_SUCCESS;
}


