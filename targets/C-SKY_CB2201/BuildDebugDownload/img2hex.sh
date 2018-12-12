#!/bin/sh

PWD=`pwd`

YOC_PATH=$1
TARGETS_ROOT_PATH=$2
OBJCOPY=$3
MK_GENERATED_PATH=$4/generated

ZIP=

IS_MINGW=`which ls | grep exe`

if [ -z "$IS_MINGW" ];then
    ZIP=zip
else
    ZIP=$YOC_PATH/tools/build/hex/zip.exe
fi

#boot to hex
if [ -f "$MK_GENERATED_PATH/imgs/boot" ]; then
    ADDR_BEG=`cat $TARGETS_ROOT_PATH/gcc_csky.ld | grep  " EFLASH " | grep -o -E "0x[[:xdigit:]]{8}"`
    $OBJCOPY -I binary -O ihex $MK_GENERATED_PATH/imgs/boot $MK_GENERATED_PATH/hexs/boot.hex --change-address=$ADDR_BEG
fi

#mtb to hex
if [ -f "$MK_GENERATED_PATH/imgs/mtb" ]; then
    ADDR_BEG=0x10000000
    $OBJCOPY -I binary -O ihex $MK_GENERATED_PATH/imgs/mtb $MK_GENERATED_PATH/hexs/mtb.hex --change-address=$ADDR_BEG
fi
