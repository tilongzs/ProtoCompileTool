# ProtoCompileTool
Proto界面编译辅助工具。支持protobuffer及gRPC的代码生成。
# 概述
本软件是protoc.exe的界面辅助工具。调用protoc.exe，将proto文件编译为对应编程语言，并支持gRPC生成插件。
由于本人目前一般只使用C++，因此虽然理论上支持其他编译语言，但不保证能正常使用。
![](https://raw.githubusercontent.com/tilongzs/ProtoCompileTool/master/doc/screenshot.png)

# 编译环境

- VisualStudio 2019（含MFC组件）
- Windows 10

# 使用方法

###  proto、gRPC plugin配置
1. 在“protoc位置”编辑框里输入protoc.exe的路径。
1. 【可选】在“gRPC plugin位置”编辑框里输入gRPC插件（如grpc_cpp_plugin.exe）的路径。
1. 在“编译语言”下拉菜单中选择要使用的编译语言。


###  生成文件的存放位置配置

可以选择与proto文件同级的文件夹，也可以指定文件夹。

### 使用proto方式配置

可以选择单个proto文件，在编辑框里输入proto文件路径。

也可以指定proto文件的文件夹，在编辑框里输入文件夹的路径，这将会把整个文件夹（**不包含**子文件夹）下的所有proto文件转换。

### 开始转换

点击“开始转换”按钮，即可开始转换。完成或者错误消息将显示在右边的窗口中。
