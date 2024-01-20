/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "custom/syscall.h"
#include "custom/lm75.h"
#include "custom/mpu6500.h"
#include "custom/adc_vcc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
static uint16_t temp = 0;
static uint16_t thyst = 0;
static uint16_t tos = 0;
static uint8_t temp_flag = FALSE;
static uint8_t thyst_flag = FALSE;
static uint8_t tos_flag = FALSE;
uint16_t temp;
uint16_t thyst;
uint16_t tos;
uint8_t temp_flag;
uint8_t thyst_flag;
uint8_t tos_flag;
float adc_vcc;
// RetargetInit(&huart1);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
  return ch;
}
uint16_t pwm = 0;
/*****************************************************
 *function: 读字符文件函�?
 *param1: 文件指针
 *return: 读取字符�? ASCII �?
 ******************************************************/
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 10);
  return (int)ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // HAL_TIM_Base_Start_IT(&htim2); // # ĺŻĺ¨ĺŽćśĺ¨äş HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);ç¨äşĺź?ĺŻçšĺŽé?é
  // HAL_TIM_Base_Start_IT(&htim3); // # ĺŻĺ¨ĺŽćśĺ¨ä¸
  LM75B_Init(0x00); // ĺĺ§ĺLM25B
  MPU6500_Init();   // ĺĺ§ĺmpu6500
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  temp = 0;
  thyst = 0;
  tos = 0;
  temp_flag = FALSE;
  thyst_flag = FALSE;
  tos_flag = FALSE;
  adc_vcc = 0;
  // RetargetInit(&huart1);
  while (1)
  {
#if 0
    Set_GPIO_Bit(4, 0);
    delay_ms_soft(500);
    Set_GPIO_Bit(4, 1);
    delay_ms_soft(500);
    /*! lm75čŻťĺćŹĄć¸ŠćšżĺşŚ čŻťĺčŚćé´é*/
    temp_flag = LM75B_Read_Temp(&temp);
    delay_ms_soft(100);
    thyst_flag = LM75B_Read_THYST(&thyst);
    delay_ms_soft(100);
    tos_flag = LM75B_Read_TOS(&tos);
    delay_ms_soft(100);
    adc_vcc = adc_get_vcc();
#endif
#if 0
//  uart ??
    char cmd;
    scanf("%c", &cmd);
    if (cmd == 'C' || cmd == 'c')
    {
      cmd = 0;
      printf("\r\nHello World.");
    }
#endif
    // #if 0
    // pwm??
    while (pwm < 500)
    {
      pwm++;
      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwm);
      delay_ms_soft(2);
    }
    while (pwm)
    {
      pwm--;
      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwm);
      delay_ms_soft(2);
    }
    // #endif
    // // ??????????
    // delay_ms_soft(200);
    // // Set_GPIO_Bit(4, 1);
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    // delay_ms_soft(200);
    // // Set_GPIO_Bit(4, 0);
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
  }

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
