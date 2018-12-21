#ifndef _SYS_DEFINE_H_
#define _SYS_DEFINE_H_

#include <stdio.h>
#include <string.h>

//#define LINUX
// #define DEBUG
#ifdef LINUX
#include <pthread.h>
#include <semaphore.h>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#else

#include "includes.h"
#include "Jz_Peripheral_Uart.h"
#include "Jz_Peripheral_Can.h"
#include "Jz_Peripheral_Led.h"
#endif



#define JZ_U32 			    unsigned int 
#define JZ_S32          int
#define JZ_U8			      unsigned char
#define JZ_U16			    unsigned short  

#define Jz_printf(...)  printf(__VA_ARGS__)


#define JZ_STACK          unsigned int 


#ifdef LINUX

#define Jz_ThreadId  	  pthread_t 
#define Jz_Sem     		  sem_t     
#define Jz_Mutex   		  pthread_mutex_t 
#define JZ_VOID         void*
// #define JZ_MutexAttr    pthread_mutexattr_t 

// typedef struct 
// { 
//     char name[32];
//     pthread_attr_t *attr;
// }JZ_ThreadAttr;

#else

#define Jz_ThreadId     JZ_S32  
#define Jz_Sem          OS_EVENT*      
#define Jz_Mutex        OS_EVENT*  

#define JZ_VOID         void


#define  JZ_ENTER_CRITICAL();   OS_CPU_SR cpu_sr=0; OS_ENTER_CRITICAL(); 
#define  JZ_EXIT_CRITICAL();   OS_EXIT_CRITICAL()

#endif

typedef struct 
{ 
    char name[32];
    JZ_U32 *stktop;
    JZ_U32 *stkbot;
    JZ_S32 stksize;
    JZ_U16 ID;
    JZ_U8  prio;
}_JZ_ThreadAttr;

typedef struct _Mutex_Attr
{
    JZ_U8  prio;
}_JZ_MutexAttr;


#define Jz_ThreadAttr    _JZ_ThreadAttr
#define JZ_MutexAttr     _JZ_MutexAttr


#define JZ_SUCCESS 0
#define JZ_FAILD   -1

typedef enum _JZ_BOOL
{
	JZ_TRUE,
	JZ_FALSE	
}JZ_BOOL;


#ifdef LINUX 
typedef struct _JZ_FILTER
{
  JZ_U32  CAN_ID;
  JZ_U32  CAN_ID_MASK;
  JZ_U32  CAN_ID_FMT;
}JZ_FILTER;
typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} CanRxMsg;
#else

typedef  FILTER JZ_FILTER;

#endif

JZ_S32 JzThreadCreate(Jz_ThreadId *pid,Jz_ThreadAttr *attr, void *task,void *arg);
JZ_S32 JzThreadJoin(Jz_ThreadId pid);
JZ_S32 JzThreadExit(void);

JZ_S32 JzMutexInit(Jz_Mutex *mutex ,const char *name);
JZ_S32 JzMutexLock(Jz_Mutex *mutex);
JZ_S32 JzMutexUnlock(Jz_Mutex *mutex);
JZ_S32 JzMutexDestroy(Jz_Mutex *mutex);


JZ_S32 JzSemCreate(Jz_Sem *sem);
JZ_S32 JzSemPost(Jz_Sem *sem);
JZ_S32 JzSemWartfor(Jz_Sem *sem ,JZ_U32 timout);
JZ_S32 JzSemDestroy(Jz_Sem *sem);

JZ_S32 JzCanReset(JZ_S32 kbps,JZ_FILTER *pstfilter,JZ_S32 num);
JZ_S32 JzCanEnable(void);
JZ_S32 JzCanDisable(void);
JZ_U8 JzGetCanErrorCode(void);


typedef void (*SystemUartReadCallBack)(JZ_U8 *msg ,JZ_S32 len);
JZ_S32 JzSystemSetUartReadCallBack(SystemUartReadCallBack pcallback);
typedef void (*SystemCanReadCallBack)(CanRxMsg *msg);
JZ_S32 JzSystemSetCanReadCallBack(SystemCanReadCallBack pcallback);

JZ_S32 JzUartWrite(JZ_U8 *buf,JZ_S32 len);
JZ_S32 JzUartRead(JZ_U8 *buf,JZ_S32 len);


JZ_S32 JzLedOn(void);
JZ_S32 JzLedOff(void);
#endif
