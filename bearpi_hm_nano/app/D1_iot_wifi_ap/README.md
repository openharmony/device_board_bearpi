# BearPi-HM_Nano开发板WiFi编程开发——Wifi AP 热点


本示例将演示如何在BearPi-HM_Nano开发板上编写一个创建Wifi热点的程序。


## Wifi API分析
本案例主要使用了以下几个API完成Wifi热点的创建。

### RegisterWifiEvent()
```c
WifiErrorCode RegisterWifiEvent(WifiEvent *event);
```
 **描述：**

为指定的Wi-Fi事件注册回调函数。当WifiEvent中定义的Wi-Fi事件发生时，将调用已注册的回调函数。

**参数：**

|参数名|描述|
|:--|:------| 
| event | 表示要注册回调函数的事件。  |


### EnableHotspot()
```c
WifiErrorCode EnableHotspot(void);
```

**描述：**

启用Wifi热点模式。

### SetHotspotConfig()
```c
WifiErrorCode SetHotspotConfig(const HotspotConfig *config);
```
**描述：**

设置指定热点的配置。

### IsHotspotActive()
```c
int IsHotspotActive(void);
```
**描述：**

检查热点模式是否启用。

### GetStationList()
```c
WifiErrorCode GetStationList(StationInfo *result, unsigned int *size);
```
**描述：**

获取连接到该热点的一系列STA。

**参数：**

|参数名|描述|
|:--|:------| 
| result | 表示连接到该热点的STA列表。  |
| size | 表示连接到该热点的STA数量。  |



## 软件设计

**主要代码分析**

完成Wifi热点的扫描需要以下几步。

1. 通过 `RegisterWifiEvent` 接口向系统注册热点状态改变事件、STA连接到热点事件、STA断开热点事件。
    
    * `OnHotspotStateChangedHandler` 用于绑定热点状态改变事件，该回调函数有一个参数 `state` 。

        * state表示是否开启Wifi热点模式，取值为1或0，1表示已启用Wifi热点模式，0表示已禁用Wifi热点模式。

    * `OnHotspotStaLeaveHandler` 用于绑定STA断开热点事件,当有STA断开热点，该回调函数会打印出已断开的STA的MAC地址。
    * `OnHotspotStaJoinHandler` 用于绑定STA连接到热点事件，当有新的STA连接热点时，该回调函数会打印出刚连接的STA的MAC地址。
2. 调用 `SetHotspotConfig` 接口，设置指定热点的配置。
3. 调用 `EnableHotspot` 接口，使能 Wifi 热点模式。


```c
static BOOL WifiAPTask(void)
{
    //延时2S便于查看日志
    osDelay(TASK_DELAY_2S);

    //注册wifi事件的回调函数
    g_wifiEventHandler.OnHotspotStaJoin = OnHotspotStaJoinHandler;
    g_wifiEventHandler.OnHotspotStaLeave = OnHotspotStaLeaveHandler;
    g_wifiEventHandler.OnHotspotStateChanged = OnHotspotStateChangedHandler;
    error = RegisterWifiEvent(&g_wifiEventHandler);
    if (error != WIFI_SUCCESS) {
        printf("RegisterWifiEvent failed, error = %d.\r\n", error);
        return -1;
    }
    printf("RegisterWifiEvent succeed!\r\n");
        //检查热点模式是否使能
    if (IsHotspotActive() == WIFI_HOTSPOT_ACTIVE) {
        printf("Wifi station is  active.\r\n");
        return -1;
    }
    //设置指定的热点配置
    HotspotConfig config = { 0 };

    strcpy_s(config.ssid, strlen(AP_SSID) + 1, AP_SSID);
    strcpy_s(config.preSharedKey, strlen(AP_PSK) + 1, AP_PSK);
    config.securityType = WIFI_SEC_TYPE_PSK;
    config.band = HOTSPOT_BAND_TYPE_2G;
    config.channelNum = CHANNEL_NUM;

    error = SetHotspotConfig(&config);
    if (error != WIFI_SUCCESS) {
        printf("SetHotspotConfig failed, error = %d.\r\n", error);
        return -1;
    }
    printf("SetHotspotConfig succeed!\r\n");

    //启动wifi热点模式
    error = EnableHotspot();
    if (error != WIFI_SUCCESS) {
        printf("EnableHotspot failed, error = %d.\r\n", error);
        return -1;
    }
    printf("EnableHotspot succeed!\r\n");

    StartUdpServer();
}
```

## 编译调试

### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app` 路径下 BUILD.gn 文件，指定 `wifi_ap` 参与编译。
```r
"D1_iot_wifi_ap:wifi_ap",
#"D2_iot_wifi_sta_connect:wifi_sta_connect",        
#"D3_iot_udp_client:udp_client",
#"D4_iot_tcp_server:tcp_server",
#"D5_iot_mqtt:iot_mqtt",        
#"D6_iot_cloud_oc:oc_mqtt",
```


### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，使用手机去连接该热点，会打印出以下信息。
```
RegisterWifiEvent succeed!
SetHotspotConfig succeed!
HotspotStateChanged:state is 1.
wifi hotspot active.
EnableHotspot succeed!
Waiting to receive data...
+NOTICE:STA CONNECTED
HotspotStaJoin: macAddress=EC:5C:68:5B:2D:3D, reason=0.
```


