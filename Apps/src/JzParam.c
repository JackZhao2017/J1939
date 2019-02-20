#include "JzParam.h"

#define STD 11
#define EXT 29


// typedef struct{
// 	JZ_U32 CAN_ID;
// 	JZ_U32 CAN_ID_MASK;
// 	JZ_U32  CAN_ID_FMT;
// } JZ_FILTER;
// static const JZ_FILTER g_stDefaultCanFilter[kDEFAULT_FILTER]={
// 						{0x00fe6c00,0x00FFFF00,EXT},//7、8字节为转速计输出车速。
// 						{0x00fef100,0x00FFFF00,EXT},//2、3字节为基于车轮的车速。
// 						{0x00feC100,0x00FFFF00,EXT},//激活时1s；或状态改变 ,行程;
// 						{0x00f00300,0x00FFFF00,EXT},//2字节 AP加速踏板位置。
// 						{0x00f00400,0x00FFFF00,EXT},//发动机转速、减速器模式
// 						{0x00f00100,0x00FFFF00,EXT},//刹车踏板位置
// 						{0x00fee900,0x00FFFF00,EXT},//油耗，请求时
// 		};

static const JZ_FILTER g_stDefaultCanFilter[kDEFAULT_FILTER]={
						{0x00f00000,0x00FFF800,EXT},
						//0x00f00100 刹车踏板位置；
						//0x00f00200
						//0x00f00300 2字节 AP加速踏板位置；
						//0x00f00400 发动机转速、减速器模式,
						//0x00f0000 
						{0x00fe6C00,0x00FFFC00,EXT},
						//0x00fe6c00 7、8字节为转速计输出车速。
						//0x00fe6d00 。
						//0x00fe6e00 。
						//0x00fe6f00 可适应的巡游控制（ACC1）5、6路面曲率
						{0x00fef000,0x00FFFA00,EXT},
						//0x00fef100 2、3字节为基于车轮的车速。
						//0x00fef000
						//0x00fef400 疲劳条件
						//0x00fef500
						{0x00feC100,0x00FFFd00,EXT},
						//0x00feC100 激活时1s；或状态改变 ,行程;
						//0x00feC300 1	"4-3低范围传感开关 2-1高范围传感开关"
						//			 2	"6-5正向开关	4-3空挡开关  2-1反向开关"
						{0x00fee000,0x00FFF600,EXT},
						//0x00fee000  行车距离 
						//0x00fee100  减速器结构
						//0x00fee800  车辆方向/速度  
						//0x00fee900  油耗，请求时
						{0x00feeA00,0x00FFEA00,EXT}
						//0x00feea00  车重
						//0x00feeb00   
						//0x00feeC00
						//0x00feed00
						//0x00feee00
						//0x00feef00  
						//0x00fefa00 
						//0x00fefb00
						//0x00fefC00
						//0x00fefd00
						//0x00fefe00 
						//0x00feff00 

		};

static JZ_FILTER g_stJzCanFilter[kMAX_FILTER];
static JZ_FILTER g_stJzCanResetFilter[kRESETFILTER];
static JZ_U8  g_ResetFilterNum = 0;
static JZ_U8  g_DefaultBaudrate = 24;


JZ_VOID Jz_ParamInitFilter(JZ_VOID)
{
	memset(g_stJzCanFilter,0,sizeof(g_stJzCanFilter));	
	memset(g_stJzCanResetFilter,0,sizeof(g_stJzCanResetFilter));	
}

JZ_FILTER *Jz_ParamGetDefaultFilter(JZ_U8 *num)
{
	// memcpy(g_stJzCanResetFilter,g_stDefaultCanFilter,sizeof(JZ_FILTER)*kDEFAULT_FILTER);
	memcpy(g_stJzCanFilter,g_stDefaultCanFilter,sizeof(JZ_FILTER)*kDEFAULT_FILTER);
	*num = kDEFAULT_FILTER;
	return g_stJzCanFilter;
}


JZ_FILTER *Jz_ParamGetResetFilter(JZ_U8 *num)
{
	JZ_U32 i=0;
	*num = kDEFAULT_FILTER+g_ResetFilterNum;
	memcpy(g_stJzCanFilter,g_stDefaultCanFilter,sizeof(JZ_FILTER)*kDEFAULT_FILTER);
	for(i=kDEFAULT_FILTER;i<kDEFAULT_FILTER+g_ResetFilterNum;i++)
	{
		g_stJzCanFilter[i].CAN_ID_FMT  = g_stJzCanResetFilter[i-kDEFAULT_FILTER].CAN_ID_FMT;
		g_stJzCanFilter[i].CAN_ID_MASK = g_stJzCanResetFilter[i-kDEFAULT_FILTER].CAN_ID_MASK;
		g_stJzCanFilter[i].CAN_ID 	   = g_stJzCanResetFilter[i-kDEFAULT_FILTER].CAN_ID;

	}
	return g_stJzCanFilter;
}

JZ_S32 Jz_ParamSetResetFilter(JZ_U32 sum,JZ_U32 index ,JZ_FILTER *pstfilter)
{
	if(index>=kRESETFILTER){
		return JZ_FAILD;
	}
	if(sum>=kRESETFILTER){
		return JZ_FAILD;
	}
	g_ResetFilterNum = sum;

	g_stJzCanResetFilter[index].CAN_ID_FMT  = pstfilter->CAN_ID_FMT;
	g_stJzCanResetFilter[index].CAN_ID      = pstfilter->CAN_ID;
	g_stJzCanResetFilter[index].CAN_ID_MASK = pstfilter->CAN_ID_MASK;

#ifdef DEBUG
	info_printf("%s %d\r\n",__func__,index);
	info_printf("CAN_ID 0x%x \r\n",g_stJzCanResetFilter[index].CAN_ID);
	info_printf("CAN_ID_MASK 0x%x \r\n",g_stJzCanResetFilter[index].CAN_ID_MASK);
	info_printf("CAN_ID_FMT %d \r\n",g_stJzCanResetFilter[index].CAN_ID_FMT);
#endif
	return JZ_SUCCESS;
}


JZ_S32 Jz_ParamSetBaudrate(JZ_U8 kbps)
{
	if((kbps!=12)&&(kbps!=24))
	{
		return JZ_FAILD;
	}
#ifdef DEBUG
	info_printf("%s %d \r\n",__func__,kbps);
#endif
	g_DefaultBaudrate = kbps;
	return JZ_SUCCESS;
}
JZ_S32 Jz_ParamGetBaudrate(void)
{
	return g_DefaultBaudrate;
}

static JZ_U8 volatile g_SystermErrCode=0;

void Jz_SetSystermErrCode(JZ_U8 err)
{
	g_SystermErrCode|=err;
}
void Jz_ClearSystermErrCode(JZ_U8 err)
{
	g_SystermErrCode&=~err;
}
JZ_U8  Jz_GetSystermErrCode(void)
{
	return g_SystermErrCode;
}


