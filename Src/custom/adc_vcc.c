#include "adc_vcc.h"

uint32_t adc_get(void)
{

    HAL_ADC_Start(&hadc1);                       // 启动adc
    HAL_ADC_PollForConversion(&hadc1, 10);       // 等待adc转换完成
    return ((uint16_t)HAL_ADC_GetValue(&hadc1)); // 返回ad转化后的数字量
}
/*
   函数定义：获取实际电压
   返回值：无
*/
float adc_get_vcc(void)
{
    uint32_t adcx;
    adcx = adc_get();                // 获取到ad转换后的数字量
    return (float)adcx * 3.3 / 4096; // 将数字量转化成实际电压
}

/*
   函数定义：获取adc转换完成后的数据
   返回值：uint32_t型的模拟数值
*/
