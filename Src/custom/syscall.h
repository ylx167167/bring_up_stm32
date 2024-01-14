#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f1xx_hal.h"
    /*!延迟*/
    void delay_ms_soft(uint32_t delay_ms);
    void delay_ms_hard(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif
#endif
