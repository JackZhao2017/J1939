#ifndef _JZ_PERIPHERAL_TIMER_H
#define _JZ_PERIPHERAL_TIMER_H

typedef void (*TimerCallBack)(void);

void Jz_Periphral_TIM3_Set(u8 sta);
void Jz_Periphral_TIM3_Init(u16 arr,u16 psc,u8 irqPriority ,u8 irqSubPriority);
void Jz_Periphral_TIM3_SetCallback(TimerCallBack ptimercallback);

#endif

