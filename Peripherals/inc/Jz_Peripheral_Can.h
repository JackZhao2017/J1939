#ifndef  _JZ_PERIPHERAL_CAN_H
#define  _JZ_PERIPHERAL_CAN_H


#include "includes.h"
#include "stm32f10x.h" 
#include <stdint.h>

#define STD 11
#define EXT 29

#define CAN_500kbps  12
#define CAN_250kbps  24
#define CAN_100kpbs  60

#define FILTER_NUM   6

typedef struct 
{
	uint8_t  CAN_SJW;
	uint8_t  CAN_BS1;
	uint8_t  CAN_BS2;
	uint8_t  CAN_Prescaler;
}BAUDRATE;


typedef struct{
		u32 CAN_ID;
		u32 CAN_ID_MASK;
		u8  CAN_ID_FMT;
}FILTER;

typedef void (*SystemCanReadCallBack)(CanRxMsg *msg);

int  Jz_Periphral_Can_NVICDisabled(void);
int  Jz_Periphral_Can_NVICEnabled(void);

void Jz_Periphral_Can_GPIOConfig(void);
void Jz_Periphral_Can_Init(BAUDRATE baudrate);
void Jz_Periphral_Can_FilterInit(FILTER *filter,int  len);
void Jz_Periphral_Can_NVICConfiguration(uint8_t Priority ,uint8_t SubPriority,FunctionalState newstate);

uint8_t Jz_Periphral_Can_GetErrorCode(void);

#endif

