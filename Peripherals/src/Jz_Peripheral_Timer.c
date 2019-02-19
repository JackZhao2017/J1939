
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	

#include "Jz_Peripheral_Timer.h"

volatile TimerCallBack g_TimerCallback=NULL;

void Jz_Periphral_TIM3_SetCallback(TimerCallBack ptimercallback)
{
	g_TimerCallback = ptimercallback;
}

void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{	 			   	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     
		Jz_Periphral_TIM3_Set(1);			 
		if(g_TimerCallback!=NULL)
		{
			g_TimerCallback();
		}
	}	    
}

void Jz_Periphral_TIM3_Set(u8 sta)
{
	if(sta)
	{      
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3, ENABLE); 
	}else TIM_Cmd(TIM3, DISABLE);	   
}
	 
void Jz_Periphral_TIM3_Init(u16 arr,u16 psc,u8 Priority ,u8 SubPriority)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);     


	TIM_TimeBaseStructure.TIM_Period = arr; 	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 

			
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
	Jz_Periphral_TIM3_Set(0);
}


