

#ifndef _SIN_FORM_H_
#define _SIN_FORM_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

#define SIN_PI_M2_DPIX 1024  // 水平分辨�?
#define SIN_PI_M2_DPIXBIT 12 // 垂直分辨�?
#define WAVE_TOP 4095        // 波形峰峰�? dac的时候可以用�?

  extern const int16_t sin_pi_m2[SIN_PI_M2_DPIX + 1];

  typedef struct Test_SinCos
  {
    int16_t sin;
    int16_t cos;
    uint16_t sconver; // SIN数组指针
    uint16_t cconver; // COS数组指针
  } Test_SinCos;

  void Test_SinCos_By_delay(Test_SinCos *tSinCos, uint32_t divide);

  // 倍�?�公�?
  uint16_t Formula_DSin(uint32_t dIvide);
  /*! @brief 生成1~20频率的波�?*/
  typedef struct Test_SinWave
  {
    int16_t sin_1h;
    int16_t sin_2h;
    int16_t sin_3h;
    int16_t sin_4h;
    int16_t sin_5h;
    int16_t sin_6h;
    int16_t sin_all;
  } Test_SinWave;

  /**
   *  @brief 通过频率来�?�置波形
   *  @param _wave 波形
   *  @param _freq 频率
   *  @return
   */
  void Test_Generate_MultiFreqWave(Test_SinWave *_wave, int16_t _freq,
                                   uint32_t divide);
  /**
   *  @brief 通过设定幅�? 并通过wave去生成一�?特定幅值的波形
   *  @param  _input 幅�?
   *  @param  _wave 波形
   *  @return
   */
  float Test_Generate_Wave_ByAmP(float _input, int16_t _wave);

#ifdef __cplusplus
}
#endif

#endif