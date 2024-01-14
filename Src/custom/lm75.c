#include "lm75.h"

#define LM75B_I2C hi2c1
#define LM75B_ADDR (0x48u << 1) // A0 A1 A2拉低 地址0b01001000=0x48
#define LM75B_TX_DELAY 100
#define LM75B_RX_DELAY 100

#define LM75B_TEMP_REG 0x00u
#define LM75B_CONF_REG 0x01u
#define LM75B_THYST_REG 0x02u
#define LM75B_TOS_REG 0x03u

uint8_t I2C_TX_DATA[2];
uint8_t I2C_RX_DATA[2];

uint8_t LM75B_Write(uint8_t mem_addr, uint8_t data[], uint16_t len)
{
    if (HAL_I2C_Mem_Write(&LM75B_I2C, LM75B_ADDR, mem_addr,
                          I2C_MEMADD_SIZE_8BIT, data, len, LM75B_TX_DELAY) == HAL_OK)
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Read(uint8_t mem_addr, uint8_t data[], uint16_t len)
{
    if (HAL_I2C_Mem_Read(&LM75B_I2C, LM75B_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT,
                         data, len, LM75B_RX_DELAY) == HAL_OK)
    {

        return TRUE;
    }

    return FALSE;
}

uint8_t LM75B_Reset_Conf(void)
{
    return LM75B_Write_Conf(0x00);
}

uint8_t LM75B_Write_Conf(uint8_t conf)
{
    I2C_TX_DATA[0] = conf & 0x1F;
    if (LM75B_Write(LM75B_CONF_REG, I2C_TX_DATA, 1))
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Read_Conf(uint8_t *res_conf)
{
    if (LM75B_Read(LM75B_CONF_REG, I2C_RX_DATA, 1))
    {
        *res_conf = I2C_RX_DATA[0];
        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Write_THYST(uint16_t thyst)
{
    I2C_TX_DATA[0] = thyst >> 1;
    I2C_TX_DATA[1] = (thyst & 0x01) << 7;

    if (LM75B_Write(LM75B_THYST_REG, I2C_TX_DATA, 2))
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Read_THYST(uint16_t *res_thyst)
{

    if (LM75B_Read(LM75B_THYST_REG, I2C_RX_DATA, 2))
    {

        *res_thyst = (uint16_t)I2C_RX_DATA[0] << 1;
        *res_thyst += I2C_RX_DATA[1] >> 7;

        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Write_TOS(uint16_t tos)
{
    I2C_TX_DATA[0] = tos >> 1;
    I2C_TX_DATA[1] = (tos & 0x01) << 7;

    if (LM75B_Write(LM75B_TOS_REG, I2C_TX_DATA, 2))
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Read_TOS(uint16_t *res_tos)
{
    if (LM75B_Read(LM75B_TOS_REG, I2C_RX_DATA, 2))
    {

        *res_tos = (uint16_t)I2C_RX_DATA[0] << 1;
        *res_tos += I2C_RX_DATA[1] >> 7;

        return TRUE;
    }
    return FALSE;
}

uint8_t LM75B_Read_Temp(uint16_t *res_temp)
{
    if (LM75B_Read(LM75B_TEMP_REG, I2C_RX_DATA, 2))
    {
        *res_temp = (uint16_t)I2C_RX_DATA[0] << 3;
        *res_temp += I2C_RX_DATA[1] >> 5;

        return TRUE;
    }
    return FALSE;
}
