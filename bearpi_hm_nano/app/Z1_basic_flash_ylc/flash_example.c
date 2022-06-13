/*
 * Copyright (c) 2020 Nanjing Xiaoxiongpai Intelligent Technology Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Date : 2022-04-29
 * Author : ylc
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_flash.h"
#include "iot_errno.h"

// hi3861
// flash地址0x400000 - 0x600000
// 每页512字节

#define TRUE 1

#define FLASH_READ_WRITE_ADDR (0x200000 - BUFSIZ)

static char *str = "flash test@ylc: Hello, BearPi!";
static uint8_t test_buf[BUFSIZ];

void in_flash_read(uint32_t addr, uint8_t *buf, uint16_t len)
{
    IoTFlashRead(addr, len, (uint8_t *)buf);
}

void in_flash_write(uint32_t addr, uint8_t *buf, uint16_t len)
{
    IoTFlashWrite(addr, len, (uint8_t *)buf, TRUE);
}

/**************测试flash读写**************/
static void in_flash_test(void)
{
    // 将数据写入flash末尾
    in_flash_write(FLASH_READ_WRITE_ADDR, (uint8_t *)str, strlen(str) + 1);
    // 将最后200字节数据读到空白缓存中
    in_flash_read(FLASH_READ_WRITE_ADDR, (uint8_t *)test_buf, BUFSIZ);
    // 展示读取结果
    printf("%s", test_buf);
}

APP_FEATURE_INIT(in_flash_test);