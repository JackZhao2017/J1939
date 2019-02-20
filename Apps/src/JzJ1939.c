#include "JzJ1939.h"
#include "SystermDefine.h"
#include "JzErrCode.h"

#define TRAVEL_PGN   0xfec1  //激活时1s；或状态改变
#define ETC5_PGN	 0xfec3  //

#define CAR_PGN      0xfee8  //车辆方向/速度1、2	方向盘负荷 3、4	基于引航的车辆速度5、6	斜度7、8	高度
#define OIL_PGN		 0xfee9  //请求时
#define DRIVER_PGN	 0xfee0  //行车距离1、2、3、4	旅程距离 5、8	总的行车距离
#define WEIGTH_PGN   0xfeea  //

#define TCO1_PGN 	 0xfe6c  //50ms
#define ACC1_PGN	 0xfe6f  //100ms  5、6 路况曲度

#define CARSPEED_PGN 0xfef1  //100ms

#define ERC1_PGN	 0xf000
#define EBC1_PGN     0xf001  //电子闸控制器 ,2字节刹车踏板位置 100ms,第一个字节
#define EEC2_PGN     0xf003	 //电子发动机控制器,2字节 AP加速踏板位置
#define EEC1_PGN	 0xf004  //发送机转速



typedef struct 
{
	JZ_U8 Speed_Cred;
	JZ_U8 TCO1_Speed  ;
	JZ_U8 CAR_Direction;
	JZ_U8 Wheel_Speed ;

	JZ_U8  EBC1_Brake ;//电子闸控制器 ,2字节刹车踏板位置
	JZ_U8  EEC2_Accelerate ;//电子发动机控制器,2字节 AP加速踏板位置
	JZ_U16 EEC1_RPM ;//发送机转速

	JZ_U32 DisOfTraval;//里程;
	JZ_U32 AllDisOfTraval;

	JZ_U32 DisOfDriver;//行车距离;
	JZ_U32 AllDisOfDriver;

	JZ_U32 FuelOil;//油耗;
	JZ_U32 AllFuelOil ;	

	JZ_U16 Curvature;
	JZ_U16 SensorDKSwitch;  

	JZ_U16 CompassDirection;
	JZ_U16 AxleWeight;
	JZ_U16 TrailWeight; 
	JZ_U16 StuffWeight;

}J939_CANINFO;

typedef struct _CLEAR_COUNT
{
	JZ_U8 TCO1_Speed_Count;
	JZ_U8 Wheel_Speed_Count;
	JZ_U8 EEC1_RPM_Count;
	JZ_U8 EBC1_Brake_Count;
	JZ_U8 EEC2_Accelerate_Count;
	JZ_U8 Traval_Count;
	JZ_U8 Driver_Count;
	JZ_U8 FuelOil_Count;
	JZ_U8 Curvature_Count;
	JZ_U8 SensorDKSwitch_Count;
	JZ_U8 CompassDirection_Count;
	JZ_U8 Weight_Count;


}CLEAR_COUNT;

static CLEAR_COUNT g_stClearCount;
static CLEAR_COUNT *g_pstClearCount=&g_stClearCount;
static J939_CANINFO g_stJ939_CanInfo;
static J939_CANINFO *g_pstJ939_CanInfo=&g_stJ939_CanInfo;

static JZ_U32 J1939_Get_ParameterGroup(JZ_U32 ExtId)
{
	return ((ExtId>>8)&0xffff);
}

static void J1939_Parse_TCO1(CanRxMsg *msg)//基于转速计的速度
{
	g_pstJ939_CanInfo->Speed_Cred |= 1;
	g_pstJ939_CanInfo->TCO1_Speed = msg->Data[7];
	g_pstClearCount->TCO1_Speed_Count =0;
	g_pstJ939_CanInfo->CAR_Direction =(msg->Data[3]>>6)&0x3;
}

static void J1939_Parse_CARWheel(CanRxMsg *msg)//基于车轮的速度
{
	g_pstJ939_CanInfo->Speed_Cred |= 2;
	g_pstJ939_CanInfo->Wheel_Speed = msg->Data[2];
	g_pstClearCount->Wheel_Speed_Count =0;
}
static void J1939_Parse_ERC1(CanRxMsg *msg)
{

}
static void J1939_Parse_EBC1(CanRxMsg *msg) //电子闸控制器 ,2字节刹车踏板位置
{
	g_pstJ939_CanInfo->EBC1_Brake = msg->Data[1];
	g_pstClearCount->EBC1_Brake_Count=0;
}

static void J1939_Parse_EEC2(CanRxMsg *msg)//电子发动机控制器,2字节 AP加速踏板位置
{
	g_pstJ939_CanInfo->EEC2_Accelerate = msg->Data[1];
	g_pstClearCount->EEC2_Accelerate_Count=0;
}

static void J1939_Parse_EEC1(CanRxMsg *msg)//发送机转速
{
	g_pstJ939_CanInfo->EEC1_RPM = msg->Data[3]+ (msg->Data[4]<<8);
	g_pstClearCount->EEC1_RPM_Count=0;
	//Jz_printf("\n g_pstJ939_CanInfo->EEC1_RPM  %d  \n",g_pstJ939_CanInfo->EEC1_RPM);
}

static void J1939_Parse_FuelOil(CanRxMsg *msg)//油耗;
{
	JZ_U32 val = msg->Data[4]+(msg->Data[5]<<8)+(msg->Data[6]<<16)+(msg->Data[7]<<24);
	g_pstJ939_CanInfo->AllFuelOil = val;
	val =msg->Data[0]+(msg->Data[1]<<8)+(msg->Data[2]<<16)+(msg->Data[3]<<24);
	g_pstJ939_CanInfo->FuelOil = val;
	//Jz_printf("\n J1939_Parse_FuelOil  %d g_J939_AllFuelOil %d \n",g_J939_FuelOil,g_J939_AllFuelOil);
	g_pstClearCount->FuelOil_Count=0;
}


static void J1939_Parse_DisOfTravel(CanRxMsg *msg)//里程;
{
	JZ_U32 val = msg->Data[4]+(msg->Data[5]<<8)+(msg->Data[6]<<16)+(msg->Data[7]<<24);
	g_pstJ939_CanInfo->DisOfTraval = val;
	val =msg->Data[0]+(msg->Data[1]<<8)+(msg->Data[2]<<16)+(msg->Data[3]<<24);
	g_pstJ939_CanInfo->AllDisOfTraval = val;
	//Jz_printf("\n g_J939_DisOfTraval  %d g_J939_AllDisOfTraval %d \n",g_J939_DisOfTraval,g_J939_AllDisOfTraval);
	g_pstClearCount->Traval_Count=0;
}
static void J1939_Parse_DisOfDriver(CanRxMsg *msg)//里程;
{
	JZ_U32 val = msg->Data[4]+(msg->Data[5]<<8)+(msg->Data[6]<<16)+(msg->Data[7]<<24);
	g_pstJ939_CanInfo->AllDisOfDriver= val;
	val =msg->Data[0]+(msg->Data[1]<<8)+(msg->Data[2]<<16)+(msg->Data[3]<<24);
	g_pstJ939_CanInfo->DisOfDriver = val;
	//Jz_printf("\n g_J939_DisOfTraval  %d g_J939_AllDisOfTraval %d \n",g_J939_DisOfTraval,g_J939_AllDisOfTraval);
	g_pstClearCount->Driver_Count=0;
}
static void J1939_Parse_Curvature(CanRxMsg *msg)
{
	g_pstJ939_CanInfo->Curvature=msg->Data[4]+(msg->Data[5]<<8);
	g_pstClearCount->Curvature_Count=0;
}
static void J1939_Parse_SensorDirectionKongSwtich(CanRxMsg *msg)
{
	g_pstJ939_CanInfo->SensorDKSwitch=msg->Data[0]+(msg->Data[1]<<8);
	g_pstClearCount->SensorDKSwitch_Count =0;
}
static void J1939_Parse_CompassDirection(CanRxMsg *msg)
{
	g_pstJ939_CanInfo->CompassDirection=msg->Data[0]+(msg->Data[1]<<8);
	g_pstClearCount->CompassDirection_Count =0;
}
static void J1939_Parse_Weight(CanRxMsg *msg)
{
	g_pstJ939_CanInfo->AxleWeight =msg->Data[1]+(msg->Data[2]<<8);
	g_pstJ939_CanInfo->TrailWeight=msg->Data[3]+(msg->Data[4]<<8);
	g_pstJ939_CanInfo->StuffWeight=msg->Data[5]+(msg->Data[6]<<8);
	g_pstClearCount->Weight_Count =0;
}
JZ_U8 Jz_J1939_Parse(CanRxMsg *msg)
{
	JZ_U8  ret = 0;
	JZ_U32 extId 		  = msg->ExtId;
	JZ_U32 parameterGroup = J1939_Get_ParameterGroup(extId);
	switch(parameterGroup)
	{
		case TCO1_PGN:
			J1939_Parse_TCO1(msg);//转速计速度;
			break;
		case CARSPEED_PGN:
			J1939_Parse_CARWheel(msg);//车轮的车辆速度;
			break;
		case ERC1_PGN:
			J1939_Parse_ERC1(msg);//
			break;
		case EBC1_PGN:
			J1939_Parse_EBC1(msg);//刹车踏板;
			break;
		case EEC2_PGN:
			J1939_Parse_EEC2(msg); //加速踏板;
			break;
		case EEC1_PGN:
			J1939_Parse_EEC1(msg);  //发动机转速;
			break;
		case TRAVEL_PGN:
			J1939_Parse_DisOfTravel(msg);//里程;
			break;
		case OIL_PGN:
			J1939_Parse_FuelOil(msg); //燃油;
			break;
		case ACC1_PGN:
			J1939_Parse_Curvature(msg); //ACC 路面曲率 0xfe6f
			break;
		case ETC5_PGN:
			J1939_Parse_SensorDirectionKongSwtich(msg);
			break;
		case CAR_PGN:
			J1939_Parse_CompassDirection(msg);
			break;
		case DRIVER_PGN:
			J1939_Parse_DisOfDriver(msg);
			break;
		case WEIGTH_PGN:
			J1939_Parse_Weight(msg);
			break;
		default:
			ret =1;
			break;
	}
	return ret;
}

#define J1939_SYNC  0x55
#define J1939_HEART 0x2f

// typedef struct 
// {
// 	JZ_U8  SYNC;
// 	JZ_U8  TYPE;
// 	JZ_U8  COUNTER;
// 	JZ_U8  Speed; //0~251 km/h
// 	JZ_U16 Rpm;   //0~8100;
// 	JZ_U8  Brake; //0~250;
// 	JZ_U8  Accelerate;//0~250;
// 	JZ_U32 DisOfTraval;
// 	JZ_U32 AllDisOfTraval;
// 	JZ_U32 FuelOil;
// 	JZ_U32 AllFuelOil;
// 	JZ_U8  SUM;
// }Jz_J1939_INFO;
static JZ_U8 msgcounter=0;
static JZ_U8 MsgCounter(void)
{
		JZ_U8 retval=msgcounter&0xff;
	    msgcounter=(msgcounter+1)&0xff;
		return retval;;
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

JZ_U32 Jz_J1939_GetCarInfomation(Jz_J1939_INFO *info)
{
	info->SYNC = J1939_SYNC;
	info->TYPE = J1939_HEART;
	info->COUNTER = MsgCounter();

	if(g_pstJ939_CanInfo->Speed_Cred==1)
	{
		info->Speed =g_pstJ939_CanInfo->TCO1_Speed;
	}else if(g_pstJ939_CanInfo->Speed_Cred==2){
		info->Speed = g_pstJ939_CanInfo->Wheel_Speed;
	}else{
		info->Speed =0xff;
	}
	info->Direction =g_pstJ939_CanInfo->CAR_Direction;
	info->Rpm =g_pstJ939_CanInfo->EEC1_RPM;
	info->Brake = g_pstJ939_CanInfo->EBC1_Brake;
	info->Accelerate = g_pstJ939_CanInfo->EEC2_Accelerate;
	info->DisOfTraval = g_pstJ939_CanInfo->DisOfTraval;
	info->AllDisOfTraval = g_pstJ939_CanInfo->AllDisOfTraval;
	info->FuelOil =g_pstJ939_CanInfo->FuelOil;
	info->AllFuelOil =g_pstJ939_CanInfo->AllFuelOil;

	info->DisOfDriver = g_pstJ939_CanInfo->DisOfDriver;
	info->AllDisOfDriver = g_pstJ939_CanInfo->AllDisOfDriver;
	info->CompassDirection =g_pstJ939_CanInfo->CompassDirection;
	info->Curvature = g_pstJ939_CanInfo->Curvature;	
	info->SensorDKSwitch =g_pstJ939_CanInfo->SensorDKSwitch;
	info->AxleWeight =g_pstJ939_CanInfo->AxleWeight;
	info->TrailWeight =g_pstJ939_CanInfo->TrailWeight;
	info->StuffWeight =g_pstJ939_CanInfo->StuffWeight;


	info->ErrCode = Jz_GetSystermErrCode();
	info->SUM = CheckSum((JZ_U8 *)info,J1939_HEART);

	return JZ_SUCCESS;
}

void Jz_J1939_Timer(void)
{
	//Jz_printf("\n Jz_J1939_Timer \r\n");
	//JzUartWrite("0123456789",strlen("0123456789"));
	if(++g_pstClearCount->TCO1_Speed_Count==10)
	{
		g_pstClearCount->TCO1_Speed_Count =0;
		g_pstJ939_CanInfo->Speed_Cred &=~0x01;
		g_pstJ939_CanInfo->CAR_Direction =0xff;
	}
	if(++g_pstClearCount->Wheel_Speed_Count==10)
	{
		g_pstClearCount->Wheel_Speed_Count =0;
		g_pstJ939_CanInfo->Speed_Cred &=~0x02;
	}

	if(++g_pstClearCount->EEC1_RPM_Count==10)
	{
		g_pstClearCount->EEC1_RPM_Count =0;
		g_pstJ939_CanInfo->EEC1_RPM =0xffff;
	}

	//100ms
	if(++g_pstClearCount->EBC1_Brake_Count==10)
	{
		g_pstClearCount->EBC1_Brake_Count =0;
		g_pstJ939_CanInfo->EBC1_Brake=0xff;
	}

	if(++g_pstClearCount->EEC2_Accelerate_Count==10)
	{
		g_pstClearCount->EEC2_Accelerate_Count =0;
		g_pstJ939_CanInfo->EEC2_Accelerate=0xff;
	}
	//请求时
	if(++g_pstClearCount->Driver_Count==30)
	{
		g_pstClearCount->Driver_Count =0;
		g_pstJ939_CanInfo->AllDisOfDriver=0xffffffff;
		g_pstJ939_CanInfo->DisOfDriver=0xffffffff;
	}
	//激活时1s；或状态改变
	if(++g_pstClearCount->Traval_Count==30)
	{
		g_pstClearCount->Traval_Count =0;
		g_pstJ939_CanInfo->AllDisOfTraval=0xffffffff;
		g_pstJ939_CanInfo->DisOfTraval=0xffffffff;
	}
	//请求时
	if(++g_pstClearCount->FuelOil_Count==30)
	{
		g_pstClearCount->FuelOil_Count =0;
		g_pstJ939_CanInfo->FuelOil=0xffffffff;
		g_pstJ939_CanInfo->AllFuelOil=0xffffffff;
	}

	if(++g_pstClearCount->Curvature_Count==10)
	{
		g_pstClearCount->Curvature_Count =0;
		g_pstJ939_CanInfo->Curvature =0xffff;
	}

	if(++g_pstClearCount->SensorDKSwitch_Count==10)
	{
		g_pstClearCount->SensorDKSwitch_Count =0;
		g_pstJ939_CanInfo->SensorDKSwitch =0xffff;
	}

	if(++g_pstClearCount->CompassDirection_Count==10)
	{
		g_pstClearCount->CompassDirection_Count =0;
		g_pstJ939_CanInfo->CompassDirection =0xffff;
	}
	if(++g_pstClearCount->Weight_Count==10)
	{
		g_pstClearCount->Weight_Count =0;
		g_pstJ939_CanInfo->StuffWeight =0xffff;
		g_pstJ939_CanInfo->AxleWeight =0xffff;
		g_pstJ939_CanInfo->TrailWeight =0xffff;
	}
}


void Jz_J1939_InitData(void)
{
	memset(&g_stClearCount,0,sizeof(g_stClearCount));
	memset(&g_stJ939_CanInfo,0xff,sizeof(g_stJ939_CanInfo));
	g_stJ939_CanInfo.Speed_Cred=0;
}
