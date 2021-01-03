[![Build Status](https://travis-ci.org/LiteOS/LiteOS.svg?branch=develop)](https://travis-ci.org/LiteOS/LiteOS)

## Huawei LiteOS Introduction

Huawei LiteOS is a lightweight operating system based on real-time kernel developed by Huawei for the Internet of Things. This project belongs to Huawei's IoT operating system Huawei LiteOS source code. The existing basic kernel supports task management, memory management, time management, communication mechanism, interrupt management, queue management, event management, timer and other operating system basic components to better support. Low-power scenarios, support for the tickless mechanism, and support for timer alignment.

At the same time, it provides end-cloud collaboration capabilities, integrates LwM2M, CoAP, mbedtls, and LwIP complete IoT interconnect protocol stacks. On the basis of LwM2M, it provides AgentTiny module. Users only need to pay attention to their own applications without paying attention to LwM2M implementation details. The agentTiny encapsulated interface can be used to quickly and securely connect to the cloud platform.

Since the release of the open source community, Huawei LiteOS has built around the NB-IoT IoT market from technology, ecology, solutions, commercial support and other multi-dimensional enabling partners to build an open source IoT ecosystem. Currently, 30+ MCUs and solutions have been aggregated. Partners, jointly launched a number of open source development kits and industry solutions to help customers in many industries to quickly launch IoT terminals and services, including customers such as meter reading, parking, street lighting, environmental protection, shared bicycles, logistics, etc. Provide a "one-stop" complete software platform to effectively lower the development threshold and shorten the development cycle.

- [LiteOS kernel source code directory description](./doc/LiteOS_Code_Info.md)

This document describes the details of the LiteOS kernel source code. Through this document readers can understand the source code structure of LiteOS and the functions of LiteOS's main() function.


## LiteOS Development Guide

[LiteOS Development Guide](./doc/Huawei_LiteOS_Developer_Guide_zh.md)

[LiteOS Migration Guide](https://liteos.github.io/porting/)

This document explains in detail the development of LiteOS modules and their implementation principles. Users can learn the use of each module based on this document.


## LiteOS Access Cloud Platform Development Guide

* [LiteOS access to Huawei cloud platform](https://github.com/SuYai/OceanConnectHelp)
  * [LiteOS SDK End Cloud Component Development Guide](./doc/Huawei_LiteOS_SDK_Developer_Guide.md)

The LiteOS SDK is a Huawei LiteOS software development kit. It can quickly and easily connect to the Huawei OceanConnect IoT platform through the LiteOS SDK-side cloud interworking component. This greatly reduces the development cycle and quickly builds IoT products.

The LiteOS SDK is a Huawei LiteOS software development kit. It can quickly and easily connect to the Huawei OceanConnect IoT platform through the LiteOS SDK-side cloud interworking component. This greatly reduces the development cycle and quickly builds IoT products.
* [LiteOS SDK End Cloud Component Coap Development Guide](./doc/Huawei_LiteOS_SDK_Coap_LwM2M_Developer_Guide_zh.md)
* [LiteOS SDK End Cloud Component MQTT Development Guide](./doc/Huawei_LiteOS_SDK_MQTT_Developer_Guide.md)
  

* [LiteOS access 3rd cloud platform](https://github.com/LiteOS/LiteOS_Connect_to_3rd_Cloud)


## LiteOS Supported Hardware

* LiteOS open source project currently supports ARM Cortex-M0, Cortex-M3, Cortex-M4, Cortex-M7 and other chip architectures

* [List of development boards supported by LiteOS](./doc/LiteOS_Supported_board_list.md)
Huawei LiteOS and the mainstream MCU manufacturers in the industry, through the developer activities, has now adapted 30+ universal MCU development kit, 5 sets of NB-IoT integrated development kit


## Open source agreement

* Follow the BSD-3 open source license agreement
* [Huawei LiteOS Intellectual Property Policy](https://support.huaweicloud.com/productdesc-LiteOS/en-us_topic_0145347224.html)

## LiteOS Git Getting Started Reading

- [LiteOS Commit Message Rule](./doc/LiteOS_Commit_Message.md)

This document describes how to submit a commit to the LiteOS repository, which is a commit rule that LiteOS development must follow, otherwise the submitted commit will be rejected. Please click on the link for details.

- [Huawei LiteOS code contribution process](./doc/LiteOS_Contribute_Guide_GitGUI.md)

This document describes how developers create their own warehouses, develop and then contribute code to the LiteOS repository. Please click on the link for details.


## join us
* Welcome to submit an issue to discuss the concerns, welcome to submit PR participation in feature building
* If you are interested in joining the Huawei LiteOS Eco Partner, please email liteos@huawei.com or visit [LiteOS official website](http://www.huawei.com/liteos) for further details.
