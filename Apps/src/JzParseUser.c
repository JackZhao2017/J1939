
#include "JzParseUser.h"
#include "JzParam.h"
#include "JzCanRead.h"
#include "JzUartCan.h"


JZ_S32 Jz_ParseUser_DetectSync(JZ_U8 *buf,JZ_S32 index)
{
	if(buf==NULL)
		return 0;
	
	if(buf[index]==0x55){	
		return 1;
	}
	return 0;
}

JZ_S32 Jz_ParseUser_DetectvaildMsgtype(JZ_U8 *buf,JZ_U8 *type)
{
	JZ_S32 ret=0;
	if(buf==NULL)
		return 0;
	*type=buf[0];
	switch(*type){
		case CANMSG:	
		case CMDIDMSG:
		case IDMASKMSG:			 	
			 ret=1;
			 break;
		default:
			 ret=0;
			 break;
	}
	return ret;
}
static JZ_U8 CheckSum(JZ_U8 *buf,JZ_U8 len)
{
	JZ_U32 sum =0,i;
	//Jz_printf("CheckSum %d \n",len);
	for(i=0;i<len;i++)
	{
		sum += buf[i];
	}
	return (sum&0xff);
}
JZ_S32 Jz_ParseUser_DetectSUM(JZ_U8 *msg, JZ_S32 len)
{
	if(msg[len]==CheckSum(msg,len))
	{
		return 1;
	}
	err_printf("%s error %d %d  \n",__func__,msg[len] ,CheckSum(msg,len));
	return 0;
}

JZ_S32 Jz_ParseUser_ProcessCmd(JZ_U8 *msg)
{
#ifdef DEBUG
	Jz_printf("%s \n",__func__);
#endif


	JZ_CMDMSG JzCmdMsg;
	JZ_FILTER filter;
	int i=0;
	if(msg==NULL){
		err_printf("%s   msg null\n",__func__);
		return JZ_FAILD;
	}

	memcpy(&JzCmdMsg,msg+3,sizeof(JzCmdMsg));

	if(JzCmdMsg.SUM>4){
		err_printf("%s   SUM %d \n",__func__,JzCmdMsg.SUM);
		return JZ_FAILD;
	}
	if((JzCmdMsg.KBPS ==12)||(JzCmdMsg.KBPS ==14)){
		Jz_ParamSetBaudrate(JzCmdMsg.KBPS);	
	}

	for(i=0;i<JzCmdMsg.SUM;i++){

		filter.CAN_ID      = JzCmdMsg.ID[i]<<8;
		filter.CAN_ID_MASK = JzCmdMsg.MARSK[i]<<8;
		filter.CAN_ID_FMT  = 29;
		Jz_ParamSetResetFilter(JzCmdMsg.SUM,i,&filter);
	}
	Jz_CanRead_ResetCan();
	return JZ_SUCCESS;
}
JZ_S32 Jz_ParseUser_ProcessIDMask(JZ_U8 *msg)
{
	JZ_U32 sum = msg[3];
	if(Jz_SetCanUartSendId(sum,(JZ_U16 *)&msg[4]))
	{

	}
	return JZ_SUCCESS;
}



