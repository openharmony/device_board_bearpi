# BearPi-HM_Nano开发板WiFi编程开发——Wifi连接热点


本示例将演示如何在BearPi-HM_Nano开发板上编写一个Wifi连接热点的业务程序。

## Wifi API分析
本案例主要使用了以下几个API完成Wifi联网。
### RegisterWifiEvent()
```c
WifiErrorCode RegisterWifiEvent(WifiEvent *event);
```
 **描述：**
为指定的Wi-Fi事件注册回调函数。当WifiEvent中定义的Wi-Fi事件发生时，将调用已注册的回调函数。

**参数：**

|参数名|描述|
|:--|:------| 
| event | 表示要注册回调的事件。  |


### EnableWifi()
```c
WifiErrorCode EnableWifi(void);
```
**描述：**

启用STA模式。

### AddDeviceConfig()
```c
WifiErrorCode AddDeviceConfig(const WifiDeviceConfig *config, int *result);
```
**描述：**

添加用于连接到指定热点的配置，此函数生成一个networkId。

**参数：**

|参数名|描述|
|:--|:------| 
| config | 表示待添加的热点配置。 |
| result | 表示生成的networkId。每个networkId匹配一个热点配置。  |

### ConnectTo()
```c
WifiErrorCode ConnectTo(int networkId);
```
**描述：**

连接到和networkId匹配的热点。

**参数：**

|参数名|描述|
|:--|:------| 
| networkId | 表示与目标热点匹配的网络id。  |


### netifapi_netif_find()
```c
struct netif *netifapi_netif_find(const char *name);
```
**描述：**

获取netif用于IP操作。

### dhcp_start()

```c
err_t dhcp_start(struct netif *netif);
```

**描述：**

启动DHCP客户端, 用于动态获取IP地址。


## 软件设计

**主要代码分析**

完成Wifi热点的连接需要以下几步

1. 通过 `RegisterWifiEvent` 接口向系统注册扫描状态变化回调函数，用于接收扫描状态变化的通知，如扫描动作是否完成等。
    
* `OnWifiConnectionChangedHandler` 用于绑定连接状态变化的回调函数，该回调函数有两个参数 `state` 和 `info` 。

    * state表示连接状态，取值为0和1，1表示热点连接成功。

    * info表示Wi-Fi连接信息，包含以下参数。


        |参数名|描述|
        |:--|:------| 
        | ssid [WIFI_MAX_SSID_LEN] | 连接的热点名称。 |
        | bssid [WIFI_MAC_LEN] | 连接的热点MAC地址。  |
        | rssi | 接收信号强度(RSSI)。  |
        | connState | Wifi连接状态。  |
        | disconnectedReason | Wi-Fi断开的原因。  |


2. 调用 `EnableWifi` 接口，使能 Wifi。
3. 调用 `AddDeviceConfig` 接口，配置连接的热点信息。
4. 调用 `ConnectTo` 接口，连接到指定networkId对应的热点。
5. 调用 `WaitConnectResult` 接口等待，该函数中会有15s的时间去轮询连接成功标志位 `g_ConnectSuccess`，当`g_ConnectSuccess` 为 1 时退出等待。
6. 调用 `netifapi_netif_find` 接口，获取 netif 用于 IP 操作。
7. 调用 `dhcp_start` 接口，启动 DHCP客户端, 获取 IP。

```c
static BOOL WifiStaTask(void)
{
    unsigned int size = WIFI_SCAN_HOTSPOT_LIMIT;

    //初始化WIFI
    if (WiFiInit() != WIFI_SUCCESS) {
        printf("WiFiInit failed, error = %d\r\n", error);
        return -1;
    }
    //分配空间，保存WiFi信息
    WifiScanInfo *info = malloc(sizeof(WifiScanInfo) * WIFI_SCAN_HOTSPOT_LIMIT);
    if (info == NULL) {
        return -1;
    }
    //轮询查找WiFi列表
    do {
        Scan();
        WaitScanResult();
        error = GetScanInfoList(info, &size);
    } while (g_staScanSuccess != 1);
    //打印WiFi列表
    printf("********************\r\n");
    for (uint8_t i = 0; i < g_ssid_count; i++) {
        printf("no:%03d, ssid:%-30s, rssi:%5d\r\n", i + 1, info[i].ssid, info[i].rssi);
    }
    printf("********************\r\n");
    //连接指定的WiFi热点
    for (uint8_t i = 0; i < g_ssid_count; i++) {
        if (WifiConnectAp(SELECT_WIFI_SSID, SELECT_WIFI_PASSWORD, info, i) == WIFI_SUCCESS) {
            printf("WiFi connect succeed!\r\n");
            break;
        }

        if (i == g_ssid_count - 1) {
            printf("ERROR: No wifi as expected\r\n");
            while (1)
                osDelay(DHCP_DELAY);
        }
    }

    //启动DHCP
    if (g_lwip_netif) {
        dhcp_start(g_lwip_netif);
        printf("begain to dhcp\r\n");
    }
    //等待DHCP
    for (;;) {
        if (dhcp_is_bound(g_lwip_netif) == ERR_OK) {
            printf("<-- DHCP state:OK -->\r\n");
            //打印获取到的IP信息
            netifapi_netif_common(g_lwip_netif, dhcp_clients_info_show, NULL);
            break;
        }
        osDelay(DHCP_DELAY);
    }

    //执行其他操作
    for (;;) {
        osDelay(DHCP_DELAY);
    }
}
```

## 编译调试
### 修改对接热点的账号密码
修改`wifi_sta_connect.c`第51行和52行的热点账号密码。
```c
#define SELECT_WIFI_SSID "BearPi"
#define SELECT_WIFI_PASSWORD "0987654321"
```
### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app` 路径下 BUILD.gn 文件，指定 `wifi_sta_connect` 参与编译。
```r
#"D1_iot_wifi_sta:wifi_sta",
"D2_iot_wifi_sta_connect:wifi_sta_connect",        
#"D3_iot_udp_client:udp_client",
#"D4_iot_tcp_server:tcp_server",
#"D5_iot_mqtt:iot_mqtt",        
#"D6_iot_cloud_oc:oc_mqtt",
```


### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手查看日志，会打印连接到的Wifi热点信息。
```
<--System Init-->

<--Wifi Init-->

register wifi event succeed!

+NOTICE:SCANFINISH
WaitScanResult:wait success[1]s


Select:  2 wireless, Waiting...

+NOTICE:CONNECTED
callback function for wifi connect

WaitConnectResult:wait success[1]s
WiFi connect succeed!

begain to dhcp

<-- DHCP state:Inprogress -->

<-- DHCP state:OK -->

server :
	server_id : 192.168.0.1
	mask : 255.255.255.0, 1
	gw : 192.168.0.1
	T0 : 7200
	T1 : 3600
	T2 : 6300
clients <1> :
	mac_idx mac             addr            state   lease   tries   rto     
	0       801131801388    192.168.0.151   10      0       1       4       

```

