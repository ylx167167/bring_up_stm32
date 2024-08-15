#include "myapp.h"
#include "BwFilter.h" //滤波器
#include "cmsis_os.h"

#include "canlocal.h"
#include "frosLocal.h"
#include "gConfig.h"
#include "gpio.h"
#include "stdio.h"
void Start_MyApp_Loop(void)
{
#if EXP_FreeRtos == 1
    Led_Ctrl(LED_UP_C13);
    /* 初始化多个Task 并启动Task */
    MX_FREERTOS_Init();
    osKernelStart();
#endif
    while (1)
    {
        Exp_BwFilter_Lowpass_1HZ_Float();
        // Exp_LocalCan_send();
        // Exp_LocalCan_send();
    }
}
