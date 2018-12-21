
#include "SystermDefine.h"
#include "JzCanParse.h"
#include "JzQueue.h"
#include "JzJ1939.h"

#define kPoolSize 20

static Jz_ThreadId g_Pid;
static Jz_Sem      g_Sem;
static JZ_BOOL     g_bExit=JZ_FALSE;
//static JZ_BOOL	   g_bfinished  = JZ_TRUE;
static CanRxMsg    g_stRxMsgPool[kPoolSize];
static JZ_QUEUE_INFO g_stQueueInfo;
static CanParseCallBack g_CanParseCallBackFunc =NULL;
static Jz_ThreadAttr g_attr={.name="canparse"};

static JZ_VOID Jz_CanParse_TaskFunc(void *arg)
{
	CanRxMsg msg;
	Jz_printf("Jz_CanParse_TaskFunc\r\n");
	while(1)
	{
		JzSemWartfor(&g_Sem,0);
		//g_bfinished = JZ_FALSE;
		if(g_bExit == JZ_TRUE){
			break;
		}
		// Jz_printf("Jz_CanParse_TaskFunc  +++++++++++\n");
		while(JZ_QueueGet(&g_stQueueInfo,&msg))
		{			
			if(Jz_J1939_Parse(&msg)==1)
			{
				if(g_CanParseCallBackFunc!=NULL)
					g_CanParseCallBackFunc(&msg);
			}			
		}
		// Jz_printf("Jz_CanParse_TaskFunc  -----------\n");
		//g_bfinished = JZ_TRUE;
	}
	JzThreadExit();
}
void Jz_CanParse_SendFrame(CanRxMsg *msg)
{
#ifdef DEBUG
	Jz_printf("%s \n",__func__);
#endif
	JZ_QueuePut(&g_stQueueInfo,msg);
	JzSemPost(&g_Sem);
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
		return ret;
	}
	
	JZ_QueueInit(&g_stQueueInfo,&g_stRxMsgPool[0],kPoolSize,"canparse");

	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_CanParse_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		return ret;
	}
	Jz_printf("Jz_CanParse_Init\n\r");
	return JZ_SUCCESS;
}

int Jz_CanParse_Join(void)
{
	g_bExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	JZ_QueueDestroy(&g_stQueueInfo);

	Jz_printf("Jz_CanParse_Join\n");
	return JZ_SUCCESS;
}

