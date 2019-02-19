
#include "Jz_Peripheral_Can.h"
#include "stdio.h"
#include "string.h"
#include "includes.h"

static uint8_t g_Priority ;
static uint8_t g_SubPriority ;

int Jz_Periphral_Can_NVICDisabled(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	OS_CPU_SR cpu_sr=0;	
	OS_ENTER_CRITICAL();
	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = g_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = g_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);	
	OS_EXIT_CRITICAL();
	return DISABLE;
}
int Jz_Periphral_Can_NVICEnabled(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	OS_CPU_SR cpu_sr=0;	
	OS_ENTER_CRITICAL();
	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = g_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = g_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	OS_EXIT_CRITICAL();
	return ENABLE;
}


void Jz_Periphral_Can_NVICConfiguration(uint8_t Priority ,uint8_t SubPriority,FunctionalState newstate)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  

	#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
	g_Priority = Priority;
	g_SubPriority = SubPriority;

	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = newstate;
	NVIC_Init(&NVIC_InitStructure);	
}
/*CAN GPIO ºÍÊ±ÖÓÅäÖÃ */
void Jz_Periphral_Can_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/* ¸´ÓÃ¹¦ÄÜºÍGPIOB¶Ë¿ÚÊ±ÖÓÊ¹ÄÜ*/	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);	                        											 

	/* CAN1 Ä£¿éÊ±ÖÓÊ¹ÄÜ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ÉÏÀ­ÊäÈë
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CAN pin: TX */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 ±¾ÊµÑéÃ»ÓÐÓÃµ½ÖØÓ³ÉäI/O
  // 
}

/*	CAN³õÊ¼»¯ */
void Jz_Periphral_Can_Init(BAUDRATE baudrate)
{
	CAN_InitTypeDef        CAN_InitStructure;

	/* CAN register init */
	CAN_DeInit(CAN1);	//½«ÍâÉèCANµÄÈ«²¿¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
	CAN_StructInit(&CAN_InitStructure);//°ÑCAN_InitStructÖÐµÄÃ¿Ò»¸ö²ÎÊý°´È±Ê¡ÖµÌîÈë

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;//Ã»ÓÐÊ¹ÄÜÊ±¼ä´¥·¢Ä£Ê½
	CAN_InitStructure.CAN_ABOM=DISABLE;//Ã»ÓÐÊ¹ÄÜ×Ô¶¯ÀëÏß¹ÜÀí
	CAN_InitStructure.CAN_AWUM=DISABLE;//Ã»ÓÐÊ¹ÄÜ×Ô¶¯»½ÐÑÄ£Ê½
	CAN_InitStructure.CAN_NART=DISABLE;//Ã»ÓÐÊ¹ÄÜ·Ç×Ô¶¯ÖØ´«Ä£Ê½
	CAN_InitStructure.CAN_RFLM=DISABLE;//Ã»ÓÐÊ¹ÄÜ½ÓÊÕFIFOËø¶¨Ä£Ê½
	CAN_InitStructure.CAN_TXFP=DISABLE;//Ã»ÓÐÊ¹ÄÜ·¢ËÍFIFOÓÅÏÈ¼¶
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//CANÉèÖÃÎªÕý³£Ä£Ê½
	CAN_InitStructure.CAN_SJW=baudrate.CAN_SJW; //ÖØÐÂÍ¬²½ÌøÔ¾¿í¶È1¸öÊ±¼äµ¥Î»
	CAN_InitStructure.CAN_BS1=baudrate.CAN_BS1; //Ê±¼ä¶Î1Îª3¸öÊ±¼äµ¥Î»
	CAN_InitStructure.CAN_BS2=baudrate.CAN_BS2; //Ê±¼ä¶Î2Îª2¸öÊ±¼äµ¥Î»
	CAN_InitStructure.CAN_Prescaler=baudrate.CAN_Prescaler;  //Ê±¼äµ¥Î»³¤¶ÈÎª60	
	CAN_Init(CAN1,&CAN_InitStructure);
	                                  //²¨ÌØÂÊÎª£º72M/2/60(1+3+2)=0.1 ¼´100K
	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); //Ê¹ÄÜFIFO0ÏûÏ¢¹ÒºÅÖÐ¶Ï
	printf("%s CAN_SJW :%d CAN_BS1:%d CAN_BS2 :%d  CAN_Prescaler:%d \r\n",__func__,baudrate.CAN_SJW,baudrate.CAN_BS1, baudrate.CAN_BS2,baudrate.CAN_Prescaler);
}  

 
void Jz_Periphral_Can_FilterInit(FILTER *filter,int  len)
{
	int i=0;
	unsigned int filterId;
	unsigned int filterMaskId;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure; 
  /* CAN filter init */
	for(i=0;i<len;i++){
		filterId=filter[i].CAN_ID<<(32-filter[i].CAN_ID_FMT);
		filterMaskId=filter[i].CAN_ID_MASK<<(32-filter[i].CAN_ID_FMT);
		CAN_FilterInitStructure.CAN_FilterNumber=i;//Ö¸¶¨¹ýÂËÆ÷Îª1
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//Ö¸¶¨¹ýÂËÆ÷Îª±êÊ¶·ûÆÁ±ÎÎ»Ä£Ê½
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//¹ýÂËÆ÷Î»¿íÎª32Î»
		CAN_FilterInitStructure.CAN_FilterIdHigh=(filterId>>16)&0xffff;// ¹ýÂËÆ÷±êÊ¶·ûµÄ¸ß16Î»Öµ
		if(filter[i].CAN_ID_FMT==EXT){
				CAN_FilterInitStructure.CAN_FilterIdLow=(filterId&0xffff)|CAN_ID_EXT;//	 ¹ýÂËÆ÷±êÊ¶·ûµÄµÍ16Î»Öµ
		}else{
				CAN_FilterInitStructure.CAN_FilterIdLow=filterId&0xffff;
		}
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(filterMaskId>>16)&0xffff;//¹ýÂËÆ÷ÆÁ±Î±êÊ¶·ûµÄ¸ß16Î»Öµ
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=(filterMaskId&0xffff)|0x6;//	¹ýÂËÆ÷ÆÁ±Î±êÊ¶·ûµÄµÍ16Î»Öµ
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;// Éè¶¨ÁËÖ¸Ïò¹ýÂËÆ÷µÄFIFOÎª0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;// Ê¹ÄÜ¹ýÂËÆ÷
		CAN_FilterInit(&CAN_FilterInitStructure);//	°´ÉÏÃæµÄ²ÎÊý³õÊ¼»¯¹ýÂËÆ÷
		// printf("idh 	=0x%4x idl 	= 0x%04x\r\n",CAN_FilterInitStructure.CAN_FilterIdHigh,CAN_FilterInitStructure.CAN_FilterIdLow);
		// printf("idMh 	=0x%4x idMl = 0x%04x\r\n",CAN_FilterInitStructure.CAN_FilterMaskIdHigh,CAN_FilterInitStructure.CAN_FilterMaskIdLow);
	}
}

volatile SystemCanReadCallBack g_SystemCanReadCallBackFunc=NULL;

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	OS_CPU_SR cpu_sr=0;	
	CanRxMsg RxMessage;	
	OS_ENTER_CRITICAL();
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
	OS_EXIT_CRITICAL();	 
	if(g_SystemCanReadCallBackFunc!=NULL){
		g_SystemCanReadCallBackFunc(&RxMessage);
	}
				
} 


uint8_t Jz_Periphral_Can_GetErrorCode(void)
{
	uint8_t error=0;
	error = CAN_GetLastErrorCode(CAN1);
	return error;
}

void Jz_Periphral_Can_Transmit(CanTxMsg *TxMessage)
{
	CanTxMsg Message;
	if(TxMessage->IDE==1){
		Message.StdId=TxMessage->StdId;	//±ê×¼±êÊ¶·ûÎª0x00
		Message.ExtId=TxMessage->ExtId; //À©Õ¹±êÊ¶·û0x0000
		Message.IDE=CAN_ID_EXT;//Ê¹ÓÃ±ê×¼±êÊ¶·û
		Message.RTR=TxMessage->RTR;//ÎªÊý¾ÝÖ¡
		Message.DLC=TxMessage->DLC;	//	ÏûÏ¢µÄÊý¾Ý³¤¶ÈÎª2¸ö×Ö½Ú
		memcpy(Message.Data,TxMessage->Data,TxMessage->DLC);
		CAN_Transmit(CAN1,&Message); //·¢ËÍÊý¾Ý
	}else if(TxMessage->IDE==0){
		Message.StdId=TxMessage->StdId;	//±ê×¼±êÊ¶·ûÎª0x00
		Message.ExtId=TxMessage->ExtId; //À©Õ¹±êÊ¶·û0x0000
		Message.IDE=CAN_ID_STD;//Ê¹ÓÃ±ê×¼±êÊ¶·û
		Message.RTR=TxMessage->RTR;//ÎªÊý¾ÝÖ¡
		Message.DLC=TxMessage->DLC;	//	ÏûÏ¢µÄÊý¾Ý³¤¶ÈÎª2¸ö×Ö½Ú
		memcpy(Message.Data,TxMessage->Data,TxMessage->DLC);
		CAN_Transmit(CAN1,&Message); //·¢ËÍÊý¾Ý
	}
}

