#ifndef _JZ_PARSEUSER_H_
#define _JZ_PARSEUSER_H_

#include "SystermDefine.h"
#include "JzParam.h"

#define CANMSG   0x11
#define CMDIDMSG 0x15
#define IDMASKMSG 0x20


typedef struct 
{
	JZ_U8  KBPS;
	JZ_U8  SUM; 
	JZ_U16 ID[kRESETFILTER];
	JZ_U16 MARSK[kRESETFILTER];
}JZ_CMDMSG;

JZ_S32 Jz_ParseUser_DetectSync(JZ_U8 *buf,JZ_S32 len);
JZ_S32 Jz_ParseUser_DetectvaildMsgtype(JZ_U8 *buf,JZ_U8 *type);
JZ_S32 Jz_ParseUser_DetectSUM(JZ_U8 *msg, JZ_S32 len);
JZ_S32 Jz_ParseUser_ProcessCmd(JZ_U8 *msg);
JZ_S32 Jz_ParseUser_ProcessIDMask(JZ_U8 *msg);
#endif


