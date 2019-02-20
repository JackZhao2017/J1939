#include "SystermDefine.h"
#include "JzQueue.h"

// static void Print_QueueInfo(const char *func,JZ_QUEUE_INFO *info)
// {
// 		Jz_printf(" %s get %d put  %d \r\n",func,info->get,info->put);
// }


int  JZ_QueueInit(JZ_QUEUE_INFO *info,CanRxMsg *buf ,int size,const char *name)
{
	if((info==NULL)||(buf==NULL)||(size<1)||(name==NULL))
		return JZ_FAILD;

	info->size =size;
	info->get =0;
	info->put =0;
	info->mark = size -1;
	info->buf = buf;
	return JzMutexInit(&info->mutex,name);

}
void JZ_QueueDestroy(JZ_QUEUE_INFO *info)
{
	memset(info,0,sizeof(JZ_QUEUE_INFO));
	JzMutexDestroy(&info->mutex);
}

int JZ_QueuePut(JZ_QUEUE_INFO *info,CanRxMsg *msg)
{
	JzMutexLock(&info->mutex);
	if(((info->put+1)%info->size)==info->get){
		err_printf("full \r\n");
		JzMutexUnlock(&info->mutex);
		return 1;
	}
	memcpy(&info->buf[info->put],msg,sizeof(CanRxMsg));
	info->put=(info->put+1)%info->size;
	//Print_QueueInfo(__func__,info);
	JzMutexUnlock(&info->mutex);
	return 0;
}
int JZ_QueueGet(JZ_QUEUE_INFO *info,CanRxMsg *msg)
{
	JzMutexLock(&info->mutex);
	if(info->get==info->put){
		JzMutexUnlock(&info->mutex);
		return 0;
	}	
	memcpy(msg,&info->buf[info->get],sizeof(CanRxMsg));
	info->get=(info->get+1)%info->size;
	//Print_QueueInfo(__func__,info);
	JzMutexUnlock(&info->mutex);
	return 1;
}
int JZ_QueueGetHead(JZ_QUEUE_INFO *info,CanRxMsg **msg)
{
	JzMutexLock(&info->mutex);
	if(info->get==info->put){
		JzMutexUnlock(&info->mutex);
		return 0;		
	}
	*msg = &info->buf[info->get];
	JzMutexUnlock(&info->mutex);
	return 1;
}
void JZ_QueueDelHead(JZ_QUEUE_INFO *info)
{
	JzMutexLock(&info->mutex);
	info->get=(info->get+1)%info->size;
	JzMutexUnlock(&info->mutex);
}
void JZ_QueueClean(JZ_QUEUE_INFO *info)
{
	JzMutexLock(&info->mutex);
	info->get=0;
	info->put=0;
	JzMutexUnlock(&info->mutex);
}

