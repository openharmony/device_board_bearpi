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
 */

#include <stdio.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "kv_store.h"

#define BOOT_WAIT_TIME 200

static void ShowValue(const char* key)
{
    if (!key) {
        return;
    }

    char buf[512] = {0};

    if (UtilsGetValue(key, buf, sizeof(buf)-1) < 0) {
        printf("get value failed!\n");
    }
    printf("===== key: %s, value: %s =====\n", key, buf);
}

static void KvStoreExampleEntry(void)
{
    const char* usernameKey = "user";
    const char* usernameValue = "Zhang San";

    // wait a while for checking log easily
    osDelay(BOOT_WAIT_TIME);

    // 1. get value failed if no key-value exist
    ShowValue(usernameKey);

    // 2. save key-value
    if (UtilsSetValue(usernameKey, usernameValue) < 0) {
        printf("set username value failed!\n");
    }

    // 3. get value success if key-value exist
    ShowValue(usernameKey);

    // 4. get value failed after key-value deleted
    if (UtilsDeleteValue(usernameKey) < 0) {
        printf("delete key-value for %s failed!\n", usernameKey);
    } else {
        printf("delete key-value for %s success.\n", usernameKey);
    }

    ShowValue(usernameKey);
}

APP_FEATURE_INIT(KvStoreExampleEntry);