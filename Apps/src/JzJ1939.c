
#include "JzJ1939.h"

#define TRAVEL_PGN   0xfec1
#define OIL_PGN		 0xfee9
#define TCO1_PGN 	 0xfe6c
#define CARSPEED_PGN 0xfef1

#define ERC1_PGN	 0xf000
#define EBC1_PGN     0xf001  //电子闸控制器 ,2字节刹车踏板位置
#define EEC2_PGN     0xf003	 //电子发动机控制器,2字节 AP加速踏板位置
#define EEC1_PGN	 0xf004  //发送机转速




static JZ_U8 g_J1939_TCO1_Speed_Cred =0;
static JZ_U8 g_J1939_TCO1_Speed  = 0xff;

static JZ_U8 g_J1939_Wheel_Speed_Cred =0;
static JZ_U8 g_J1939_Wheel_Speed = 0xff;

static JZ_U8 g_J1939_EBC1_Brake = 0xff;//电子闸控制器 ,2字节刹车踏板位置

static JZ_U8 g_J1939_EEC2_Accelerate = 0xff;//电子发动机控制器,2字节 AP加速踏板位置

static JZ_U16 g_J1939_EEC1_RPM = 0xffff;//发送机转速

static JZ_U32 g_J939_DisOfTraval    = 0xffffffff;//里程;
static JZ_U32 g_J939_AllDisOfTraval = 0xffffffff;

static JZ_U32 g_J939_FuelOil    = 0xffffffff;//油耗;
static JZ_U32 g_J939_AllFuelOil = 0xffffffff;



static JZ_U32 J1939_Get_ParameterGroup(JZ_U32 ExtId)
{
	return ((ExtId>>8)&0xffff);
}

static void J1939_Parse_TCO1(CanRxMsg *msg)//基于转速计的速度
{
	g_J1939_TCO1_Speed_Cred =1;
	g_J1939_TCO1_Speed = msg->Data[7];
}

static void J1939_Parse_CARWheel(CanRxMsg *msg)//基于车轮的速度
{
	g_J1939_Wheel_Speed_Cred = 1;
	g_J1939_Wheel_Speed = msg->Data[2];
}
static void J1939_Parse_ERC1(CanRxMsg *msg)
{

}
static void J1939_Parse_EBC1(CanRxMsg *msg) //电子闸控制器 ,2字节刹车踏板位置
{
	g_J1939_EBC1_Brake = msg->Data[1];
}

static void J1939_Parse_EEC2(CanRxMsg *msg)//电子发动机控制器,2字节 AP加速踏板位置
{
	g_J1939_EEC2_Accelerate = msg->Data[1];
}

static void J1939_Parse_EEC1(CanRxMsg *msg)//发送机转速
{
	g_J1939_EEC1_RPM = msg->Data[3]+ (msg->Data[4]<<8);
	//Jz_printf("\n g_J1939_EEC1_RPM  %d  \n",g_J1939_EEC1_RPM);
}

static void J1939_Parse_FuelOil(CanRxMsg *msg)//油耗;
{
	JZ_U32 val = msg->Data[4]+(msg->Data[5]<<8)+(msg->Data[6]<<16)+(msg->Data[7]<<24);
	g_J939_AllFuelOil = val>>2;
	val =msg->Data[0]+(msg->Data[1]<<8)+(msg->Data[2]<<16)+(msg->Data[3]<<24);
	g_J939_FuelOil = val>>2;
	//Jz_printf("\n J1939_Parse_FuelOil  %d g_J939_AllFuelOil %d \n",g_J939_FuelOil,g_J939_AllFuelOil);
}


static void J1939_Parse_DisOfTravel(CanRxMsg *msg)//里程;
{
	JZ_U32 val = msg->Data[4]+(msg->Data[5]<<8)+(msg->Data[6]<<16)+(msg->Data[7]<<24);
	g_J939_DisOfTraval = val/200;
	val =msg->Data[0]+(msg->Data[1]<<8)+(msg->Data[2]<<16)+(msg->Data[3]<<24);
	g_J939_AllDisOfTraval = val/200;
	//Jz_printf("\n g_J939_DisOfTraval  %d g_J939_AllDisOfTraval %d \n",g_J939_DisOfTraval,g_J939_AllDisOfTraval);
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
		default:
			ret =1;
			break;
	}
	return ret;
}

#define J1939_SYNC  0x55
#define J1939_HEART 0x18

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

	if(g_J1939_TCO1_Speed_Cred)
	{
		info->Speed =g_J1939_TCO1_Speed;
	}else if(g_J1939_Wheel_Speed_Cred){
		info->Speed = g_J1939_Wheel_Speed;
	}else{
		info->Speed =0xff;
	}
	
	info->Rpm =g_J1939_EEC1_RPM;
	info->Brake = g_J1939_EBC1_Brake;
	info->Accelerate = g_J1939_EEC2_Accelerate;
	info->DisOfTraval = g_J939_DisOfTraval;
	info->AllDisOfTraval = g_J939_AllDisOfTraval;
	info->FuelOil =g_J939_FuelOil;
	info->AllFuelOil =g_J939_AllFuelOil;
	info->SUM = CheckSum((JZ_U8 *)info,J1939_HEART);

	return JZ_SUCCESS;
}


