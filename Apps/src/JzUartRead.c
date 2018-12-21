#include "SystermDefine.h"
#include "JzUartRead.h"

#define kREADBUFFER_SIZE 200

static Jz_ThreadId g_Pid;
static Jz_Sem      g_Sem;
static JZ_BOOL     g_btaskExit= JZ_FALSE;
static JZ_BOOL 	   g_bFinished = JZ_FALSE;
static UartReadCallBack  g_UartReadCallBackFunc=NULL;
static JZ_U8       g_ReadBuffer[kREADBUFFER_SIZE];
static JZ_U32      g_VaildLen = 0;
static Jz_ThreadAttr g_attr={.name="uartread"};
static JZ_VOID Jz_UartRead_TaskFunc(void *arg)
{
	Jz_printf("Jz_UartRead_TaskFunc\r\n");
	while(1)
	{
		JzSemWartfor(&g_Sem,0);

		JZ_S32 i;
		Jz_printf("%s \r\n",__func__);
		for (i = 0; i < g_VaildLen; ++i)
		{
			Jz_printf("0x%02x ",g_ReadBuffer[i]);
		}
		if(g_VaildLen>0){
			Jz_printf("\r\n");
		}
		if(g_UartReadCallBackFunc!=NULL){
			g_UartReadCallBackFunc(g_ReadBuffer,g_VaildLen);
		}
		g_bFinished = JZ_TRUE; 		
		if(g_btaskExit == JZ_TRUE){
			break;
		}
	}
	JzThreadExit();
}

JZ_S32 Jz_UartRead_Init(void)
{
	JZ_S32 ret = JzSemCreate(&g_Sem);
	if(ret !=JZ_SUCCESS)
	{
		return ret;
	}
	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_UartRead_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		return ret;
	}
	memset(g_ReadBuffer,0,sizeof(g_ReadBuffer));

	g_btaskExit = JZ_FALSE;
	g_bFinished = JZ_TRUE;
	Jz_printf("Jz_UartRead_Init\n\r");
	return JZ_SUCCESS;
}

JZ_S32 Jz_UartRead_Suspend(void)
{
	return JZ_SUCCESS;
}	

JZ_S32 Jz_UartRead_SetCallBack(UartReadCallBack pCallBackFunc)
{
	g_UartReadCallBackFunc = pCallBackFunc;
	return JZ_SUCCESS;
}

void Jz_UartRead_SendFrame(JZ_U8  *msg,JZ_S32 len)
{
	if(g_bFinished==JZ_TRUE)
	{
		g_bFinished = JZ_FALSE;
		memcpy(g_ReadBuffer,msg,len);
		g_VaildLen = len;
		JzSemPost(&g_Sem);
	}else{
		Jz_printf("Jz_CanRead_SendFrame not finished!!!!!!!!!!!!!!!!\n");
	}
}

JZ_S32 Jz_UartRead_Join(void)
{
	g_btaskExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	Jz_printf("Jz_CanRead_Join\n");
	return JZ_SUCCESS;
}

