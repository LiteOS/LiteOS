
# Huawei LiteOS Kconfig Tool User Manual


LiteOS uses the `Kconfig` files to configure the system and uses the open source parsing library [python kconfiglib](https://pypi.org/project/kconfiglib/) to parse the configuration.

## Install kconfiglib 14.1.0

Python (2.7/3.2+) is supported, and use the following command to install `kconfiglib`.

`pip install kconfiglib`

## Getting started

The compiler needs to be preconfigured, for example:

`export PATH="/LiteOS/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH`

The following commands are supported. For the command, like `savemenuconfig`, `defconfig`, `allyesconfig` or `allnoconfig`, firstly you need to copy the `${platform}.config` file for your platform in hand from the `tools/build/config/` directory to the `.config` file in the root directory.

When everything is done, change to the project root directory and then run one of those commands.

- make menuconfig

    Run the `make menuconfig` command to display the system configuration graphical interface. After completing your configuration, press `S` to save the configuration, and press `ESC` to exit.

- make savemenuconfig

    Run the `make savemenuconfig` command to automatically generate the header file `/Huawei_LiteOS/platform/bsp/board/${platform}/include/menuconfig. h` and to update the `.config` file.

- make defconfig

    Run the `make defconfig` command and all symbols in the generated configuration file are set to their default values.

- make allyesconfig

    Run the `make allyesconfig` command and in the generated configuration file, as many symbols as possible are set to `y`.

- make allnoconfig

    Run the `make allnoconfig` command, and in the generated configuration file, as many symbols as possible are set to `n`.
