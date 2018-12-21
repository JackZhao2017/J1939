
#include "SystermDefine.h"
#include "JzRingbuffer.h"
#include "JzParseUser.h"

static Jz_ThreadId g_Pid;
static Jz_Sem      g_Sem;
static JZ_BOOL     g_bExit=JZ_FALSE;
//static JZ_BOOL	   g_bfinished  = JZ_TRUE;

#define  kPARSEBUFFER_SIZE  256
#define  kRINGBUFFER_SIZE   512
static   JZ_U8  g_Buffer[kRINGBUFFER_SIZE];
static   JZ_U8  g_Parse[kPARSEBUFFER_SIZE];
static   JZ_RINGBUFFER g_RingbufferInfo;
static Jz_ThreadAttr g_attr={.name="uartparse"};
static JZ_VOID Jz_UartParse_TaskFunc(void *arg)
{
	JZ_S32 syncindex=0,readlen;
	JZ_U8 type;
	Jz_printf("Jz_UartParse_TaskFunc\r\n");
	while(1)
	{
		JzSemWartfor(&g_Sem,0);
		//g_bfinished = JZ_FALSE;
		if(g_bExit == JZ_TRUE){
			break;
		}
		readlen = Jz_Ringbuffer_GetbufferHeardata(&g_RingbufferInfo,g_Parse,kPARSEBUFFER_SIZE);
#ifdef DEBUG
		Jz_printf("Jz_UartParse_TaskFunc %d :",readlen);
#endif
		for (syncindex = 0; syncindex < readlen; ++syncindex)
		{
			if(!Jz_ParseUser_DetectSync(g_Parse,syncindex))
			{
				continue;
			}
			if(syncindex+1>=readlen){
			 	break;
			}
			if(Jz_ParseUser_DetectvaildMsgtype(&g_Parse[syncindex+1],&type)){
			 	if(syncindex+type>readlen){
			 		break;
			 	}
			 	if(Jz_ParseUser_DetectSUM(&g_Parse[syncindex],type-1)){
					if(type == CANMSG)
					{

					}
					if(type == CMDIDMSG)
					{
						Jz_ParseUser_Process(&g_Parse[syncindex]);
					}
					syncindex+=type-1;
			 	}
			}
		}
#ifdef DEBUG
		Jz_printf("\n");
#endif
		Jz_Ringbuffer_AddBufferHearaddr(&g_RingbufferInfo,syncindex);		
		//g_bfinished = JZ_TRUE;
	}
	JzThreadExit();
}

void Jz_UartParse_SendFrame(JZ_U8 *buf, JZ_S32 len)
{
	Jz_Ringbuffer_PutDataToBuffer(&g_RingbufferInfo,buf,len);
	JzSemPost(&g_Sem);
}


JZ_S32 Jz_UartParse_Init(void)
{
	JZ_S32 ret = JzSemCreate(&g_Sem);
	if(ret !=JZ_SUCCESS)
	{
		return ret;
	}
	ret = JzThreadCreate(&g_Pid,&g_attr,Jz_UartParse_TaskFunc,NULL);
	if(ret!=JZ_SUCCESS)
	{
		return ret;
	}
	Jz_Ringbuffer_Init(&g_RingbufferInfo,g_Buffer,kRINGBUFFER_SIZE,"uartparse");

	Jz_printf("Jz_UartParse_Init\n\r");
	return JZ_SUCCESS;
}
JZ_S32 Jz_UartParse_Join(void)
{
	g_bExit = JZ_TRUE;
	JzThreadJoin(g_Pid);
	Jz_printf("Jz_UartParse_Join\n");
	return JZ_SUCCESS;
}
