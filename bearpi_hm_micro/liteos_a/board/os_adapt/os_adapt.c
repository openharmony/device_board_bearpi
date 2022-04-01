/*
 * Copyright (c) 2021 Nanjing Xiaoxiongpai Intelligent Technology Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "target_config.h"
#include "los_typedef.h"
#include <unistd.h>
#include "stdlib.h"
#include "stdio.h"
#include "los_process_pri.h"
#include "disk.h"
#include "sys/mount.h"
#include "los_rootfs.h"

#include "los_config.h"
#include "gic_common.h"
#include "los_printf.h"
#include "los_smp.h"
#include "los_vm_map.h"
#include "los_vm_zone.h"
#include "los_vm_boot.h"
#include "los_mmu_descriptor_v6.h"
#include "los_init.h"

#include "mtd_partition.h"

#ifdef LOSCFG_FS_VFS
#include "disk.h"
#endif
#include "los_rootfs.h"
#ifdef LOSCFG_SHELL
#include "shell.h"
#include "shcmd.h"
#endif

#ifdef LOSCFG_DRIVERS_MEM
#include "los_dev_mem.h"
#endif

#ifdef LOSCFG_DRIVERS_HDF
#include "devmgr_service_start.h"
#endif

#ifdef LOSCFG_DRIVERS_HDF_PLATFORM_WATCHDOG
#include "watchdog_if.h"
#endif

#include "los_task.h"

#include "los_bootargs.h"
#include "los_rootfs.h"


#ifdef LOSCFG_DRIVERS_RANDOM
#include "los_random.h"
#include "hisoc/random.h"
#endif

#ifdef LOSCFG_DRIVERS_MEM
int mem_dev_register(void)
{
    return DevMemRegister();
}
#endif

VOID HalClockIrqClear(VOID) {}

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"

#define SLEEP_TIME_MS 60
#define NETIF_SETUP_OVERTIME 100

void net_init(void)
{
    extern void tcpip_init(tcpip_init_done_fn initfunc, void *arg);
    dprintf("\ntcpip_init start\n");
    tcpip_init(NULL, NULL);
    dprintf("\ntcpip_init end\n");
    PRINTK("Ethernet start.");
}


#ifdef LOSCFG_KERNEL_SMP
extern int Mp1SmpSecCoreOn(UINT32 cpuid, UINTPTR entry_point);
STATIC struct SmpOps g_stm32Mp1SmpOps = {
    .SmpCpuOn = Mp1SmpSecCoreOn,
};

STATIC VOID stm32Mp1SmpInit(VOID)
{
    LOS_SmpOpsSet(&g_stm32Mp1SmpOps);
}

LOS_MODULE_INIT(stm32Mp1SmpInit, LOS_INIT_LEVEL_EARLIEST);
#endif

void SystemInit(void)
{
#ifdef LOSCFG_DRIVERS_RANDOM
    dprintf("dev random init ...\n");
    Mp1xxRngInit();
#endif
#ifdef LOSCFG_DRIVERS_MEM
    dprintf("mem dev init ...\n");
    extern int mem_dev_register(void);
    mem_dev_register();
#endif

    dprintf("Date:%s.\n", __DATE__);
    dprintf("Time:%s.\n", __TIME__);

#ifdef LOSCFG_DRIVERS_HDF
    dprintf("DeviceManagerStart start ...\n");
    if (DeviceManagerStart()) {
        PRINT_ERR("No drivers need load by hdf manager!");
    }
    dprintf("DeviceManagerStart end ...\n");
#endif
    net_init();

#ifdef LOSCFG_PLATFORM_ROOTFS
    dprintf("OsMountRootfs start ...\n");
    if (LOS_GetCmdLine()) {
        dprintf("get cmdline error!\n");
    }
    if (LOS_ParseBootargs()) {
        dprintf("parse bootargs error!\n");
    }
    if (OsMountRootfs()) {
        dprintf("mount rootfs error!\n");
    }
    dprintf("OsMountRootfs end ...\n");
#endif

    dprintf("Before PLATFORM_UART ...\n");

#ifdef LOSCFG_DRIVERS_HDF_PLATFORM_UART
    if (virtual_serial_init(TTY_DEVICE) != 0) {
        PRINT_ERR("virtual_serial_init failed");
    }
    if (system_console_init(SERIAL) != 0) {
        PRINT_ERR("system_console_init failed\n");
    }
#endif

    dprintf("After PLATFORM_UART ...\n");

    if (OsUserInitProcess()) {
        PRINT_ERR("Create user init process failed!\n");
        return;
    }
    dprintf("cat log shell end\n");
    return;
}

#ifdef LOSCFG_KERNEL_MMU
LosArchMmuInitMapping g_archMmuInitMapping[] = {
    {
        .phys = SYS_MEM_BASE,
        .virt = KERNEL_VMM_BASE,
        .size = KERNEL_VMM_SIZE,
        .flags = MMU_DESCRIPTOR_KERNEL_L1_PTE_FLAGS,
        .name = "KernelCached",
    },
    {
        .phys = SYS_MEM_BASE,
        .virt = UNCACHED_VMM_BASE,
        .size = UNCACHED_VMM_SIZE,
        .flags = MMU_INITIAL_MAP_NORMAL_NOCACHE,
        .name = "KernelUncached",
    },
    {
        .phys = PERIPH_PMM_BASE,
        .virt = PERIPH_DEVICE_BASE,
        .size = PERIPH_DEVICE_SIZE,
        .flags = MMU_INITIAL_MAP_DEVICE,
        .name = "PeriphDevice",
    },
    {
        .phys = PERIPH_PMM_BASE,
        .virt = PERIPH_CACHED_BASE,
        .size = PERIPH_CACHED_SIZE,
        .flags = MMU_DESCRIPTOR_KERNEL_L1_PTE_FLAGS,
        .name = "PeriphCached",
    },
    {
        .phys = PERIPH_PMM_BASE,
        .virt = PERIPH_UNCACHED_BASE,
        .size = PERIPH_UNCACHED_SIZE,
        .flags = MMU_INITIAL_MAP_STRONGLY_ORDERED,
        .name = "PeriphStronglyOrdered",
    },
    {
        .phys = GIC_PHY_BASE,
        .virt = GIC_VIRT_BASE,
        .size = GIC_VIRT_SIZE,
        .flags = MMU_INITIAL_MAP_DEVICE,
        .name = "GIC",
    },
    {
        .phys = FB_PHY_BASE,
        .virt = FB_VIRT_BASE,
        .size = FB_SIZE,
        .flags = MMU_INITIAL_MAP_DEVICE,
        .name = "FB",
    },
    {0}
};

struct ArchMmuInitMapping *OsGetMmuPartition(const char *name)
{
    UINT32 index = 0;
    UINT32 mmuBlockCount = sizeof(g_archMmuInitMapping) / sizeof(g_archMmuInitMapping[0]);

    for (index = 0; index < (mmuBlockCount - 1); index++) {
        if (strncmp(name, g_archMmuInitMapping[index].name, 20) == 0) {
            return &g_archMmuInitMapping[index];
        }
    }
    return NULL;
}

#endif
