#include "syscall.h"

void delay_ms_soft(uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}
void delay_ms_hard(uint32_t dleay_ms)
{
}
