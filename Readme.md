# 2024年1月14日
    * bring up stm32 
    * PC13 led 点亮
    * 更改c_cpp_properties.json 设置头文件路径和配置文件方便后续调试
    * 新增time.c/h hal层调用滴答时钟 软件延迟 进行led交替闪烁
    * 使能PB4中断控制 从而控制PC13 led的开关
    * 使能PB6 PB7的I2C1 读取lm75b温湿度传感器
    * 增加mpu6500驱动代码 功能待完善