# bounds_checking_function

#### Description

- following the standard of C11 Annex K (bound-checking interfaces), functions of the common memory/string operation classes, such as memcpy_s, strcpy_s, are selected and implemented.

- other standard functions in C11 Annex K will be analyzed in the future and implemented in this organization if necessary.

- handles the release, update, and maintenance of bounds_checking_function.

#### Function List

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


#### Building 

- compilation steps

1. Add all the .c files under /src to the source code listing for the build script.

2. In the build options, specify the header directory and the build options required for the project (for example, add  
-Ipath_to_include -fstack-protector-strong -fPIC -Wall -D_FORTIFY_SOURCE=2 -O2 in CFLAGS).

3. Generate .o files for each .c file.

4. Generate static or shared libraries for .o files according to project requirements.

- compiling examples:
```
gcc -o memcpy_s.o -c -Iinclude -fstack-protector-strong -fPIC -Wall -D_FORTIFY_SOURCE=2 -O2 src/memcpy_s.c
```


