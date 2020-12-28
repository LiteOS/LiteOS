#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved
# Description: Get compiler path.

set -e
declare TEMP="$1"
function get_compiler_path()
{
    local system=$(uname -s)
    local user_gcc="${TEMP}"
    local gcc_install_path=$(which "${user_gcc}")
    if [ "$system" = "Linux" ] ; then
        gcc_install_path=${gcc_install_path/\/bin\/"${user_gcc}"/}
    else
        gcc_install_path=${gcc_install_path/\/bin\/"${user_gcc}".exe/}
    fi
    echo "$gcc_install_path"
}
get_compiler_path
