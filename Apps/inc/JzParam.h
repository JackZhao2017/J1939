#ifndef _JZ_PARAM_H_
#define _JZ_PARAM_H_

#include "SystermDefine.h"

#define kMAX_FILTER     13
// #define kDEFAULT_FILTER 7
#define kDEFAULT_FILTER 6
#define kRESETFILTER    4

typedef struct 
{
	JZ_U8  KBPS;
	JZ_FILTER Default[kDEFAULT_FILTER];	
}JZCAN_PARAM;
JZ_VOID Jz_ParamInitFilter(JZ_VOID);
JZ_FILTER * Jz_ParamGetDefaultFilter(JZ_U8 *num);
JZ_FILTER *Jz_ParamGetResetFilter(JZ_U8 *num);
JZ_S32 Jz_ParamSetResetFilter(JZ_U32  sum,JZ_U32 index ,JZ_FILTER *pstfilter);
JZ_S32 Jz_ParamSetBaudrate(JZ_U8 kbps);
JZ_S32 Jz_ParamGetBaudrate(void);

#endif


