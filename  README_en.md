# device_board_bearpi

# Introduction
This warehouse is used to place the content related to the BearPi development board，So far, There are two development boards available for OpenHarmony.They are BearPi-HM Nano and BearPi-HM Micro.BearPi-HM Nano development board is mini system based on Hisilicon Hi3861. BearPi-HM Micro development board is small system based on STMicroelectronics STM32MP157.Please refer to each README of each development board for details.

- [mini system BearPi-HM Nano development board description](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发板介绍.md)

- [small system BearPi-HM Micro development board description](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发板介绍.md)

## Development environment setup

For the development environment setup tutorial, see readme of each development board.

- [mini system BearPi-HM Nano development environment construction ](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发搭建环境.md)

- [small system BearPi-HM Micro development environment construction](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发环境搭建.md)

## Debugging

Please refer to each README of each development board.

- [mini system BearPi-HM Nano compilation and debugging](bearpi_hm_nano/docs/quick-start/BearPi-HM_Nano开发板编译调试.md)

- [small system BearPi-HM Micro compilation and debugging](bearpi_hm_micro/docs/device-dev/BearPi-HM_Micro开发板编译调试.md)

## Directory Structure
```
/device/board/bearpi
├── bearpi_hm_micro                 # Bearpi mini system BearPi-HM Micro STM32MP157 development board
    └── app                         # application case
    └── docs                        # docs
    └── hal                         # Peripheral HDI adaptation
    └── liteos_a                    # liteos_m kernel adaptation
    └── tools                       # tf-a、uboot image ghost
├── bearpi_hm_nano                  # Bearpi small system BearPi-HM Nano Hi3861 development board
    └── app                         # application case
    └── config                      # Drive Differentiation Enablement Configuration
    └── docs                        # docs
    └── iot_hardware_hals           # Peripheral adaptation
    └── liteos_m                    # liteos_m kernel adaptation
    └── third_party                 # third_party
```
## Repositories Involved 

**[device_board_bearpi](https://gitee.com/openharmony/device_board_bearpi)**

- [device_soc_st](https://gitee.com/openharmony/device_soc_st)

- [vendor_bearpi](https://gitee.com/openharmony/vendor_bearpi)