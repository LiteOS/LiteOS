#!/bin/sh

#获取当前路径的命令
PWD=`pwd`

#目标路径
#	dirname $0，		取得当前执行的脚本文件的父目录
#	cd `dirname $0`，	进入这个目录(切换当前工作目录)
#	pwd，				显示当前工作目录(cd执行后的)
#本项目所对应的TOPDIR路径为：(其他)\csi-rtos-v1.4-cb2201\utilities\elf2hex\hex
TOPDIR=$(cd `dirname $0`;pwd)

#本项目所对应的YOC_PATH路径为：(其他)\csi-rtos-v1.4-cb2201
YOC_PATH=$TOPDIR
#/../../..

#本项目所对应的MK_GENERATED_PATH路径为：(其他)\csi-rtos-v1.4-cb2201\utilities\elf2hex\hex\generated
MK_GENERATED_PATH=$PWD/generated
MK_GENERATED_HEXS_PATH=$TOPDIR/generated/hexs
MK_GENERATED_IMGS_PATH=$TOPDIR/generated/imgs

#编译时的提示文字：
echo "[Kshine in atf_build.sh][INFO] Generated output files ..."

#创建上述的3个文件夹
mkdir -p $MK_GENERATED_PATH
mkdir -p $MK_GENERATED_IMGS_PATH
mkdir -p $MK_GENERATED_HEXS_PATH

#
OBJCOPY=csky-abiv2-elf-objcopy

#本项目所对应的TARGETS_ROOT_PATH 路径为：（其他）\csi-rtos-v1.4-cb2201\board\cb2201
TARGETS_ROOT_PATH=$YOC_PATH/board/cb2201

#下方是具体的编译逻辑，暂时不去理解它们。（只需要看上方的代码，知道该文件运行的周围条件）====================================

#定义BIN_NAME变量不给初值
BIN_NAME=

CONTENTS=Obj
#output yoc.bin
if [ -d $CONTENTS ];then
ELF_NAME=`ls Obj/*.elf`
else
ELF_NAME=`ls *.elf`
BIN_NAME=`ls *.elf | awk -F '.' '{print $1}'`
fi

$OBJCOPY -O binary $ELF_NAME $MK_GENERATED_IMGS_PATH/boot

#create mtb
#执行mtbhex.sh脚本，生成mtb文件
#脚本后面跟着的是 传递的参数
sh $TOPDIR/mtbhex.sh $YOC_PATH $TARGETS_ROOT_PATH $TOPDIR

#bin to hex
sh $TOPDIR/img2hex.sh $YOC_PATH $TARGETS_ROOT_PATH $OBJCOPY $TOPDIR

cp -rf $MK_GENERATED_IMGS_PATH $MK_GENERATED_HEXS_PATH $MK_GENERATED_PATH

if [ -n "$BIN_NAME" ];then
mv $MK_GENERATED_PATH/imgs/boot $MK_GENERATED_PATH/imgs/$BIN_NAME.bin
mv $MK_GENERATED_PATH/hexs/boot.hex $MK_GENERATED_PATH/hexs/$BIN_NAME.hex
fi

rm -rf $MK_GENERATED_HEXS_PATH $MK_GENERATED_IMGS_PATH

