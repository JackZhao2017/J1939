#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	

#include "Jz_Peripheral_Uart.h"



__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 

#ifdef USART2_RX_EN   								  

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				
u16 USART2_RX_STA=0;  

void USART2_IRQHandler(void)
{
	u8 res;	 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	 
	  	res =USART_ReceiveData(USART2);		
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		
		{
			TIM_SetCounter(TIM4,0);        				 
			if(USART2_RX_STA==0)
				Jz_Periphral_TIM4_Set(1);	 	 
			USART2_RX_BUF[USART2_RX_STA++]=res;			 
		}else 
		{
			USART2_RX_STA|=1<<15;			
		} 
	}  											 
}   
 
void Jz_Periphral_USART2_Init(u32 bound,uint8_t irqPriority,uint8_t irqSubPriority)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

 	USART_DeInit(USART2);  
		 //USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  
	USART_Init(USART2, &USART_InitStructure); 
  

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  	
	Jz_Periphral_UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);
	USART_Cmd(USART2, ENABLE); 

#ifdef USART2_RX_EN		  	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=irqPriority ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = irqSubPriority;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	//TIM4_Init(99,7199);//10ms
	USART2_RX_STA=0;		
	Jz_Periphral_TIM4_Set(0);
#endif	 	

}

void Jz_Periphral_UART2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA_GetCurrDataCounter(DMA1_Channel7)!=0);	 
	Jz_Periphral_UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF)); 
}

typedef void (*SystemUartReadCallBack)(unsigned char  *msg ,int  len);
SystemUartReadCallBack g_SystemUartReadCallBackFunc=NULL;
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{	 			   
		USART2_RX_STA|=1<<15;	
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     
		Jz_Periphral_TIM4_Set(0);			 
		if(g_SystemUartReadCallBackFunc!=NULL)
		{
			g_SystemUartReadCallBackFunc(USART2_RX_BUF,USART2_RX_STA&0X3FFF);
		}
		USART2_RX_STA =0;
	}	    
}

void Jz_Periphral_TIM4_Set(u8 sta)
{
	if(sta)
	{      
		TIM_SetCounter(TIM4,0);
		TIM_Cmd(TIM4, ENABLE); 
	}else TIM_Cmd(TIM4, DISABLE);	   
}
	 
void Jz_Periphral_TIM4_Init(u16 arr,u16 psc,u8 Priority ,u8 SubPriority)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);     
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
	Jz_Periphral_TIM4_Set(0);
}
#endif		 
  
void Jz_Periphral_UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//Ê¹ÄÜDMA´«Êä
	DMA_DeInit(DMA_CHx);   //½«DMAµÄÍ¨µÀ1¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMAÍâÉèADC»ùµØÖ·
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMAÄÚ´æ»ùµØÖ·
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //Êý¾Ý´«Êä·½Ïò£¬´ÓÄÚ´æ¶ÁÈ¡·¢ËÍµ½ÍâÉè
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAÍ¨µÀµÄDMA»º´æµÄ´óÐ¡
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //ÍâÉèµØÖ·¼Ä´æÆ÷²»±ä
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //ÄÚ´æµØÖ·¼Ä´æÆ÷µÝÔö
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //Êý¾Ý¿í¶ÈÎª8Î»
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //Êý¾Ý¿í¶ÈÎª8Î»
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //¹¤×÷ÔÚÕý³£»º´æÄ£Ê½
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAÍ¨µÀ xÓµÓÐÖÐÓÅÏÈ¼¶ 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAÍ¨µÀxÃ»ÓÐÉèÖÃÎªÄÚ´æµ½ÄÚ´æ´«Êä
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //¸ù¾ÝDMA_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯DMAµÄÍ¨µÀUSART1_Tx_DMA_ChannelËù±êÊ¶µÄ¼Ä´æÆ÷	
} 
//¿ªÆôÒ»´ÎDMA´«Êä
void Jz_Periphral_UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //¹Ø±Õ Ö¸Ê¾µÄÍ¨µÀ        
	DMA_SetCurrDataCounter(DMA_CHx,len);//DMAÍ¨µÀµÄDMA»º´æµÄ´óÐ¡	
	DMA_Cmd(DMA_CHx, ENABLE);           //¿ªÆôDMA´«Êä
}	   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 
void Jz_Periphral_UART_SEND_DATA(char *buf,uint8_t len)
{
	memcpy(USART2_TX_BUF,buf,len); 
	Jz_Periphral_UART_DMA_Enable(DMA1_Channel7,len);		
	while(DMA_GetCurrDataCounter(DMA1_Channel7)!=0);	
}

