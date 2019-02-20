
#include "SystermDefine.h"
#include "JzCanParse.h"
#include "JzQueue.h"
#include "JzJ1939.h"
#include "JzUartCan.h"
#include "JzErrCode.h"
#define kPoolSize 20

static Jz_ThreadId g_Pid;
static Jz_Sem      g_Sem;
static JZ_BOOL     g_bExit=JZ_FALSE;
static JZ_BOOL	volatile   g_bfinished  = JZ_TRUE;
static CanRxMsg    g_stRxMsgPool[kPoolSize];
static JZ_QUEUE_INFO g_stQueueInfo;
static CanParseCallBack g_CanParseCallBackFunc =NULL;
static Jz_ThreadAttr g_attr={.name="canparse"};

static JZ_VOID Jz_CanParse_TaskFunc(void *arg)
{
	CanRxMsg msg;
	init_printf("Jz_CanParse_TaskFunc\r\n");
	while(1)
	{
		JzSemWartfor(&g_Sem,0);

		//g_bfinished = JZ_FALSE;
		if(g_bExit == JZ_TRUE){
			break;
		}
		while(JZ_QueueGet(&g_stQueueInfo,&msg))
		{		
			Jz_J1939_Parse(&msg);
			if(Jz_IsCanUartSend(msg.ExtId))
			{
				if(g_CanParseCallBackFunc!=NULL){
					g_CanParseCallBackFunc(&msg);
				}
			}			
		}
		g_bfinished = JZ_TRUE;
	}
	JzThreadExit();
}
void Jz_CanParse_SendFrame(CanRxMsg *msg)
{
	if(JZ_QueuePut(&g_stQueueInfo,msg))
	{
		Jz_SetSystermErrCode(CAN_PARSE_FULL);
	}
	if(g_bfinished==JZ_TRUE){
		g_bfinished = JZ_FALSE;
		JzSemPost(&g_Sem);
		Jz_ClearSystermErrCode(CAN_PARSE_FULL);
	}
}
JZ_S32 Jz_CanParse_SetCallBack(CanParseCallBack pCallBackFunc)
{
	g_CanParseCallBackFunc = pCallBackFunc;
	return 0;
}

int Jz_CanParse_Init(void)
{
	int ret = JzSemCreate(&g_Sem);
	if(ret !=JZ_SUCCESS)
	{
		err_printf("%s JzSemCreate \r\n",__func__);
		return ret;
	}
	
	if(JZ_QueueInit(&g_stQueueInfo,&g_stRxMsgPool[0],kPoolSize,"canparse"))
	{
		err_printf("%s JZ_QueueInit \r\n",__func__);
		return JZ_FAILD;
	}
	Jz_J1939_InitData();
	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_CanParse_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		err_printf("%s JzThreadCreate \r\n",__func__);
		return ret;
	}
	init_printf("%s prio %d \r\n",__func__,g_attr.prio);
	return JZ_SUCCESS;
}

int Jz_CanParse_Join(void)
{
	g_bExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	JZ_QueueDestroy(&g_stQueueInfo);

	init_printf("Jz_CanParse_Join\n");
	return JZ_SUCCESS;
}

