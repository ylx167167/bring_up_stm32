/*
 *Filename         :frosLocal.c
 *Description      : freeRtos的实验 对于stm32 也可以用Stm32cubeMx生成freeRtos相关代码
 *Author           :wayneyao
 *Time             :2024/05/04 18:52:23*/
#include "frosLocal.h"
#include "stdio.h"

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "event_groups.h"
#include "timers.h"

#include "main.h"
#include "queue.h"
#include "task.h"

#include "canlocal.h"
/* Private function prototypes -----------------------------------------------*/

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTaskCAN(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
osThreadId_t g_defaultId;
osStatus_t g_defaultState;
uint16_t tempId = 0;
uint16_t tempId3 = 0;
uint16_t defaulttemp = 0;

QueueHandle_t g_queue1;
uint16_t g_recqueue1;

EventGroupHandle_t g_eventGroup; // 事件句柄

osTimerId_t g_softtimer; // 软件定时器
uint16_t g_softtemp = 0;
const osTimerAttr_t LedBlink_attributes = {.name = "softtimer"};
/* CallbackBlink function */
void softTimerHandler(void *argument)
{
    g_softtemp++;
}
/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /*! 初始化队列*/
    g_queue1 = xQueueCreate(10, sizeof(uint16_t));

    /*! 初始化事件组*/
    g_eventGroup = xEventGroupCreate();

    /*! 创建软件定时器 */
    g_softtimer = osTimerNew(softTimerHandler, osTimerPeriodic, NULL, &LedBlink_attributes);
    osTimerStart(g_softtimer, 500);
    /*! 初始化三个队列 */
    /* Definitions for defaultTask */
    osThreadId_t defaultTaskHandle;
    const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask", .priority = (osPriority_t)osPriorityNormal, .stack_size = 128 * 4};
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* Definitions for myTask02 */
    osThreadId_t myTask02Handle;
    const osThreadAttr_t myTask02_attributes = {
        .name = "myTask02", .priority = (osPriority_t)osPriorityLow, .stack_size = 128 * 4};
    /* creation of myTask02 */
    myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

    /* Definitions for myTask03 */
    osThreadId_t myTask03Handle;
    const osThreadAttr_t myTask03_attributes = {
        .name = "myTask03", .priority = (osPriority_t)osPriorityLow, .stack_size = 128 * 4};
    /* creation of myTask03 */
    myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

    /* Definitions for CanTask */
    osThreadId_t myTaskcanHandle;
    const osThreadAttr_t myCan04_attributes = {
        .name = "myCan04", .priority = (osPriority_t)osPriorityLow, .stack_size = 128 * 4};
    /* creation of myCanTask04 */
    myTaskcanHandle = osThreadNew(StartTaskCAN, NULL, &myCan04_attributes);
}

/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
void StartDefaultTask(void *argument)
{
    g_defaultId = osThreadGetId();
    uint16_t _temp = 1000;
    osThreadSuspend(g_defaultId);
    defaulttemp = 10;
    for (;;)
    {

        xQueueSend(g_queue1, &_temp, portMAX_DELAY);
        // 阻塞等待
        if (xEventGroupWaitBits(g_eventGroup, (1 << 0) || (1 << 1), pdTRUE, pdTRUE, portMAX_DELAY))
        {
            defaulttemp = 0;
        }
        osDelay(20);
    }
    /* USER CODE END StartDefaultTask */
}

/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
void StartTask02(void *argument)
{
    for (;;)
    {

        g_defaultState = osThreadGetState(g_defaultId);
        switch (tempId)
        {
        case 10:
            osThreadResume(g_defaultId); // 恢复Task
            // 如果在这里接受，则只接收一次，此后队列由于再次写入数值，DefaultTask直接进入阻塞
            xQueueReceive(g_queue1, &g_recqueue1, portMAX_DELAY);

            break;
        case 20:
            // osThreadTerminate(g_defaultId); // 删除Task
            // 20计数已到 事件通知
            xEventGroupSetBits(g_eventGroup, 1 << 1);
            break;
        default:
            break;
        }
        tempId++;
        // 如果在这里接收则 一开始队列里面没有数据就会导致程序直接阻塞
        //  xQueueReceive(g_queue1, &g_recqueue1, portMAX_DELAY);
        osDelay(300);
    }
}

/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */

void StartTask03(void *argument)
{
    for (;;)
    {
        tempId3++;
        if (tempId3 == 30)
        {
            // 30计数已到 事件通知
            xEventGroupSetBits(g_eventGroup, 1 << 0);
        }
        osDelay(300);
    }
}

void StartTaskCAN(void *argument)
{
    while (1)
        Exp_LocalCan_send();
}