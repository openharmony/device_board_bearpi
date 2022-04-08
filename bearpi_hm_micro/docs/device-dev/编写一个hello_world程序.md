# 编写一个hello_world应用

本示例将演示如何在开发板上运行第一个应用程序，输出“Hello World”。

## 编写源码

1.  确定目录结构。

    开发者编写业务时，务必先在./applications/sample路径下新建一个目录（或一套目录结构），用于存放业务源码文件。

    例如：在app下新增业务my\_first\_app，其中hello\_world.c为业务代码，BUILD.gn为编译脚本，具体规划目录结构如下：

    ```
    .
    └── applications        
        └── sample
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

    如步骤1所述，BUILD.gn文件由三部分内容（目标、源文件、头文件路径）构成，需由开发者完成填写。以my\_first\_app为例，需要创建./applications/sample/my\_first\_app/BUILD.gn，并完如下配置。

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

4. 添加新组件

    修改文件build/lite/components/applications.json，添加组件my_sample的配置，如下所示为applications.json文件片段，"##start##"和"##end##"之间为新增配置（"##start##"和"##end##"仅用来标识位置，添加完配置后删除这两行）：

    ```
    {
    "components": [
        ##start##
        {
            "component": "my_sample",
            "description": "my samples",
            "optional": "true",
            "dirs": [
            "applications/sample/my_first_app"
            ],
            "targets": [
            "//applications/sample/my_first_app:my_app"
            ],
            "rom": "",
            "ram": "",
            "output": [],
            "adapted_kernel": [ "liteos_a" ],
            "features": [],
            "deps": {
            "components": [],
            "third_party": [ ]
            }
        },
        ##end##
        {
        "component": "camera_sample_communication",
        "description": "Communication related samples.",
        "optional": "true",
        "dirs": [
            "applications/sample/camera/communication"
        ],
        "targets": [
            "//applications/sample/camera/communication:sample"
        ],
        "rom": "",
        "ram": "",
        "output": [],
        "adapted_kernel": [ "liteos_a" ],
        "features": [],
        "deps": {
            "components": [],
            "third_party": [ ]
        }
        },
    ```
5. 修改单板配置文件

    修改文件vendor/bearpi/bearpi_hm_micro/config.json，新增my_sample组件的条目，如下所示代码片段为applications子系统配置，"##start##"和"##end##"之间为新增条目（"##start##"和"##end##"仅用来标识位置，添加完配置后删除这两行）：

    ```
    {
    "subsystem": "applications",
        "components": [
        ##start##
          { "component": "my_sample", "features":[] },
        ##end##
          { "component": "bearpi_hm_micro_sample", "features":[] }    
        ]
      },
    ```


## 运行结果<a name="section18115713118"></a>

示例代码[编译、烧录](BearPi-HM_Micro开发板编译调试.md)后，在命令行输入指令“./bin/hello_world”执行写入的demo程序，会显示如下结果：


![](figures/运行hello_world.png)




在此希望开发者能仔细琢磨并掌握整个流程，有利于学习OpenHarmony的编译框架，以及为后续自主开发应用打下基础
