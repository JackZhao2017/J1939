#ifndef _JZ_UARTREAD_H_
#define _JZ_UARTREAD_H_

#include "SystermDefine.h"



typedef void (*UartReadCallBack)(JZ_U8 *msg,JZ_S32 len);

JZ_S32 Jz_UartRead_Init(void);
JZ_S32 Jz_UartRead_Suspend(void);
JZ_S32 Jz_UartRead_Resume(void);
JZ_S32 Jz_UartRead_SetCallBack(UartReadCallBack pCallBackFunc);
void Jz_UartRead_SendFrame(JZ_U8  *msg,JZ_S32 len);
JZ_S32 Jz_UartRead_Join(void);

#endif
