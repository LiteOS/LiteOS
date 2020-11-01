# bounds_checking_function

#### 介绍
- 遵循C11 Annex K (Bounds-checking interfaces)的标准，选取并实现了常见的内存/字符串操作类的函数，如memcpy_s、strcpy_s等函数。
- 未来将分析C11 Annex K中的其他标准函数，如果有必要，将在该组织中实现。
- 处理边界检查函数的版本发布、更新以及维护。

#### 函数清单

- memcpy_s
- wmemcpy_s
- memmove_s
- wmemmove_s
- memset_s
- strcpy_s
- wcscpy_s
- strncpy_s
- wcsncpy_s
- strcat_s
- wcscat_s
- strncat_s
- wcsncat_s
- strtok_s
- cwcstok_s
- sprintf_s
- swprintf_s
- vsprintf_s
- vswprintf_s
- snprintf_s
- vsnprintf_s
- scanf_s
- wscanf_s
- vscanf_s
- vwscanf_s
- fscanf_s
- fwscanf_s
- vfscanf_s
- vfwscanf_s
- sscanf_s
- swscanf_s
- vsscanf_s
- vswscanf_s
- gets_s


#### 构建方法

- 编译步骤

1. 将src下的.c文件添加到构建脚本的源码清单中。

2. 在编译选项中指定头文件目录以及项目需要的编译选项（例如：在CFLAGS中添加 -Ipath_to_include -fstack-protector-strong -fPIC    -Wall -D_FORTIFY_SOURCE=2 -O2）。

3. 为每个.c文件编译生成.o文件 。

4. 根据项目需要将.o文件生成静态库或共享库使用。

- 编译示例：
```
gcc -o memcpy_s.o -c -Iinclude -fstack-protector-strong -fPIC -Wall -D_FORTIFY_SOURCE=2 -O2 src/memcpy_s.c
```