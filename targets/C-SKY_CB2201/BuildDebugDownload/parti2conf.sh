#!/bin/sh

echo '#****************************#'

if [ "$3" = "v2" ];then
echo "CONFIG_PARTION_NUM=6"
else
echo "CONFIG_PARTION_NUM=1"
fi

echo
echo 'CONFIG_PARTION_NAME0=boot'
ADDR_BEG=`cat $1/gcc_csky.ld | grep "    EFLASH " | grep -o -E "0x[[:xdigit:]]{8}"`
ADDR_END=`printf "0x%x" $(( $ADDR_BEG + $2 ))`

echo CONFIG_PARTION_ADDR0=$ADDR_BEG
echo CONFIG_PARTION_END0=$ADDR_END
if [ "$3" = "v2" ];then
echo CONFIG_STATIC_ADDR0=$ADDR_BEG
fi
echo CONFIG_IMG_LOAD_ADDR0=$ADDR_BEG

echo '#****************************#'
