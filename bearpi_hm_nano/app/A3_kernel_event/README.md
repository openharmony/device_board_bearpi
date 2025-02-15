# BearPi-HM_Nano开发板OpenHarmony内核编程开发——事件标志
本示例将演示如何在BearPi-HM_Nano开发板上使用cmsis 2.0 接口使用事件标志同步线程。


## EventFlags API分析


### osEventFlagsNew()

```c
osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
```
**描述：**

osEventFlagsNew函数创建了一个新的事件标志对象，用于跨线程发送事件，并返回事件标志对象标识符的指针，或者在出现错误时返回NULL。可以在RTOS启动(调用 osKernelStart)之前安全地调用该函数，但不能在内核初始化 (调用 osKernelInitialize)之前调用该函数。
> **注意** :不能在中断服务中调用该函数。


**参数：**

|参数名|描述|
|:--|:------| 
| attr |事件标志属性;空:默认值.  |

### osEventFlagsSet()

```c
uint32_t osEventFlagsSet(osEventFlagsId_t ef_id,uint32_t flags)
```
**描述：**
osEventFlagsSet函数在一个由参数ef_id指定的事件标志对象中设置由参数flags指定的事件标志。

> **注意** :不能在中断服务中调用该函数。


**参数：**

|参数名|描述|
|:--|:------| 
| ef_id | 由osEventFlagsNew获得的事件标志ID 。 |
| flags | 指定设置的标志。 |

### osEventFlagsWait()

```c
uint32_t osEventFlagsWait(osEventFlagsId_t ef_id,uint32_t flags,uint32_t options,uint32_t timeout)
```
**描述：**
osEventFlagsWait函数挂起当前运行线程，直到设置了由参数ef_id指定的事件对象中的任何或所有由参数flags指定的事件标志。当这些事件标志被设置，函数立即返回。否则，线程将被置于阻塞状态。

> **注意** :如果参数timeout设置为0，可以在中断服务例程中调用。


**参数：**

|参数名|描述|
|:--|:------| 
| ef_id | 由osEventFlagsNew获得的事件标志ID。  |
| flags | 指定要等待的标志。 |
| options | 指定标志选项。 |
| timeout | 超时时间，0表示不超时。 |


## 软件设计

**主要代码分析**

在EventExample函数中，通过osEventFlagsNew()函数创建了事件标志ID，EventReceiverThread()函数中通过osEventFlagsWait()函数一直将线程置于阻塞状态，等待从EventSenderThread()函数中通过osEventFlagsSet()函数每隔1s设置的标志，实现任务间的同步。

```c
/**
 * @brief Event sender thread used to set event flag
 *
 */
void EventSenderThread(void)
{
    while (1) {
        osEventFlagsSet(g_eventFlagsId, FLAGS_MSK1);

        // suspend thread
        osThreadYield();

        osDelay(SEND_THREAD_DELAY_1S);
    }
}

/**
 * @brief Event receiver thread blocking wait event flag
 *
 */
void EventReceiverThread(void)
{
    uint32_t flags;

    while (1) {
        flags = osEventFlagsWait(g_eventFlagsId, FLAGS_MSK1, osFlagsWaitAny, osWaitForever);
        printf("Receive Flags is %d\n", flags);
    }
}

/**
 * @brief Main Entry of the Event Example
 *
 */
static void EventExample(void)
{
    g_eventFlagsId = osEventFlagsNew(NULL);
    if (g_eventFlagsId == NULL) {
        printf("Failed to create EventFlags!\n");
    }

    osThreadAttr_t attr;

    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = THREAD_STACK_SIZE;
    attr.priority = THREAD_PRIO;

    attr.name = "EventSenderThread";
    if (osThreadNew(EventSenderThread, NULL, &attr) == NULL) {
        printf("Failed to create EventSenderThread!\n");
    }

    attr.name = "EventReceiverThread";
    if (osThreadNew(EventReceiverThread, NULL, &attr) == NULL) {
        printf("Failed to create EventReceiverThread!\n");
    }
}

```

## 编译调试

### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app`路径下 BUILD.gn 文件，指定 `event_example` 参与编译。

```r
#"A1_kernal_thread:thread_example",
#"A2_kernel_timer:timer_example",
"A3_kernel_event:event_example",
#"A4_kernel_mutex:mutex_example",
#"A5_kernel_semaphore:semaphore_example",
#"A6_kernel_message:message_example",
```

### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，会每隔1s输出一次日志。
```
Receive Flags is 1
Receive Flags is 1
Receive Flags is 1
Receive Flags is 1
Receive Flags is 1
```
