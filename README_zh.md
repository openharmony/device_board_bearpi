# device_board_bearpi
-   [简介](#section469617221261)
-   [开发环境搭建](#section469617221262)
-   [编译调试](#section469617221263)
-   [目录](#section161941989596)
-   [相关仓](#section641143415335)
## 简介<a name="section469617221261"></a>

本仓用于放置小熊派BearPi开发板相关内容，目前，小熊派已有两款开发板适配OpenHarmony，分别是基于海思Hi3861的轻量系统BearPi-HM Nano开发板和基于意法半导体STM32MP157的小型系统BearPi-HM Micro开发板，详情请参见各开发板readme。

[轻量系统BearPi-HM Nano开发板介绍](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发板介绍.md)


[小型系统BearPi-HM Micro开发板介绍](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发板介绍.md)

## 开发环境搭建<a name="section469617221262"></a>

开发环境搭建教程请参见各开发板readme。

[轻量系统BearPi-HM Nano开发环境搭建](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发搭建环境.md)

[小型系统BearPi-HM Micro开发环境搭建](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发环境搭建.md)

## 编译调试<a name="section469617221263"></a>

请参见各开发板readme。

[轻量系统BearPi-HM Nano开发板编译调试](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发板编译调试.md)

[小型系统BearPi-HM Micro开发板编译调试](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发板编译调试.md)
## 目录<a name="section161941989596"></a>
```
/device/board/bearpi
├── bearpi_hm_micro                 # 小熊派小型系统STM32MP157开发板
    └── app                         # 应用案例
    └── docs                        # 文档
    └── hal                         # 外设HDI适配
    └── liteos_a                    # liteos_a 内核适配
    └── tools                       # tf-a、uboot镜像
├── bearpi_hm_nano                  # 小熊派轻量系统Hi3861开发板
    └── app                         # 应用案例
    └── config                      # 驱动差异化使能配置
    └── docs                        # 文档
    └── iot_hardware_hals           # 外设适配
    └── liteos_m                    # liteos_m 内核适配
    └── third_party                 # 三方库
```
## 相关仓<a name="section641143415335"></a>

**[device_board_bearpi](https://gitee.com/openharmony/device_board_bearpi)**

[device_soc_st](https://gitee.com/openharmony/device_soc_st)

[vendor_bearpi](https://gitee.com/openharmony/vendor_bearpi)
