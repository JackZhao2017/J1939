#ifndef _JZ_RINGBUFFER_H_
#define _JZ_RINGBUFFER_H_


#include "SystermDefine.h"

typedef struct{
	JZ_U32     put; 		//put first data index;
	JZ_U32     get; 		//get first data index;
	JZ_U32     size;	 		//buffer size;
	JZ_U32     num;     		//vaild data len; 
	JZ_U8      *data;
	Jz_Mutex   mutex;
}JZ_RINGBUFFER;

JZ_S32  Jz_Ringbuffer_Init(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 size,const char *name);
JZ_S32  Jz_Ringbuffer_PutDataToBuffer(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 len);
JZ_S32  Jz_Ringbuffer_GetbufferHeardata(JZ_RINGBUFFER *info,JZ_U8 *buf,JZ_S32 len);
void    Jz_Ringbuffer_AddBufferHearaddr(JZ_RINGBUFFER *info,JZ_S32 len);
void  	Jz_Ringbuffer_CleanBuffer(JZ_RINGBUFFER *info);


#endif
