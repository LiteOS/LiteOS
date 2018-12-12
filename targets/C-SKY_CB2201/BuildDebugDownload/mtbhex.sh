#!/bin/sh

YOC_PATH=$1
TARGETS_ROOT_PATH=$2
IMAGE_SIZE=
MK_GENERATED_PATH=$3/generated
MTBCREATE=

IS_MINGW=`which ls | grep exe`

if [ -z "$IS_MINGW" ];then
 #   chmod 755 $YOC_PATH/utilities/elf2hex/hex/mtbcreate_i386
  #  chmod 755 $YOC_PATH/utilities/elf2hex/hex/mtbcreate_x86_64
  chmod 755 $YOC_PATH/mtbcreate_i386
  chmod 755 $YOC_PATH/mtbcreate_x86_64

UNAME=`uname -m`

if [ $UNAME = "x86_64" ];then
    MTBCREATE=./mtbcreate_x86_64
else
    MTBCREATE=./mtbcreate_i386
fi
    DD=dd
else
    MTBCREATE=./mtbcreate.exe
    DD=./dd.exe
fi

cd $YOC_PATH
#/utilities/elf2hex/hex

# mtb input file boot tee prim
cp -fr $MK_GENERATED_PATH/imgs ./

IMAGE_SIZE=`ls -l imgs/boot | awk -F ' ' '{print $5}'`

# mtb input file partion.cfg
sh parti2conf.sh $TARGETS_ROOT_PATH $IMAGE_SIZE > partion.cfg

#Create mtb
$MTBCREATE 1

#mtb mtb bak merge
#$DD if=img.manifest bs=2048 conv=sync > $MK_GENERATED_PATH/imgs/mtb 2>/dev/null
#$DD if=img.manifest bs=2048 conv=sync >> $MK_GENERATED_PATH/imgs/mtb 2>/dev/null
cp img.manifest $MK_GENERATED_PATH/imgs/mtb
#Clean tmp files
rm -fr partion.cfg img.manifest imgs
