

#ifndef _SIN_FORM_H_
#define _SIN_FORM_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define SIN_DPIX 1024   // 水平分辨率
#define SIN_DPIXBIT 12  // 垂直分辨率
#define WAVE_TOP 4095   // 波形峰峰值 dac的时候可以用到

extern const int16_t sin_1024[SIN_DPIX + 1];

typedef struct Test_SinCos {
  int16_t sin;
  int16_t cos;
  uint16_t sconver;  // SIN数组指针
  uint16_t cconver;  // COS数组指针
} Test_SinCos;

/**
 *  @brief 生成sin函数 _freq频率
 *  @param _dIvide 虚拟的时间偏移量
 *  @param _freq频率
 *  @return
 */
int16_t Generate_sin(uint32_t _dIvide, uint16_t _freq);
#ifdef __cplusplus
}
#endif

#endif