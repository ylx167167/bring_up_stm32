#ifndef _CANLOCAL_H
#define _CANLOCAL_H
#include "stdio.h"
#if (HARDWARE_CAN == 1) && (EXP_CAN == 1)
void CAN_Local_Init(void);
void Exp_LocalCan_send(void);
#endif
#endif
