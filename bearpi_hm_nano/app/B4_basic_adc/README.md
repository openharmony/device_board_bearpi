# BearPi-HM_Nano开发板基础外设开发——ADC采样
本示例将演示如何在BearPi-HM_Nano开发板上通过按下按键改变GPIO口的电压，并使用ADC读取GPIO的电压值。

## ADC API分析
本案例主要使用了以下API完成ADC采样的功能。
### IoTAdcRead()
```c
unsigned int IoTAdcRead(unsigned int channel, unsigned short *data, IotAdcEquModelSel equModel, IotAdcCurBais curBais, unsigned short rstCnt);
```
 **描述：**

根据输入参数从指定的ADC通道读取一段采样数据。


**参数：**

|参数名|描述|
|:--|:------| 
| channel | 表示ADC通道。  |
| data | 表示读取的ADC数据保存地址。 |
| equModel | 表示平均算法模式。 |
| curBais | 表示模拟电源控制。 |
| rstCnt | 表示从配置采样到启动采样的延时时间计数，一次计数是334ns，其值需在0~0xFF0之间。|


## 硬件设计
本案例将使用板载用户按键F1来模拟GPIO口电压的变化。通过查看芯片手册可知GPIO_11对应的是 ADC Channel 5，所以需要编写软件去读取ADC Channel 5的电压，程序设计时先将GPIO_11上拉，使GPIO_11的电压一直处于高电平，当按键按下时GPIO_11接地，此时GPIO_11的电压变为 0 V。

![buttoncircuit](../../docs/figures/B4_basic_adc/buttoncircuit.png "buttoncircuit")

## 软件设计

**主要代码分析**
 
 ADC测试任务，主要的功能实现函数为GetVoltage()，
该函数通过使用IoTAdcRead()函数来读取 ADC Channel 5 的数值并存储在data中， `IOT_ADC_EQU_MODEL_8` 表示8次平均算法模式，`IOT_ADC_CUR_BAIS_DEFAULT` 表示默认使用自动识别模式，最后通过算式 `data * 1.8 * 4 / 4096.0` 计算出实际的电压值。
```c
/**
 * @brief get ADC sampling value and convert it to voltage
 * 
 */
static float GetVoltage(void)
{
    unsigned int ret;
    unsigned short data;

    ret = IoTAdcRead(ADC_CHANNEL, &data, IOT_ADC_EQU_MODEL_8, IOT_ADC_CUR_BAIS_DEFAULT, 0xff);
    if (ret != IOT_SUCCESS) {
        printf("ADC Read Fail\n");
    }

    return (float)data * ADC_VREF_VOL * ADC_COEFFICIENT / ADC_RATIO;
}
```


## 编译调试

### 修改 BUILD.gn 文件


修改`device\board\bearpi\bearpi_hm_nano\app` 路径下 BUILD.gn 文件，指定 `adc_example` 参与编译。

```r
#"B1_basic_led_blink:led_example",
#"B2_basic_button:button_example",
# "B3_basic_pwm_led:pwm_example",
"B4_basic_adc:adc_example",
#"B5_basic_i2c_nfc:i2c_example",
# "B6_basic_uart:uart_example",
```


### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，当F1按键未按下时采集到的电压为3.3V左右，当按键按下时，电压变为0.2V左右。
```c
=======================================
*************ADC_example***********
=======================================
vlt:3.371V
=======================================
*************ADC_example***********
=======================================
vlt:3.371V
=======================================
*************ADC_example***********
=======================================
vlt:3.373V
=======================================
*************ADC_example***********
=======================================
vlt:0.248V
=======================================
*************ADC_example***********
=======================================
vlt:0.244V
```
