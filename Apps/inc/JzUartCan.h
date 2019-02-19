#ifndef _JZ_UARTCAN_H_
#define _JZ_UARTCAN_H_

#include "SystermDefine.h"


typedef struct {
	JZ_U8   SYNC;
	JZ_U8   TYPE;
	JZ_U8   COUNTER;
	JZ_U8   IDE_RTR;
	JZ_U8   ID[4];  
	JZ_U8   Data[8];
	JZ_U8   CRC8;
}JZ_UartTxMsg;

JZ_S32 Jz_Can2Uart(CanRxMsg *canmsg,JZ_UartTxMsg *uartmsg);
JZ_U8  Jz_SetCanUartSendId(JZ_U32 num,JZ_U16 *id);
JZ_U8  Jz_IsCanUartSend(JZ_U32 id);
JZ_S32 Jz_Can2UartInit(void);



#endif

