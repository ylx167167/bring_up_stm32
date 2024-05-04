/*
 *Filename         :frosLocal.c
 *Description      : freeRtos的实验 对于stm32 也可以用Stm32cubeMx生成freeRtos相关代码
 *Author           :wayneyao
 *Time             :2024/05/04 18:52:23*/
#include "frosLocal.h"
#include "stdio.h"

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private function prototypes -----------------------------------------------*/

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
}

/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(100);
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
    /* USER CODE BEGIN StartTask02 */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartTask02 */
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
        osDelay(1);
    }
}
