# Mini-Monitor

> **视频演示**：[【自制一个迷你电流表】](https://www.bilibili.com/video/BV1RD4y1r7Lr/?spm_id_from=333.999.0.0&vd_source=f48d85f7ac2dd03394a7e2311b7bbc09)
>

## 功能特性
* 电流量程：-655.36mA ~ +655.36mA  
* 电流分辨率：0.02mA  
* 电压量程：0~36V  
* 电压分辨率：1.25mV  
* ADC精度：16-bit  
* 增益误差：0.1% (Max)  
* 输入失调电压：10μV (Max)  
* 供电：5V USB-C  
* 尺寸：48.0*22.4*11.0mm (LWH)  
* 工作温度：-40°C ~ 85°C  
* 主要功能：测电流、电压、计算平均电流  
* 其他优点：体积小巧、操作简单、精度较高
>

> 注：至于为何不设计成更大的量程，因为对于INA226而言，更大的量程意味着降低分辨率。该项目设计的初衷是测量Cat.1模组低功耗，655.36mA对应0.02的分辨率，精度基本满足。调整采样电阻和芯片寄存器值，可改变电流量程和分辨率，下文会有介绍。
> 

## MCU选型
> MCU选用STM32F401CCU6，虽然性能有些过剩，但这个型号是权衡多个因素的结果：  
* 需要支持USB device，设计之初打算再做一个上位机软件，可通过USB在电脑上实时显示曲线图。但后来因为时间原因，这个功能搁置了，直接OLED显示效果也不错。  
* 需要QFN封装，因为 "Mini" 是该产品的特点之一，所以PCB要尽可能设计的小巧，所以QFN是比较合适的封装，QFP封装略显过大。  
* 需要开发方便，ST的芯片工具和资料相对完善。  
* 需要价格适中，考虑过小批量，所以价格因素必须考虑，而当时正处于进口芯片疯狂涨到天价时期，综合比较下来发现芯片价格与性能不完全成正比，与市场需求也有关系。最后选择了STM32F401CCU6，当时拿样价格是 ¥16.8，现在价格肯定有变化。
>

## ADC选型
> 电流测量芯片选用INA226，16-bit，量程和分辨率均可编程。
> 

## INA226的采样电阻取值
> INA226的 Shunt voltage 输入范围是 –81.9175mV~81.92mV；根据所测最大电流可以计算出最大的shunt resistor值，shunt resistor电阻越大，测小电流时误差越小，但是shunt resistor越大，在shunt resistor上的功率损耗也越大，所以需要根据情况折中选择。
> 
> `这里我们选择阻值R100，即0.1欧。` 
> 

## INA226的Calibration Register寄存器取值计算
> 写入 Calibration Register 的值需要根据实际电路中 采样电阻的阻值 和 电流分辨率 来设置，CAL 就是需要写入 Calibration Register 寄存器的值。
> Currnet_LSB 是电流分辨率，这里我们取 0.02mA。
> R_SHUNT 是采样电阻，前面我们取0.1欧。
> 
> `所以 CAL = 0.00512 / (Current_LSB*R_SHUNT)*1000 = 0.00512 / (0.02*0.1)*1000 = 2560`
> 

## INA226的电流量程计算
> 因为INA226的 Shunt voltage input range 最大为 81.92mV，而前面我们采样电阻取值0.1欧，所以在该前提下，最大检测电流为：
> 
> 81.92 / 0.1 = 819.2mA
> 
> 最后再计算 Maximum Expected Current :
> 
> `Maximum Expected Current = Currnet_LSB * 2^15 = 0.02 * 2*15 = 655.36mA`
> 
> 655.36mA < 819.2mA，满足条件，故前面计算的 Calibration Register 寄存器的值，没有问题。
>

注：关于ADC中LSB和MSB的概念理解，可参考如下文章：  
[ADC的最低有效位（LSB）和最高有效位（MSB）的理解](https://blog.csdn.net/wordwarwordwar/article/details/53208584)  
>

## 后记
> 本项目程序代码目前还比较粗糙，欢迎各位大佬前来优化，如有bug请不吝指正，谢谢^_^。
>

