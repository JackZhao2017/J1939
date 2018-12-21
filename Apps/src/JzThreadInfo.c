
#include "JzParseUser.h"
#include <string.h>
#include <stdlib.h>

#define UARTREAD  "uartread"
#define UARTPARSE "uartparse"
#define CANREAD   "canread"
#define CANPARSE  "canparse"
#define UARTSEND  "uartsend"



#define UART_READ_PRIO       		  11 	//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define UART_PARSE_PRIO               10
#define CAN_READ_PRIO				  7 
#define CAN_PARSE_PRIO				  8
#define UART_SEND_PRIO       		  9

#define CAN_READ_STK_SIZE 			  128
#define CAN_PARSE_STK_SIZE			  96
#define UART_SEND_STK_SIZE  		  96 
#define UART_READ_STK_SIZE  		  128 	//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define UART_PARSE_STK_SIZE  		  128	//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡

static JZ_STACK      UART_PARSE_TASK_STK[UART_PARSE_STK_SIZE];
static JZ_STACK      UART_READ_TASK_STK[UART_READ_STK_SIZE];
static JZ_STACK 	 UART_SEND_TASK_STK[UART_SEND_STK_SIZE];

static JZ_STACK 	 CAN_READ_TASK_STK[CAN_READ_STK_SIZE];
static JZ_STACK 	 CAN_PARSE_TASK_STK[CAN_PARSE_STK_SIZE];


#define RINGBUF_MUTEX_PRIO 6
#define QUEUE_MUTEX_PRIO  4
#define QUEUE_MUTEX_PRIO1 5

JZ_S32 Jz_SystermGetMutexAttrByName(const char *name ,JZ_MutexAttr *attr)
{
	if(name==NULL || attr==NULL)
		return -1;
	
	if(!strcmp(name,CANPARSE))
	{
		attr->prio = QUEUE_MUTEX_PRIO;
	}
	else if(!strcmp(name,UARTPARSE))
	{
		attr->prio = RINGBUF_MUTEX_PRIO;
	}
	else if(!strcmp(name,UARTSEND))
	{
		attr->prio = QUEUE_MUTEX_PRIO1;
	}
	Jz_printf("name %s prio %d \r\n",name ,attr->prio);
	return JZ_SUCCESS;
}

JZ_S32 Jz_SystermGetThreadInfoByName(const char *name ,Jz_ThreadAttr *attr)
{
	if(name==NULL || attr==NULL)
		return -1;

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
		return -1;
	}
	Jz_printf("name %s prio %d \r\n",name ,attr->prio);
	return JZ_SUCCESS;
}
