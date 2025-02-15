# BearPi-HM_Nano开发板OpenHarmony内核编程开发——信号量
本示例将演示如何在BearPi-HM_Nano开发板上使用cmsis 2.0 接口通过信号量同时从不同的线程访问共享资源。


## Semaphore API分析

###  osSemaphoreNew()

```c
osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
```
**描述：**

osSemaphoreNew 函数创建并初始化一个信号量对象，该对象用于管理对共享资源的访问，并返回指向信号量对象标识符的指针或在发生错误时返回 NULL 。可以在 RTOS 启动之前（调用 osKernelStart）安全地调用该函数，但不能在内核初始化之前（调用 osKernelInitialize）调用该函数。
> **注意** :不能在中断服务中调用该函数。


**参数：**

|参数名|描述|
|:--|:------| 
| max_count |可用令牌的最大数量。  |
| initial_count |可用令牌的初始数量。  |
| attr |信号量的属性; 空:默认值。  |

###  osSemaphoreRelease()

```c
osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
```
**描述：**
函数osSemaphoreRelease释放由参数semaphore_id指定的信号量令牌，最多释放个数为初始设置的最大值。

> **注意** :该函数可以在中断服务例程中调用。


**参数：**

|参数名|描述|
|:--|:------| 
| semaphore_id | 由osSemaphoreNew获得的信号量ID。  |


###  osSemaphoreAcquire()

```c
osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
```
**描述：**
阻塞函数osSemaphoreAcquire在超时时间结束前一直等待，直到由参数semaphore_id指定的信号量令牌可用为止。如果一个信号量令牌可用，该函数立即返回并递减可用的信号量令牌计数。

> **注意** :如果参数timeout设置为0，可以在中断服务例程中调用。


**参数：**

|参数名|描述|
|:--|:------| 
| semaphore_id | 由osSemaphoreNew获得的信号量ID。  |
| timeout | 超时值。 |


## 软件设计

**主要代码分析**

在SemaphoreExample()函数中，通过osSemaphoreNew()函数创建了g_semaphoreId信号量，并初始化信号量令牌数为4。Semaphore2Thread()和Semaphore3Thread()函数中，刚开始阻塞等待g_semaphoreId信号量中可用令牌。只有当Semaphore1Thread()函数中通过osSemaphoreRelease()函数释放两个信号量令牌后，Semaphore2Thread()和Semaphore3Thread()才能继续同步运行。若Semaphore1Thread()函数中只释放一个信号量令牌，那么Semaphore2Thread()和Semaphore3Thread()只能轮流执行。
```c
void Semaphore1Thread(void)
{
    while (1) {
        // release Semaphores twice so that Semaphore2Thread and Semaphore3Thread can execute synchronously
        osSemaphoreRelease(g_semaphoreId);

        // if the Semaphore is released only once, Semaphore2Thread and Semaphore3Thread will run alternately.
        osSemaphoreRelease(g_semaphoreId);

        printf("Semaphore1Thread Release  Semap \n");
        osDelay(THREAD_DELAY_1S);
    }
}
void Semaphore2Thread(void)
{
    while (1) {
        // wait Semaphore
        osSemaphoreAcquire(g_semaphoreId, osWaitForever);

        printf("Semaphore2Thread get Semap \n");
        osDelay(THREAD_DELAY_10MS);
    }
}

void Semaphore3Thread(void)
{
    while (1) {
        // wait Semaphore
        osSemaphoreAcquire(g_semaphoreId, osWaitForever);

        printf("Semaphore3Thread get Semap \n");
        osDelay(THREAD_DELAY_10MS);
    }
}

/**
 * @brief Main Entry of the Semaphore Example
 *
 */
void SemaphoreExample(void)
{
    osThreadAttr_t attr;

    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = THREAD_STACK_SIZE;
    attr.priority = THREAD_PRIO;

    attr.name = "Semaphore1Thread";
    if (osThreadNew((osThreadFunc_t)Semaphore1Thread, NULL, &attr) == NULL) {
        printf("Failed to create Semaphore1Thread!\n");
    }

    attr.name = "Semaphore2Thread";
    if (osThreadNew((osThreadFunc_t)Semaphore2Thread, NULL, &attr) == NULL) {
        printf("Failed to create Semaphore2Thread!\n");
    }

    attr.name = "Semaphore3Thread";
    if (osThreadNew((osThreadFunc_t)Semaphore3Thread, NULL, &attr) == NULL) {
        printf("Failed to create Semaphore3Thread!\n");
    }

    g_semaphoreId = osSemaphoreNew(SEM_MAX_COUNT, 0, NULL);
    if (g_semaphoreId == NULL) {
        printf("Failed to create Semaphore!\n");
    }
}
```

## 编译调试

### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app`路径下 BUILD.gn 文件，指定 `semaphore_example` 参与编译。

```r
#"A1_kernal_thread:thread_example",
#"A2_kernel_timer:timer_example",
#"A3_kernel_event:event_example",
#"A4_kernel_mutex:mutex_example",
"A5_kernel_semaphore:semaphore_example",
#"A6_kernel_message:message_example",
```

### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，Semaphore1Thread一次释放两个信号量令牌，Semaphore2Thread和Semaphore3Thread同步执行。
```
Semaphore1Thread Release  Semap 
Semaphore2Thread get Semap 
Semaphore3Thread get Semap 
Semaphore1Thread Release  Semap 
Semaphore2Thread get Semap 
Semaphore3Thread get Semap 
Semaphore1Thread Release  Semap 
Semaphore2Thread get Semap 
Semaphore3Thread get Semap 
```
