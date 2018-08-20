## 单元测试说明
注: 本说明使用的linux环境为Ubuntu

## 一、编译
1. 进入tests目录
2. 输入如下命令
```
sudo make clean
sudo make
sudo make cov
```
*详情见Makefile文件*

## 二、查看覆盖率和测试结果
make cov后会在tests目录下生成build目录，build目录下包括html和result两个目录。
1. 查看覆盖率，文件为：build/html/index.html
2. 查看测试结果，文件为：build/result/*.htm

