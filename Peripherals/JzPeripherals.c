#include "Jz_Peripheral_Uart.h"
#include "Jz_Peripheral_Can.h"
#include "Jz_Peripheral_Led.h"

#include "usart.h"

#define  CAN_IRQPriority      0
#define  CAN_IRQSubPriority   1


#define  TIM4_IRQPriority     1
#define  TIM4_IRQSubPriority  2

#define  UART2_IRQPriority    2
#define  UART2_IRQSubPriority 3

#define  UART_IRQPriority     3
#define  UART_IRQSubPriority  3


void Jz_Periphral_Init(void)
{
	BAUDRATE baudrate;
	UART_Init(115200,UART_IRQPriority,UART_IRQSubPriority);
	Jz_Periphral_Can_GPIOConfig();//CAN¹Ü½Å³õÊ¼»¯ 
	Jz_Periphral_Can_NVICConfiguration(CAN_IRQPriority ,CAN_IRQSubPriority,ENABLE);

	baudrate.CAN_SJW=CAN_SJW_1tq;
	baudrate.CAN_BS1=CAN_BS1_3tq;
	baudrate.CAN_BS2=CAN_BS2_2tq;
	baudrate.CAN_Prescaler=CAN_250kbps;
	Jz_Periphral_Can_Init(baudrate);
	Jz_Periphral_USART2_Init(19200,UART2_IRQPriority,UART2_IRQSubPriority);
	Jz_Periphral_TIM4_Init(99,7199,TIM4_IRQPriority,TIM4_IRQSubPriority);		//10msÖÐ¶Ï
	Jz_Periphral_TIM4_Set(0);
#ifdef  LED_USER_EN
	Jz_Periphral_LED_Init();
#endif		
}

