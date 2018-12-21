#ifndef _JZ_CANPARSE_H_
#define _JZ_CANPARSE_H_

#include "SystermDefine.h"


typedef void (*CanParseCallBack)(CanRxMsg *msg);

JZ_S32 Jz_CanParse_Init(void);
JZ_S32 Jz_CanParse_Join(void);
void Jz_CanParse_SendFrame(CanRxMsg *msg);
JZ_S32 Jz_CanParse_SetCallBack(CanParseCallBack pCallBackFunc);

#endif


