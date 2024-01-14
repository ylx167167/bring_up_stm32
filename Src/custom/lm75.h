#ifndef __LM75_H
#define __LM75_H

#include "i2c.h"
#include "main.h"
#ifndef TRUE
#define TRUE 1u
#endif

#ifndef FALSE
#define FALSE 0u
#endif

#define LM75B_CONF_OS_F_QUE_1 0b00000u
#define LM75B_CONF_OS_F_QUE_2 0b01000u
#define LM75B_CONF_OS_F_QUE_4 0b10000u
#define LM75B_CONF_OS_F_QUE_6 0b11000u

#define LM75B_CONF_OS_POL_L 0b00000u
#define LM75B_CONF_OS_POL_H 0b00100u

#define LM75B_CONF_OS_COMP_INT_CP 0b00000u
#define LM75B_CONF_OS_COMP_INT_IT 0b00010u

#define LM75B_CONF_OS_SHUTDOWN_FLASE 0b00000u
#define LM75B_CONF_OS_SHUTDOWN_TRUE 0b00001u

uint8_t LM75B_Reset_Conf(void);
uint8_t LM75B_Write_Conf(uint8_t conf);
uint8_t LM75B_Read_Conf(uint8_t *res_conf);
uint8_t LM75B_Write_THYST(uint16_t thyst);
uint8_t LM75B_Read_THYST(uint16_t *res_thyst);
uint8_t LM75B_Write_TOS(uint16_t tos);
uint8_t LM75B_Read_TOS(uint16_t *res_tos);
uint8_t LM75B_Read_Temp(uint16_t *res_temp);

#endif
