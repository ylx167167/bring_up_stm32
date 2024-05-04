/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "adc.h"
#include "can.h"
#include "cmsis_os.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "custom/adc_vcc.h"
#include "custom/lm75.h"
#include "custom/mpu6500.h"
#include "custom/syscall.h"
#include "filter.h"
#include "stdio.h"
#include "stdlib.h"

void SystemClock_Config(void);

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
    return ch;
}
uint16_t pwm = 0;
/*****************************************************
 *function: 读字符文件函
 *param1: 文件指针
 *return: 读取字符
 ******************************************************/
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 10);
    return (int)ch;
}
uint8_t TxData[8] = {0};
uint8_t length = 0x00;

float AmpInput = 2;
float AmpOutput = 0;
float fillterOutput = 0;
double fillterOutputDouble = 0;

uint32_t gt_sincos_divide;
int waveflag1 = 0;
int waveflag2 = 0;

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
/**
 *  @brief stm32cubeMx生成的的代码进行Init
 *  @return
 */
void MX_Init()
{
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_CAN_Init();
    MX_USART1_UART_Init();

    /* Init scheduler */
    osKernelInitialize();
}

/**
 *  @brief 本地对相关接口的自定义初始化
 *  @return
 */
void Local_Init()
{
    CAN_Local_Init();
}

/**
 * @brief  The application entry point.
 *  @return
 */
int main(void)
{
    /*1:系统初始化 */
    HAL_Init();
    SystemClock_Config();
    MX_Init();
    Local_Init();

    /*2:进入主循环 */
    Start_MyApp_Loop(); // appLoop
}
