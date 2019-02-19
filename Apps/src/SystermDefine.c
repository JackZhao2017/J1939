#include "SystermDefine.h"

#ifndef LINUX

#include "Jz_Peripheral_Uart.h"
#include "Jz_Peripheral_Can.h"
#include "Jz_Peripheral_Led.h"

#endif





#ifdef LINUX
JZ_S32 JzThreadCreate(Jz_ThreadId *pid,Jz_ThreadAttr *attr, void *task,void *arg)
#else
JZ_S32 JzThreadCreate(Jz_ThreadId *pid,Jz_ThreadAttr *attr, void (*task)(void *arg),void *arg )
#endif
{
	
	extern JZ_S32 Jz_SystermGetThreadInfoByName(const char *name ,Jz_ThreadAttr *attr);

	if(Jz_SystermGetThreadInfoByName(attr->name,attr)<0)
	{
		return JZ_FAILD;
	}
	
#ifdef LINUX
	JZ_S32 ret  = JZ_SUCCESS;

	ret = pthread_create(pid,NULL,task,arg);
	if(ret <0)
	{
		return JZ_FAILD;
	}
#else

#ifdef DEBUG
	Jz_printf("%s \r\n",__func__);
#endif
	JZ_S32 ret =JZ_SUCCESS;
	ret = OSTaskCreateExt(task,
					arg,
					attr->stktop,
					attr->prio,
					attr->ID,
					attr->stkbot,
					attr->stksize,
					(void *)0,
					OS_TASK_OPT_STK_CHK
	);
	if(ret!=0)
	{
		return JZ_FAILD;
	} 

#endif	
	return JZ_SUCCESS;
}

JZ_S32 JzThreadExit(void)
{
#ifdef LINUX
	pthread_exit(NULL);
#else
	return 0;
#endif
}

JZ_S32 JzThreadJoin(Jz_ThreadId pid)
{
#ifdef LINUX
	return pthread_join(pid, NULL);
#endif
	return 0;
}

JZ_S32 JzMutexInit(Jz_Mutex *mutex ,const char  *name )
{
	JZ_MutexAttr attr;
	extern JZ_S32 Jz_SystermGetMutexAttrByName(const char *name ,JZ_MutexAttr *attr);
	Jz_SystermGetMutexAttrByName(name ,&attr);
#ifdef LINUX
	JZ_S32 ret = pthread_mutex_init(mutex, NULL);
	if(ret <0)
	{
		return JZ_FAILD;
	}
#else
	JZ_U8 err;
	mutex->mutex = OSMutexCreate(attr.prio,&err);
	if(mutex->mutex==0)
	{
		err_printf("%s  err %d \r\n",__func__,err);
		return JZ_FAILD;
	}
#endif
	return JZ_SUCCESS;
}

JZ_S32 JzMutexLock(Jz_Mutex *mutex)
{
#ifdef LINUX
	return pthread_mutex_lock(mutex);
#else
	JZ_U8 err;
	OSMutexPend(mutex->mutex,0,&err);
	return err;
#endif
}
JZ_S32 JzMutexUnlock(Jz_Mutex *mutex)
{
#ifdef LINUX
	return pthread_mutex_unlock(mutex);
#else
	return OSMutexPost(mutex->mutex);
#endif
}
JZ_S32 JzMutexDestroy(Jz_Mutex *mutex)
{
#ifdef LINUX
	return pthread_mutex_destroy(mutex);
#endif
	return 0;
}


JZ_S32 JzSemCreate(Jz_Sem *sem)
{
#ifdef LINUX
	sem->count =0;
	JZ_S32 ret = sem_init(&sem->sem,0,0);
	if(ret <0)
	{
		return JZ_FAILD;
	}	
#else
	sem->sem= OSSemCreate(0);
	if(sem->sem==0)
	{
		return JZ_FAILD;
	}
	if(sem->sem->OSEventType!=OS_EVENT_TYPE_SEM)
	{
		err_printf("%s  type %d \r\n",__func__,sem->sem->OSEventType);
		return JZ_FAILD;
	}
#ifdef DEBUG
		Jz_printf("%s  type %d \r\n",__func__,sem->sem->OSEventType);
#endif
#endif
	return  JZ_SUCCESS;
}

JZ_S32 JzSemPost(Jz_Sem *sem)
{
#ifdef LINUX 
	sem->count = 0;
	return  sem_post(&sem->sem);
#else
	OSSemPost(sem->sem);
#endif
	return 0;
}

JZ_S32 JzSemWartfor(Jz_Sem *sem ,JZ_U32 timout)
{
#ifdef LINUX 
	sem->count = timout;
	if(timout){
		while(sem->count>1)
		{	
			sem->count--;
			usleep(1000);
		}
	}else{
		return sem_wait(&sem->sem);
	}
	return 0;
#else
	JZ_U8 err;
	OSSemPend(sem->sem,timout,&err);	
#ifdef DEBUG
	Jz_printf("%s  timout %d err %d  type %d \r\n",__func__,timout,err,sem->sem->OSEventType);
#endif
	return err;
#endif
}

JZ_S32 JzSemDestroy(Jz_Sem *sem)
{
#ifdef LINUX 
	return sem_destroy(&sem->sem);
#else
	return 0;
#endif
}

JZ_S32 JzCanSetFilter(JZ_FILTER *pstfilter,JZ_S32 num)
{
#ifndef LINUX
	Jz_Periphral_Can_FilterInit(pstfilter,num);
#endif
	return JZ_SUCCESS;
}


JZ_S32 JzCanReset(JZ_S32 kbps,JZ_FILTER *pstfilter,JZ_S32 num)
{
#ifndef LINUX	
	BAUDRATE baudrate ;
	baudrate.CAN_SJW=CAN_SJW_1tq;
	baudrate.CAN_BS1=CAN_BS1_3tq;
	baudrate.CAN_BS2=CAN_BS2_2tq;
	baudrate.CAN_Prescaler=kbps;
	JZ_ENTER_CRITICAL();
	Jz_Periphral_Can_Init(baudrate);
	Jz_Periphral_Can_FilterInit(pstfilter,num);
	JZ_EXIT_CRITICAL(); 
#endif	
	return JZ_SUCCESS;
}

JZ_U8 JzGetCanErrorCode(void)
{
#ifdef LINUX
	return 0;
#else
	return Jz_Periphral_Can_GetErrorCode();
#endif
}

JZ_S32 JzCanEnable(void)
{
	Jz_printf("%s \n",__func__);
#ifndef LINUX	
 	return Jz_Periphral_Can_NVICEnabled();
#else
 	return 1;
#endif
}

JZ_S32 JzCanDisable(void)
{
	Jz_printf("%s \n",__func__);
#ifndef LINUX
	return Jz_Periphral_Can_NVICDisabled();
#else
	return 0;
#endif
}
JZ_S32 JzLedOn(void)
{
#ifndef LINUX
	Jz_Periphral_LED_On();
#else
	Jz_printf("%s \n",__func__);
#endif
	return 0;
}
JZ_S32 JzLedOff(void)
{
#ifndef LINUX
	Jz_Periphral_LED_Off();
#else
	Jz_printf("%s \n",__func__);
#endif
	return 0;
}

JZ_S32 JzSystemSetUartReadCallBack(SystemUartReadCallBack pcallback)
{
	extern SystemUartReadCallBack g_SystemUartReadCallBackFunc;
	g_SystemUartReadCallBackFunc = pcallback;
	return 0;
}

JZ_S32 JzSystemSetCanReadCallBack(SystemCanReadCallBack pcallback)
{
	extern SystemCanReadCallBack g_SystemCanReadCallBackFunc;
	g_SystemCanReadCallBackFunc = pcallback;
	return 0;
}
JZ_S32 JzSystemSetTimerCallBack(TimerCallBack ptimercallback)
{
	extern volatile TimerCallBack g_TimerCallback;
	g_TimerCallback = ptimercallback;
	return 0;
}

JZ_S32 JzUartWrite(JZ_S8 *buf,JZ_S32 len)
{
#ifdef LINUX
	JZ_S32 i;
	Jz_printf("%s \r\n",__func__);
	for (i = 0; i < len; ++i)
	{
		Jz_printf("0x%02x ",(JZ_U8)buf[i]);
	}
	if(len>0){
		Jz_printf("\r\n");
	}
#else 
	Jz_Periphral_UART_SEND_DATA(buf,len);
#endif
	return 0;
}

