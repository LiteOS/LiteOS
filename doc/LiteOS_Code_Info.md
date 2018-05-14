

## LiteOS代码入口

LiteOS入口在工程对应的main.c中，基本流程如下：

int main(void)

{

    UINT32 uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Inspect_Entry();

    LOS_Start();
}

1. 首先进行硬件初始化  HardWare_Init();
   
2. 初始化LiteOS内核 LOS_KernelInit();

3. 初始化内核例程 LOS_Inspect_Entry();
   
4. 最后调用LOS_Start();开始task调度，LiteOS开始正常工作;


## LiteOS的代码目录结构说明

关于代码树中各个目录存放的源代码的相关内容简介如下：

<table>
<tr>
	<td width="12%">一级目录</td>
	<td width="21%">二级目录</td>
	<td width="12%">三级目录</td>
	<td >说明</td>
</tr>
<tr>
	<td>arch</td>
	<td>arm</td>
	<td>arm-m</td>
	<td>M核中断、调度、tick相关代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>common</td>
	<td>arm核公用的cmsis core接口</td>
</tr>
<tr>
	<td>components</td>
	<td>cmsis</td>
	<td></td>
	<td>LiteOS提供的cmsis os接口实现</td>
</tr>
<tr>
	<td></td>
	<td>connectivity</td>
	<td>agent_tiny</td>
	<td>agent_tiny端云互通组件，包括公共头文件、示例代码、客户端实现代码、操作系统适配层代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>lwm2m</td>
	<td>lwm2m协议实现</td>
</tr>
<tr>
	<td></td>
	<td>net</td>
	<td>lwip_port</td>
	<td>lwip驱动及OS适配代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>lwip-2.0.3</td>
	<td>lwip协议实现</td>
</tr>
<tr>
	<td></td>
	<td>security</td>
	<td>mbedtls/mbedtls_port</td>
	<td>MBEDTLS的OS适配代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>mbedtls/mbedtl-2.6.0</td>
	<td>MBEDTLS协议实现</td>
</tr>
<tr>
	<td>doc</td>
	<td></td>
	<td></td>
	<td>此目录存放的是LiteOS的使用文档和API说明等文档</td>
</tr>
<tr>
	<td>examples</td>
	<td>API</td>
	<td></td>
	<td>供开发者测试LiteOS内核的demo示例，此目录存放的是内核功能测试用的相关用例的代码</td>
</tr>
<tr>
	<td></td>
	<td>include</td>
	<td></td>
	<td>API功能头文件存放目录</td>
</tr>
<tr>
	<td>kernel</td>
	<td>base</td>
	<td>core</td>
	<td>LiteOS基础内核代码，包括队列、task调度、软timer、时间片计算等功能</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>OM</td>
	<td>与错误处理相关的文件</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>include</td>
	<td>LiteOS内核内部使用的头文件</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>ipc</td>
	<td>LiteOS中task间通讯的相关接口，包括事件、信号量、消息队列、互斥锁等</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>mem</td>
	<td>LiteOS中的内核内存管理的相关代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>misc</td>
	<td>内存对齐功能以及毫秒级休眠sleep功能</td>
</tr>

<tr>
	<td></td>
	<td>include</td>
	<td></td>
	<td>LiteOS开源内核头文件</td>
</tr>
<tr>
	<td></td>
	<td>extended</td>
	<td>tickless</td>
	<td>低功耗框架代码</td>
</tr>

<tr>
	<td>targets</td>
	<td>FRDM_KW41Z</td>
	<td></td>
	<td>FRDM（ARM Cortex M0+）板端工程代码（含原厂芯片驱动）</td>
</tr>
<tr>
	<td></td>
	<td>STM32F103RB_NUCLEO</td>
	<td></td>
	<td>F103（ARM Cortex M3）板端工程代码（含原厂芯片驱动）</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429IGTx_FIREL</td>
	<td></td>
	<td>F429（ARM Cortex M4）板端工程代码（含原厂芯片驱动）</td>
</tr>
<tr>
	<td></td>
	<td>STM32F746ZG_NUCLEO</td>
	<td></td>
	<td>F746（ARM Cortex M7）板端工程代码（含原厂芯片驱动）</td>
</tr>
</table>
