/*
*********************************************************************************************************
*
*	模块名称 :头文件库
*	文件名称 : 
*	版    本 : V1.0
*	说    明 : 
*						
*	修改记录 :
*		版本号     日期        作者      
*		V1.0    2025-05-14  shisanxiang1
*
*********************************************************************************************************
*/
#ifndef __INCLUDES_H__
#define __INCLUDES_H__


/*  标准库 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*  hal库生成 */
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "usart.h"
#include "gpio.h"

/*  私有库 */
#include "key.h"
#include "MyRTOS.h"
#include "tcp_server.h"


/*  freertos */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"

/*  宏定义 */
#define usart_printf_open   /*  串口打印宏 */



#define BIT_0	(1 << 0)			/* 用于事件标志组  */
#define BIT_1	(1 << 1)
#define BIT_ALL (BIT_0 | BIT_1)


#endif
