# BearPi-HM_Nano开发板基础外设开发——I2C控制NFC芯片
本示例将演示如何在BearPi-HM_Nano开发板上使用I2C协议向NFC芯片写入数据。


## I2C API分析
本示例主要使用了以下API完成I2C采样的功能。
### IoTI2cInit()
```c
unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate);
```
 **描述：**

用指定的频率初始化I2C设备。


**参数：**

|参数名|描述|
|:--|:------| 
| id | I2C设备ID。  |
| baudrate |I2C频率。|

## IoTI2cSetBaudrate()
```c
unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate);
```
 **描述：**

为I2C设备设置频率。

**参数：**

|参数名|描述|
|:--|:------| 
| id | I2C设备ID。  |
| baudrate |I2C频率。|

## IoTI2cWrite()
```c
unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen);

```
 **描述：**

将数据写入I2C设备。


**参数：**

|参数名|描述|
|:--|:------| 
| id | I2C设备ID。  |
| deviceAddr |I2C设备地址。|
| data |表示待写入的数据。|
| dataLen |表示待写入的数据长度。|

## IoTI2cRead()
```c
unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen);
```
 **描述：**

从I2C设备读取数据。读取的数据将保存到i2cData指定的地址。


**参数：**

|参数名|描述|
|:--|:------| 
| id | I2C设备ID。  |
| deviceAddr |I2C设备地址。|
| data |表示指向要读取的数据的指针。|
| dataLen |表示要读取的数据长度。|



## 硬件设计
如下图所示，NFC芯片使用的是I2C协议，I2C1_SCL与GPIO_1相连接，I2C1_SDA与GPIO_0相连接，所以需要编写软件使用GPIO_0和GPIO_1产生I2C信号去控制NFC芯片。

![E53接口电路](../../docs/figures/B5_basic_i2c_nfc/NFCCircuit.png "E53接口电路")

## 软件设计

**主要代码分析**

这部分代码为I2C初始化的代码，首先用 `IoTGpioSetFunc()` 函数将GPIO_0复用为I2C1_SDA，GPIO_1复用为I2C1_SCL。然后调用IoTI2cInit()函数初始化I2C1端口，最后使用 `I2cSetBaudrate()` 函数设置I2C1的频率为400kbps。
```c
    //GPIO_0 multiplexed to I2C1_SDA
    IoTGpioInit(0);
    IoTGpioSetFunc(0, WIFI_IOT_IO_FUNC_GPIO_0_I2C1_SDA);

    //GPIO_1 multiplexed to I2C1_SCL
    IoTGpioInit(1);
    IoTGpioSetFunc(1, WIFI_IOT_IO_FUNC_GPIO_1_I2C1_SCL);

    //baudrate: 400kbps
    IoTI2cInit(WIFI_IOT_I2C_IDX_1, 400000);
```

这部分的代码是向NFC芯片写入数据，当需要写入2个记录时，第2个记录的位置需要用`NDEFLastPos`来定义；当需要写入3个记录时，第2个和第3个记录的位置分别需要用`NDEFMiddlePos`和`NDEFLastPos`来定义。
```c
ret = storeText(NDEFFirstPos, (uint8_t *)TEXT);
if (ret != 1) {
    printf("NFC Write Data Failed :%d ",ret);
}
ret = storeUrihttp(NDEFLastPos, (uint8_t *)WEB);
if (ret != 1) {
    printf("NFC Write Data Failed :%d ",ret);
}
```


## 编译调试

### 修改 BUILD.gn 文件


修改 `device\board\bearpi\bearpi_hm_nano\app` 路径下 BUILD.gn 文件，指定 `i2c_example` 参与编译。

```r
#"B1_basic_led_blink:led_example",
#"B2_basic_button:button_example",
#"B3_basic_pwm_led:pwm_example",
#"B4_basic_adc:adc_example",
"B5_basic_i2c_nfc:i2c_example",
#"B6_basic_uart:uart_example",
```


### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，并请使用带有NFC功能的手机靠近开发板，能读取数据。
```c
=======================================

***********I2C_NFC_example**********

=======================================

Please use the mobile phone with NFC function close to the development board!

=======================================

***********I2C_NFC_example**********

=======================================

Please use the mobile phone with NFC function close to the development board!
```
