#ifndef _JZ_CANREAD_H_
#define _JZ_CANREAD_H_

#include "SystermDefine.h"
#include "JzParam.h"




typedef void (*CanReadCallBack)(CanRxMsg *msg);

JZ_S32 Jz_CanRead_Init(void);
JZ_S32 Jz_CanRead_Suspend(void);
JZ_S32 Jz_CanRead_Resume(void);
JZ_S32 Jz_CanRead_ResetCan(void);
JZ_S32 Jz_CanRead_SetCallBack(CanReadCallBack pCallBackFunc);
void Jz_CanRead_SendFrame(CanRxMsg *msg);
JZ_S32 Jz_CanRead_Join(void);

#endif
