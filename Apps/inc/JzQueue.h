#ifndef _JZ_QUEUE_H_
#define _JZ_QUEUE_H_

#include "SystermDefine.h"
#include "JzCanRead.h"

typedef struct _JZ_QUEUE_INFO
{
	Jz_Mutex	 mutex;
	uint8_t 	 size;
	uint8_t 	 get;
	uint8_t 	 put;
	uint8_t 	 mark;
	CanRxMsg     *buf;
}JZ_QUEUE_INFO;


int  JZ_QueueInit(JZ_QUEUE_INFO *info,CanRxMsg *buf ,int size,const char *name);
void JZ_QueueDestroy(JZ_QUEUE_INFO *info);
void JZ_QueuePut(JZ_QUEUE_INFO *info,CanRxMsg *msg);
int  JZ_QueueGet(JZ_QUEUE_INFO *info,CanRxMsg *msg);
int  JZ_QueueGetHead(JZ_QUEUE_INFO *info,CanRxMsg **msg);
void JZ_QueueDelHead(JZ_QUEUE_INFO *info);
void JZ_QueueClean(JZ_QUEUE_INFO *info);

#endif

