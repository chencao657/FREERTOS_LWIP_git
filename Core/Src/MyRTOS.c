#include "MyRTOS.h"

/*  任务句柄 */
static TaskHandle_t xHandleTaskLED1 = NULL;
static TaskHandle_t xHandleTaskLED2 = NULL;
static TaskHandle_t xHandleTaskkey = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskTCP = NULL;
/*  事件标志组句柄 */
static EventGroupHandle_t xCreatedEventGroup = NULL;


/* 任务函数声明  */
static void vTaskLED1(void *pvParameters);
static void vTaskLED2(void *pvParameters);
static void vTaskkey(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskTCP(void *pvParameters);
/*  任务创建函数 */
void AppTaskCreate (void);

/*  任务通信创建函数 */
void AppObjCreate (void);


/*
*********************************************************************************************************
*	函 数 名: LED1任务
*	功能说明:
*	形    参: 
*********************************************************************************************************
*/
static void vTaskLED1(void *pvParameters)
{
    while(1)
    {
			
		  HAL_GPIO_TogglePin(GPIOA, led1_Pin);
			printf("led1\r\n");
		  vTaskDelay(2000);
			
	  }
}
/*
*********************************************************************************************************
*	函 数 名:LED2任务 
*	功能说明:
*	形    参: 
*********************************************************************************************************
*/
static void vTaskLED2(void *pvParameters)
{

    while(1)
    {
			
			HAL_GPIO_TogglePin(GPIOA, led2_Pin);   
			 vTaskDelay(500);
    }
}

/*
*********************************************************************************************************
*	函 数 名: 按键扫描任务
*	功能说明:
*	形    参: 
*********************************************************************************************************
*/
static void vTaskkey(void *pvParameters)
{
		uint8_t key;
		EventBits_t uxBits;
    while(1)
    {
		// 处理按键事件
    
    while ((key = bsp_GetKey()) != KEY_NONE)
			{
        switch (key)
					{
						case KEY_1_DOWN:			/* K1键按下 */
							printf("K1键按下\r\n");
							
							break;

						case KEY_2_DOWN:				/* K2按下 */
							printf("K2键按下\r\n");
							uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_0);
							if((uxBits & BIT_0) != 0)
								{
									printf("K2键按下，事件标志的bit0被设置 \r\n");
								}
								else
								{
									printf("K2键按下，事件标志的bit0被清除，说明任务vTaskMsgPro已经接受到bit0和bit1被设置的情况 \r\n");
								}
							break;

						case KEY_3_DOWN:			/* K3键按下 */
							printf("K3键按下\r\n");
							uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_1);
							if((uxBits & BIT_1) != 0)
								{
									printf("K3键按下，事件标志的bit1被设置 \r\n");
								}
								else
								{
									printf("K3键按下，事件标志的bit1被清除，说明任务vTaskMsgPro已经接受到bit0和bit1被设置的情况 \r\n");
								}
							break;

						case KEY_4_DOWN:				/* K4键按下 */
							printf("K4键按下\r\n");
						
							break;
						
						default:
							/* 其它的键值不处理 */
							break;
					}
			}
			vTaskDelay(20);
    }
}
/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 消息处理，使用函数xEventGroupWaitBits接收任务vTaskTaskUserIF发送的事件标志
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
	EventBits_t uxBits;
	const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大延迟100ms */
	
    while(1)
    {
		/* 等K2按键按下设置bit0和K3按键按下设置bit1 */
		uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* 事件标志组句柄 */
							         BIT_ALL,            /* 等待bit0和bit1被设置 */
							         pdTRUE,             /* 退出前bit0和bit1被清除，这里是bit0和bit1都被设置才表示“退出”*/
							         pdTRUE,             /* 设置为pdTRUE表示等待bit1和bit0都被设置*/
							         xTicksToWait); 	 /* 等待延迟时间 */
		
		if((uxBits & BIT_ALL) == BIT_ALL)
		{
			/* 接收到bit1和bit0都被设置的消息 */
			printf("接收到bit0和bit1都被设置的消息\r\n");
			HAL_GPIO_TogglePin(GPIOA, led2_Pin);
		}
		else
		{
			/* 超时，另外注意仅接收到一个按键按下的消息时，变量uxBits的相应bit也是被设置的 */
		}
    }
		//vTaskDelay(20);
}
static void vTaskTCP(void *pvParameters)
{
		MX_LWIP_Init();
    while(1)
    {
			
			 vTCPServer_Task();
			 vTaskDelay(1);
    }
}
/*
*********************************************************************************************************
*	函 数 名: 创建任务应用
*	功能说明:
*	形    参: 
*********************************************************************************************************
*/
void AppTaskCreate (void)
{
    xTaskCreate( vTaskLED1,   	/* 任务函数  */
                 "vTaskLED1",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskLED1 );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED2,    		/* 任务函数  */
                 "vTaskLED2",  		/* 任务名    */
                 512,         		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED2 ); /* 任务句柄  */
	
	xTaskCreate( vTaskkey,    		/* 任务函数  */
                 "vTaskkey",  		/* 任务名    */
                 512,         		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 3,           		/* 任务优先级*/
                 &xHandleTaskkey ); /* 任务句柄  */
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	xTaskCreate( vTaskTCP,     		/* 任务函数  */
                 "vTaskTCP",   		/* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 4,               		/* 任务优先级*/
                 &xHandleTaskTCP );  /* 任务句柄  */
}
/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppObjCreate (void)
{
	/* 创建事件标志组 */
	xCreatedEventGroup = xEventGroupCreate();
	
	if(xCreatedEventGroup == NULL)
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
}