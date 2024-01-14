#include "mpu6500.h"

// 定义MPU9250内部地址
//****************************************
#define SMPLRT_DIV 0x19   // 陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG 0x1A       // 低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG 0x1B  // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define MAG_XOUT_L 0x03
#define MAG_XOUT_H 0x04
#define MAG_YOUT_L 0x05
#define MAG_YOUT_H 0x06
#define MAG_ZOUT_L 0x07
#define MAG_ZOUT_H 0x08

#define PWR_MGMT_1 0x6B // 电源管理，典型值：0x00(正常启用)
#define WHO_AM_I 0x75   // IIC地址寄存器(默认数值0x68，只读)

//****************************

#define GYRO_ADDRESS 0xD0  // 陀螺地址 也是从机地址
#define MAG_ADDRESS 0x18   // 磁场地址
#define ACCEL_ADDRESS 0xD0 // 加速度地址
#define MPU6500_ADDR GYRO_ADDRESS

#define MPU6500_I2C hi2c1   // 定义本机 I2C 地址
#define MPU6500_TX_DELAY 30 // 读写延迟 可调 最少5还能够读写
#define MPU6500_RX_DELAY 30

uint8_t MPU6500_Write(uint8_t mem_addr, uint8_t data, uint16_t len)
{
    if (HAL_I2C_Mem_Write(&MPU6500_I2C, MPU6500_ADDR, mem_addr,
                          I2C_MEMADD_SIZE_8BIT, &data, len, MPU6500_TX_DELAY) == HAL_OK)
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t MPU6500_Read(uint8_t mem_addr, uint8_t data, uint16_t len)
{
    if (HAL_I2C_Mem_Read(&MPU6500_I2C, MPU6500_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT,
                         &data, len, MPU6500_RX_DELAY) == HAL_OK)
    {

        return TRUE;
    }

    return FALSE;
}

// 初始化MPU6500，根据需要请参考pdf进行修改************************
uint8_t MPU6500_Init(void)
{
    /*
       Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
       Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
       Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
       Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
       Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
    */
    if (MPU6500_Write(PWR_MGMT_1, 0x00, 2) ||
        MPU6500_Write(SMPLRT_DIV, 0x07, 2) ||
        MPU6500_Write(CONFIG, 0x06, 2) ||
        MPU6500_Write(GYRO_CONFIG, 0x18, 2) ||
        MPU6500_Write(ACCEL_CONFIG, 0x01, 2))
    {
        return TRUE;
    }
    return FALSE;
    //----------------
    //	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode
}

uint8_t MPU6500_READ_GYRO(struct mpu6500_gyro *gyro)
{
    if (MPU6500_Read(GYRO_XOUT_H, gyro->x_gyro_h, 1) ||
        MPU6500_Read(GYRO_XOUT_L, gyro->x_gyro_l, 1) ||
        MPU6500_Read(GYRO_YOUT_H, gyro->y_gyro_h, 1) ||
        MPU6500_Read(GYRO_YOUT_L, gyro->y_gyro_l, 1) ||
        MPU6500_Read(GYRO_ZOUT_H, gyro->z_gyro_h, 1) ||
        MPU6500_Read(GYRO_ZOUT_L, gyro->z_gyro_l, 1))
    {
        return TRUE;
    }
    return FALSE;
}
uint8_t MPU6500_READ_ACCEL(struct mpu6500_mag *mag)
{
    if (MPU6500_Read(MAG_XOUT_H, mag->x_mag_h, 1) ||
        MPU6500_Read(MAG_XOUT_L, mag->x_mag_l, 1) ||
        MPU6500_Read(MAG_YOUT_H, mag->y_mag_h, 1) ||
        MPU6500_Read(MAG_YOUT_L, mag->y_mag_l, 1) ||
        MPU6500_Read(MAG_ZOUT_H, mag->z_mag_h, 1) ||
        MPU6500_Read(MAG_ZOUT_L, mag->z_mag_l, 1))
    {
        return TRUE;
    }
    return FALSE;
}
uint8_t MPU6500_READ_MAG(struct mpu6500_accel *accel)
{
    if (MPU6500_Read(ACCEL_XOUT_H, accel->x_accel_h, 1) ||
        MPU6500_Read(ACCEL_XOUT_L, accel->x_accel_l, 1) ||
        MPU6500_Read(ACCEL_YOUT_H, accel->y_accel_h, 1) ||
        MPU6500_Read(ACCEL_YOUT_L, accel->y_accel_l, 1) ||
        MPU6500_Read(ACCEL_ZOUT_H, accel->z_accel_h, 1) ||
        MPU6500_Read(ACCEL_ZOUT_L, accel->z_accel_l, 1))
    {
        return TRUE;
    }
    return FALSE;
}
