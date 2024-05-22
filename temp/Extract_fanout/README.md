# 解析器

## 1 说明

赛题四：parser

## 2 第三方库

算法中使用的第三方库有Flex & Bison、正则表达式:
（1）	Flex & Bison为词法分析器和语法分析器,需要单独下载和安装
（2）	所使用正则表达式源于Boost库
安装命令： 
     sudo apt-get update
     sudo apt-get install flex bison

## 3 编译运行脚本

编译脚本run.sh文件一键执行即可,在终端输入：
     ./run.sh

在这里，设置的确保cmake编译类型为release，即  编译运行脚本文件内的cmake -DCMAKE_BUILD_TYPE=Release ..
输入文件路径：test/benchmarks下的所有.v文件
输出文件路径：test/output/output.txt文件打印出解析到的信息。
