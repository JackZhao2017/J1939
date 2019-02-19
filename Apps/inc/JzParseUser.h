#ifndef _JZ_PARSEUSER_H_
#define _JZ_PARSEUSER_H_

#include "SystermDefine.h"

#define CANMSG   0x11
#define CMDIDMSG 0x0c
#define IDMASKMSG 0x20

#define ID_MASK_NUM 4

typedef struct 
{
	JZ_U8  KBPS;
	JZ_U8  SUM; 
	JZ_U16 ID[ID_MASK_NUM];
	JZ_U16 MARSK[ID_MASK_NUM];
}JZ_CMDMSG;

JZ_S32 Jz_ParseUser_DetectSync(JZ_U8 *buf,JZ_S32 len);
JZ_S32 Jz_ParseUser_DetectvaildMsgtype(JZ_U8 *buf,JZ_U8 *type);
JZ_S32 Jz_ParseUser_DetectSUM(JZ_U8 *msg, JZ_S32 len);
JZ_S32 Jz_ParseUser_ProcessCmd(JZ_U8 *msg);
JZ_S32 Jz_ParseUser_ProcessIDMask(JZ_U8 *msg);
#endif


