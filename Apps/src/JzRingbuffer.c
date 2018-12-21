#include "SystermDefine.h"
#include "JzRingbuffer.h"


static void Print_Info(const char *func,JZ_RINGBUFFER *info)
{
	Jz_printf(" %s get %d put  %d num %d \r\n",func,info->get,info->put,info->num);
}


JZ_S32  Jz_Ringbuffer_Init(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 size,const char *name)
{
	if(buf==NULL){
		Jz_printf("Jz_Ringbuffer_Init buf is NULL\n");
	}
	info->data=buf;
	info->get =0;
	info->put =0;
	info->num =0;
	info->size=size;
	JzMutexInit(&info->mutex,name);
	return JZ_SUCCESS;
}
JZ_S32  Jz_Ringbuffer_PutDataToBuffer(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 len)
{
	if(info==NULL){
		Jz_printf(" %s  info is NULL\r\n",__func__);
		return JZ_FAILD;
	}
	if(info->data==NULL||info->size<1){
		Jz_printf(" %s  data is NULL\r\n",__func__);
		return JZ_FAILD;
	}
	if(len<=0){
		Jz_printf(" %s  len is wrong \r\n",__func__);
		return JZ_FAILD;
	}

	JzMutexLock(&info->mutex);

	if(info->put+len>info->size){
		memcpy(&info->data[info->put],buf,info->size-info->put);
		memcpy(&info->data[0],&buf[info->size-info->put],info->put+len-info->size);
	}else{
		memcpy(&info->data[info->put],buf,len);
	}

	info->num+= len;
	info->put = (info->put+len)%(info->size);
	//Print_Info(__func__,info);
	JzMutexUnlock(&info->mutex);

	return JZ_SUCCESS;
}
JZ_S32  Jz_Ringbuffer_GetbufferHeardata(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 len)
{
	if(info==NULL){
		Jz_printf(" %s  info is NULL\r\n",__func__);
		return 0;
	}
	if(info->data==NULL){
		Jz_printf(" %s  info buf is NULL\r\n",__func__);
		return 0;
	}

	JzMutexLock(&info->mutex);
	
	if(len>info->num){
		len=info->num;
	}

	if(info->get+len>=info->size){
		memcpy(buf,&info->data[info->get],info->size-info->get);
		memcpy(&buf[info->size-info->get],&info->data[0],info->get+len-info->size);
	}else{
		memcpy(buf,&info->data[info->get],len);
	}

	//Print_Info(__func__,info);
	JzMutexUnlock(&info->mutex);
	return len;
}
void  Jz_Ringbuffer_AddBufferHearaddr(JZ_RINGBUFFER *info,JZ_S32 len)
{
	JzMutexLock(&info->mutex);
	info->num-=len;
	info->get=(info->get+len)%(info->size); 
	//Print_Info(__func__,info);
	JzMutexUnlock(&info->mutex);
}
void  	Jz_Ringbuffer_CleanBuffer(JZ_RINGBUFFER *info)
{
	JzMutexLock(&info->mutex);
	info->get = 0;
	info->put = 0;
	JzMutexUnlock(&info->mutex);
}

