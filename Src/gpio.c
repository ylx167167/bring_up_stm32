/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "custom/syscall.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 2 */
/**
 *  @b Description 设置不同引脚状�??
 *  @n pin bit
 *  @retval
 */
void Set_GPIO_Bit(uint32_t pin, uint32_t bit)
{
  uint16_t _pin = 0;
  switch (pin)
  {
  case 4:
    _pin = GPIO_PIN_4;
  case 13:
    _pin = GPIO_PIN_13;
    break;
    break;
  default:
    return;
  }
  bit == 0 ? HAL_GPIO_WritePin(GPIOC, _pin, GPIO_PIN_RESET) : HAL_GPIO_WritePin(GPIOC, _pin, GPIO_PIN_SET);
}
/**
 *  @b Description PB3 中断回调
 *  @n
 *  @retval
 */
void KEY_LEFT_CallBack()
{
}

/**
 *  @b Description HAL_GPIO_EXTI_Callback
 *  @n
 *  @retval
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static int flag = 0;
  if (GPIO_Pin == GPIO_PIN_4)
  {
    if (flag == 0)
    {
      Set_GPIO_Bit(13, 0);
      flag = 1;
    }
    else
    {
      Set_GPIO_Bit(13, 1);
      flag = 0;
    }
  }
}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
