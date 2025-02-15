# BearPi-HM_Nano开发板WiFi编程开发——UDP客户端
本示例将演示如何在BearPi-HM_Nano开发板上使用socket编程创建UDP客户端，向服务端发送固定消息并接收服务端返回的消息。


## socket API分析
本案例主要使用了以下几个API完socket编程实验。
### socket()

```c
sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) //AF_INT:ipv4, SOCK_DGRAM:udp协议
```
**描述：**

在网络编程中所需要进行的第一件事情就是创建一个socket，无论是客户端还是服务器端，都需要创建一个socket，该函数返回socket文件描述符，类似于文件描述符。socket是一个结构体，被创建在内核中。
### sendto()
```c
int sendto(socket s, const void *msg, int len, unsigned int flags, const struct sockaddr *to , int token);
```
**描述：**

sendto() 用来将数据由指定的socket传给对方主机。参数s为已建立连接的socket。参数msg指向待发送的数据，参数flags 一般设0。

### recvfrom()
```c
int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
```
**描述：**
从指定地址接收UDP数据包。


**参数：**

|参数名|描述|
|:--|:------| 
| s | socket描述符。  |
| buf | 消息接收后的缓冲区。  |
| len | 缓冲区的容量。  |
| flags | 该参数一般为0。  |
| from | 指向发送数据的对端地址信息的结构体，如果为NULL，不储存对端地址。  |
| fromlen | 指向from结构体长度值，如果from为NULL，该值也应为NULL。  |



## 软件设计

**主要代码分析**

完成UDP的通信过程需要以下几步。

1. 通过 `socket` 接口创建一个socket，`AF_INT`表示ipv4，`SOCK_DGRAM`表示使用dup协议。
2. 调用 `sendto` 接口发送数据到服务端。
3. 调用 `recvfrom` 接口接收服务端发来的数据。


```c
static void UDPClientTask(void)
{
    //在sock_fd 进行监听，在 new_fd 接收新的链接
    int sock_fd;

    //服务器的地址信息
    struct sockaddr_in send_addr;
    socklen_t addr_length = sizeof(send_addr);
    char recvBuf[512];

    //连接Wifi
    WifiConnect(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);

    //创建socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("create socket failed!\r\n");
        exit(1);
    }

    //初始化预连接的服务端地址
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(CONFIG_SERVER_PORT);
    send_addr.sin_addr.s_addr = inet_addr(CONFIG_SERVER_IP);
    addr_length = sizeof(send_addr);

    //总计发送 count 次数据
    while (1) {
        bzero(recvBuf, sizeof(recvBuf));

        //发送数据到服务远端
        sendto(sock_fd, send_data, strlen(send_data), 0, (struct sockaddr *)&send_addr, addr_length);

        //线程休眠一段时间
        sleep(TASK_DELAY_10S);

        //接收服务端返回的字符串
        recvfrom(sock_fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *)&send_addr, &addr_length);
        printf("%s:%d=>%s\n", inet_ntoa(send_addr.sin_addr), ntohs(send_addr.sin_port), recvBuf);
    }

    //关闭这个 socket
    closesocket(sock_fd);
}
```

## 编译调试

### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app` 路径下 BUILD.gn 文件，指定 `udp_client` 参与编译。
```r
#"D1_iot_wifi_sta:wifi_sta",
#"D2_iot_wifi_sta_connect:wifi_sta_connect",
"D3_iot_udp_client:udp_client",
#"D4_iot_tcp_server:tcp_server",
#"D5_iot_mqtt:iot_mqtt",        
#"D6_iot_cloud_oc:oc_mqtt",
```


### 运行结果<a name="section18115713118"></a>

使用 Socket tool 创建UDP服务端用于测试，如下图所示。

![创建UDP服务端](../../docs/figures/D3_iot_udp_client/createupdservice.png)

示例代码编译烧录后，按下开发板的RESET按键，在socket tool的数据发送窗口输入要发送的数据，点击发送后开发板会回复固定消息，如下图所示，且开发板收到消息后会通过日志打印出来。

```
192.168.0.175:8888=>Hello! BearPi-HM_nano UDP Client!
```

![UDP发送数据](../../docs/figures/D3_iot_udp_client/UPDSendData.png)
