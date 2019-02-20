
#include "SystermDefine.h"
#include "JzCanRead.h"
#include "JzParam.h"
#include "JzParseUser.h"
#include "JzErrCode.h"

static Jz_ThreadId g_Pid;
static Jz_Sem      g_CanReadSem;
static JZ_BOOL     g_btaskExit= JZ_FALSE;
static JZ_BOOL 	volatile g_bFinished = JZ_TRUE;
static CanReadCallBack  g_CanReadCallBackFunc=NULL;
static CanRxMsg	g_CanRxMsg;
static JZ_U32 g_CanReadEn =0;
static Jz_ThreadAttr g_attr={.name="canread"};

static JZ_VOID Jz_CanRead_TaskFunc(void *arg)
{
	JZ_U8 errorcode=0;
	init_printf("Jz_CanRead_TaskFunc\r\n");
	while(1)
	{
		g_bFinished = JZ_TRUE;
		JZ_S32 ret =JzSemWartfor(&g_CanReadSem,0);
		if(ret ==0)
		{
			if(g_CanReadCallBackFunc!=NULL){
				g_CanReadCallBackFunc(&g_CanRxMsg);
			}
		}else {
			errorcode = JzGetCanErrorCode();
//#ifdef DEBUG
			Jz_printf("%s  errorcode %d   \r\n",__func__,errorcode);
//#endif
		}		 	
		if(g_btaskExit == JZ_TRUE){
			break;
		}
	}
	JzThreadExit();
}


JZ_S32 Jz_CanRead_SetCallBack(CanReadCallBack pCallBackFunc)
{
	g_CanReadCallBackFunc = pCallBackFunc;
	return JZ_SUCCESS;
}

JZ_S32 Jz_CanRead_Init(void)
{
	JZ_U8 num=0;
	JZ_U32 ret = JzSemCreate(&g_CanReadSem);
	if(ret !=JZ_SUCCESS)
	{
		return ret;
	}
	Jz_ParamInitFilter();
	JZ_FILTER *pfilter = Jz_ParamGetDefaultFilter(&num);
	JzCanSetFilter(pfilter,num);
	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_CanRead_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		return ret;
	}
	memset(&g_CanRxMsg,0,sizeof(CanRxMsg));
	g_btaskExit = JZ_FALSE;
	g_bFinished = JZ_TRUE;
	g_CanReadEn = 1;
	init_printf("%s prio %d \r\n",__func__,g_attr.prio);
	return JZ_SUCCESS;
}
void Jz_CanRead_SendFrame(CanRxMsg *msg)
{	
	if(g_bFinished==JZ_TRUE)
	{
		g_bFinished = JZ_FALSE;
		Jz_ClearSystermErrCode(CAN_READ_FULL);
		memcpy(&g_CanRxMsg,msg,sizeof(CanRxMsg));
		JzSemPost(&g_CanReadSem);
	}else{
		Jz_SetSystermErrCode(CAN_READ_FULL);
	}
}



JZ_S32 Jz_CanRead_Suspend(void)
{
	if(g_CanReadEn==1){
		g_CanReadEn = JzCanDisable();
	}
	return g_CanReadEn;	
}


JZ_S32 Jz_CanRead_Resume(void)
{
	if(g_CanReadEn==0){
		g_CanReadEn = JzCanEnable();
	}
	return g_CanReadEn;	
}

JZ_S32 Jz_CanRead_ResetCan(void)
{
	JZ_U8 num= 0;

	JZ_FILTER * pstfilter = Jz_ParamGetResetFilter(&num);
	if(pstfilter ==NULL)
		return -1;
	JZ_U32 Baudrate = Jz_ParamGetBaudrate();
	Jz_CanRead_Suspend();
	JzCanSetFilter(pstfilter,num);
	Jz_CanRead_Resume();
	return JZ_SUCCESS;	
}

JZ_S32 Jz_CanRead_Join(void)
{
	g_btaskExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	init_printf("Jz_CanRead_Join\n");
	return JZ_SUCCESS;
}

