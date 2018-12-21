#ifndef _JZ_UARTRSEND_H_
#define _JZ_UARTRSEND_H_


#include "SystermDefine.h"


JZ_S32 Jz_UartSend_Init(void);
JZ_S32 Jz_UartSend_Join(void);
void Jz_UartSend_SendFrame(CanRxMsg *msg);


#endif

