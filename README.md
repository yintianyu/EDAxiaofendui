# EDAxiaofendui

这是EDA小分队关于参加2019年集成电路EDA设计精英挑战赛的代码仓库

现在把它的框架搬过来了，测试用例只搬过来最小的那个（用于基本测试即可）

lib文件应该只在Linux上可用

## Build the program
* 编译连接过程在Centos服务器上测试通过，不能用于macOS以及Windows平台
* 在Makefile的OBJS中，添加所有.cpp文件所对应的.o文件
* `make`，生成可执行文件edaxiaofendui

## 编码规则 (In Progress)
* 参考[Google 开源项目风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)中的命名规范等
* 在每个文件的开头添加如main.cpp现有的信息
* 缩进一律使用4个空格，不使用制表符
* 文件编码使用UTF-8
* 换行符使用Unix标准的LF，而**不是**Windows的CRLF

## 关于差值量化编码的接口说明
在Regulation.hpp中定义了虚基类Regulation，用于派生出A压缩律的A_Regulation类以及u压缩律的u_Regulation类。
虚基类Regulation的成员函数声明前有注释相关说明。

如派生类实现过程中觉得基类借口有问题可在群中讨论商量修改。
### A压缩律
建立A_regulation.cpp，实现A_regulation.hpp中的类成员函数

类中所需其他辅助成员函数、成员变量可自行定义并实现（定义为private权限）。
### u压缩律
建立u_regulation.cpp，实现u_regulation.hpp中的类成员函数

类中所需其他辅助成员函数、成员变量可自行定义并实现（定义为private权限）。