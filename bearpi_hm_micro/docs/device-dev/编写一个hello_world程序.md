# 编写一个hello_world应用

本示例将演示如何在开发板上运行第一个应用程序，输出“Hello World”。

## 编写源码

1.  确定目录结构。

    开发者编写业务时，务必先在./device/board/bearpi/bearpi\_hm\_micro/app路径下新建一个目录（或一套目录结构），用于存放业务源码文件。

    例如：在app下新增业务my\_first\_app，其中hello\_world.c为业务代码，BUILD.gn为编译脚本，具体规划目录结构如下：

    ```
    .
    └── app        
        │── my_first_app
            │── hello_world.c
            └── BUILD.gn

    ```

2.  编写业务代码。

    在hello\_world.c中编写业务代码。

    ```
    #include <stdio.h>
    
    int main(int argc, char **argv)
    {
        printf("\n************************************************\n");
        printf("\n\t\tHello BearPi!\n");
        printf("\n************************************************\n\n");
        
        return 0;
    }
    ```

3.  编写将构建业务代码的BUILD.gn文件。

    如步骤1所述，BUILD.gn文件由三部分内容（目标、源文件、头文件路径）构成，需由开发者完成填写。以my\_first\_app为例，需要创建./device/board/bearpi/bearpi\_hm\_micro/app/BUILD.gn，并完如下配置。

    ```
    import("//build/lite/config/component/lite_component.gni")

    executable("hello_world_lib") {
        output_name = "hello_world"
        sources = [ "hello_world.c" ]
        include_dirs = []
        defines = []
        cflags_c = []
        ldflags = []
    }

    lite_component("my_app") {
        features = [
            ":hello_world_lib",
        ]
    }
    ```

    -   首先导入 gni 组件，将源码hello_world.c编译成hello_world_lib库文件
    -   然后将hello_world_lib打包成 lite_component，命名为my_app组件。

    -   输出的可执行文件名称由output_name定义为hello_world

4. 修改bundle.json配置文件

    修改文件./device/board/bearpi/bearpi_hm_micro/app/bundle.json，新增编译my\_first\_app:my\_app条目，如下所示，"##start##"和"##end##"之间为新增条目（"##start##"和"##end##"仅用来标识位置，添加完配置后删除这两行）：

    ```
    {
        "sub_component": [
            "//device/board/bearpi/bearpi_hm_micro/app/launcher:launcher_hap",
            "//device/board/bearpi/bearpi_hm_micro/app/setting:setting_hap",
            "//device/board/bearpi/bearpi_hm_micro/app/screensaver:screensaver_hap",
            "//device/board/bearpi/bearpi_hm_micro/app/communication:sample",
            ##start##
            "//device/board/bearpi/bearpi_hm_micro/app/my_first_app:my_app"
            ##end##
        ],
    }
    ```


## 运行结果<a name="section18115713118"></a>

示例代码[编译、烧录](BearPi-HM_Micro开发板编译调试.md)后，在命令行输入指令“./bin/hello_world”执行写入的demo程序，会显示如下结果：


![](figures/运行hello_world.png)




在此希望开发者能仔细琢磨并掌握整个流程，有利于学习OpenHarmony的编译框架，以及为后续自主开发应用打下基础
