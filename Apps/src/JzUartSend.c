

#include "SystermDefine.h"
#include "JzUartSend.h"
#include "JzQueue.h"
#include "JzJ1939.h"
#include "JzUartCan.h"

#define kPoolSize 10

static Jz_ThreadId g_Pid;
static Jz_Sem      g_Sem;
static JZ_BOOL     g_btaskExit= JZ_FALSE;
//static JZ_BOOL 	   g_bFinished = JZ_TRUE;

static CanRxMsg      g_stSendMsgPool[kPoolSize];
static JZ_QUEUE_INFO g_stSendQueueInfo;

static CanRxMsg  mSendMsg;
static JZ_UartTxMsg mUartMsg;
static Jz_J1939_INFO mJ1939Info;
static Jz_ThreadAttr g_attr={.name="uartsend"};
static JZ_VOID Jz_UartSend_TaskFunc(void *arg)
{
	Jz_printf("Jz_UartSend_TaskFunc\r\n");
	static int flag=0;
	while(1)
	{
		JzSemWartfor(&g_Sem,200);
		while(JZ_QueueGet(&g_stSendQueueInfo,&mSendMsg))
		{
			Jz_Can2Uart(&mSendMsg,&mUartMsg);
			JzUartWrite((JZ_U8 *)&mUartMsg,sizeof(JZ_UartTxMsg));
		}
		Jz_J1939_GetCarInfomation(&mJ1939Info);
		JzUartWrite((JZ_U8 *)&mJ1939Info,sizeof(Jz_J1939_INFO)-3);
		if(flag){
			JzLedOn();
			flag =0;
		}else{
			flag =1;
			JzLedOff();
		}

		//g_bFinished = JZ_TRUE; 		
		if(g_btaskExit == JZ_TRUE){
			break;
		}
	}
	JzThreadExit();
}

void Jz_UartSend_SendFrame(CanRxMsg *msg)
{
#ifdef DEBUG
	Jz_printf("%s \n",__func__);
#endif
	JZ_QueuePut(&g_stSendQueueInfo,msg);
	JzSemPost(&g_Sem);
}

JZ_S32 Jz_UartSend_Init(void)
{
	JZ_S32 ret = JzSemCreate(&g_Sem);
	if(ret !=JZ_SUCCESS)
	{
		return ret;
	}
	JZ_QueueInit(&g_stSendQueueInfo,&g_stSendMsgPool[0],kPoolSize,"uartsend");

	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_UartSend_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		return ret;
	}
	g_btaskExit = JZ_FALSE;
	//g_bFinished = JZ_TRUE;
	Jz_printf("Jz_UartSend_Init \r\n");
	return JZ_SUCCESS;
}

JZ_S32 Jz_UartSend_Join(void)
{
	g_btaskExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	JZ_QueueDestroy(&g_stSendQueueInfo);
	Jz_printf("Jz_CanParse_Join\n");	
	return JZ_SUCCESS;
}

