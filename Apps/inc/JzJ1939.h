#ifndef _JZ_J1939_H_
#define _JZ_J1939_H_

#include "SystermDefine.h"
#include "JzCanRead.h"


typedef struct 
{
	JZ_U8  SYNC;
	JZ_U8  TYPE;
	JZ_U8  COUNTER;
	JZ_U8  Speed; //0~251 km/h
	JZ_U16 Rpm;   //0~8100;
	JZ_U8  Brake; //0~250;
	JZ_U8  Accelerate;//0~250;
	JZ_U32 DisOfTraval;
	JZ_U32 AllDisOfTraval;
	JZ_U32 FuelOil;
	JZ_U32 AllFuelOil;
	JZ_U8  SUM;
}Jz_J1939_INFO;


JZ_U8 Jz_J1939_Parse(CanRxMsg *msg);

JZ_U32 Jz_J1939_GetCarInfomation(Jz_J1939_INFO *info);

#endif
