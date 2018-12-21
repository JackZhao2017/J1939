#include "JzParam.h"

#define STD 11
#define EXT 29


// typedef struct{
// 	JZ_U32 CAN_ID;
// 	JZ_U32 CAN_ID_MASK;
// 	JZ_U32  CAN_ID_FMT;
// } JZ_FILTER;
static const JZ_FILTER g_stDefaultCanFilter[kDEFAULT_FILTER]={
						{0x0cfe6c00,0x00FFFF00,EXT},//7、8字节为转速计输出车速。
						{0x0cfef100,0x00FFFF00,EXT},//2、3字节为基于车轮的车速。
						{0x00feC100,0x00FFFF00,EXT},//减速器控制器;
						{0x00f00300,0x00FFFF00,EXT},//2字节 AP加速踏板位置。
						{0x00f00400,0x00FFFF00,EXT},//发动机转速、减速器模式
						{0x00f00100,0x00FFFF00,EXT},//刹车踏板位置
						{0x00fee900,0x00FFFF00,EXT},//油耗
		};

static JZ_FILTER g_stJzCanFilter[kMAX_FILTER];
static JZ_FILTER g_stJzCanResetFilter[kMAX_FILTER];
static JZ_U8  g_ResetFilterNum = kDEFAULT_FILTER;
static JZ_U8  g_DefaultBaudrate = 24;


//static void Print_FilterInfo(JZ_FILTER *pstfilter,JZ_U32 num)
//{
//	JZ_U32 i=0;
//	for(i=0;i<num;i++)
//	{
//		
//	}
//}

JZ_FILTER *Jz_ParamGetDefaultFilter(JZ_U8 *num)
{

	memset(g_stJzCanFilter,0,sizeof(g_stJzCanFilter));
	
	memset(g_stJzCanResetFilter,0,sizeof(g_stJzCanResetFilter));
	
	memcpy(g_stJzCanResetFilter,g_stDefaultCanFilter,sizeof(JZ_FILTER)*kDEFAULT_FILTER);

	memcpy(g_stJzCanFilter,g_stDefaultCanFilter,sizeof(JZ_FILTER)*g_ResetFilterNum);

	*num = g_ResetFilterNum;

	return g_stJzCanFilter;
}


JZ_FILTER *Jz_ParamGetResetFilter(JZ_U8 *num)
{
	JZ_U32 i=0,index=0;
	for(i=0;i<kMAX_FILTER;i++)
	{
		if(g_stJzCanResetFilter[i].CAN_ID_FMT==0)
			continue;	
		g_stJzCanFilter[index].CAN_ID_FMT  = g_stJzCanResetFilter[i].CAN_ID_FMT;
		g_stJzCanFilter[index].CAN_ID_MASK = g_stJzCanResetFilter[i].CAN_ID_MASK;
		g_stJzCanFilter[index].CAN_ID 	   = g_stJzCanResetFilter[i].CAN_ID;
		index+=1;
	}
	*num = index;
	return g_stJzCanFilter;
}

JZ_S32 Jz_ParamSetResetFilter(JZ_U32 index ,JZ_FILTER *pstfilter)
{
	if(index>=kMAX_FILTER){
		return JZ_FAILD;
	}

	g_stJzCanResetFilter[index].CAN_ID_FMT  = pstfilter->CAN_ID_FMT;
	g_stJzCanResetFilter[index].CAN_ID      = pstfilter->CAN_ID;
	g_stJzCanResetFilter[index].CAN_ID_MASK = pstfilter->CAN_ID_MASK;

	return JZ_SUCCESS;
}


JZ_S32 Jz_ParamSetBaudrate(JZ_U8 kbps)
{
	if((kbps!=12)||(kbps!=24))
	{
		return JZ_FAILD;
	}

	g_DefaultBaudrate = kbps;

	return JZ_SUCCESS;
}
JZ_S32 Jz_ParamGetBaudrate(void)
{
	return g_DefaultBaudrate;
}

