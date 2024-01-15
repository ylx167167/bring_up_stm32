# 2024年1月14日
    * bring up stm32 
    * PC13 led 点亮
    * 更改c_cpp_properties.json 设置头文件路径和配置文件方便后续调试
    * 新增time.c/h hal层调用滴答时钟 软件延迟 进行led交替闪烁
    * 使能PB4中断控制 从而控制PC13 led的开关
    * 使能PB6 PB7的I2C1 读取lm75b温湿度传感器
    * 增加mpu6500驱动代码 功能待完善
    * 使能ADC1的A1 A0两个通道 ，实现单通道电压采集
# 2024年1月15日
### ADC
~~~
https://blog.csdn.net/m0_49968063/article/details/131121486
https://blog.csdn.net/weixin_45935114
模式切换：http://www.51hei.com/bbs/dpj-221633-1.html
ADC:采样 保持 转换 输出
不同ADC转换模式：
* 软件触发 轮询转换 HAL_ADC_GetValue
* 软件触发 中断转换 HAL_ADC_GetValue HAL_ADC_ConvCpltCallback（重写中断回调函数）
* 定时器触发 中断转换 开启定时器中断 HAL_ADC_ConvCpltCallback （重写中断回调函数）
* DMA模式 HAL_ADC_Start_DMA
单次转换模式（Single Conversion Mode）：在单次转换模式下，ADC按照设定的配置执行一次转换，然后停止转换并等待进一步的触发。这种模式适用于需要单次获取转换结果的应用场景。
连续转换模式（Continuous Conversion Mode）：在连续转换模式下，ADC在完成一次转换后自动开始下一次转换，无需外部触发。这种模式适用于需要持续采样的应用场景，如数据采集。
非扫描模式（Single Conversion Mode）：在非扫描模式下，只能选择一个通道进行转换。转换完成后，可以手动或通过中断等方式改变通道选择来执行下一次转换。
扫描模式（Scan Mode）：在扫描模式下，可以通过设置多个输入通道进行连续转换。ADC将按照通道号的顺序对设置的多个通道进行转换，并将结果按顺序保存在对应的数据寄存器中。可以通过使用EOC（End of Conversion）标志或DMA传输等方式来获取每个通道的转换结果。
实验：
    A0 A1 A2 A3 DMA 扫描模式，单次转换
    A0 A1 A2 A3 DMA 单次模式，连续转换
    (在DMA中断中关闭ADC存入DMA则为单次转换) HAL_ADC_Start  HAL_ADC_Stop
    (在函数调用时关闭ADC扫描模式,则为单次模式) HAL_ADC_Start_DMA HAL_ADC_Stop_DMA
    只读取某个通道的 HAL_ADC_GetValue(通道号)
~~~
    * 增加代码A0 A1 A2 A3 DMA 单次模式，连续转换 待测试