#ifndef _SYSTERM_ERRCODE_H
#define _SYSTERM_ERRCODE_H
#include "SystermDefine.h"

#define CAN_READ_FULL  0x01
#define CAN_PARSE_FULL 0x02
#define UART_SEND_FULL 0x04
#define UART_CMD_ERR   0x08
#define SYS_ERR		   0x10

void  Jz_SetSystermErrCode(JZ_U8 err); 
JZ_U8  Jz_GetSystermErrCode(void); 
#endif

