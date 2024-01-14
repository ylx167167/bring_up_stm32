#ifndef _MPU6500_H_
#define _MPU6500_H_

#include "i2c.h"
#include "main.h"

struct mpu6500_gyro
{
    unsigned char x_gyro_h;
    unsigned char x_gyro_l;
    unsigned char y_gyro_h;
    unsigned char y_gyro_l;
    unsigned char z_gyro_h;
    unsigned char z_gyro_l;
};
struct mpu6500_mag
{
    unsigned char x_mag_h;
    unsigned char x_mag_l;
    unsigned char y_mag_h;
    unsigned char y_mag_l;
    unsigned char z_mag_h;
    unsigned char z_mag_l;
};
struct mpu6500_accel
{
    unsigned char x_accel_h;
    unsigned char x_accel_l;
    unsigned char y_accel_h;
    unsigned char y_accel_l;
    unsigned char z_accel_h;
    unsigned char z_accel_l;
};
uint8_t MPU6500_READ_GYRO(struct mpu6500_gyro *gyro);
uint8_t MPU6500_READ_ACCEL(struct mpu6500_mag *mag);
uint8_t MPU6500_READ_MAG(struct mpu6500_accel *accel);
uint8_t MPU6500_Init(void);

#endif
