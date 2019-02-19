#include "JzUartCan.h"

#define JZ_SYNC 			0x55
#define JZ_CANMSG   	0x10
#define JZ_CANUARTMSG_MAX 14

typedef struct _Can2UartId
{
	JZ_U8  volatile enable;
	JZ_U8  volatile busy;
	JZ_U8  volatile sum;
	JZ_U16 ID[JZ_CANUARTMSG_MAX];
	Jz_Mutex   mutex;
}Can2UartId;

static Can2UartId g_stCan2UartId;
static JZ_U8 msgcounter=0;
static JZ_U8 MsgCounter(void)
{
		JZ_U8 retval=msgcounter&0xff;
	    msgcounter=(msgcounter+1)&0xff;
		return retval;;
}
static JZ_U8 CheckSum(JZ_U8 *buf,JZ_U8 len)
{
	JZ_U32 sum =0,i;
#ifdef DEBUG
	Jz_printf("%s \n",__func__);
	Jz_printf("CheckSum %d \n",len);
#endif
	
	for(i=0;i<len;i++)
	{
		sum += buf[i];
	}
	return (sum&0xff);
}

JZ_S32 Jz_Can2Uart(CanRxMsg *canmsg,JZ_UartTxMsg *uartmsg)
{
	if(canmsg==NULL||uartmsg==NULL)
		return JZ_FAILD;

	uartmsg->SYNC = JZ_SYNC;
	uartmsg->TYPE = JZ_CANMSG;
	uartmsg->COUNTER = MsgCounter();
	if(canmsg->IDE){
		uartmsg->IDE_RTR=(1<<4)|(canmsg->RTR&0x0f);
		//uartmsg->ID=canmsg->ExtId;
		memcpy(uartmsg->ID,&canmsg->ExtId,4);
	}else{
		uartmsg->IDE_RTR=(0<<4)|(canmsg->RTR&0x0f);
		//uartmsg->ID=canmsg->StdId;
		memcpy(uartmsg->ID,&canmsg->StdId,4);				 
	}
	memcpy(uartmsg->Data,canmsg->Data,8);
	uartmsg->CRC8 = CheckSum((JZ_U8 *)uartmsg,JZ_CANMSG);

	return JZ_SUCCESS;
}
JZ_S32 Jz_Can2UartInit(void)
{
	msgcounter = 0;
	memset(&g_stCan2UartId,0,sizeof(g_stCan2UartId));
	if(JzMutexInit(&g_stCan2UartId.mutex,"uartcan"))
	{
		err_printf("%s \r\n",__func__);
		return JZ_FAILD;
	}
	info_printf("Can2UartId %d  Jz_Mutex %d \r\n",sizeof(Can2UartId),sizeof(Jz_Mutex));
	return JZ_SUCCESS;
}
JZ_U8  Jz_SetCanUartSendId(JZ_U32 num,JZ_U16 *id)
{
	int i=0;
	int sum =num>JZ_CANUARTMSG_MAX?JZ_CANUARTMSG_MAX:num;
	if(id ==NULL){
		Jz_printf(" %s \r\n",__func__);
		return 1;
	}
	JzMutexLock(&g_stCan2UartId.mutex);
	for(i=0;i<sum;i++)
	{
		g_stCan2UartId.ID[i] = id[i];
		info_printf("IDMASK %x \r\n",g_stCan2UartId.ID[i]);
	}
	g_stCan2UartId.sum=sum&0xff;
	g_stCan2UartId.enable =1;
	JzMutexUnlock(&g_stCan2UartId.mutex);
	return 0;
}
JZ_U8  Jz_IsCanUartSend(JZ_U32 id)
{
	int i=0,ret=0;
	JzMutexLock(&g_stCan2UartId.mutex);
	if(g_stCan2UartId.enable==1)
	{
		for(i=0;i<g_stCan2UartId.sum;i++)
		{
			if(g_stCan2UartId.ID[i]==((id>>8)&0xffff))
			{
				ret =  1;
				break;
			}
		}		
	}
	JzMutexUnlock(&g_stCan2UartId.mutex);
	return ret;
}

