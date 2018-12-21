#ifndef _JZ_UARTPARSE_H_
#define _JZ_UARTPARSE_H_



#include "SystermDefine.h"


JZ_S32 Jz_UartParse_Init(void);
JZ_S32 Jz_UartParse_Join(void);
void Jz_UartParse_SendFrame(JZ_U8 *buf, JZ_S32 len);


#endif

