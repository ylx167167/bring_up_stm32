#include "myapp.h"
#include "BwFilter.h" //滤波器
#include "cmsis_os.h"
#include "frosLocal.h"
#include "gConfig.h"
#include "stdio.h"
void Start_MyApp_Loop(void)
{
    /* 初始化多个Task 并启动Task */
    MX_FREERTOS_Init();
    osKernelStart();

    while (1)
    {
        Exp_BwFilter_Lowpass_1HZ_Float();
    }
}