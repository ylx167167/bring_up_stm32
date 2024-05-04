#include "filter.h"

// 定义butterworth系数
#define NUMERATOR_COEFF
#define DENOMINATOR_COEFF
#define IIR_FILTER_COEFS_SECOND_ORDER 6
#define IIR_FILTER_NUM_STAGES 2

float pFilterCoefs[IIR_FILTER_NUM_STAGES * IIR_FILTER_COEFS_SECOND_ORDER] = {
    1, 2, 1, 1, 1.709088, -0.792801, 1, 2, 1, 1, 1.490685, -0.563700};
double pScaleVals[IIR_FILTER_COEFS_SECOND_ORDER] = {0.020928246435308509,
                                                    0.018253844725076802};
// 0 1 2 3 第一个biquad 4 5 6 7第二个biquad x[n-1] x[n-2] y[n-1] y[n-2]
float pDelay[8] = {0};
double pDelayDouble[8] = {0};

/**
 *  @brief 输入一个固定的float值，该float值为正弦信号的幅值， 使用4介
 * 2级biquad及2session 第一个输出的y[n]是第二个biquad的输入 x[n]位input
 * y[n]=b1*x[n]+b2*x[n-1]+b3*x[n-2]+a1*y[n-1]+a2*y[n-2]
 *  @param
 *  @return
 */
float butterWorth2Order(float input) {
  float y, output;
  uint16_t indexTemp;
  float b0, b1, b2;
  float a1, a2;
  float scaleVal;
  float acc;
  uint16_t indexstage;
  float Xn, Xn1, Xn2, Yn1, Yn2;
  float *pState = &pDelay[0];

  Xn = input;
  Xn1 = pDelay[0];
  Xn2 = pDelay[1];
  Yn1 = pDelay[2];
  Yn2 = pDelay[3];
  b0 = pFilterCoefs[0];
  b1 = pFilterCoefs[1];
  b2 = pFilterCoefs[2];  // 第三个参数不需要使用
  a1 = pFilterCoefs[4];
  a2 = pFilterCoefs[5];
  output = b0 * Xn + b1 * Xn1 + b2 * Xn2 + a1 * Yn1 + a2 * Yn2;
  pDelay[0] = Xn;
  pDelay[1] = Xn1;
  pDelay[2] = output;
  pDelay[3] = Yn1;

  Xn = output;
  Xn1 = pDelay[4];
  Xn2 = pDelay[5];
  Yn1 = pDelay[6];
  Yn2 = pDelay[7];
  b0 = pFilterCoefs[6];
  b1 = pFilterCoefs[7];
  b2 = pFilterCoefs[8];  // 第三个参数不需要使用
  a1 = pFilterCoefs[10];
  a2 = pFilterCoefs[11];
  output = b0 * Xn + b1 * Xn1 + b2 * Xn2 + a1 * Yn1 + a2 * Yn2;
  pDelay[4] = Xn;
  pDelay[5] = Xn1;
  pDelay[6] = output;
  pDelay[7] = Yn1;
  // 1：butterworth计算公式
  for (indexstage = 0; indexstage < IIR_FILTER_NUM_STAGES; indexstage++) {
  }

  // for (indexStage = 0; indexStage < IIR_FILTER_NUM_STAGES; indexStage++)
  // {
  //   /* code */
  //   indexTemp = IIR_FILTER_COEFS_SECOND_ORDER * indexStage;
  //   b0 = pFilterCoefs[indexTemp];
  //   b1 = pFilterCoefs[indexTemp + 1];
  //   b2 = pFilterCoefs[indexTemp + 2];  // 第三个参数不需要使用
  //   a1 = pFilterCoefs[indexTemp + 4];
  //   a2 = pFilterCoefs[indexTemp + 5];
  //   scaleVal = pScaleVals[indexStage];

  //   pDelay[indexTemp] = scaleVal * input - a1 * pDelay[indexTemp + 1] -
  //                       a2 * pDelay[indexTemp + 2];
  //   y = b0 * pDelay[indexTemp] + b1 * pDelay[indexTemp + 1] +
  //       b2 * pDelay[indexTemp + 2];
  //   pDelay[indexTemp + 2] = pDelay[indexTemp + 1];
  //   pDelay[indexTemp + 1] = pDelay[indexTemp];

  //   input = y;
  // }

  // 2：保留状态

  return output * pScaleVals[0] * pScaleVals[1];
}

double butterWorth2Orderdouble(float input) {
  double y, output;
  uint16_t indexTemp;
  double b0, b1, b2;
  double a1, a2;
  double scaleVal;
  double acc;
  uint16_t indexstage;
  double Xn, Xn1, Xn2, Yn1, Yn2;

  Xn = input;
  Xn1 = pDelayDouble[0];
  Xn2 = pDelayDouble[1];
  Yn1 = pDelayDouble[2];
  Yn2 = pDelayDouble[3];
  b0 = pFilterCoefs[0];
  b1 = pFilterCoefs[1];
  b2 = pFilterCoefs[2];  // 第三个参数不需要使用
  a1 = pFilterCoefs[4];
  a2 = pFilterCoefs[5];
  output = b0 * Xn + b1 * Xn1 + b2 * Xn2 + a1 * Yn1 + a2 * Yn2;
  pDelayDouble[0] = Xn;
  pDelayDouble[1] = Xn1;
  pDelayDouble[2] = output;
  pDelayDouble[3] = Yn1;

  Xn = output;
  Xn1 = pDelayDouble[4];
  Xn2 = pDelayDouble[5];
  Yn1 = pDelayDouble[6];
  Yn2 = pDelayDouble[7];
  b0 = pFilterCoefs[6];
  b1 = pFilterCoefs[7];
  b2 = pFilterCoefs[8];  // 第三个参数不需要使用
  a1 = pFilterCoefs[10];
  a2 = pFilterCoefs[11];
  output = b0 * Xn + b1 * Xn1 + b2 * Xn2 + a1 * Yn1 + a2 * Yn2;
  pDelayDouble[4] = Xn;
  pDelayDouble[5] = Xn1;
  pDelayDouble[6] = output;
  pDelayDouble[7] = Yn1;
  // 1：butterworth计算公式
  for (indexstage = 0; indexstage < IIR_FILTER_NUM_STAGES; indexstage++) {
  }

  // for (indexStage = 0; indexStage < IIR_FILTER_NUM_STAGES; indexStage++)
  // {
  //   /* code */
  //   indexTemp = IIR_FILTER_COEFS_SECOND_ORDER * indexStage;
  //   b0 = pFilterCoefs[indexTemp];
  //   b1 = pFilterCoefs[indexTemp + 1];
  //   b2 = pFilterCoefs[indexTemp + 2];  // 第三个参数不需要使用
  //   a1 = pFilterCoefs[indexTemp + 4];
  //   a2 = pFilterCoefs[indexTemp + 5];
  //   scaleVal = pScaleVals[indexStage];

  //   pDelay[indexTemp] = scaleVal * input - a1 * pDelay[indexTemp + 1] -
  //                       a2 * pDelay[indexTemp + 2];
  //   y = b0 * pDelay[indexTemp] + b1 * pDelay[indexTemp + 1] +
  //       b2 * pDelay[indexTemp + 2];
  //   pDelay[indexTemp + 2] = pDelay[indexTemp + 1];
  //   pDelay[indexTemp + 1] = pDelay[indexTemp];

  //   input = y;
  // }

  // 2：保留状态

  return output * pScaleVals[0] * pScaleVals[1];
}