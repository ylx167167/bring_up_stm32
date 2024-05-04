#include "BwFilter.h"

#include "sin.h"

/**
 *  命名风格 ：
 *      g:全局 lowpass 功能 1：处理对象 IIRcoeffs：参数名称
 *      I：区分直接1型和直接2型 2级biquad及2session
 *  公式：
 *      2级biquad及2session 第一个输出的y[n]是第二个biquad的输入 x[n]位input
 *      I型：y[n]=b1*x[n]+b2*x[n-1]+b3*x[n-2]+a1*y[n-1]+a2*y[n-2]
 *      II型：
 *  注意事项：
 *      1：器的输出值必须乘以定标值（缩放系数）才行
 */
#if 0
double g_lowpass_1hz_IIRCoeffs_I[6 * 2] = {1, 2, 1, 1, -1.9952772372946446, 0.99531479853184868,
                                           1, 2, 1, 1, -1.9886888350211553, 0.98872627223121401};

double g_lowpass_1hz_Scalevalue_I[2] = {0.0000093903093010300073, 0.0000093593025146089099};

double g_lowpass_1hz_IIRCoeffs_II[6 * 2] = {1, 2, 1, 1, -1.9952772372946446, 0.99531479853184868,
                                            1, 2, 1, 1, -1.9886888350211553, 0.98872627223121401};

double g_lowpass_1hz_Scalevalue_II[2] = {0.0000093903093010300073, 0.0000093593025146089099};

/**
 *  @brief  通过幅值去生成信号 返回double型数据
 *  @param _amp 幅值
 *  @param _wave 波形
 *  @return
 */
double Generate_Wave_ByAmP(int16_t _amp, int16_t _wave)
{

    double output = (double)-1 / 4096;
    // output = _amp * (_wave / (4096 * 6)); // 幅值

    // output = (double)(_amp * temp); // 幅值
    return output;
}

/**
 *  @brief butterworth 滤波器直接一型计算公式
 *          y[n]=b1*x[n]+b2*x[n-1]+b3*x[n-2]+a1*y[n-1]+a2*y[n-2]
 *          y[n]=scale*(b1*x[n]+b2*x[n-1]+b3*x[n-2])-a1*y[n-1]-a2*y[n-2]
 *  @param  p_IIRCoeffs IIR滤波器系数
 *  @param  p_Scalevalue 缩放系数
 *  @param  p_Delay 保存的状态变量 直接1型四个时延单位
 *  @param  _Input 此刻输入值
 *  @param  _stage // 有几个biquad 一个biquad对应一个session
 *  参考链接：https://blog.csdn.net/ether_7/article/details/119795721
 *  @return
 */
double butterWorth2Order_I_Double(double *p_IIRCoeffs, double *p_Scalevalue, double *p_Delay, double _input,
                                  int16_t _stage)
{
    double output = 0;
    int16_t _coefStep, _delayStep;
    double temp[2];
    double _scale = 0;
    for (size_t i = 0; i < _stage - 1; i++)
    {
        _coefStep = i * 6;
        _delayStep = i * 4;
        _scale = p_Scalevalue[i];
        temp[i] = _scale * (p_IIRCoeffs[_coefStep] * _input + p_IIRCoeffs[_coefStep + 1] * p_Delay[_delayStep] +
                            p_IIRCoeffs[_coefStep + 2] * p_Delay[_delayStep + 1]);

        output = temp[i] - p_IIRCoeffs[_coefStep + 4] * p_Delay[_delayStep + 2] -
                 p_IIRCoeffs[_coefStep + 5] * p_Delay[_delayStep + 3];

        p_Delay[_delayStep] = _input;                      // Xn
        p_Delay[_delayStep + 1] = p_Delay[_delayStep];     // X[n-1]=X[n]
        p_Delay[_delayStep + 2] = output;                  // Yn
        p_Delay[_delayStep + 3] = p_Delay[_delayStep + 2]; // Y[n-1]=Y[n]
        _input = output;                                   // 下一级的输入等于上一级的输出
    }
    return output;
}
/**
 *  @brief butterworth 滤波器直接II型计算公式
 *          y[n]=scale*(b1*x[n]+b2*x[n-1]+b3*x[n-2])-a1*y[n-1]-a2*y[n-2]
 *  @param 与直接I型相同 直接II型3个时延单位
 *  @return
 */
double butterWorth2Order_II_Double(double *p_IIRCoeffs, double *p_Scalevalue, double *p_Delay, double _input,
                                   int16_t _stage)
{
    double output = 0;
    int16_t _coefStep, _delayStep;
    double temp[2];
    double _scale = 0;
    for (size_t i = 0; i < _stage - 1; i++)
    {
        _coefStep = i * 6;
        _delayStep = i * 3;
        _scale = p_Scalevalue[i];
        p_Delay[_delayStep] = _scale * _input - p_IIRCoeffs[_coefStep + 4] * p_Delay[_delayStep + 1] -
                              p_IIRCoeffs[_coefStep + 5] * p_Delay[_delayStep + 3];

        output = p_IIRCoeffs[_coefStep] * p_Delay[_delayStep] + p_IIRCoeffs[_coefStep + 1] * p_Delay[_delayStep + 1] +
                 p_IIRCoeffs[_coefStep + 2] * p_Delay[_delayStep + 2];

        p_Delay[_delayStep + 2] = p_Delay[_delayStep + 1]; // X[n-2]=X[n-1]
        p_Delay[_delayStep + 1] = p_Delay[_delayStep];     // X[n-1]=X[N]

        _input = output; // 下一级的输入等于上一级的输出
    }
    return output;
}
volatile double _wave = 1.0;
volatile double _wave_1hz = 0;      // 1hz的原始信号
volatile double _wave_1hz_frI = 0;  // 直接I型 filter result信号
volatile double _wave_1hz_frII = 0; // 直接II型 filter result信号
double pDelayI[4] = {0};            // Xn1 Xn2 Yn1 Yn2
double pDelayII[3] = {0};
int16_t sin_1hz, sin_2hz, sin_3hz, sin_4hz, sin_5hz, sin_6hz, sin_all;
/**
 *  @brief butterworth 1hz 滤波器 c代码实现
 *  @param
 *  @return
 */
void Exp_BwFilter_Lowpass_1HZ_Double(double *result)
{

    uint32_t divide = 0;
    result[0] = 1.0;
    result[1] = 1.0;
    while (1)
    {
        // 1step: 生成1~6hz的信号
        sin_1hz = Generate_sin(divide, 1);
        sin_2hz = Generate_sin(divide, 2);
        sin_3hz = Generate_sin(divide, 3);
        sin_4hz = Generate_sin(divide, 4);
        sin_5hz = Generate_sin(divide, 5);
        sin_6hz = Generate_sin(divide, 6);
        sin_all = (sin_1hz + sin_2hz + sin_3hz + sin_4hz + sin_5hz + sin_6hz);
        result[0] = Generate_Wave_ByAmP(1, sin_all);
        result[1] = Generate_Wave_ByAmP(1, sin_1hz);
        divide++;
        // 2step: 进行巴特沃斯滤波
        _wave_1hz_frI = butterWorth2Order_I(g_lowpass_1hz_IIRCoeffs_I, g_lowpass_1hz_Scalevalue_I, pDelayI, result[0],
                                            sizeof(g_lowpass_1hz_IIRCoeffs_I) / sizeof(g_lowpass_1hz_IIRCoeffs_I[0]));
        _wave_1hz_frII =
            butterWorth2Order_II(g_lowpass_1hz_IIRCoeffs_II, g_lowpass_1hz_Scalevalue_II, pDelayII, result[1],
                                 sizeof(g_lowpass_1hz_IIRCoeffs_II) / sizeof(g_lowpass_1hz_IIRCoeffs_II[0]));
    }
}
#endif
#if 1

float Generate_Wave_ByAmP(int16_t _amp, float _wave)
{

    float output = (float)_amp * _wave / (4096);
    // output = _amp * (_wave / (4096 * 6)); // 幅值

    // output = (double)(_amp * temp); // 幅值
    return output;
}

float g_lowpass_1hz_IIRCoeffs_I[6 * 2] = {1, 2, 1, 1, -1.9952772372946446, 0.99531479853184868,
                                          1, 2, 1, 1, -1.9886888350211553, 0.98872627223121401};

float g_lowpass_1hz_Scalevalue_I[2] = {0.0000093903093010300073, 0.0000093593025146089099};

float g_lowpass_1hz_IIRCoeffs_II[6 * 2] = {1, 2, 1, 1, -1.9952772372946446, 0.99531479853184868,
                                           1, 2, 1, 1, -1.9886888350211553, 0.98872627223121401};

float g_lowpass_1hz_Scalevalue_II[2] = {0.093903093010300073, 0.093593025146089099};

int16_t sin_1HZ, sin_2HZ, sin_3HZ, sin_4HZ, sin_5HZ, sin_6HZ;
float sin_all;
float pDelayI[8] = {0}; // Xn1 Xn2 Yn1 Yn2 二阶的话就有两个 2*4
float pDelayII[6] = {0};

volatile float _wave_1hz = 0; // 1hz的原始信号
volatile float _waveI, _waveII = 0;
float *_waveaddr = NULL;
volatile float _wave_1hz_frI = 0; // 直接I型 filter result信号
float _wave_1hz_frII = 0;         // 直接II型 filter result信号

/**
 *  @brief butterworth 滤波器直接一型计算公式
 *          y[n]=b1*x[n]+b2*x[n-1]+b3*x[n-2]+a1*y[n-1]+a2*y[n-2]
 *          y[n]=scale*(b1*x[n]+b2*x[n-1]+b3*x[n-2])-a1*y[n-1]-a2*y[n-2]
 *  @param  p_IIRCoeffs IIR滤波器系数
 *  @param  p_Scalevalue 缩放系数
 *  @param  p_Delay 保存的状态变量 直接1型四个时延单位
 *  @param  _Input 此刻输入值
 *  @param  _stage // 有几个biquad 一个biquad对应一个session
 *  参考链接：https://blog.csdn.net/ether_7/article/details/119795721
 *  @return
 */
float butterWorth2Order_I_Float(float *p_IIRCoeffs, float *p_Scalevalue, float *p_Delay, float _input, int16_t _stage)
{
    float output;
    int16_t _coefStep, _delayStep;
    float temp[2] = {0};
    float _scale = 0;
    for (int16_t i = 0; i < _stage - 1; i++)
    {
        _coefStep = i * 6;
        _delayStep = i * 4;
        _scale = p_Scalevalue[i];
        temp[i] = _scale * (p_IIRCoeffs[_coefStep] * _input + p_IIRCoeffs[_coefStep + 1] * p_Delay[_delayStep] +
                            p_IIRCoeffs[_coefStep + 2] * p_Delay[_delayStep + 1]);

        output = temp[i] - p_IIRCoeffs[_coefStep + 4] * p_Delay[_delayStep + 2] -
                 p_IIRCoeffs[_coefStep + 5] * p_Delay[_delayStep + 3];

        p_Delay[_delayStep] = _input;                      // Xn
        p_Delay[_delayStep + 1] = p_Delay[_delayStep];     // X[n-1]=X[n]
        p_Delay[_delayStep + 2] = output;                  // Yn
        p_Delay[_delayStep + 3] = p_Delay[_delayStep + 2]; // Y[n-1]=Y[n]
        _input = output;                                   // 下一级的输入等于上一级的输出
    }
    return output;
}
/**
 *  @brief butterworth 滤波器直接II型计算公式
 *          y[n]=scale*(b1*x[n]+b2*x[n-1]+b3*x[n-2])-a1*y[n-1]-a2*y[n-2]
 *  @param 与直接I型相同 直接II型3个时延单位
 *  @return
 */
volatile int16_t tes = 0;
float butterWorth2Order_II_Float(float *p_IIRCoeffs, float *p_Scalevalue, float *p_Delay, float _input, uint16_t _stage)
{
    float output = 0;
    uint16_t _coefStep, _delayStep;
    float _scale = 0;
    float b0, b1, b2;
    float a1, a2;
    for (uint16_t i = 0; i < _stage - 1; i++)
    {
        _coefStep = i * 6;
        _delayStep = i * 3;
        _scale = p_Scalevalue[i];
        p_Delay[_delayStep] = _scale * _input - p_IIRCoeffs[_coefStep + 4] * p_Delay[_delayStep + 1] -
                              p_IIRCoeffs[_coefStep + 5] * p_Delay[_delayStep + 2];

        output = p_IIRCoeffs[_coefStep] * p_Delay[_delayStep] + p_IIRCoeffs[_coefStep + 1] * p_Delay[_delayStep + 1] +
                 p_IIRCoeffs[_coefStep + 2] * p_Delay[_delayStep + 2];

        p_Delay[_delayStep + 2] = p_Delay[_delayStep + 1]; // X[n-2]=X[n-1]
        p_Delay[_delayStep + 1] = p_Delay[_delayStep];     // X[n-1]=X[N]

        _input = output; // 下一级的输入等于上一级的输出
    }

    return output;
}
/**
 *  @brief butterworth 1hz 滤波器 c代码实现
 *  @param
 *  @return
 */
void Exp_BwFilter_Lowpass_1HZ_Float()
{
#if EXP_BwFilter
    uint32_t divide = 0;
    _waveaddr = &_waveI;
    while (1)
    {
        // 1step: 生成1~6hz的信号
        sin_1HZ = Generate_sin(divide, 1);
        sin_2HZ = Generate_sin(divide, 2);
        sin_3HZ = Generate_sin(divide, 3);
        sin_4HZ = Generate_sin(divide, 4);
        sin_5HZ = Generate_sin(divide, 5);
        sin_6HZ = Generate_sin(divide, 6);
        sin_all = sin_1HZ + sin_2HZ + sin_3HZ + sin_4HZ + sin_5HZ + sin_6HZ;
        _waveI = Generate_Wave_ByAmP(100, sin_all);
        _waveII = _waveI;
        _wave_1hz = Generate_Wave_ByAmP(100, sin_1HZ);

        divide++;

        // // 2step: 进行巴特沃斯滤波
        _wave_1hz_frI =
            butterWorth2Order_I_Float(g_lowpass_1hz_IIRCoeffs_I, g_lowpass_1hz_Scalevalue_I, pDelayI, _waveI, 2);
        _wave_1hz_frII =
            butterWorth2Order_II_Float(g_lowpass_1hz_IIRCoeffs_II, g_lowpass_1hz_Scalevalue_II, pDelayII, _waveII, 2);
    }
#endif
}

#endif