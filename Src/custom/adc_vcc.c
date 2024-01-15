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
   返回值：
*/
uint16_t adc_get_dma(uint16_t *adc_values,adc){
 HAL_ADC_Start(&hadc1);                       // 启动adc
 HAL_ADC_PollForConversion(&hadc1,100);//延迟100ms
 HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_values,4);//读取4个通道
 HAL_ADC_Stop(&hadc1);                          //关闭adc
}
/*
   函数定义：重写DMA中断处理函数 每次扫描完关闭DMA ADC采样
   返回值：
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//DMA采集完成中断服务函数
{
	HAL_ADC_Stop_DMA(&hadc1);// 每一次采集完关闭DMA的ADC采集
}