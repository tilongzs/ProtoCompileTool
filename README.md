# ProtoCompileTool
Proto界面编译辅助工具。支持protobuffer及gRPC的代码生成。
# 概述
本软件是protoc.exe的界面辅助工具。调用protoc.exe，将proto文件编译为对应编程语言，并支持gRPC生成插件。
目前支持生成C++、Java代码。
![](https://raw.githubusercontent.com/tilongzs/ProtoCompileTool/master/doc/screenshot.png)

# 编译环境

- VisualStudio 2022（含MFC组件）
- Windows 11

# 使用方法

###  proto、gRPC plugin配置
1. 输入protoc.exe的路径。
2. 【可选】输入通用proto的include路径。
3. 【可选】输入gRPC插件路径。
4. 选择生成代码文件的存放路径，可以与proto文件同级别，或者指定文件夹。
5. 选择使用proto方式，可以每次转换单个proto文件，也可以一次性转换整个文件夹中的所有proto文件。
6. 点击“开始转换”按钮，即可开始转换。完成或者错误消息将显示在右边的窗口中。
