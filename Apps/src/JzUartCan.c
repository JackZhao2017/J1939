#include "JzUartCan.h"
#define JZ_SYNC 	0x55
#define JZ_CANMSG   0x11

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

JZ_U32 Jz_Can2Uart(CanRxMsg *canmsg,JZ_UartTxMsg *uartmsg)
{
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
	uartmsg->CRC8 = CheckSum((JZ_U8 *)uartmsg,sizeof(JZ_UartTxMsg)-1);

	return JZ_SUCCESS;
}

