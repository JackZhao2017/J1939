#include "SystermDefine.h"
#include "JzCanRead.h"
#include "JzCanParse.h"
#include "JzUartRead.h"
#include "JzUartParse.h"
#include "JzUartSend.h"
#include "JzParseUser.h"

int JzAppMainInit(void)
{
	Jz_printf("JzAppMainInit start \r\n");
	JzSystemSetUartReadCallBack(Jz_UartRead_SendFrame);
	JzSystemSetCanReadCallBack(Jz_CanRead_SendFrame);
	Jz_CanRead_SetCallBack(Jz_CanParse_SendFrame);
	Jz_CanParse_Init();
	Jz_CanRead_Init();
	Jz_CanParse_SetCallBack(Jz_UartSend_SendFrame);
	Jz_UartSend_Init();
	Jz_UartRead_SetCallBack(Jz_UartParse_SendFrame);
	Jz_UartParse_Init();
	Jz_UartRead_Init();

	return 0;
}

#ifdef LINUX

static JZ_U8 CheckSum(JZ_U8 *buf,JZ_U8 len)
{
	JZ_U32 sum =0,i;
	Jz_printf("CheckSum %d \n",len);
	for(i=0;i<len;i++)
	{
		sum += buf[i];
	}
	return (sum&0xff);
}

char data0[] = {00, 00, 00, 00, 0x4C, 0x4E, 00, 00};
char data1[] = {0x0C,0x7D ,0x7D ,0x00 ,0x00 ,0x00 ,0xF0 ,0x7D};
char data2[] = {0x58,0x34,0x55 ,0x00 ,0x24 ,0xAF ,0x04 ,0x00};
unsigned char cmd[15];

SystemCanReadCallBack g_SystemCanReadCallBackFunc=NULL;
SystemUartReadCallBack g_SystemUartReadCallBackFunc=NULL;

typedef struct _Version
{
	char	name[32];	///< 组件名称
	int		major;		///< 主版本号，架构变动时增加
	int		minor;		///< 次版本号，接口变化或严重缺陷修正时增加
	int		revision;	///< 修订版本号，缺陷修正时增加
	int		svn;		///< svn版本号，记录上述3版本号变更时对应的svn版本号
	char	date[32];	///< 编译日期
} ADAS_VERSION;

static const ADAS_VERSION g_version={"adas",0,1,2,0,__DATE__};

int main(int argc, char const *argv[])
{
	JzAppMainInit();
	int count=0;
	CanRxMsg msg;
	Jz_printf("%s start\n\n\n\n\n\n",__func__);
	JZ_CMDMSG cmdmsg;
	printf("%s success version:v%d.%d.%d date:%s  \n",__func__,g_version.major,g_version.minor,g_version.revision,g_version.date);
	while(1)
	{
		// msg.ExtId= 0x18FEE900;
		// msg.FMI  = (++count)&0xff;
		// msg.Data[0] =  msg.FMI;
		// memcpy(msg.Data,data0,8);
		// Jz_printf("\n main  msg.ExtId 0x%x FMI %d \n",msg.ExtId,msg.FMI);
		// Jz_CanRead_SendFrame(&msg);
		// usleep(50);

		// sleep(1);

		// msg.ExtId= 0x0CF00700;
		// msg.FMI  = (count)&0xff;
		// msg.Data[0] =  msg.FMI;
		// memcpy(msg.Data,data1,8);
		// //Jz_printf("\n main  msg.ExtId 0x%x FMI %d \n",msg.ExtId,msg.FMI);
		// Jz_CanRead_SendFrame(&msg);
		// usleep(50);

		sleep(1);
		// msg.ExtId= 0x0CF00400;
		// msg.FMI  = (++count)&0xff;
		// memcpy(msg.Data,data1,8);
		// //Jz_printf("\n main  msg.ExtId 0x%x FMI %d \n",msg.ExtId,msg.FMI);
		// Jz_CanRead_SendFrame(&msg);
		// usleep(50);

		// msg.ExtId= 0x18FEC1EE;
		// msg.FMI  = (++count)&0xff;
		// memcpy(msg.Data,data2,8);
		// //Jz_printf("\n main  msg.ExtId 0x%x FMI %d \n",msg.ExtId,msg.FMI);
		// Jz_CanRead_SendFrame(&msg);
		// usleep(50);


		// typedef struct 
		// {
		// 	JZ_U8  STATE;
		// 	JZ_U8  INDEX;
		// 	JZ_U8  KBPS;
		// 	JZ_U8  IDE; 
		// 	JZ_U32 ID;
		// 	JZ_U32 MARSK;
			
		// }JZ_CMDMSG;
		cmd[0]=0x55;
		cmd[1]=0xf;
		cmdmsg.STATE = (++count)&0xff;;
		cmdmsg.INDEX = (count)&0x07;
		cmdmsg.KBPS  = 12;
		cmdmsg.IDE =1;
		cmdmsg.ID = 0x125;
		cmdmsg.MARSK = 0xfff;
		memcpy(&cmd[2],&cmdmsg,12);
		cmd[14]=CheckSum(cmd,14);
		if(g_SystemUartReadCallBackFunc!=NULL)
		{
			g_SystemUartReadCallBackFunc(cmd,15);
		}
		Jz_printf(" \n");		
	}
	return 0;
}

#endif


