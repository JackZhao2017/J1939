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
	JZ_U32 DisOfDriver ;//行车距离;
	JZ_U32 AllDisOfDriver ;
	JZ_U32 FuelOil;
	JZ_U32 AllFuelOil;
	
	JZ_U16 Curvature;
	JZ_U16 SensorDKSwitch;

	JZ_U16 CompassDirection;
	JZ_U16 AxleWeight;
	JZ_U16 TrailWeight; 
	JZ_U16 StuffWeight;

	JZ_U8  Direction; 
	JZ_U8  ErrCode;
	JZ_U8  Reserved;
	JZ_U8  SUM;
}Jz_J1939_INFO;

void Jz_J1939_InitData(void);
JZ_U8 Jz_J1939_Parse(CanRxMsg *msg);
void Jz_J1939_Timer(void);
JZ_U32 Jz_J1939_GetCarInfomation(Jz_J1939_INFO *info);

#endif
