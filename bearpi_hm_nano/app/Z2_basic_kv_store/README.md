# 公共基础子系统开发 —— kv存储（基于BearPi-HM_Nano开发板）
本示例将演示如何使用kv存储相关的API

## kv_store API分析
本示例主要使用了以下API完成kv存储相关的验证。

### UtilsGetValue()
```c
int UtilsGetValue(const char* key, char* value, unsigned int len);
```
**描述：**

从文件系统或cache中获取键 key 对应的值

**参数：**

|名字|描述|
|:--|:------|
| key | 键  |
| value | 指向存储键所对应的值所在的地址 |
| len | 存放value的存储区大小 |

### UtilsSetValue()
```c
int UtilsSetValue(const char* key, const char* value);
```
**描述：**

新增或更新存储在文件系统或cache中的键 key 所对应的值

**参数：**

|名字|描述|
|:--|:------|
| key | 键  |
| value | 待新增或更新的值 |

### UtilsDeleteValue()
```c
int UtilsDeleteValue(const char* key);
```
**描述：**

删除存储在文件系统或cache中的键 key 所对应的值

**参数：**

|名字|描述|
|:--|:------|
| key | 键  |


## 编译调试

### 修改 BUILD.gn 文件

修改 `device\board\bearpi\bearpi_hm_nano\app` 路径下的 BUILD.gn 文件，指定 `Z2_basic_kv_store` 参与编译。

```r
    # "D11_iot_cloud_oc_agriculture:cloud_oc_agriculture",
    # "D12_iot_cloud_oc_gps:cloud_oc_gps",

    # "Z1_basic_flash_ylc:flash_example",
    "Z2_basic_kv_store:kv_store_example",
```

### 运行结果

示例代码编译烧录后，按下开发板的RESET按键，通过串口助手可以看到kv操作过程。

```sh
hiview init success.
[PARAM][param_service.c:125]ParamServiceTask start
get value failed!
===== key: user, value:  =====
===== key: user, value: Zhang San =====
delete key-value for user success.
get value failed!
===== key: user, value:  =====

```

