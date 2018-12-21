#ifndef  _JZ_PERIPHERAL_UART_H
#define  _JZ_PERIPHERAL_UART_H

#include "includes.h"
#include "stm32f10x.h" 
#include <stdint.h>

#define USART2_MAX_RECV_LEN		200					//×î´ó½ÓÊÕ»º´æ×Ö½ÚÊý
#define USART2_MAX_SEND_LEN		200					//×î´ó·¢ËÍ»º´æ×Ö½ÚÊý
#define USART2_RX_EN 			1					//0,²»½ÓÊÕ;1,½ÓÊÕ.

// extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//½ÓÊÕ»º³å,×î´óUSART2_MAX_RECV_LEN×Ö½Ú
// extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//·¢ËÍ»º³å,×î´óUSART2_MAX_SEND_LEN×Ö½Ú
// extern u16 USART2_RX_STA;   						//½ÓÊÕÊý¾Ý×´Ì¬



void Jz_Periphral_USART2_Init(u32 bound,uint8_t irqPriority,uint8_t irqSubPriority);
void Jz_Periphral_TIM4_Set(u8 sta);
void Jz_Periphral_TIM4_Init(u16 arr,u16 psc,u8 irqPriority ,u8 irqSubPriority);
void Jz_Periphral_UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
void Jz_Periphral_UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len);
void Jz_Periphral_UART2_printf(char* fmt, ...);
void Jz_Periphral_UART_SEND_DATA(char *buf,uint8_t len);
#endif

