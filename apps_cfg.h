#ifndef _APPS_CFG_H_
#define _APPS_CFG_H_


#define PRIO_BASE     4
#define QUEUE_MUTEX_CANPARSE_PRIO  	PRIO_BASE
#define QUEUE_MUTEX_UARTSEND_PRIO 	PRIO_BASE+1
#define RINGBUF_MUTEX_PRIO 	PRIO_BASE+2
#define UARTCAN_MUTEX_PRIO 	PRIO_BASE+3

#define CAN_READ_PRIO				  PRIO_BASE+4
#define CAN_PARSE_PRIO				  PRIO_BASE+5
#define UART_SEND_PRIO       		  PRIO_BASE+6
#define UART_READ_PRIO       		  PRIO_BASE+7 	//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define UART_PARSE_PRIO               PRIO_BASE+8


#define CAN_READ_STK_SIZE 			  128
#define CAN_PARSE_STK_SIZE			  96
#define UART_SEND_STK_SIZE  		  96 
#define UART_READ_STK_SIZE  		  128 	//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define UART_PARSE_STK_SIZE  		  128	//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡




#endif

