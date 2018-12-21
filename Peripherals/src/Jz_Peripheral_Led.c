#include "Jz_Peripheral_Led.h"
#include "sys.h"

#define LED_ON() 		PCout(13)=0	
#define LED_OFF() 		PCout(13)=1	
#define LED()			PCout(13)	

#define LED0 			PAout(8)	// PA8
#define LED1 			PDout(2)	// PD2	

void Jz_Periphral_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PB.5 ¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //¸ù¾ÝÉè¶¨²ÎÊý³õÊ¼»¯GPIOB.5
	GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PB.5 Êä³ö¸ß
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 ¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //¸ù¾ÝÉè¶¨²ÎÊý³õÊ¼»¯GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PB.5 Êä³ö¸ß

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PE.5 ¶Ë¿ÚÅäÖÃ, ÍÆÍìÊä³ö
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //ÍÆÍìÊä³ö £¬IO¿ÚËÙ¶ÈÎª50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PE.5 Êä³ö¸ß 
}

void Jz_Periphral_LED_On(void)
{
	LED0=0;
	LED1=1;
	LED_ON();

}

void Jz_Periphral_LED_Off(void)
{
	LED_OFF();
	LED0=1;
	LED1=0;
}

