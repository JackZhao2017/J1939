#ifndef _JZ_PARSEUSER_H_
#define _JZ_PARSEUSER_H_

#include "SystermDefine.h"

#define CANMSG   0x11
#define CMDIDMSG 0x0f


typedef struct 
{
	JZ_U8  STATE;
	JZ_U8  INDEX;
	JZ_U8  KBPS;
	JZ_U8  IDE; 
	JZ_U32 ID;
	JZ_U32 MARSK;
}JZ_CMDMSG;

JZ_S32 Jz_ParseUser_DetectSync(JZ_U8 *buf,JZ_S32 len);
JZ_S32 Jz_ParseUser_DetectvaildMsgtype(JZ_U8 *buf,JZ_U8 *type);
JZ_S32 Jz_ParseUser_DetectSUM(JZ_U8 *msg, JZ_S32 len);
JZ_S32 Jz_ParseUser_Process(JZ_U8 *msg);
#endif


