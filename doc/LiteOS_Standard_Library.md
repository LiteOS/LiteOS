# Huawei LiteOS 标准库

## 目 录
-   [POSIX接口](#1)
-   [C++兼容规格](#2)
-   [CMSIS接口](#3)
    -   [CMSIS v1.0](#v1)
    -   [CMSIS v2.0](#v2)

<h2 id="1">POSIX接口</h2>

<h3 id="POSIX支持接口">POSIX支持接口</h3>

Huawei LiteOS提供一套POSIX适配接口，支持的接口参见下表。

<a name="table7251821191815"></a>
<table><thead align="left"><tr id="row132381621141816"><th class="cellrowborder" valign="top" width="17.03%" id="mcps1.1.5.1.1"><p id="p4238821141818"><a name="p4238821141818"></a><a name="p4238821141818"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="22.830000000000002%" id="mcps1.1.5.1.2"><p id="p12238192111185"><a name="p12238192111185"></a><a name="p12238192111185"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="11.5%" id="mcps1.1.5.1.3"><p id="p02381021171812"><a name="p02381021171812"></a><a name="p02381021171812"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.64%" id="mcps1.1.5.1.4"><p id="p1023882151811"><a name="p1023882151811"></a><a name="p1023882151811"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row22381321151819"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p7238121101820"><a name="p7238121101820"></a><a name="p7238121101820"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p142381621101814"><a name="p142381621101814"></a><a name="p142381621101814"></a>mq_close</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1823812171817"><a name="p1823812171817"></a><a name="p1823812171817"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1923802112183"><a name="p1923802112183"></a><a name="p1923802112183"></a>关闭消息队列</p>
</td>
</tr>
<tr id="row10238192131816"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p323862121816"><a name="p323862121816"></a><a name="p323862121816"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p923812219182"><a name="p923812219182"></a><a name="p923812219182"></a>mq_getattr</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p8238121141819"><a name="p8238121141819"></a><a name="p8238121141819"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1623892141819"><a name="p1623892141819"></a><a name="p1623892141819"></a>获取消息队列属性</p>
</td>
</tr>
<tr id="row32382213185"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p17238221151817"><a name="p17238221151817"></a><a name="p17238221151817"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p162387216183"><a name="p162387216183"></a><a name="p162387216183"></a>mq_open</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p102381221161812"><a name="p102381221161812"></a><a name="p102381221161812"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1623813216185"><a name="p1623813216185"></a><a name="p1623813216185"></a>打开消息队列</p>
</td>
</tr>
<tr id="row1223814210185"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1323862121815"><a name="p1323862121815"></a><a name="p1323862121815"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1523802141813"><a name="p1523802141813"></a><a name="p1523802141813"></a>mq_receive</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p12381215186"><a name="p12381215186"></a><a name="p12381215186"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1923816215184"><a name="p1923816215184"></a><a name="p1923816215184"></a>接受一个消息队列中的消息</p>
</td>
</tr>
<tr id="row3238122111812"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1923872171816"><a name="p1923872171816"></a><a name="p1923872171816"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1523822111820"><a name="p1523822111820"></a><a name="p1523822111820"></a>mq_send</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1423892121810"><a name="p1423892121810"></a><a name="p1423892121810"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p20238621101811"><a name="p20238621101811"></a><a name="p20238621101811"></a>发送一个消息到消息队列</p>
</td>
</tr>
<tr id="row16238021151810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p623852118186"><a name="p623852118186"></a><a name="p623852118186"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1823815217185"><a name="p1823815217185"></a><a name="p1823815217185"></a>mq_setattr</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p923882112185"><a name="p923882112185"></a><a name="p923882112185"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p323832191817"><a name="p323832191817"></a><a name="p323832191817"></a>设置消息队列属性</p>
</td>
</tr>
<tr id="row523813212188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p122385217182"><a name="p122385217182"></a><a name="p122385217182"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p923814217181"><a name="p923814217181"></a><a name="p923814217181"></a>mq_timedreceive</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p20238142111814"><a name="p20238142111814"></a><a name="p20238142111814"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p5238142113188"><a name="p5238142113188"></a><a name="p5238142113188"></a>定时接收消息</p>
</td>
</tr>
<tr id="row1023852181818"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p42381321181814"><a name="p42381321181814"></a><a name="p42381321181814"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p17238172141816"><a name="p17238172141816"></a><a name="p17238172141816"></a>mq_timedsend</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p132386217185"><a name="p132386217185"></a><a name="p132386217185"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p023832118183"><a name="p023832118183"></a><a name="p023832118183"></a>定时发送消息</p>
</td>
</tr>
<tr id="row16238521131810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p42381021131818"><a name="p42381021131818"></a><a name="p42381021131818"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p62381121151811"><a name="p62381121151811"></a><a name="p62381121151811"></a>mq_unlink</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p92382021131810"><a name="p92382021131810"></a><a name="p92382021131810"></a>消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p823815218181"><a name="p823815218181"></a><a name="p823815218181"></a>移除消息队列</p>
</td>
</tr>
<tr id="row1323842110187"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p82381621171816"><a name="p82381621171816"></a><a name="p82381621171816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p323917218181"><a name="p323917218181"></a><a name="p323917218181"></a>pthread_attr_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p22391521131811"><a name="p22391521131811"></a><a name="p22391521131811"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1623922151819"><a name="p1623922151819"></a><a name="p1623922151819"></a>删除线程的属性</p>
</td>
</tr>
<tr id="row152398216183"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p132391421101816"><a name="p132391421101816"></a><a name="p132391421101816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1239152117186"><a name="p1239152117186"></a><a name="p1239152117186"></a>pthread_attr_getdetachstate</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p14239221151817"><a name="p14239221151817"></a><a name="p14239221151817"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p112392216186"><a name="p112392216186"></a><a name="p112392216186"></a>获取脱离状态的属性</p>
</td>
</tr>
<tr id="row423912191818"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p12239102121819"><a name="p12239102121819"></a><a name="p12239102121819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p3239182117183"><a name="p3239182117183"></a><a name="p3239182117183"></a>pthread_attr_getinheritsched</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1623972112184"><a name="p1623972112184"></a><a name="p1623972112184"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p3239202114182"><a name="p3239202114182"></a><a name="p3239202114182"></a>获取任务调度方式</p>
</td>
</tr>
<tr id="row92395213186"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p123962119183"><a name="p123962119183"></a><a name="p123962119183"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1239132117182"><a name="p1239132117182"></a><a name="p1239132117182"></a>pthread_attr_getschedparam</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p172399212186"><a name="p172399212186"></a><a name="p172399212186"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1923918211189"><a name="p1923918211189"></a><a name="p1923918211189"></a>获取任务调度参数</p>
</td>
</tr>
<tr id="row11239142191817"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p7239142118182"><a name="p7239142118182"></a><a name="p7239142118182"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1623913214189"><a name="p1623913214189"></a><a name="p1623913214189"></a>pthread_attr_getschedpolicy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p16239421121818"><a name="p16239421121818"></a><a name="p16239421121818"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1523952151817"><a name="p1523952151817"></a><a name="p1523952151817"></a>获取任务调度策略属性，Huawei LiteOS目前仅支持SCHED_RR调度策略，不支持SCHED_OTHERSCHED_FIFOSCHED_RR</p>
</td>
</tr>
<tr id="row823913218181"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p223942118182"><a name="p223942118182"></a><a name="p223942118182"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1123972116189"><a name="p1123972116189"></a><a name="p1123972116189"></a>pthread_attr_getscope</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p22395218186"><a name="p22395218186"></a><a name="p22395218186"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p4239721201815"><a name="p4239721201815"></a><a name="p4239721201815"></a>获取任务范围属性，Huawei LiteOS任务使用范围目前只支持PTHREAD_SCOPE_SYSTEM，不支持PTHREAD_SCOPE_PROCESS</p>
</td>
</tr>
<tr id="row8239021161811"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p10239172112189"><a name="p10239172112189"></a><a name="p10239172112189"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p623911215182"><a name="p623911215182"></a><a name="p623911215182"></a>pthread_attr_getstackaddr</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p142391721191820"><a name="p142391721191820"></a><a name="p142391721191820"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p823920218189"><a name="p823920218189"></a><a name="p823920218189"></a>获取任务堆栈的起始地址</p>
</td>
</tr>
<tr id="row723972171811"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p42391221111810"><a name="p42391221111810"></a><a name="p42391221111810"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p202399217187"><a name="p202399217187"></a><a name="p202399217187"></a>pthread_attr_getstacksize</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p323962116188"><a name="p323962116188"></a><a name="p323962116188"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p142391021171816"><a name="p142391021171816"></a><a name="p142391021171816"></a>获取任务属性堆栈大小</p>
</td>
</tr>
<tr id="row1623919212184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p423952118184"><a name="p423952118184"></a><a name="p423952118184"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p423982161813"><a name="p423982161813"></a><a name="p423982161813"></a>pthread_attr_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p423972121817"><a name="p423972121817"></a><a name="p423972121817"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p16239182141811"><a name="p16239182141811"></a><a name="p16239182141811"></a>初始化任务属性</p>
</td>
</tr>
<tr id="row723962110181"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p223972111183"><a name="p223972111183"></a><a name="p223972111183"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p82393214188"><a name="p82393214188"></a><a name="p82393214188"></a>pthread_attr_setdetachstate</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p52391821191819"><a name="p52391821191819"></a><a name="p52391821191819"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1723912116187"><a name="p1723912116187"></a><a name="p1723912116187"></a>设置任务属性分离状态</p>
</td>
</tr>
<tr id="row6239122115189"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p423952117181"><a name="p423952117181"></a><a name="p423952117181"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p623952171818"><a name="p623952171818"></a><a name="p623952171818"></a>pthread_attr_setinheritsched</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p18239122151817"><a name="p18239122151817"></a><a name="p18239122151817"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p18239102191810"><a name="p18239102191810"></a><a name="p18239102191810"></a>设置任务调度方式</p>
</td>
</tr>
<tr id="row8239521191815"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p723972114182"><a name="p723972114182"></a><a name="p723972114182"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p13239162112185"><a name="p13239162112185"></a><a name="p13239162112185"></a>pthread_attr_setschedparam</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p023912116182"><a name="p023912116182"></a><a name="p023912116182"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p323962181814"><a name="p323962181814"></a><a name="p323962181814"></a>设置任务调度参数，Huawei LiteOS设置任务优先级的参数值越小，任务在系统中的优先级就越高，与标准库函数相反。注意：需要将pthread_attr_t任务属性的inheritsched字段设置为PTHREAD_EXPLICIT_SCHED，否则设置的任务调度优先级将不会生效，系统默认设置为PTHREAD_INHERIT_SCHED</p>
</td>
</tr>
<tr id="row523962114184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p6239152141816"><a name="p6239152141816"></a><a name="p6239152141816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p2239152115188"><a name="p2239152115188"></a><a name="p2239152115188"></a>pthread_attr_setschedpolicy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p523911215180"><a name="p523911215180"></a><a name="p523911215180"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p5239112115188"><a name="p5239112115188"></a><a name="p5239112115188"></a>设置任务调度策略属性，Huawei LiteOS目前仅支持SCHED_RR调度策略，不支持SCHED_OTHERSCHED_FIFOSCHED_RR</p>
</td>
</tr>
<tr id="row723919218181"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p182394214180"><a name="p182394214180"></a><a name="p182394214180"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p162391321141814"><a name="p162391321141814"></a><a name="p162391321141814"></a>pthread_attr_setscope</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1823962116183"><a name="p1823962116183"></a><a name="p1823962116183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1239102117182"><a name="p1239102117182"></a><a name="p1239102117182"></a>设置任务范围属性，Huawei LiteOS任务使用范围目前只支持PTHREAD_SCOPE_SYSTEM，不支持PTHREAD_SCOPE_PROCESS</p>
</td>
</tr>
<tr id="row12392021121818"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p723952181819"><a name="p723952181819"></a><a name="p723952181819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p2239221111810"><a name="p2239221111810"></a><a name="p2239221111810"></a>pthread_attr_setstackaddr</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p123932181815"><a name="p123932181815"></a><a name="p123932181815"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p8239102151811"><a name="p8239102151811"></a><a name="p8239102151811"></a>设置任务堆栈的起始地址</p>
</td>
</tr>
<tr id="row13239192131814"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p7239192121815"><a name="p7239192121815"></a><a name="p7239192121815"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p0239142111815"><a name="p0239142111815"></a><a name="p0239142111815"></a>pthread_attr_setstacksize</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p8240152110180"><a name="p8240152110180"></a><a name="p8240152110180"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p92401821181813"><a name="p92401821181813"></a><a name="p92401821181813"></a>设置任务属性堆栈大小</p>
</td>
</tr>
<tr id="row924002113182"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p11240172119186"><a name="p11240172119186"></a><a name="p11240172119186"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p20240122121819"><a name="p20240122121819"></a><a name="p20240122121819"></a>pthread_cancel</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p8240192111186"><a name="p8240192111186"></a><a name="p8240192111186"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p52406217183"><a name="p52406217183"></a><a name="p52406217183"></a>取消任务，Huawei LiteOS仅支持先设置PTHREAD_CANCEL_ASYNCHRONOUS状态，再调用pthread_cancel取消任务</p>
</td>
</tr>
<tr id="row2240921161812"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p18240721171816"><a name="p18240721171816"></a><a name="p18240721171816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p0240142111185"><a name="p0240142111185"></a><a name="p0240142111185"></a>pthread_cond_broadcast</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p132401212188"><a name="p132401212188"></a><a name="p132401212188"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p11240152171815"><a name="p11240152171815"></a><a name="p11240152171815"></a>唤醒所有被阻塞在条件变量上的线程</p>
</td>
</tr>
<tr id="row02400217183"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p9240192191810"><a name="p9240192191810"></a><a name="p9240192191810"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p9240172101816"><a name="p9240172101816"></a><a name="p9240172101816"></a>pthread_cond_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1924022112188"><a name="p1924022112188"></a><a name="p1924022112188"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p2240202113188"><a name="p2240202113188"></a><a name="p2240202113188"></a>释放条件变量</p>
</td>
</tr>
<tr id="row624002116184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1224022121810"><a name="p1224022121810"></a><a name="p1224022121810"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p112401121201818"><a name="p112401121201818"></a><a name="p112401121201818"></a>pthread_cond_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p13240172161817"><a name="p13240172161817"></a><a name="p13240172161817"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p192401921101812"><a name="p192401921101812"></a><a name="p192401921101812"></a>初始化条件变量</p>
</td>
</tr>
<tr id="row122401321191818"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p92401215181"><a name="p92401215181"></a><a name="p92401215181"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p18240172131818"><a name="p18240172131818"></a><a name="p18240172131818"></a>pthread_cond_signal</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p624010217185"><a name="p624010217185"></a><a name="p624010217185"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p102401221131810"><a name="p102401221131810"></a><a name="p102401221131810"></a>释放被阻塞在条件变量上的一个线程</p>
</td>
</tr>
<tr id="row1240202110180"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p4240112116181"><a name="p4240112116181"></a><a name="p4240112116181"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p18240721131819"><a name="p18240721131819"></a><a name="p18240721131819"></a>pthread_cond_timedwait</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p924019215185"><a name="p924019215185"></a><a name="p924019215185"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p102401821121812"><a name="p102401821121812"></a><a name="p102401821121812"></a>超时时限内等待一个条件变量，当超时等待时间为相对时间，Huawei LiteOS不能处理早已超时的情况</p>
</td>
</tr>
<tr id="row524011219184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p11240102114184"><a name="p11240102114184"></a><a name="p11240102114184"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p224011210180"><a name="p224011210180"></a><a name="p224011210180"></a>pthread_cond_wait</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p22402216183"><a name="p22402216183"></a><a name="p22402216183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p2240132111187"><a name="p2240132111187"></a><a name="p2240132111187"></a>等待一个条件变量</p>
</td>
</tr>
<tr id="row172406213188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p112401721191817"><a name="p112401721191817"></a><a name="p112401721191817"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p14240202141818"><a name="p14240202141818"></a><a name="p14240202141818"></a>pthread_condattr_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p12240182113183"><a name="p12240182113183"></a><a name="p12240182113183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1824022114187"><a name="p1824022114187"></a><a name="p1824022114187"></a>删除存储并使属性对象无效</p>
</td>
</tr>
<tr id="row1724012212188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p3240162117185"><a name="p3240162117185"></a><a name="p3240162117185"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p324010215184"><a name="p324010215184"></a><a name="p324010215184"></a>pthread_condattr_getclock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p102401321191810"><a name="p102401321191810"></a><a name="p102401321191810"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1224092117188"><a name="p1224092117188"></a><a name="p1224092117188"></a>获取任务时钟</p>
</td>
</tr>
<tr id="row1924042181818"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1924072113188"><a name="p1924072113188"></a><a name="p1924072113188"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p224013216187"><a name="p224013216187"></a><a name="p224013216187"></a>pthread_condattr_getpshared</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p5240132191810"><a name="p5240132191810"></a><a name="p5240132191810"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p10240122151814"><a name="p10240122151814"></a><a name="p10240122151814"></a>获取条件变量属性，Huawei LiteOS只支持获取PTHREAD_PROCESS_PRIVATE条件变量属性</p>
</td>
</tr>
<tr id="row524092119184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p624072112183"><a name="p624072112183"></a><a name="p624072112183"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1724042151820"><a name="p1724042151820"></a><a name="p1724042151820"></a>pthread_condattr_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p132409218185"><a name="p132409218185"></a><a name="p132409218185"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p162401121191815"><a name="p162401121191815"></a><a name="p162401121191815"></a>初始化条件变量属性</p>
</td>
</tr>
<tr id="row11240142121813"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1624052110184"><a name="p1624052110184"></a><a name="p1624052110184"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p142401521121820"><a name="p142401521121820"></a><a name="p142401521121820"></a>pthread_condattr_setclock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p122404210189"><a name="p122404210189"></a><a name="p122404210189"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p824042111817"><a name="p824042111817"></a><a name="p824042111817"></a>设置任务时钟</p>
</td>
</tr>
<tr id="row3240321131812"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p102401021101819"><a name="p102401021101819"></a><a name="p102401021101819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p112401821161815"><a name="p112401821161815"></a><a name="p112401821161815"></a>pthread_condattr_setpshared</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1924042161813"><a name="p1924042161813"></a><a name="p1924042161813"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p524020214189"><a name="p524020214189"></a><a name="p524020214189"></a>设置条件变量属性</p>
</td>
</tr>
<tr id="row1240321181817"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1324014211187"><a name="p1324014211187"></a><a name="p1324014211187"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1424052161817"><a name="p1424052161817"></a><a name="p1424052161817"></a>pthread_create</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1824012191811"><a name="p1824012191811"></a><a name="p1824012191811"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p62411621181816"><a name="p62411621181816"></a><a name="p62411621181816"></a>创建任务</p>
</td>
</tr>
<tr id="row52411021161813"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p2024122161814"><a name="p2024122161814"></a><a name="p2024122161814"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p6241152131813"><a name="p6241152131813"></a><a name="p6241152131813"></a>pthread_detach</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p024162121812"><a name="p024162121812"></a><a name="p024162121812"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p13241162112181"><a name="p13241162112181"></a><a name="p13241162112181"></a>分离任务</p>
</td>
</tr>
<tr id="row5241152114184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p52411621151816"><a name="p52411621151816"></a><a name="p52411621151816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p13241132121816"><a name="p13241132121816"></a><a name="p13241132121816"></a>pthread_equal</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p82418218184"><a name="p82418218184"></a><a name="p82418218184"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p16241172117181"><a name="p16241172117181"></a><a name="p16241172117181"></a>判断是否为同一任务</p>
</td>
</tr>
<tr id="row17241421131819"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p3241122161813"><a name="p3241122161813"></a><a name="p3241122161813"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p6241421121818"><a name="p6241421121818"></a><a name="p6241421121818"></a>pthread_exit</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p192418213180"><a name="p192418213180"></a><a name="p192418213180"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p19241132141814"><a name="p19241132141814"></a><a name="p19241132141814"></a>任务退出</p>
</td>
</tr>
<tr id="row18241102110185"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p4241421151819"><a name="p4241421151819"></a><a name="p4241421151819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1324112114188"><a name="p1324112114188"></a><a name="p1324112114188"></a>pthread_getschedparam</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p19241112111819"><a name="p19241112111819"></a><a name="p19241112111819"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p112416215185"><a name="p112416215185"></a><a name="p112416215185"></a>获取任务优先级及调度参数，Huawei LiteOS目前仅支持SCHED_RR调度策略，不支持SCHED_OTHERSCHED_FIFOSCHED_RR</p>
</td>
</tr>
<tr id="row224114213181"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1424132116180"><a name="p1424132116180"></a><a name="p1424132116180"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p122415217182"><a name="p122415217182"></a><a name="p122415217182"></a>pthread_getspecific</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p172419215183"><a name="p172419215183"></a><a name="p172419215183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1224142151817"><a name="p1224142151817"></a><a name="p1224142151817"></a>获取调用线程的键绑定</p>
</td>
</tr>
<tr id="row1824111219189"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p4241122181817"><a name="p4241122181817"></a><a name="p4241122181817"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p15241192131814"><a name="p15241192131814"></a><a name="p15241192131814"></a>pthread_join</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p10241142116188"><a name="p10241142116188"></a><a name="p10241142116188"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1824162151819"><a name="p1824162151819"></a><a name="p1824162151819"></a>阻塞任务</p>
</td>
</tr>
<tr id="row624115212187"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1624192131819"><a name="p1624192131819"></a><a name="p1624192131819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p19241172131814"><a name="p19241172131814"></a><a name="p19241172131814"></a>pthread_key_create</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p9241152120189"><a name="p9241152120189"></a><a name="p9241152120189"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p162411321161812"><a name="p162411321161812"></a><a name="p162411321161812"></a>分配用于标识进程中线程特定数据的键</p>
</td>
</tr>
<tr id="row9241221131813"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p324122191816"><a name="p324122191816"></a><a name="p324122191816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p9241521161813"><a name="p9241521161813"></a><a name="p9241521161813"></a>pthread_key_delete</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1424111217182"><a name="p1424111217182"></a><a name="p1424111217182"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p2241112112185"><a name="p2241112112185"></a><a name="p2241112112185"></a>销毁现有线程特定数据键</p>
</td>
</tr>
<tr id="row2241112111819"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p124162151818"><a name="p124162151818"></a><a name="p124162151818"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p19241182121812"><a name="p19241182121812"></a><a name="p19241182121812"></a>pthread_mutex_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p924119216188"><a name="p924119216188"></a><a name="p924119216188"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p19241122111815"><a name="p19241122111815"></a><a name="p19241122111815"></a>删除互斥锁</p>
</td>
</tr>
<tr id="row132412213188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p5241122111816"><a name="p5241122111816"></a><a name="p5241122111816"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p17241162114184"><a name="p17241162114184"></a><a name="p17241162114184"></a>pthread_mutex_getprioceiling</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p5241321181812"><a name="p5241321181812"></a><a name="p5241321181812"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p324162110183"><a name="p324162110183"></a><a name="p324162110183"></a>获取互斥锁的优先级上限</p>
</td>
</tr>
<tr id="row624142120188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p22411721111811"><a name="p22411721111811"></a><a name="p22411721111811"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p7241112117186"><a name="p7241112117186"></a><a name="p7241112117186"></a>pthread_mutex_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p142412021101815"><a name="p142412021101815"></a><a name="p142412021101815"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p924119210185"><a name="p924119210185"></a><a name="p924119210185"></a>初始化互斥锁</p>
</td>
</tr>
<tr id="row15241132117182"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p52412021151810"><a name="p52412021151810"></a><a name="p52412021151810"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1424192114182"><a name="p1424192114182"></a><a name="p1424192114182"></a>pthread_mutex_lock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1024152115183"><a name="p1024152115183"></a><a name="p1024152115183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p124112217186"><a name="p124112217186"></a><a name="p124112217186"></a>申请互斥锁（阻塞操作）</p>
</td>
</tr>
<tr id="row9241182111810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p4241182181811"><a name="p4241182181811"></a><a name="p4241182181811"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p224132112185"><a name="p224132112185"></a><a name="p224132112185"></a>pthread_mutex_setprioceiling</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p2024114215182"><a name="p2024114215182"></a><a name="p2024114215182"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p112416210182"><a name="p112416210182"></a><a name="p112416210182"></a>设置互斥锁的优先级上限</p>
</td>
</tr>
<tr id="row0241521171816"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p824112120185"><a name="p824112120185"></a><a name="p824112120185"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p10241122113181"><a name="p10241122113181"></a><a name="p10241122113181"></a>pthread_mutex_timedlock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1024114217189"><a name="p1024114217189"></a><a name="p1024114217189"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p8241621101817"><a name="p8241621101817"></a><a name="p8241621101817"></a>申请互斥锁（只在设定时间内阻塞）</p>
</td>
</tr>
<tr id="row182417215189"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1224116212180"><a name="p1224116212180"></a><a name="p1224116212180"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1624182111189"><a name="p1624182111189"></a><a name="p1624182111189"></a>pthread_mutex_trylock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p12411221181813"><a name="p12411221181813"></a><a name="p12411221181813"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p18241182141811"><a name="p18241182141811"></a><a name="p18241182141811"></a>尝试申请互斥锁（非阻塞）</p>
</td>
</tr>
<tr id="row5241821191817"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p7241102131812"><a name="p7241102131812"></a><a name="p7241102131812"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p4241192151811"><a name="p4241192151811"></a><a name="p4241192151811"></a>pthread_mutex_unlock</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p17241152101818"><a name="p17241152101818"></a><a name="p17241152101818"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p124192116188"><a name="p124192116188"></a><a name="p124192116188"></a>释放互斥锁</p>
</td>
</tr>
<tr id="row162411421121816"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p112413215182"><a name="p112413215182"></a><a name="p112413215182"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p20242421151816"><a name="p20242421151816"></a><a name="p20242421151816"></a>pthread_mutexattr_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p624282171816"><a name="p624282171816"></a><a name="p624282171816"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p9242172111816"><a name="p9242172111816"></a><a name="p9242172111816"></a>销毁互斥锁属性对象</p>
</td>
</tr>
<tr id="row7242162131810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1824282110189"><a name="p1824282110189"></a><a name="p1824282110189"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p13242921121817"><a name="p13242921121817"></a><a name="p13242921121817"></a>pthread_mutexattr_getprioceiling</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1724272171814"><a name="p1724272171814"></a><a name="p1724272171814"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p11242102161818"><a name="p11242102161818"></a><a name="p11242102161818"></a>获取互斥锁属性的优先级上限</p>
</td>
</tr>
<tr id="row8242162111814"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1242132191819"><a name="p1242132191819"></a><a name="p1242132191819"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p19242021121813"><a name="p19242021121813"></a><a name="p19242021121813"></a>pthread_mutexattr_getprotocol</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p424211214182"><a name="p424211214182"></a><a name="p424211214182"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p224232141814"><a name="p224232141814"></a><a name="p224232141814"></a>获取互斥锁属性的协议属性</p>
</td>
</tr>
<tr id="row5242172117186"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p224292181818"><a name="p224292181818"></a><a name="p224292181818"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1224211213181"><a name="p1224211213181"></a><a name="p1224211213181"></a>pthread_mutexattr_gettype</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p924242141812"><a name="p924242141812"></a><a name="p924242141812"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p124272131814"><a name="p124272131814"></a><a name="p124272131814"></a>获取互斥锁的类型属性</p>
</td>
</tr>
<tr id="row1824262118186"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1924214214180"><a name="p1924214214180"></a><a name="p1924214214180"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p3242621171814"><a name="p3242621171814"></a><a name="p3242621171814"></a>pthread_mutexattr_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p132426216183"><a name="p132426216183"></a><a name="p132426216183"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1524232111811"><a name="p1524232111811"></a><a name="p1524232111811"></a>初始化互斥锁属性对象</p>
</td>
</tr>
<tr id="row15242132181811"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p324211211181"><a name="p324211211181"></a><a name="p324211211181"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p12422211186"><a name="p12422211186"></a><a name="p12422211186"></a>pthread_mutexattr_setprioceiling</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p824211215182"><a name="p824211215182"></a><a name="p824211215182"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p182421721161813"><a name="p182421721161813"></a><a name="p182421721161813"></a>设置互斥锁属性的优先级上限</p>
</td>
</tr>
<tr id="row10242142116183"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1424222141818"><a name="p1424222141818"></a><a name="p1424222141818"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p13242172181812"><a name="p13242172181812"></a><a name="p13242172181812"></a>pthread_mutexattr_setprotocol</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p124222161818"><a name="p124222161818"></a><a name="p124222161818"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p112421021101810"><a name="p112421021101810"></a><a name="p112421021101810"></a>设置互斥锁属性的协议属性</p>
</td>
</tr>
<tr id="row1824212117183"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p142427213185"><a name="p142427213185"></a><a name="p142427213185"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p3242921111817"><a name="p3242921111817"></a><a name="p3242921111817"></a>pthread_mutexattr_settype</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p17242142115185"><a name="p17242142115185"></a><a name="p17242142115185"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p5242142161817"><a name="p5242142161817"></a><a name="p5242142161817"></a>设置互斥锁的类型属性</p>
</td>
</tr>
<tr id="row1624213213184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p14242221171811"><a name="p14242221171811"></a><a name="p14242221171811"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p42422021161816"><a name="p42422021161816"></a><a name="p42422021161816"></a>pthread_once</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p142421321141820"><a name="p142421321141820"></a><a name="p142421321141820"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p112421721131818"><a name="p112421721131818"></a><a name="p112421721131818"></a>一次性操作任务</p>
</td>
</tr>
<tr id="row172421521131812"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1324218212183"><a name="p1324218212183"></a><a name="p1324218212183"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p19242112171819"><a name="p19242112171819"></a><a name="p19242112171819"></a>pthread_self</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p52421221151813"><a name="p52421221151813"></a><a name="p52421221151813"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p3242521171815"><a name="p3242521171815"></a><a name="p3242521171815"></a>获取任务ID</p>
</td>
</tr>
<tr id="row42428211182"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p152423215187"><a name="p152423215187"></a><a name="p152423215187"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p12242182161816"><a name="p12242182161816"></a><a name="p12242182161816"></a>pthread_setcancelstate</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p024272116181"><a name="p024272116181"></a><a name="p024272116181"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p18242921171810"><a name="p18242921171810"></a><a name="p18242921171810"></a>任务cancel功能开关</p>
</td>
</tr>
<tr id="row1424212214184"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p19242202191818"><a name="p19242202191818"></a><a name="p19242202191818"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p52421621111811"><a name="p52421621111811"></a><a name="p52421621111811"></a>pthread_setcanceltype</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1242162151811"><a name="p1242162151811"></a><a name="p1242162151811"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p8242202118181"><a name="p8242202118181"></a><a name="p8242202118181"></a>设置任务cancel类型</p>
</td>
</tr>
<tr id="row6242921121813"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1224212118183"><a name="p1224212118183"></a><a name="p1224212118183"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p17242172117180"><a name="p17242172117180"></a><a name="p17242172117180"></a>pthread_setschedparam</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p15242132121819"><a name="p15242132121819"></a><a name="p15242132121819"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p172421621191810"><a name="p172421621191810"></a><a name="p172421621191810"></a>设置任务优先级及调度策略，Huawei LiteOS目前仅支持SCHED_RR调度策略，不支持SCHED_OTHERSCHED_FIFOSCHED_RR</p>
</td>
</tr>
<tr id="row824252121810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p102428219182"><a name="p102428219182"></a><a name="p102428219182"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p19242142118188"><a name="p19242142118188"></a><a name="p19242142118188"></a>pthread_setschedprio</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p17242162171818"><a name="p17242162171818"></a><a name="p17242162171818"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p13242821111817"><a name="p13242821111817"></a><a name="p13242821111817"></a>设置任务优先级</p>
</td>
</tr>
<tr id="row1924202181819"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p162421521181814"><a name="p162421521181814"></a><a name="p162421521181814"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p924212181817"><a name="p924212181817"></a><a name="p924212181817"></a>pthread_setspecific</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p62421221111817"><a name="p62421221111817"></a><a name="p62421221111817"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p524292120188"><a name="p524292120188"></a><a name="p524292120188"></a>设置线程数据</p>
</td>
</tr>
<tr id="row3242192181819"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p14242321191818"><a name="p14242321191818"></a><a name="p14242321191818"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p16242121141810"><a name="p16242121141810"></a><a name="p16242121141810"></a>pthread_testcancel</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p142421121151818"><a name="p142421121151818"></a><a name="p142421121151818"></a>pthread</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p324212117183"><a name="p324212117183"></a><a name="p324212117183"></a>cancel任务</p>
</td>
</tr>
<tr id="row1224232111820"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p9242421121819"><a name="p9242421121819"></a><a name="p9242421121819"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1524272171819"><a name="p1524272171819"></a><a name="p1524272171819"></a>sched_get_priority_max</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1242102171816"><a name="p1242102171816"></a><a name="p1242102171816"></a>调度函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p92421221151812"><a name="p92421221151812"></a><a name="p92421221151812"></a>获取系统支持的最大的优先级值</p>
</td>
</tr>
<tr id="row22425216188"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p52428212181"><a name="p52428212181"></a><a name="p52428212181"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p6242112119182"><a name="p6242112119182"></a><a name="p6242112119182"></a>sched_get_priority_min</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p152425216184"><a name="p152425216184"></a><a name="p152425216184"></a>调度函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p12242192151811"><a name="p12242192151811"></a><a name="p12242192151811"></a>获取系统支持的最小的优先级值</p>
</td>
</tr>
<tr id="row1724252112187"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p112425217185"><a name="p112425217185"></a><a name="p112425217185"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p162421321111816"><a name="p162421321111816"></a><a name="p162421321111816"></a>sched_yield</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1524212115185"><a name="p1524212115185"></a><a name="p1524212115185"></a>调度函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p5242221181820"><a name="p5242221181820"></a><a name="p5242221181820"></a>使当前线程放弃占用CPU</p>
</td>
</tr>
<tr id="row12421621131812"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1024222119182"><a name="p1024222119182"></a><a name="p1024222119182"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p32421421121817"><a name="p32421421121817"></a><a name="p32421421121817"></a>sem_destroy</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p132422218189"><a name="p132422218189"></a><a name="p132422218189"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p5242921141811"><a name="p5242921141811"></a><a name="p5242921141811"></a>销毁无名信号量</p>
</td>
</tr>
<tr id="row82431021111810"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p12431121121811"><a name="p12431121121811"></a><a name="p12431121121811"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p62431721131820"><a name="p62431721131820"></a><a name="p62431721131820"></a>sem_getvalue</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p52431221141811"><a name="p52431221141811"></a><a name="p52431221141811"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p192431218188"><a name="p192431218188"></a><a name="p192431218188"></a>获取指定信号量的值</p>
</td>
</tr>
<tr id="row924392114187"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p424382112181"><a name="p424382112181"></a><a name="p424382112181"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1424312213181"><a name="p1424312213181"></a><a name="p1424312213181"></a>sem_init</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p324312113181"><a name="p324312113181"></a><a name="p324312113181"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p6243202141820"><a name="p6243202141820"></a><a name="p6243202141820"></a>初始化无名信号量</p>
</td>
</tr>
<tr id="row724362117186"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p102434212184"><a name="p102434212184"></a><a name="p102434212184"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p1024310218186"><a name="p1024310218186"></a><a name="p1024310218186"></a>sem_post</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1224392111182"><a name="p1224392111182"></a><a name="p1224392111182"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p102431121121814"><a name="p102431121121814"></a><a name="p102431121121814"></a>释放一个指定的无名信号量</p>
</td>
</tr>
<tr id="row52431721131815"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p19243102151819"><a name="p19243102151819"></a><a name="p19243102151819"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p22431621131819"><a name="p22431621131819"></a><a name="p22431621131819"></a>sem_timedwait</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p152431121171813"><a name="p152431121171813"></a><a name="p152431121171813"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1424318211185"><a name="p1424318211185"></a><a name="p1424318211185"></a>申请一个超时等待的无名信号量，当超时等待时间为相对时间时，Huawei LiteOS不能处理早已超时的情况</p>
</td>
</tr>
<tr id="row12436219183"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p7243202116189"><a name="p7243202116189"></a><a name="p7243202116189"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p124312211186"><a name="p124312211186"></a><a name="p124312211186"></a>sem_trywait</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p1424392101812"><a name="p1424392101812"></a><a name="p1424392101812"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p62431721151820"><a name="p62431721151820"></a><a name="p62431721151820"></a>尝试申请一个无名信号量</p>
</td>
</tr>
<tr id="row1824311212185"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p17243122121819"><a name="p17243122121819"></a><a name="p17243122121819"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p12243202118188"><a name="p12243202118188"></a><a name="p12243202118188"></a>sem_wait</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p182431421191819"><a name="p182431421191819"></a><a name="p182431421191819"></a>信号量</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p122431421181815"><a name="p122431421181815"></a><a name="p122431421181815"></a>申请等待一个无名信号量</p>
</td>
</tr>
<tr id="row1724372111187"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p624314212183"><a name="p624314212183"></a><a name="p624314212183"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p9243221191813"><a name="p9243221191813"></a><a name="p9243221191813"></a>nanosleep</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p2243202117181"><a name="p2243202117181"></a><a name="p2243202117181"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p22431213186"><a name="p22431213186"></a><a name="p22431213186"></a>进程以纳秒为单位休眠，但Huawei LiteOS目前只支持tick（默认10ms）休眠，第二个参数不支持，且传参所设置的秒数不能大于4292秒</p>
</td>
</tr>
<tr id="row16243182181817"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1243162119182"><a name="p1243162119182"></a><a name="p1243162119182"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p824382121812"><a name="p824382121812"></a><a name="p824382121812"></a>timer_create</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p20243521171812"><a name="p20243521171812"></a><a name="p20243521171812"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p192432219188"><a name="p192432219188"></a><a name="p192432219188"></a>创建定时器，只支持SIGEV_THREAD在线程内处理（Huawei LiteOS有两种实现方式，在线程或者在中断中处理timer的回调函数，具体采用哪种方式由宏开关LOSCFG_BASE_CORE_SWTMR_IN_ISR控制）；clock_id只支持CLOCK_REALTIME</p>
</td>
</tr>
<tr id="row924332111811"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p162439218183"><a name="p162439218183"></a><a name="p162439218183"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p32439212185"><a name="p32439212185"></a><a name="p32439212185"></a>timer_delete</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p4243182121812"><a name="p4243182121812"></a><a name="p4243182121812"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1424312121818"><a name="p1424312121818"></a><a name="p1424312121818"></a>删除定时器</p>
</td>
</tr>
<tr id="row132431321131820"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p1324313213182"><a name="p1324313213182"></a><a name="p1324313213182"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p13243421191817"><a name="p13243421191817"></a><a name="p13243421191817"></a>timer_getoverrun</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p624322151815"><a name="p624322151815"></a><a name="p624322151815"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p824332114189"><a name="p824332114189"></a><a name="p824332114189"></a>获取定时器超时次数</p>
</td>
</tr>
<tr id="row42431621131815"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p324315217182"><a name="p324315217182"></a><a name="p324315217182"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p724312218187"><a name="p724312218187"></a><a name="p724312218187"></a>timer_gettime</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p112438216183"><a name="p112438216183"></a><a name="p112438216183"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p192431021151817"><a name="p192431021151817"></a><a name="p192431021151817"></a>获得一个定时器剩余时间</p>
</td>
</tr>
<tr id="row162431821181814"><td class="cellrowborder" valign="top" width="17.03%" headers="mcps1.1.5.1.1 "><p id="p18243132181814"><a name="p18243132181814"></a><a name="p18243132181814"></a>time.h</p>
</td>
<td class="cellrowborder" valign="top" width="22.830000000000002%" headers="mcps1.1.5.1.2 "><p id="p152431321181817"><a name="p152431321181817"></a><a name="p152431321181817"></a>timer_settime</p>
</td>
<td class="cellrowborder" valign="top" width="11.5%" headers="mcps1.1.5.1.3 "><p id="p112431321101810"><a name="p112431321101810"></a><a name="p112431321101810"></a>时间函数</p>
</td>
<td class="cellrowborder" valign="top" width="48.64%" headers="mcps1.1.5.1.4 "><p id="p1124316213181"><a name="p1124316213181"></a><a name="p1124316213181"></a>初始化或者撤销定时器</p>
</td>
</tr>
</tbody>
</table>

<h3 id="POSIX-NP支持接口">POSIX NP支持接口</h3>

Huawei LiteOS还提供了一套POSIX NP适配接口，来提供POSIX对SMP的支持。以下接口只有在开启多核模式下可以操作亲和性，在单核模式下直接返回ENOERR。

<a name="table15187811191616"></a>
<table><thead align="left"><tr id="row51877117164"><th class="cellrowborder" valign="top" width="17.381738173817382%" id="mcps1.1.4.1.1"><p id="p18554101919162"><a name="p18554101919162"></a><a name="p18554101919162"></a>文件名</p>
</th>
<th class="cellrowborder" valign="top" width="23.18231823182318%" id="mcps1.1.4.1.2"><p id="p1555481921613"><a name="p1555481921613"></a><a name="p1555481921613"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="59.43594359435944%" id="mcps1.1.4.1.3"><p id="p105541119141616"><a name="p105541119141616"></a><a name="p105541119141616"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row125001527111612"><td class="cellrowborder" valign="top" width="17.381738173817382%" headers="mcps1.1.4.1.1 "><p id="p8123112613163"><a name="p8123112613163"></a><a name="p8123112613163"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="23.18231823182318%" headers="mcps1.1.4.1.2 "><p id="p212332691611"><a name="p212332691611"></a><a name="p212332691611"></a>pthread_attr_setaffinity_np</p>
</td>
<td class="cellrowborder" valign="top" width="59.43594359435944%" headers="mcps1.1.4.1.3 "><p id="p121231126111611"><a name="p121231126111611"></a><a name="p121231126111611"></a>设置attr亲和性属性</p>
</td>
</tr>
<tr id="row1850032741611"><td class="cellrowborder" valign="top" width="17.381738173817382%" headers="mcps1.1.4.1.1 "><p id="p612342631618"><a name="p612342631618"></a><a name="p612342631618"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="23.18231823182318%" headers="mcps1.1.4.1.2 "><p id="p142865499328"><a name="p142865499328"></a><a name="p142865499328"></a>pthread_attr_getaffinity_np</p>
</td>
<td class="cellrowborder" valign="top" width="59.43594359435944%" headers="mcps1.1.4.1.3 "><p id="p13320230348"><a name="p13320230348"></a><a name="p13320230348"></a>获取attr亲和性属性</p>
</td>
</tr>
<tr id="row95000277165"><td class="cellrowborder" valign="top" width="17.381738173817382%" headers="mcps1.1.4.1.1 "><p id="p119713377322"><a name="p119713377322"></a><a name="p119713377322"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="23.18231823182318%" headers="mcps1.1.4.1.2 "><p id="p3123102611610"><a name="p3123102611610"></a><a name="p3123102611610"></a>pthread_setaffinity_np</p>
</td>
<td class="cellrowborder" valign="top" width="59.43594359435944%" headers="mcps1.1.4.1.3 "><p id="p112332641618"><a name="p112332641618"></a><a name="p112332641618"></a>设置pthread任务的亲和性</p>
</td>
</tr>
<tr id="row3499227101618"><td class="cellrowborder" valign="top" width="17.381738173817382%" headers="mcps1.1.4.1.1 "><p id="p5123152631619"><a name="p5123152631619"></a><a name="p5123152631619"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="23.18231823182318%" headers="mcps1.1.4.1.2 "><p id="p121231226171619"><a name="p121231226171619"></a><a name="p121231226171619"></a>pthread_getaffinity_np</p>
</td>
<td class="cellrowborder" valign="top" width="59.43594359435944%" headers="mcps1.1.4.1.3 "><p id="p112302612168"><a name="p112302612168"></a><a name="p112302612168"></a>获取pthread任务的亲和性</p>
</td>
</tr>
</tbody>
</table>

<h3 id="POSIX不支持接口">POSIX不支持接口</h3>

Huawei LiteOS的POSIX接口中，有一些未支持，具体参见下表。

<a name="table1468184223610"></a>
<table><thead align="left"><tr id="row6768114223617"><th class="cellrowborder" valign="top" width="27.71%" id="mcps1.1.3.1.1"><p id="p1768164273619"><a name="p1768164273619"></a><a name="p1768164273619"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="72.28999999999999%" id="mcps1.1.3.1.2"><p id="p97685429365"><a name="p97685429365"></a><a name="p97685429365"></a>接口名</p>
</th>
</tr>
</thead>
<tbody><tr id="row11768942133618"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p976817425369"><a name="p976817425369"></a><a name="p976817425369"></a>mqueue.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p676815425363"><a name="p676815425363"></a><a name="p676815425363"></a>mq_notify</p>
</td>
</tr>
<tr id="row7768114263612"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p576814211367"><a name="p576814211367"></a><a name="p576814211367"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p16768642103619"><a name="p16768642103619"></a><a name="p16768642103619"></a>pthread_atfork</p>
</td>
</tr>
<tr id="row1076874253612"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p157682042163610"><a name="p157682042163610"></a><a name="p157682042163610"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17768144212369"><a name="p17768144212369"></a><a name="p17768144212369"></a>pthread_attr_getguardsize</p>
</td>
</tr>
<tr id="row276818426364"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p147681342193612"><a name="p147681342193612"></a><a name="p147681342193612"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p187681423365"><a name="p187681423365"></a><a name="p187681423365"></a>pthread_attr_getstack</p>
</td>
</tr>
<tr id="row376824263612"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p176824217364"><a name="p176824217364"></a><a name="p176824217364"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1276834273611"><a name="p1276834273611"></a><a name="p1276834273611"></a>pthread_attr_setguardsize</p>
</td>
</tr>
<tr id="row1576854203615"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p876812426361"><a name="p876812426361"></a><a name="p876812426361"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p176816427365"><a name="p176816427365"></a><a name="p176816427365"></a>pthread_attr_setstack</p>
</td>
</tr>
<tr id="row1476816420363"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p14768144283611"><a name="p14768144283611"></a><a name="p14768144283611"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1276854233610"><a name="p1276854233610"></a><a name="p1276854233610"></a>pthread_barrier_destroy</p>
</td>
</tr>
<tr id="row137681442203611"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p167681942143613"><a name="p167681942143613"></a><a name="p167681942143613"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p876834217368"><a name="p876834217368"></a><a name="p876834217368"></a>pthread_barrier_init</p>
</td>
</tr>
<tr id="row2076804253615"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p17682428367"><a name="p17682428367"></a><a name="p17682428367"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p127689429365"><a name="p127689429365"></a><a name="p127689429365"></a>pthread_barrier_wait</p>
</td>
</tr>
<tr id="row77681426365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p5768542133612"><a name="p5768542133612"></a><a name="p5768542133612"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p187681742123619"><a name="p187681742123619"></a><a name="p187681742123619"></a>pthread_barrierattr_destroy</p>
</td>
</tr>
<tr id="row076811421365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p176813428363"><a name="p176813428363"></a><a name="p176813428363"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1676854213361"><a name="p1676854213361"></a><a name="p1676854213361"></a>pthread_barrierattr_getpshared</p>
</td>
</tr>
<tr id="row376864273616"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1576834243610"><a name="p1576834243610"></a><a name="p1576834243610"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p20768142173614"><a name="p20768142173614"></a><a name="p20768142173614"></a>pthread_barrierattr_init</p>
</td>
</tr>
<tr id="row1976894253613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1076804215361"><a name="p1076804215361"></a><a name="p1076804215361"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p376810420360"><a name="p376810420360"></a><a name="p376810420360"></a>pthread_barrierattr_setpshared</p>
</td>
</tr>
<tr id="row176818425367"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p2076817424369"><a name="p2076817424369"></a><a name="p2076817424369"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p27681242103611"><a name="p27681242103611"></a><a name="p27681242103611"></a>pthread_getconcurrency</p>
</td>
</tr>
<tr id="row3768114213611"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p57687428362"><a name="p57687428362"></a><a name="p57687428362"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p97686429366"><a name="p97686429366"></a><a name="p97686429366"></a>pthread_getcpuclockid</p>
</td>
</tr>
<tr id="row137681542103617"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p976814215369"><a name="p976814215369"></a><a name="p976814215369"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p19768124216366"><a name="p19768124216366"></a><a name="p19768124216366"></a>pthread_mutex_consistent</p>
</td>
</tr>
<tr id="row137681428361"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p17768134263610"><a name="p17768134263610"></a><a name="p17768134263610"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p276820424367"><a name="p276820424367"></a><a name="p276820424367"></a>pthread_mutexattr_getpshared</p>
</td>
</tr>
<tr id="row77686427368"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p9768142183613"><a name="p9768142183613"></a><a name="p9768142183613"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p676810421365"><a name="p676810421365"></a><a name="p676810421365"></a>pthread_mutexattr_getrobust</p>
</td>
</tr>
<tr id="row8768194213616"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p6769164211362"><a name="p6769164211362"></a><a name="p6769164211362"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p4769174283613"><a name="p4769174283613"></a><a name="p4769174283613"></a>pthread_mutexattr_setpshared</p>
</td>
</tr>
<tr id="row876954223610"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p4769942193612"><a name="p4769942193612"></a><a name="p4769942193612"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p15769194219365"><a name="p15769194219365"></a><a name="p15769194219365"></a>pthread_mutexattr_setrobust</p>
</td>
</tr>
<tr id="row17769154214367"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p12769114212365"><a name="p12769114212365"></a><a name="p12769114212365"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p147692421363"><a name="p147692421363"></a><a name="p147692421363"></a>pthread_rwlock_destroy</p>
</td>
</tr>
<tr id="row576916427362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p376924210369"><a name="p376924210369"></a><a name="p376924210369"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p2769104211367"><a name="p2769104211367"></a><a name="p2769104211367"></a>pthread_rwlock_init</p>
</td>
</tr>
<tr id="row67691642163613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p27691542143617"><a name="p27691542143617"></a><a name="p27691542143617"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p47691942113619"><a name="p47691942113619"></a><a name="p47691942113619"></a>pthread_rwlock_rdlock</p>
</td>
</tr>
<tr id="row1976904217366"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p07691442113616"><a name="p07691442113616"></a><a name="p07691442113616"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p476911427366"><a name="p476911427366"></a><a name="p476911427366"></a>pthread_rwlock_timedrdlock</p>
</td>
</tr>
<tr id="row1976934211362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p16769184223611"><a name="p16769184223611"></a><a name="p16769184223611"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p3769144253611"><a name="p3769144253611"></a><a name="p3769144253611"></a>pthread_rwlock_timedwrlock</p>
</td>
</tr>
<tr id="row976917426365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1776915420364"><a name="p1776915420364"></a><a name="p1776915420364"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p12769104233610"><a name="p12769104233610"></a><a name="p12769104233610"></a>pthread_rwlock_tryrdlock</p>
</td>
</tr>
<tr id="row57691242193618"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p97695427369"><a name="p97695427369"></a><a name="p97695427369"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p18769184293610"><a name="p18769184293610"></a><a name="p18769184293610"></a>pthread_rwlock_trywrlock</p>
</td>
</tr>
<tr id="row11769124243611"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1176912425364"><a name="p1176912425364"></a><a name="p1176912425364"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p7769142113615"><a name="p7769142113615"></a><a name="p7769142113615"></a>pthread_rwlock_unlock</p>
</td>
</tr>
<tr id="row2769164219364"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p6769174215360"><a name="p6769174215360"></a><a name="p6769174215360"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p976954210361"><a name="p976954210361"></a><a name="p976954210361"></a>pthread_rwlock_wrlock</p>
</td>
</tr>
<tr id="row7769174220362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p7769942133618"><a name="p7769942133618"></a><a name="p7769942133618"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p15769342153612"><a name="p15769342153612"></a><a name="p15769342153612"></a>pthread_rwlockattr_destroy</p>
</td>
</tr>
<tr id="row11769184233615"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p5769184217364"><a name="p5769184217364"></a><a name="p5769184217364"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p10769104217361"><a name="p10769104217361"></a><a name="p10769104217361"></a>pthread_rwlockattr_getpshared</p>
</td>
</tr>
<tr id="row12769134214366"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p776944215366"><a name="p776944215366"></a><a name="p776944215366"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p8769174223612"><a name="p8769174223612"></a><a name="p8769174223612"></a>pthread_rwlockattr_init</p>
</td>
</tr>
<tr id="row15769194211362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1576914220364"><a name="p1576914220364"></a><a name="p1576914220364"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p57692423369"><a name="p57692423369"></a><a name="p57692423369"></a>pthread_rwlockattr_setpshared</p>
</td>
</tr>
<tr id="row17769134218360"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p16769842203615"><a name="p16769842203615"></a><a name="p16769842203615"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p776994293616"><a name="p776994293616"></a><a name="p776994293616"></a>pthread_setconcurrency</p>
</td>
</tr>
<tr id="row4769194233614"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p776917426369"><a name="p776917426369"></a><a name="p776917426369"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p276914219367"><a name="p276914219367"></a><a name="p276914219367"></a>pthread_spin_destroy</p>
</td>
</tr>
<tr id="row37691642123613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p18769124219365"><a name="p18769124219365"></a><a name="p18769124219365"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p8769542123615"><a name="p8769542123615"></a><a name="p8769542123615"></a>pthread_spin_init</p>
</td>
</tr>
<tr id="row13769194219367"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p147691742163615"><a name="p147691742163615"></a><a name="p147691742163615"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1676974293612"><a name="p1676974293612"></a><a name="p1676974293612"></a>pthread_spin_lock</p>
</td>
</tr>
<tr id="row776910428369"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p776912429361"><a name="p776912429361"></a><a name="p776912429361"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p19769142173613"><a name="p19769142173613"></a><a name="p19769142173613"></a>pthread_spin_trylock</p>
</td>
</tr>
<tr id="row147691742133615"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p18769742113616"><a name="p18769742113616"></a><a name="p18769742113616"></a>pthread.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17769154218365"><a name="p17769154218365"></a><a name="p17769154218365"></a>pthread_spin_unlock</p>
</td>
</tr>
<tr id="row17691142133616"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1076984212367"><a name="p1076984212367"></a><a name="p1076984212367"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p676994213618"><a name="p676994213618"></a><a name="p676994213618"></a>sched_getparam</p>
</td>
</tr>
<tr id="row13769124213367"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p17769194211368"><a name="p17769194211368"></a><a name="p17769194211368"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p107691342113616"><a name="p107691342113616"></a><a name="p107691342113616"></a>sched_getscheduler</p>
</td>
</tr>
<tr id="row1476994233613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p176974219368"><a name="p176974219368"></a><a name="p176974219368"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p13769184213614"><a name="p13769184213614"></a><a name="p13769184213614"></a>sched_rr_get_interval</p>
</td>
</tr>
<tr id="row17691142143615"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p177691428361"><a name="p177691428361"></a><a name="p177691428361"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p197697425362"><a name="p197697425362"></a><a name="p197697425362"></a>sched_setparam</p>
</td>
</tr>
<tr id="row1876916423361"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p11769114263610"><a name="p11769114263610"></a><a name="p11769114263610"></a>sched.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p2076984293612"><a name="p2076984293612"></a><a name="p2076984293612"></a>sched_setscheduler</p>
</td>
</tr>
<tr id="row19769194212365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p776916427367"><a name="p776916427367"></a><a name="p776916427367"></a>sem.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p15769342203612"><a name="p15769342203612"></a><a name="p15769342203612"></a>semctl</p>
</td>
</tr>
<tr id="row11769542173619"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1976954219362"><a name="p1976954219362"></a><a name="p1976954219362"></a>sem.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p97691742103616"><a name="p97691742103616"></a><a name="p97691742103616"></a>semget</p>
</td>
</tr>
<tr id="row676974210361"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1276984212362"><a name="p1276984212362"></a><a name="p1276984212362"></a>sem.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17692424369"><a name="p17692424369"></a><a name="p17692424369"></a>semop</p>
</td>
</tr>
<tr id="row176917420365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1676914273619"><a name="p1676914273619"></a><a name="p1676914273619"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p177699429368"><a name="p177699429368"></a><a name="p177699429368"></a>sem_close</p>
</td>
</tr>
<tr id="row157697429364"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p10769542113612"><a name="p10769542113612"></a><a name="p10769542113612"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p107692042123619"><a name="p107692042123619"></a><a name="p107692042123619"></a>sem_open</p>
</td>
</tr>
<tr id="row37691542143617"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p676913424363"><a name="p676913424363"></a><a name="p676913424363"></a>semaphore.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p676944273619"><a name="p676944273619"></a><a name="p676944273619"></a>sem_unlink</p>
</td>
</tr>
<tr id="row77691342143619"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1776915428362"><a name="p1776915428362"></a><a name="p1776915428362"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p2769642153617"><a name="p2769642153617"></a><a name="p2769642153617"></a>kill</p>
</td>
</tr>
<tr id="row19769742163613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1676984283618"><a name="p1676984283618"></a><a name="p1676984283618"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p19769184243612"><a name="p19769184243612"></a><a name="p19769184243612"></a>killpg</p>
</td>
</tr>
<tr id="row577084214369"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1477019425366"><a name="p1477019425366"></a><a name="p1477019425366"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17770184293616"><a name="p17770184293616"></a><a name="p17770184293616"></a>psiginfo</p>
</td>
</tr>
<tr id="row177019426361"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p4770842113612"><a name="p4770842113612"></a><a name="p4770842113612"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17701942103619"><a name="p17701942103619"></a><a name="p17701942103619"></a>psignal</p>
</td>
</tr>
<tr id="row57709426365"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p15770144219363"><a name="p15770144219363"></a><a name="p15770144219363"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p14770342123616"><a name="p14770342123616"></a><a name="p14770342123616"></a>pthread_kill</p>
</td>
</tr>
<tr id="row16770242143610"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1077015423369"><a name="p1077015423369"></a><a name="p1077015423369"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1377084213619"><a name="p1377084213619"></a><a name="p1377084213619"></a>pthread_sigmask</p>
</td>
</tr>
<tr id="row1777019423360"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p97709424368"><a name="p97709424368"></a><a name="p97709424368"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p14770194273618"><a name="p14770194273618"></a><a name="p14770194273618"></a>raise</p>
</td>
</tr>
<tr id="row1377074203616"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p277034219362"><a name="p277034219362"></a><a name="p277034219362"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p07701442103617"><a name="p07701442103617"></a><a name="p07701442103617"></a>sigaction</p>
</td>
</tr>
<tr id="row977019425363"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p11770194253617"><a name="p11770194253617"></a><a name="p11770194253617"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p477044218369"><a name="p477044218369"></a><a name="p477044218369"></a>sigaddset</p>
</td>
</tr>
<tr id="row777014214368"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p87701428363"><a name="p87701428363"></a><a name="p87701428363"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p6770124213617"><a name="p6770124213617"></a><a name="p6770124213617"></a>sigaltstack</p>
</td>
</tr>
<tr id="row07701542113618"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p16770104233618"><a name="p16770104233618"></a><a name="p16770104233618"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p6770342183618"><a name="p6770342183618"></a><a name="p6770342183618"></a>sigdelset</p>
</td>
</tr>
<tr id="row11770842163612"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p15770144210365"><a name="p15770144210365"></a><a name="p15770144210365"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p15770342133616"><a name="p15770342133616"></a><a name="p15770342133616"></a>sigemptyset</p>
</td>
</tr>
<tr id="row777034219369"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1577014283611"><a name="p1577014283611"></a><a name="p1577014283611"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p16770114210365"><a name="p16770114210365"></a><a name="p16770114210365"></a>sigfillset</p>
</td>
</tr>
<tr id="row377034233610"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p577014212365"><a name="p577014212365"></a><a name="p577014212365"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p10770204216368"><a name="p10770204216368"></a><a name="p10770204216368"></a>sighold</p>
</td>
</tr>
<tr id="row377064253620"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p17701642103620"><a name="p17701642103620"></a><a name="p17701642103620"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p11770142123613"><a name="p11770142123613"></a><a name="p11770142123613"></a>sigignore</p>
</td>
</tr>
<tr id="row7770642113612"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p37701742133611"><a name="p37701742133611"></a><a name="p37701742133611"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1377024212362"><a name="p1377024212362"></a><a name="p1377024212362"></a>siginterrupt</p>
</td>
</tr>
<tr id="row5770104293613"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1377064223613"><a name="p1377064223613"></a><a name="p1377064223613"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p2770174293611"><a name="p2770174293611"></a><a name="p2770174293611"></a>sigismember</p>
</td>
</tr>
<tr id="row1177074211362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p13770204214365"><a name="p13770204214365"></a><a name="p13770204214365"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1777014263610"><a name="p1777014263610"></a><a name="p1777014263610"></a>sigpause</p>
</td>
</tr>
<tr id="row777094263610"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1577015426369"><a name="p1577015426369"></a><a name="p1577015426369"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p27701942193610"><a name="p27701942193610"></a><a name="p27701942193610"></a>sigpending</p>
</td>
</tr>
<tr id="row27709421362"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p137708429361"><a name="p137708429361"></a><a name="p137708429361"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p11770742153617"><a name="p11770742153617"></a><a name="p11770742153617"></a>sigprocmask</p>
</td>
</tr>
<tr id="row4770124210368"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p477044223618"><a name="p477044223618"></a><a name="p477044223618"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p2077054212367"><a name="p2077054212367"></a><a name="p2077054212367"></a>sigqueue</p>
</td>
</tr>
<tr id="row14770942163617"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1877024233618"><a name="p1877024233618"></a><a name="p1877024233618"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p17701242203612"><a name="p17701242203612"></a><a name="p17701242203612"></a>sigrelse</p>
</td>
</tr>
<tr id="row57702429366"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p1977034283614"><a name="p1977034283614"></a><a name="p1977034283614"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p177064223618"><a name="p177064223618"></a><a name="p177064223618"></a>sigsuspend</p>
</td>
</tr>
<tr id="row67701342133618"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p4770542133611"><a name="p4770542133611"></a><a name="p4770542133611"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p277084243610"><a name="p277084243610"></a><a name="p277084243610"></a>sigtimedwait</p>
</td>
</tr>
<tr id="row977010425361"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p127701427369"><a name="p127701427369"></a><a name="p127701427369"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p3770194219364"><a name="p3770194219364"></a><a name="p3770194219364"></a>sigwait</p>
</td>
</tr>
<tr id="row4770242103611"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p977013421363"><a name="p977013421363"></a><a name="p977013421363"></a>signal.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p377016426367"><a name="p377016426367"></a><a name="p377016426367"></a>sigwaitinfo</p>
</td>
</tr>
<tr id="row777013424369"><td class="cellrowborder" valign="top" width="27.71%" headers="mcps1.1.3.1.1 "><p id="p117701542183613"><a name="p117701542183613"></a><a name="p117701542183613"></a>signalfd.h</p>
</td>
<td class="cellrowborder" valign="top" width="72.28999999999999%" headers="mcps1.1.3.1.2 "><p id="p1977012428362"><a name="p1977012428362"></a><a name="p1977012428362"></a>signalfd</p>
</td>
</tr>
</tbody>
</table>

<h2 id="2">C++兼容规格</h2>

下述表格为C++标准库/STL兼容规格。

>![](public_sys-resources/icon-note.gif) **说明：** 
>支持异常和RTTI特性，其他特性由编译器支持；对于STL的特性，支持清单如下，其它特性暂不支持。

-   支持语言支持功能

<a name="tb5011c86880b4071888baf7bb46a3a7c"></a>
<table><thead align="left"><tr id="r0fa40d72a3664de99bdcaf34c4a7b544"><th class="cellrowborder" valign="top" width="25.2%" id="mcps1.1.3.1.1"><p id="aab8c98734c40437fb69b2f9677630c04"><a name="aab8c98734c40437fb69b2f9677630c04"></a><a name="aab8c98734c40437fb69b2f9677630c04"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.8%" id="mcps1.1.3.1.2"><p id="a19ff5cd219d643dda48a647b3c068f08"><a name="a19ff5cd219d643dda48a647b3c068f08"></a><a name="a19ff5cd219d643dda48a647b3c068f08"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r22c09c66d66a4443b04f39d289ba60db"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="a2c02254ebf2946c68f2323f7b65a5826"><a name="a2c02254ebf2946c68f2323f7b65a5826"></a><a name="a2c02254ebf2946c68f2323f7b65a5826"></a>&lt;limits&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="adbcdd7184b35489b848d339608bf8440"><a name="adbcdd7184b35489b848d339608bf8440"></a><a name="adbcdd7184b35489b848d339608bf8440"></a>提供与基本数据类型相关的定义，例如，对于每个数值数据类型，它定义了可以表示出来的最大值和最小值以及二进制数字的位数</p>
</td>
</tr>
<tr id="rdfdd5b6871f14188ad347b830ce133cd"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="a833da918654a483dbcfea2c391e6d3fe"><a name="a833da918654a483dbcfea2c391e6d3fe"></a><a name="a833da918654a483dbcfea2c391e6d3fe"></a>&lt;new&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="a8eafe0ce367145d6bbe4a34121b579d0"><a name="a8eafe0ce367145d6bbe4a34121b579d0"></a><a name="a8eafe0ce367145d6bbe4a34121b579d0"></a>支持动态内存分配</p>
</td>
</tr>
</tbody>
</table>

-   支持工具函数

<a name="t5dc8542eda604913ae73c3b44512d0d7"></a>
<table><thead align="left"><tr id="rd69744342edb4d6790411dd579ec7d63"><th class="cellrowborder" valign="top" width="25.2%" id="mcps1.1.3.1.1"><p id="ab1073e98fe164e928634e747114f6f42"><a name="ab1073e98fe164e928634e747114f6f42"></a><a name="ab1073e98fe164e928634e747114f6f42"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.8%" id="mcps1.1.3.1.2"><p id="a8dfc3b965a7746a1889f9deb36bc49c1"><a name="a8dfc3b965a7746a1889f9deb36bc49c1"></a><a name="a8dfc3b965a7746a1889f9deb36bc49c1"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r8a853e91ba384d38a054d37e42329cfa"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="a085f0de11b724508a401d8eede3451f9"><a name="a085f0de11b724508a401d8eede3451f9"></a><a name="a085f0de11b724508a401d8eede3451f9"></a>&lt;utility&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="a028c631d87e34bdab8d8babeeefe728f"><a name="a028c631d87e34bdab8d8babeeefe728f"></a><a name="a028c631d87e34bdab8d8babeeefe728f"></a>定义重载的关系运算符，简化关系运算符的写入，它还定义了pair类型，该类型是一种模板类型，可以存储一对值。这些功能在库的其他地方使用</p>
</td>
</tr>
<tr id="r25455838a07a4780b63e7dfeeff4a985"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="a823bfd0552f54377b77377f5d1671aab"><a name="a823bfd0552f54377b77377f5d1671aab"></a><a name="a823bfd0552f54377b77377f5d1671aab"></a>&lt;functional&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="ab05a751ca3e74b35b34bf07eb318e794"><a name="ab05a751ca3e74b35b34bf07eb318e794"></a><a name="ab05a751ca3e74b35b34bf07eb318e794"></a>定义了许多函数对象类型和支持函数对象的功能，函数对象是支持operator函数调用运算符的任意对象</p>
</td>
</tr>
<tr id="r9511e1af4e0249ad952af05686a86b3e"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="ac406aea40a9549949004bfea6a54e6da"><a name="ac406aea40a9549949004bfea6a54e6da"></a><a name="ac406aea40a9549949004bfea6a54e6da"></a>&lt;memory&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="af8bda69fb34a4898a038def942c969a2"><a name="af8bda69fb34a4898a038def942c969a2"></a><a name="af8bda69fb34a4898a038def942c969a2"></a>给容器、管理内存的函数和auto_ptr模板类定义标准内存分配器</p>
</td>
</tr>
</tbody>
</table>

-   支持字符串处理

<a name="t1dd8d1470cfb436aa9abb123a093e946"></a>
<table><thead align="left"><tr id="r9855aaba8d3448d9bc0035b6d5e1ae02"><th class="cellrowborder" valign="top" width="25.2%" id="mcps1.1.3.1.1"><p id="a632d94dc2c3a4255804fe160b00433a9"><a name="a632d94dc2c3a4255804fe160b00433a9"></a><a name="a632d94dc2c3a4255804fe160b00433a9"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.8%" id="mcps1.1.3.1.2"><p id="a17022f0de8a64e4f96012d78cd990ac7"><a name="a17022f0de8a64e4f96012d78cd990ac7"></a><a name="a17022f0de8a64e4f96012d78cd990ac7"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="ra0aa2a71ef2d49e8bdb7f4cc323660a6"><td class="cellrowborder" valign="top" width="25.2%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0035786690_p854766173929"><a name="zh-cn_topic_0035786690_p854766173929"></a><a name="zh-cn_topic_0035786690_p854766173929"></a>&lt;string&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.8%" headers="mcps1.1.3.1.2 "><p id="a64a918ac96074738b5cb821e7d01cadc"><a name="a64a918ac96074738b5cb821e7d01cadc"></a><a name="a64a918ac96074738b5cb821e7d01cadc"></a>为字符串类型提供支持和定义，包括单字节字符串(由char组成)的string和多字节字符串(由wchar_t组成)</p>
</td>
</tr>
</tbody>
</table>

-   支持容器类模板

<a name="t87c00d989ee94e51839d0ac94b2f2bf9"></a>
<table><thead align="left"><tr id="r722c380696db4380867eb7127640c7f9"><th class="cellrowborder" valign="top" width="25.290000000000003%" id="mcps1.1.3.1.1"><p id="a86f7f8430ce749fc8149733509749199"><a name="a86f7f8430ce749fc8149733509749199"></a><a name="a86f7f8430ce749fc8149733509749199"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.71%" id="mcps1.1.3.1.2"><p id="aefc1d84e7e6944d78c50a480d0388421"><a name="aefc1d84e7e6944d78c50a480d0388421"></a><a name="aefc1d84e7e6944d78c50a480d0388421"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r0924471e43b9445285c7a5189119a252"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="af570d88dfdc945b2b9b946a38827b596"><a name="af570d88dfdc945b2b9b946a38827b596"></a><a name="af570d88dfdc945b2b9b946a38827b596"></a>&lt;vector&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a92367331ff454723a2a2cb6ff963c136"><a name="a92367331ff454723a2a2cb6ff963c136"></a><a name="a92367331ff454723a2a2cb6ff963c136"></a>定义vector序列模板，这是一个大小可以重新设置的数组类型，比普通数组更安全、更灵活</p>
</td>
</tr>
<tr id="rdebdf8ee0cab4dfb927647ccab6275a0"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="a81f807f2e3fd49c283f6d5e0d30b7bf6"><a name="a81f807f2e3fd49c283f6d5e0d30b7bf6"></a><a name="a81f807f2e3fd49c283f6d5e0d30b7bf6"></a>&lt;list&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a7f6c774dbd5043eeb83d03d3f87f4dcc"><a name="a7f6c774dbd5043eeb83d03d3f87f4dcc"></a><a name="a7f6c774dbd5043eeb83d03d3f87f4dcc"></a>定义list序列模板，这是一个序列的链表，常常在任意位置插入和删除元素</p>
</td>
</tr>
<tr id="r13c1c4714ef748be9a7c75bfa597043e"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="a72a1691eaa03469ab9de0f6d38d94e66"><a name="a72a1691eaa03469ab9de0f6d38d94e66"></a><a name="a72a1691eaa03469ab9de0f6d38d94e66"></a>&lt;deque&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a12a3f11b3a364e63ad85cb3df5118b85"><a name="a12a3f11b3a364e63ad85cb3df5118b85"></a><a name="a12a3f11b3a364e63ad85cb3df5118b85"></a>定义deque序列模板，支持在开始和结尾的高效插入和删除操作</p>
</td>
</tr>
<tr id="r069d71dc0020447da7b26b0049014876"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="aaa47f7cdd16b4cdf971fe3be95a09453"><a name="aaa47f7cdd16b4cdf971fe3be95a09453"></a><a name="aaa47f7cdd16b4cdf971fe3be95a09453"></a>&lt;queue&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="adc1451ea4b8f433e94e275ef0d40395f"><a name="adc1451ea4b8f433e94e275ef0d40395f"></a><a name="adc1451ea4b8f433e94e275ef0d40395f"></a>为队列(先进先出)数据结构定义序列适配器queue和priority_queue</p>
</td>
</tr>
<tr id="r603feeca67d846909640c84a414ec995"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="a4fe52d4774764786900799e286a3b3c8"><a name="a4fe52d4774764786900799e286a3b3c8"></a><a name="a4fe52d4774764786900799e286a3b3c8"></a>&lt;stack&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a063270c45c9546b9b4a459c0e5740424"><a name="a063270c45c9546b9b4a459c0e5740424"></a><a name="a063270c45c9546b9b4a459c0e5740424"></a>为堆栈(后进先出)数据结构定义序列适配器stack</p>
</td>
</tr>
<tr id="r12df200b4bbb45b28a159f57f4451ffc"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="a43ed56c272974b37b2c50b63cce1879b"><a name="a43ed56c272974b37b2c50b63cce1879b"></a><a name="a43ed56c272974b37b2c50b63cce1879b"></a>&lt;map&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a52619552023f4eedadb53a9c794fbb5a"><a name="a52619552023f4eedadb53a9c794fbb5a"></a><a name="a52619552023f4eedadb53a9c794fbb5a"></a>map是一个关联容器类型，允许根据唯一键值，按照升序存储</p>
</td>
</tr>
<tr id="r38a9a1a465d346c9877e010e195e28e6"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0035786690_p368191173929"><a name="zh-cn_topic_0035786690_p368191173929"></a><a name="zh-cn_topic_0035786690_p368191173929"></a>&lt;set&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="a1b9597733d1e4753aa3bb05451e5101a"><a name="a1b9597733d1e4753aa3bb05451e5101a"></a><a name="a1b9597733d1e4753aa3bb05451e5101a"></a>set是一个关联容器类型，用于以升序方式存储唯一值</p>
</td>
</tr>
<tr id="rf15d89f52a8347a5994b73e14deaf2e4"><td class="cellrowborder" valign="top" width="25.290000000000003%" headers="mcps1.1.3.1.1 "><p id="a73ae6025f0d847828bd2280388dcfaf0"><a name="a73ae6025f0d847828bd2280388dcfaf0"></a><a name="a73ae6025f0d847828bd2280388dcfaf0"></a>&lt;bitset&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.71%" headers="mcps1.1.3.1.2 "><p id="ab3b919b043424b4bb33358ee9167fa32"><a name="ab3b919b043424b4bb33358ee9167fa32"></a><a name="ab3b919b043424b4bb33358ee9167fa32"></a>为固定长度的位序列定义bitset模板，它可以看作固定长度的紧凑型bool数组</p>
</td>
</tr>
</tbody>
</table>

-   支持迭代器

<a name="t3496afb069254804ade36deec669e208"></a>
<table><thead align="left"><tr id="rced263611b014d13ad581925684a5e7f"><th class="cellrowborder" valign="top" width="25.64%" id="mcps1.1.3.1.1"><p id="ae49479598f754ae2803864adc4baea85"><a name="ae49479598f754ae2803864adc4baea85"></a><a name="ae49479598f754ae2803864adc4baea85"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.36%" id="mcps1.1.3.1.2"><p id="a79c4d19f99cc488e84f89ab4cf8afb3e"><a name="a79c4d19f99cc488e84f89ab4cf8afb3e"></a><a name="a79c4d19f99cc488e84f89ab4cf8afb3e"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="rf16e1c6a393e4d569ec997c0ffa8cfd7"><td class="cellrowborder" valign="top" width="25.64%" headers="mcps1.1.3.1.1 "><p id="a08c776960017433eb3b345bd0ad51adb"><a name="a08c776960017433eb3b345bd0ad51adb"></a><a name="a08c776960017433eb3b345bd0ad51adb"></a>&lt;iterator&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.36%" headers="mcps1.1.3.1.2 "><p id="a5ef86f908ec0454f8f286ec79d8f689a"><a name="a5ef86f908ec0454f8f286ec79d8f689a"></a><a name="a5ef86f908ec0454f8f286ec79d8f689a"></a>给迭代器提供定义和支持</p>
</td>
</tr>
</tbody>
</table>

-   支持算法

<a name="t25dc6a6b4ac24d9f9c786db422078443"></a>
<table><thead align="left"><tr id="rb0a002a4f1b34bec88ce84a4cbe94f13"><th class="cellrowborder" valign="top" width="25.900000000000002%" id="mcps1.1.3.1.1"><p id="ab8dda9f82aa84646a5895bde82299a3f"><a name="ab8dda9f82aa84646a5895bde82299a3f"></a><a name="ab8dda9f82aa84646a5895bde82299a3f"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.1%" id="mcps1.1.3.1.2"><p id="a069fa62bf58e42e8aa098a0dd6b08c81"><a name="a069fa62bf58e42e8aa098a0dd6b08c81"></a><a name="a069fa62bf58e42e8aa098a0dd6b08c81"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r9107b7864fbd466eab6a3fa6af5266b8"><td class="cellrowborder" valign="top" width="25.900000000000002%" headers="mcps1.1.3.1.1 "><p id="a44c9027b360c42e6aa296d8012a31f55"><a name="a44c9027b360c42e6aa296d8012a31f55"></a><a name="a44c9027b360c42e6aa296d8012a31f55"></a>&lt;algorithm&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.1%" headers="mcps1.1.3.1.2 "><p id="a120082ce4b2c46c38a76ab8590db7dfc"><a name="a120082ce4b2c46c38a76ab8590db7dfc"></a><a name="a120082ce4b2c46c38a76ab8590db7dfc"></a>提供一组基于算法的函数，包括置换、排序、合并和搜索</p>
</td>
</tr>
</tbody>
</table>

-   支持数值操作

<a name="te1ead6aa39484ae4861ecb6725f7fcba"></a>
<table><thead align="left"><tr id="r14ae2c6324124a0499b770acaa8b5613"><th class="cellrowborder" valign="top" width="25.900000000000002%" id="mcps1.1.3.1.1"><p id="a04a6753e10714ff399aeb9c74ad2c747"><a name="a04a6753e10714ff399aeb9c74ad2c747"></a><a name="a04a6753e10714ff399aeb9c74ad2c747"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="74.1%" id="mcps1.1.3.1.2"><p id="a34b561f179024325b25f833f1c83a0ea"><a name="a34b561f179024325b25f833f1c83a0ea"></a><a name="a34b561f179024325b25f833f1c83a0ea"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r91d3d6b841bc44a593e87fee98f00aa2"><td class="cellrowborder" valign="top" width="25.900000000000002%" headers="mcps1.1.3.1.1 "><p id="a97d6280dfb534b82afea8568d9d75c57"><a name="a97d6280dfb534b82afea8568d9d75c57"></a><a name="a97d6280dfb534b82afea8568d9d75c57"></a>&lt;complex&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.1%" headers="mcps1.1.3.1.2 "><p id="a3565f1f7e3474bf498c081caaf316009"><a name="a3565f1f7e3474bf498c081caaf316009"></a><a name="a3565f1f7e3474bf498c081caaf316009"></a>支持复杂数值的定义和操作</p>
</td>
</tr>
<tr id="r7a1b3473a6124fa3a2c240631e506133"><td class="cellrowborder" valign="top" width="25.900000000000002%" headers="mcps1.1.3.1.1 "><p id="a0c7cb9bf0c9349bb857f3ab28c074efa"><a name="a0c7cb9bf0c9349bb857f3ab28c074efa"></a><a name="a0c7cb9bf0c9349bb857f3ab28c074efa"></a>&lt;valarray&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.1%" headers="mcps1.1.3.1.2 "><p id="ac81a8283c2f24bda9ddd310c39b4adf6"><a name="ac81a8283c2f24bda9ddd310c39b4adf6"></a><a name="ac81a8283c2f24bda9ddd310c39b4adf6"></a>支持数值矢量的操作</p>
</td>
</tr>
<tr id="ra208486270ff4362b2161244e581ca7c"><td class="cellrowborder" valign="top" width="25.900000000000002%" headers="mcps1.1.3.1.1 "><p id="a8b54a285ca71466e91c4c9ba8ea7bd0a"><a name="a8b54a285ca71466e91c4c9ba8ea7bd0a"></a><a name="a8b54a285ca71466e91c4c9ba8ea7bd0a"></a>&lt;numeric&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="74.1%" headers="mcps1.1.3.1.2 "><p id="abd10bf26221c433ab85491a3e11182d0"><a name="abd10bf26221c433ab85491a3e11182d0"></a><a name="abd10bf26221c433ab85491a3e11182d0"></a>在数值序列上定义一组一般数学操作，例如accumulate和inner_product</p>
</td>
</tr>
</tbody>
</table>

-   支持线程处理

<a name="table1384122162712"></a>
<table><thead align="left"><tr id="row98413262712"><th class="cellrowborder" valign="top" width="26.08%" id="mcps1.1.3.1.1"><p id="p084118232717"><a name="p084118232717"></a><a name="p084118232717"></a>头文件</p>
</th>
<th class="cellrowborder" valign="top" width="73.92%" id="mcps1.1.3.1.2"><p id="p178413212710"><a name="p178413212710"></a><a name="p178413212710"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12841829275"><td class="cellrowborder" valign="top" width="26.08%" headers="mcps1.1.3.1.1 "><p id="p17841102182719"><a name="p17841102182719"></a><a name="p17841102182719"></a>&lt;mutex&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="73.92%" headers="mcps1.1.3.1.2 "><p id="p1684132192711"><a name="p1684132192711"></a><a name="p1684132192711"></a>支持互斥锁的操作</p>
</td>
</tr>
<tr id="row38413219276"><td class="cellrowborder" valign="top" width="26.08%" headers="mcps1.1.3.1.1 "><p id="p10841132112712"><a name="p10841132112712"></a><a name="p10841132112712"></a>&lt;thread&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="73.92%" headers="mcps1.1.3.1.2 "><p id="p48411129271"><a name="p48411129271"></a><a name="p48411129271"></a>支持线程函数的操作</p>
</td>
</tr>
<tr id="row784118282719"><td class="cellrowborder" valign="top" width="26.08%" headers="mcps1.1.3.1.1 "><p id="p484122122710"><a name="p484122122710"></a><a name="p484122122710"></a>&lt;condition_variable&gt;</p>
</td>
<td class="cellrowborder" valign="top" width="73.92%" headers="mcps1.1.3.1.2 "><p id="p208411272711"><a name="p208411272711"></a><a name="p208411272711"></a>支持条件变量的操作</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **须知：** 
>Huawei Liteos提供的memory，uninitialized\_fill函数存在内存泄露，慎用。

<h2 id="3">CMSIS接口</h2>

<h3 id="v1">CMSIS v1.0</h3>

<a href="https://www.keil.com/pack/doc/CMSIS_Dev/RTOS/html/index.html" target="_blank">CMSIS v1.0接口官方手册</a>中包含了接口声明、入参介绍、返回值类型、接口使用条件等详细介绍，下面简要介绍Huawei LiteOS适配CMSIS v1.0接口的情况。

<h4 id="CMSIS-v1-0适配接口">CMSIS v1.0适配接口</h4>

LiteOS目前已支持大部分CMSIS v1.0接口，接口声明在compat/cmsis/1.0/cmsis\_os1.h，接口说明详见下表：

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="23.532353235323534%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="23.092309230923092%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.375337533753374%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p2034116332198"><a name="p2034116332198"></a><a name="p2034116332198"></a>osThreadCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p43417338191"><a name="p43417338191"></a><a name="p43417338191"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>创建一个任务</p>
</td>
</tr>
<tr id="row1597219341198"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p13341143314191"><a name="p13341143314191"></a><a name="p13341143314191"></a>osThreadGetId</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1468155104211"><a name="p1468155104211"></a><a name="p1468155104211"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p16341163311918"><a name="p16341163311918"></a><a name="p16341163311918"></a>获取当前的任务句柄</p>
</td>
</tr>
<tr id="row169721334161913"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p33411833151910"><a name="p33411833151910"></a><a name="p33411833151910"></a>osThreadTerminate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p897315560427"><a name="p897315560427"></a><a name="p897315560427"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p113411833141914"><a name="p113411833141914"></a><a name="p113411833141914"></a>终止某个任务</p>
</td>
</tr>
<tr id="row13972133416199"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p11341233141911"><a name="p11341233141911"></a><a name="p11341233141911"></a>osThreadYield</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p19691258124218"><a name="p19691258124218"></a><a name="p19691258124218"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p183411233201914"><a name="p183411233201914"></a><a name="p183411233201914"></a>切换至同优先级的就绪任务</p>
</td>
</tr>
<tr id="row1497243491910"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p834113331911"><a name="p834113331911"></a><a name="p834113331911"></a>osThreadSetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1223110252484"><a name="p1223110252484"></a><a name="p1223110252484"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1341933181918"><a name="p1341933181918"></a><a name="p1341933181918"></a>设置任务优先级</p>
</td>
</tr>
<tr id="row597213415193"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p8341433101912"><a name="p8341433101912"></a><a name="p8341433101912"></a>osThreadGetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p159758266482"><a name="p159758266482"></a><a name="p159758266482"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p23421133131915"><a name="p23421133131915"></a><a name="p23421133131915"></a>获取任务优先级</p>
</td>
</tr>
<tr id="row197263401911"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p834223371919"><a name="p834223371919"></a><a name="p834223371919"></a>osDelay</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1661406104916"><a name="p1661406104916"></a><a name="p1661406104916"></a>延时类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p14342163315198"><a name="p14342163315198"></a><a name="p14342163315198"></a>任务延时处理</p>
</td>
</tr>
<tr id="row397219346193"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p118111629205016"><a name="p118111629205016"></a><a name="p118111629205016"></a>osTimerCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1934243314192"><a name="p1934243314192"></a><a name="p1934243314192"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p534219338193"><a name="p534219338193"></a><a name="p534219338193"></a>创建定时器</p>
</td>
</tr>
<tr id="row597133421911"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1234213320193"><a name="p1234213320193"></a><a name="p1234213320193"></a>osTimerStart</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1134263310194"><a name="p1134263310194"></a><a name="p1134263310194"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p12342193341912"><a name="p12342193341912"></a><a name="p12342193341912"></a>启动定时器（若定时器正在计时会先停止该定时器）</p>
</td>
</tr>
<tr id="row39711334161919"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p183421033111910"><a name="p183421033111910"></a><a name="p183421033111910"></a>osTimerStop</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p133341241185319"><a name="p133341241185319"></a><a name="p133341241185319"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p93421133131915"><a name="p93421133131915"></a><a name="p93421133131915"></a>停止定时器</p>
</td>
</tr>
<tr id="row79710343194"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p20958133205212"><a name="p20958133205212"></a><a name="p20958133205212"></a>osTimerDelete</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p114231242195317"><a name="p114231242195317"></a><a name="p114231242195317"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p834213330193"><a name="p834213330193"></a><a name="p834213330193"></a>删除定时器</p>
</td>
</tr>
<tr id="row142701955144014"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1527115517407"><a name="p1527115517407"></a><a name="p1527115517407"></a>osSignalSet</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1227185520404"><a name="p1227185520404"></a><a name="p1227185520404"></a>信号类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p427165512403"><a name="p427165512403"></a><a name="p427165512403"></a>设置信号</p>
</td>
</tr>
<tr id="row164462038135216"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p744783885219"><a name="p744783885219"></a><a name="p744783885219"></a>osSignalClear</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p94474384525"><a name="p94474384525"></a><a name="p94474384525"></a>信号类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1844715388528"><a name="p1844715388528"></a><a name="p1844715388528"></a>清除信号</p>
</td>
</tr>
<tr id="row39881241125219"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p159884417527"><a name="p159884417527"></a><a name="p159884417527"></a>osSignalWait</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1988541105218"><a name="p1988541105218"></a><a name="p1988541105218"></a>信号类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p79881841195210"><a name="p79881841195210"></a><a name="p79881841195210"></a>等待信号</p>
</td>
</tr>
<tr id="row5212647195217"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1021284745214"><a name="p1021284745214"></a><a name="p1021284745214"></a>osMutexCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p2212104715218"><a name="p2212104715218"></a><a name="p2212104715218"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1021211478529"><a name="p1021211478529"></a><a name="p1021211478529"></a>创建互斥锁</p>
</td>
</tr>
<tr id="row157080514520"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p177081551155211"><a name="p177081551155211"></a><a name="p177081551155211"></a>osMutexWait</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p19708135114526"><a name="p19708135114526"></a><a name="p19708135114526"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p4708155118523"><a name="p4708155118523"></a><a name="p4708155118523"></a>获取互斥锁（阻塞等待）</p>
</td>
</tr>
<tr id="row561255611527"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1561235635216"><a name="p1561235635216"></a><a name="p1561235635216"></a>osMutexRelease</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p136121256205214"><a name="p136121256205214"></a><a name="p136121256205214"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p361395612526"><a name="p361395612526"></a><a name="p361395612526"></a>释放互斥锁</p>
</td>
</tr>
<tr id="row1896581395413"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p896681313545"><a name="p896681313545"></a><a name="p896681313545"></a>osMutexDelete</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p8966913165417"><a name="p8966913165417"></a><a name="p8966913165417"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p16966151312548"><a name="p16966151312548"></a><a name="p16966151312548"></a>删除互斥锁</p>
</td>
</tr>
<tr id="row105318219544"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1053721205411"><a name="p1053721205411"></a><a name="p1053721205411"></a>osSemaphoreCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p853112114544"><a name="p853112114544"></a><a name="p853112114544"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p95322112545"><a name="p95322112545"></a><a name="p95322112545"></a>创建信号量</p>
</td>
</tr>
<tr id="row1087882495419"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p68789244541"><a name="p68789244541"></a><a name="p68789244541"></a>osSemaphoreWait</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1079213566596"><a name="p1079213566596"></a><a name="p1079213566596"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p98789244543"><a name="p98789244543"></a><a name="p98789244543"></a>获取信号量（阻塞等待）</p>
</td>
</tr>
<tr id="row20147917155412"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1814731715410"><a name="p1814731715410"></a><a name="p1814731715410"></a>osSemaphoreRelease</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p944465716592"><a name="p944465716592"></a><a name="p944465716592"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p314715176548"><a name="p314715176548"></a><a name="p314715176548"></a>释放信号量</p>
</td>
</tr>
<tr id="row18879185165616"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p587919518562"><a name="p587919518562"></a><a name="p587919518562"></a>osSemaphoreDelete</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p172046587593"><a name="p172046587593"></a><a name="p172046587593"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1087919515564"><a name="p1087919515564"></a><a name="p1087919515564"></a>删除信号量</p>
</td>
</tr>
<tr id="row14558189125615"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p11558291561"><a name="p11558291561"></a><a name="p11558291561"></a>osPoolCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p175581092564"><a name="p175581092564"></a><a name="p175581092564"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p655816945610"><a name="p655816945610"></a><a name="p655816945610"></a>创建块状内存池</p>
</td>
</tr>
<tr id="row49021821135618"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p179024214565"><a name="p179024214565"></a><a name="p179024214565"></a>osPoolAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p2969101017115"><a name="p2969101017115"></a><a name="p2969101017115"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p12902192165614"><a name="p12902192165614"></a><a name="p12902192165614"></a>申请内存</p>
</td>
</tr>
<tr id="row695819121569"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p19958171213564"><a name="p19958171213564"></a><a name="p19958171213564"></a>osPoolCAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1170719111617"><a name="p1170719111617"></a><a name="p1170719111617"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p18958121215569"><a name="p18958121215569"></a><a name="p18958121215569"></a>申请内存并清零</p>
</td>
</tr>
<tr id="row3416132545618"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1041612253568"><a name="p1041612253568"></a><a name="p1041612253568"></a>osPoolFree</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p17131313916"><a name="p17131313916"></a><a name="p17131313916"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p17416162585612"><a name="p17416162585612"></a><a name="p17416162585612"></a>释放内存</p>
</td>
</tr>
<tr id="row6711153318569"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1371118334562"><a name="p1371118334562"></a><a name="p1371118334562"></a>osMessageCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p10711143317566"><a name="p10711143317566"></a><a name="p10711143317566"></a>指针消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p207111133125612"><a name="p207111133125612"></a><a name="p207111133125612"></a>创建消息队列（不带内容，一般为数据指针）</p>
</td>
</tr>
<tr id="row114711730185617"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1747163011569"><a name="p1747163011569"></a><a name="p1747163011569"></a>osMessagePut</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1047153020564"><a name="p1047153020564"></a><a name="p1047153020564"></a>指针消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1537916414229"><a name="p1537916414229"></a><a name="p1537916414229"></a>往消息队列里放入消息</p>
</td>
</tr>
<tr id="row15847175113570"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p8848651205712"><a name="p8848651205712"></a><a name="p8848651205712"></a>osMessageGet</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p5848551145710"><a name="p5848551145710"></a><a name="p5848551145710"></a>指针消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p171594463523"><a name="p171594463523"></a><a name="p171594463523"></a>从消息队列里获取消息</p>
</td>
</tr>
<tr id="row1096142315819"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p18962232585"><a name="p18962232585"></a><a name="p18962232585"></a>osMailCreate</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p99632325814"><a name="p99632325814"></a><a name="p99632325814"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1296152375817"><a name="p1296152375817"></a><a name="p1296152375817"></a>创建消息队列（带内容，可理解为在osMessage的基础上增加osPool）</p>
</td>
</tr>
<tr id="row542482611586"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p17424926105812"><a name="p17424926105812"></a><a name="p17424926105812"></a>osMailAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p842452614585"><a name="p842452614585"></a><a name="p842452614585"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p184243260589"><a name="p184243260589"></a><a name="p184243260589"></a>申请内存（用于存放消息内容）</p>
</td>
</tr>
<tr id="row11632729115819"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p3632122905817"><a name="p3632122905817"></a><a name="p3632122905817"></a>osMailCAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1663272905812"><a name="p1663272905812"></a><a name="p1663272905812"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p5632729145817"><a name="p5632729145817"></a><a name="p5632729145817"></a>申请内存并清零（用于存放消息内容）</p>
</td>
</tr>
<tr id="row1663103515813"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p8663123555816"><a name="p8663123555816"></a><a name="p8663123555816"></a>osMailPut</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p66639354588"><a name="p66639354588"></a><a name="p66639354588"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p12663193575816"><a name="p12663193575816"></a><a name="p12663193575816"></a>往消息队列里放入消息</p>
</td>
</tr>
<tr id="row1544010331594"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p1044003335914"><a name="p1044003335914"></a><a name="p1044003335914"></a>osMailGet</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p11440103325910"><a name="p11440103325910"></a><a name="p11440103325910"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p4440143375915"><a name="p4440143375915"></a><a name="p4440143375915"></a>从消息队列里获取消息</p>
</td>
</tr>
<tr id="row15944143635914"><td class="cellrowborder" valign="top" width="23.532353235323534%" headers="mcps1.1.4.1.1 "><p id="p14944133645915"><a name="p14944133645915"></a><a name="p14944133645915"></a>osMailFree</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p10944236125910"><a name="p10944236125910"></a><a name="p10944236125910"></a>内容消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.375337533753374%" headers="mcps1.1.4.1.3 "><p id="p1294423635910"><a name="p1294423635910"></a><a name="p1294423635910"></a>释放已申请的内存</p>
</td>
</tr>
</tbody>
</table>

<h4 id="CMSIS-v1-0不支持接口">CMSIS v1.0不支持接口</h4>

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="23.27232723272327%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="23.002300230023003%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.725372537253726%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="23.27232723272327%" headers="mcps1.1.4.1.1 "><p id="p4342183318199"><a name="p4342183318199"></a><a name="p4342183318199"></a>osWait</p>
</td>
<td class="cellrowborder" valign="top" width="23.002300230023003%" headers="mcps1.1.4.1.2 "><p id="p1270584811264"><a name="p1270584811264"></a><a name="p1270584811264"></a>延时类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.725372537253726%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>等待任一信号或消息发生</p>
</td>
</tr>
</tbody>
</table>

<h4 id="CMSIS-v1-0标准接口适配差异">CMSIS v1.0标准接口适配差异</h4>

考虑接口的易用性和Huawei LiteOS内部机制与CMSIS标准接口的差异，在适配CMSIS v1.0接口时，对部分接口进行了修改，详见下表：

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="23.352335233523352%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="22.91229122912291%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.73537353735374%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="23.352335233523352%" headers="mcps1.1.4.1.1 "><p id="p19708591458"><a name="p19708591458"></a><a name="p19708591458"></a>osTimerStart</p>
</td>
<td class="cellrowborder" valign="top" width="22.91229122912291%" headers="mcps1.1.4.1.2 "><p id="p43417338191"><a name="p43417338191"></a><a name="p43417338191"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.73537353735374%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>增加一种定时器类型osTimerDelay，与osTimerOnce同为单次定时器，差别在于osTimerOnce超时后会删除定时器，但osTimerDelay不会，可以重复启动（osTimerStart）</p>
</td>
</tr>
<tr id="row1597219341198"><td class="cellrowborder" valign="top" width="23.352335233523352%" headers="mcps1.1.4.1.1 "><p id="p13341143314191"><a name="p13341143314191"></a><a name="p13341143314191"></a>osBinarySemaphoreCreate</p>
</td>
<td class="cellrowborder" valign="top" width="22.91229122912291%" headers="mcps1.1.4.1.2 "><p id="p1468155104211"><a name="p1468155104211"></a><a name="p1468155104211"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.73537353735374%" headers="mcps1.1.4.1.3 "><p id="p16341163311918"><a name="p16341163311918"></a><a name="p16341163311918"></a>新增接口，用于创建二值信号量</p>
</td>
</tr>
</tbody>
</table>

<h3 id="v2">CMSIS v2.0</h3>

<a href="https://www.keil.com/pack/doc/CMSIS_Dev/RTOS2/html/group__CMSIS__RTOS.html" target="_blank">CMSIS v2.0接口官方手册</a>中包含了接口声明、入参介绍、返回值类型、接口使用条件等详细介绍，下面简要介绍Huawei LiteOS适配CMSIS v2.0接口的情况。

<h4 id="CMSIS-v2-0适配接口">CMSIS v2.0适配接口</h4>

LiteOS目前已支持大部分CMSIS v2.0接口，接口声明在compat/cmsis/2.0/cmsis\_os2.h，接口说明详见下表：

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="23.97239723972397%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="22.21222122212221%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.815381538153815%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p2034116332198"><a name="p2034116332198"></a><a name="p2034116332198"></a>osKernelInitialize</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p43417338191"><a name="p43417338191"></a><a name="p43417338191"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>初始化操作系统</p>
</td>
</tr>
<tr id="row1597219341198"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p13341143314191"><a name="p13341143314191"></a><a name="p13341143314191"></a>osKernelGetInfo</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p0981626153315"><a name="p0981626153315"></a><a name="p0981626153315"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p16341163311918"><a name="p16341163311918"></a><a name="p16341163311918"></a>获取系统版本信息</p>
</td>
</tr>
<tr id="row169721334161913"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p33411833151910"><a name="p33411833151910"></a><a name="p33411833151910"></a>osKernelGetState</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p897315560427"><a name="p897315560427"></a><a name="p897315560427"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p113411833141914"><a name="p113411833141914"></a><a name="p113411833141914"></a>获取系统状态（osThreadState_t）</p>
</td>
</tr>
<tr id="row13972133416199"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p11341233141911"><a name="p11341233141911"></a><a name="p11341233141911"></a>osKernelStart</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p19691258124218"><a name="p19691258124218"></a><a name="p19691258124218"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p183411233201914"><a name="p183411233201914"></a><a name="p183411233201914"></a>启动操作系统</p>
</td>
</tr>
<tr id="row1497243491910"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p834113331911"><a name="p834113331911"></a><a name="p834113331911"></a>osKernelLock</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1223110252484"><a name="p1223110252484"></a><a name="p1223110252484"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1341933181918"><a name="p1341933181918"></a><a name="p1341933181918"></a>锁内核（锁调度）</p>
</td>
</tr>
<tr id="row597213415193"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p8341433101912"><a name="p8341433101912"></a><a name="p8341433101912"></a>osKernelUnlock</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p159758266482"><a name="p159758266482"></a><a name="p159758266482"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p23421133131915"><a name="p23421133131915"></a><a name="p23421133131915"></a>解锁内核（解锁调度）</p>
</td>
</tr>
<tr id="row197263401911"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p834223371919"><a name="p834223371919"></a><a name="p834223371919"></a>osKernelRestoreLock</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p816263520330"><a name="p816263520330"></a><a name="p816263520330"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p14342163315198"><a name="p14342163315198"></a><a name="p14342163315198"></a>恢复内核锁状态</p>
</td>
</tr>
<tr id="row2097217345193"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p4342183318199"><a name="p4342183318199"></a><a name="p4342183318199"></a>osKernelGetTickCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p634273371916"><a name="p634273371916"></a><a name="p634273371916"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p18342123316192"><a name="p18342123316192"></a><a name="p18342123316192"></a>获取系统启动后时间（单位：tick）</p>
</td>
</tr>
<tr id="row397219346193"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p118111629205016"><a name="p118111629205016"></a><a name="p118111629205016"></a>osKernelGetTickFreq</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1934243314192"><a name="p1934243314192"></a><a name="p1934243314192"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p534219338193"><a name="p534219338193"></a><a name="p534219338193"></a>获取每秒的tick数</p>
</td>
</tr>
<tr id="row597133421911"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p09501022183114"><a name="p09501022183114"></a><a name="p09501022183114"></a>osKernelGetSysTimerCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1134263310194"><a name="p1134263310194"></a><a name="p1134263310194"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p12342193341912"><a name="p12342193341912"></a><a name="p12342193341912"></a>获取系统启动后时间（单位：cycle）</p>
</td>
</tr>
<tr id="row39711334161919"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p13168153017316"><a name="p13168153017316"></a><a name="p13168153017316"></a>osKernelGetSysTimerFreq</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p133341241185319"><a name="p133341241185319"></a><a name="p133341241185319"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p93421133131915"><a name="p93421133131915"></a><a name="p93421133131915"></a>获取每秒的CPU cycle数</p>
</td>
</tr>
<tr id="row79710343194"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p15941103913317"><a name="p15941103913317"></a><a name="p15941103913317"></a>osThreadNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p114231242195317"><a name="p114231242195317"></a><a name="p114231242195317"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p834213330193"><a name="p834213330193"></a><a name="p834213330193"></a>创建任务</p>
</td>
</tr>
<tr id="row142701955144014"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p13862184233115"><a name="p13862184233115"></a><a name="p13862184233115"></a>osThreadGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1227185520404"><a name="p1227185520404"></a><a name="p1227185520404"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p427165512403"><a name="p427165512403"></a><a name="p427165512403"></a>获取任务名</p>
</td>
</tr>
<tr id="row164462038135216"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p4694184633119"><a name="p4694184633119"></a><a name="p4694184633119"></a>osThreadGetId</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p94474384525"><a name="p94474384525"></a><a name="p94474384525"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1844715388528"><a name="p1844715388528"></a><a name="p1844715388528"></a>获取任务句柄</p>
</td>
</tr>
<tr id="row39881241125219"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p52351153163116"><a name="p52351153163116"></a><a name="p52351153163116"></a>osThreadGetState</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1988541105218"><a name="p1988541105218"></a><a name="p1988541105218"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p79881841195210"><a name="p79881841195210"></a><a name="p79881841195210"></a>获取任务状态</p>
</td>
</tr>
<tr id="row5212647195217"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1021284745214"><a name="p1021284745214"></a><a name="p1021284745214"></a>osThreadGetStackSize</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p2212104715218"><a name="p2212104715218"></a><a name="p2212104715218"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1021211478529"><a name="p1021211478529"></a><a name="p1021211478529"></a>获取任务栈大小</p>
</td>
</tr>
<tr id="row157080514520"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p177081551155211"><a name="p177081551155211"></a><a name="p177081551155211"></a>osThreadGetStackSpace</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p19708135114526"><a name="p19708135114526"></a><a name="p19708135114526"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p4708155118523"><a name="p4708155118523"></a><a name="p4708155118523"></a>获取未使用过的任务栈空间</p>
</td>
</tr>
<tr id="row561255611527"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1561235635216"><a name="p1561235635216"></a><a name="p1561235635216"></a>osThreadSetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p136121256205214"><a name="p136121256205214"></a><a name="p136121256205214"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p361395612526"><a name="p361395612526"></a><a name="p361395612526"></a>设置任务优先级</p>
</td>
</tr>
<tr id="row1896581395413"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p896681313545"><a name="p896681313545"></a><a name="p896681313545"></a>osThreadGetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p8966913165417"><a name="p8966913165417"></a><a name="p8966913165417"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p16966151312548"><a name="p16966151312548"></a><a name="p16966151312548"></a>获取任务优先级</p>
</td>
</tr>
<tr id="row105318219544"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p4385193833316"><a name="p4385193833316"></a><a name="p4385193833316"></a>osThreadYield</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p853112114544"><a name="p853112114544"></a><a name="p853112114544"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p95322112545"><a name="p95322112545"></a><a name="p95322112545"></a>切换至同优先级的就绪任务</p>
</td>
</tr>
<tr id="row1087882495419"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p68789244541"><a name="p68789244541"></a><a name="p68789244541"></a>osThreadSuspend</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1079213566596"><a name="p1079213566596"></a><a name="p1079213566596"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p98789244543"><a name="p98789244543"></a><a name="p98789244543"></a>挂起任务（恢复前无法得到调度）</p>
</td>
</tr>
<tr id="row20147917155412"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1814731715410"><a name="p1814731715410"></a><a name="p1814731715410"></a>osThreadResume</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p944465716592"><a name="p944465716592"></a><a name="p944465716592"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p7140564358"><a name="p7140564358"></a><a name="p7140564358"></a>恢复任务</p>
</td>
</tr>
<tr id="row18879185165616"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p587919518562"><a name="p587919518562"></a><a name="p587919518562"></a>osThreadTerminate</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p13048993410"><a name="p13048993410"></a><a name="p13048993410"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1087919515564"><a name="p1087919515564"></a><a name="p1087919515564"></a>终止任务（建议不要主动终止任务）</p>
</td>
</tr>
<tr id="row14558189125615"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p11558291561"><a name="p11558291561"></a><a name="p11558291561"></a>osThreadGetCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1011617116346"><a name="p1011617116346"></a><a name="p1011617116346"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p655816945610"><a name="p655816945610"></a><a name="p655816945610"></a>获取已创建的任务数量</p>
</td>
</tr>
<tr id="row49021821135618"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p179024214565"><a name="p179024214565"></a><a name="p179024214565"></a>osThreadFlagsSet</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p2969101017115"><a name="p2969101017115"></a><a name="p2969101017115"></a>任务事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p12902192165614"><a name="p12902192165614"></a><a name="p12902192165614"></a>写入指定事件</p>
</td>
</tr>
<tr id="row695819121569"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p19958171213564"><a name="p19958171213564"></a><a name="p19958171213564"></a>osThreadFlagsClear</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p3769154312138"><a name="p3769154312138"></a><a name="p3769154312138"></a>任务事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p18958121215569"><a name="p18958121215569"></a><a name="p18958121215569"></a>清除指定事件</p>
</td>
</tr>
<tr id="row3416132545618"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1041612253568"><a name="p1041612253568"></a><a name="p1041612253568"></a>osThreadFlagsGet</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p17131313916"><a name="p17131313916"></a><a name="p17131313916"></a>任务事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p17416162585612"><a name="p17416162585612"></a><a name="p17416162585612"></a>获取当前任务事件</p>
</td>
</tr>
<tr id="row6711153318569"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1371118334562"><a name="p1371118334562"></a><a name="p1371118334562"></a>osThreadFlagsWait</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p10711143317566"><a name="p10711143317566"></a><a name="p10711143317566"></a>任务事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p207111133125612"><a name="p207111133125612"></a><a name="p207111133125612"></a>等待指定事件</p>
</td>
</tr>
<tr id="row114711730185617"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1747163011569"><a name="p1747163011569"></a><a name="p1747163011569"></a>osDelay</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1047153020564"><a name="p1047153020564"></a><a name="p1047153020564"></a>延时类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p94711030145615"><a name="p94711030145615"></a><a name="p94711030145615"></a>任务延时（单位：tick）</p>
</td>
</tr>
<tr id="row15847175113570"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p8848651205712"><a name="p8848651205712"></a><a name="p8848651205712"></a>osDelayUntil</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p5848551145710"><a name="p5848551145710"></a><a name="p5848551145710"></a>指针消息类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p6874164816912"><a name="p6874164816912"></a><a name="p6874164816912"></a>延时至某一时刻（单位：tick）</p>
</td>
</tr>
<tr id="row1096142315819"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p18962232585"><a name="p18962232585"></a><a name="p18962232585"></a>osTimerNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p99632325814"><a name="p99632325814"></a><a name="p99632325814"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1296152375817"><a name="p1296152375817"></a><a name="p1296152375817"></a>创建定时器</p>
</td>
</tr>
<tr id="row542482611586"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p17424926105812"><a name="p17424926105812"></a><a name="p17424926105812"></a>osTimerGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p842452614585"><a name="p842452614585"></a><a name="p842452614585"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p184243260589"><a name="p184243260589"></a><a name="p184243260589"></a>获取定时器名称（目前固定返回NULL）</p>
</td>
</tr>
<tr id="row11632729115819"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p3632122905817"><a name="p3632122905817"></a><a name="p3632122905817"></a>osTimerStart</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1663272905812"><a name="p1663272905812"></a><a name="p1663272905812"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p5632729145817"><a name="p5632729145817"></a><a name="p5632729145817"></a>启动定时器（若定时器正在计时会先停止该定时器）</p>
</td>
</tr>
<tr id="row1663103515813"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p8663123555816"><a name="p8663123555816"></a><a name="p8663123555816"></a>osTimerStop</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p66639354588"><a name="p66639354588"></a><a name="p66639354588"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p9831926913"><a name="p9831926913"></a><a name="p9831926913"></a>停止定时器</p>
</td>
</tr>
<tr id="row11711133313313"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p37111633103315"><a name="p37111633103315"></a><a name="p37111633103315"></a>osTimerIsRunning</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1471114338336"><a name="p1471114338336"></a><a name="p1471114338336"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p67112033163319"><a name="p67112033163319"></a><a name="p67112033163319"></a>定时器是否在计时中</p>
</td>
</tr>
<tr id="row2754142916229"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p47551829192212"><a name="p47551829192212"></a><a name="p47551829192212"></a>osTimerDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p275592942210"><a name="p275592942210"></a><a name="p275592942210"></a>定时器类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1775572982216"><a name="p1775572982216"></a><a name="p1775572982216"></a>删除定时器</p>
</td>
</tr>
<tr id="row1544010331594"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1044003335914"><a name="p1044003335914"></a><a name="p1044003335914"></a>osEventFlagsNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p221885301716"><a name="p221885301716"></a><a name="p221885301716"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p4440143375915"><a name="p4440143375915"></a><a name="p4440143375915"></a>创建事件（与任务事件ThreadFlags的差别在于有独立的句柄和控制块）</p>
</td>
</tr>
<tr id="row15944143635914"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p14944133645915"><a name="p14944133645915"></a><a name="p14944133645915"></a>osEventFlagsGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p9774754171716"><a name="p9774754171716"></a><a name="p9774754171716"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1294423635910"><a name="p1294423635910"></a><a name="p1294423635910"></a>获取事件名称（目前固定返回NULL）</p>
</td>
</tr>
<tr id="row157141419141011"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p871414199107"><a name="p871414199107"></a><a name="p871414199107"></a>osEventFlagsSet</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p4715171918104"><a name="p4715171918104"></a><a name="p4715171918104"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p6715101951012"><a name="p6715101951012"></a><a name="p6715101951012"></a>写入指定事件</p>
</td>
</tr>
<tr id="row148351315181017"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p08365155106"><a name="p08365155106"></a><a name="p08365155106"></a>osEventFlagsClear</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p158364152104"><a name="p158364152104"></a><a name="p158364152104"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p683620151106"><a name="p683620151106"></a><a name="p683620151106"></a>清楚指定事件</p>
</td>
</tr>
<tr id="row170614235105"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1770612321013"><a name="p1770612321013"></a><a name="p1770612321013"></a>osEventFlagsGet</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p770612331015"><a name="p770612331015"></a><a name="p770612331015"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p16706723161012"><a name="p16706723161012"></a><a name="p16706723161012"></a>获取当前事件值</p>
</td>
</tr>
<tr id="row86661527201012"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p46661127181011"><a name="p46661127181011"></a><a name="p46661127181011"></a>osEventFlagsWait</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p156662277104"><a name="p156662277104"></a><a name="p156662277104"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p13666827151015"><a name="p13666827151015"></a><a name="p13666827151015"></a>等待指定事件</p>
</td>
</tr>
<tr id="row4818123121014"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p081883115105"><a name="p081883115105"></a><a name="p081883115105"></a>osEventFlagsDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p8818133131011"><a name="p8818133131011"></a><a name="p8818133131011"></a>事件类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p5818123111011"><a name="p5818123111011"></a><a name="p5818123111011"></a>删除事件</p>
</td>
</tr>
<tr id="row1492213518105"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p292212351108"><a name="p292212351108"></a><a name="p292212351108"></a>osMutexNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p29221135201013"><a name="p29221135201013"></a><a name="p29221135201013"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p092223519106"><a name="p092223519106"></a><a name="p092223519106"></a>创建互斥锁</p>
</td>
</tr>
<tr id="row19143162171810"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p4144202191815"><a name="p4144202191815"></a><a name="p4144202191815"></a>osMutexGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1814412211183"><a name="p1814412211183"></a><a name="p1814412211183"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p314416241815"><a name="p314416241815"></a><a name="p314416241815"></a>获取互斥锁名称（目前固定返回NULL）</p>
</td>
</tr>
<tr id="row858166161819"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p135815641814"><a name="p135815641814"></a><a name="p135815641814"></a>osMutexAcquire</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p16581267181"><a name="p16581267181"></a><a name="p16581267181"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p17581116171818"><a name="p17581116171818"></a><a name="p17581116171818"></a>获取互斥锁（阻塞等待）</p>
</td>
</tr>
<tr id="row1034111018187"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p18341410131816"><a name="p18341410131816"></a><a name="p18341410131816"></a>osMutexRelease</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p143411210121817"><a name="p143411210121817"></a><a name="p143411210121817"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p13341111010189"><a name="p13341111010189"></a><a name="p13341111010189"></a>释放互斥锁</p>
</td>
</tr>
<tr id="row3500320163717"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p85011020203713"><a name="p85011020203713"></a><a name="p85011020203713"></a>osMutexGetOwner</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1450162053719"><a name="p1450162053719"></a><a name="p1450162053719"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1350182010370"><a name="p1350182010370"></a><a name="p1350182010370"></a>获取持有该互斥锁的任务句柄</p>
</td>
</tr>
<tr id="row15884624133711"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p8884182410370"><a name="p8884182410370"></a><a name="p8884182410370"></a>osMutexDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p888419240376"><a name="p888419240376"></a><a name="p888419240376"></a>互斥锁类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p988420245378"><a name="p988420245378"></a><a name="p988420245378"></a>删除互斥锁</p>
</td>
</tr>
<tr id="row1053312298374"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p653332916374"><a name="p653332916374"></a><a name="p653332916374"></a>osSemaphoreNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1353315291379"><a name="p1353315291379"></a><a name="p1353315291379"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1253322993717"><a name="p1253322993717"></a><a name="p1253322993717"></a>创建信号量</p>
</td>
</tr>
<tr id="row141141253101918"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p6115205315193"><a name="p6115205315193"></a><a name="p6115205315193"></a>osSemaphoreGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p18115175312195"><a name="p18115175312195"></a><a name="p18115175312195"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p12115175361912"><a name="p12115175361912"></a><a name="p12115175361912"></a>获取信号量名称（目前固定返回NULL）</p>
</td>
</tr>
<tr id="row119461156171910"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p15946256101919"><a name="p15946256101919"></a><a name="p15946256101919"></a>osSemaphoreAcquire</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p2946856201913"><a name="p2946856201913"></a><a name="p2946856201913"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p4946105612195"><a name="p4946105612195"></a><a name="p4946105612195"></a>获取信号量（阻塞等待）</p>
</td>
</tr>
<tr id="row5594140162014"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p185944016209"><a name="p185944016209"></a><a name="p185944016209"></a>osSemaphoreRelease</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p259450102013"><a name="p259450102013"></a><a name="p259450102013"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p55941908206"><a name="p55941908206"></a><a name="p55941908206"></a>释放信号量</p>
</td>
</tr>
<tr id="row112544206"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p19210418203"><a name="p19210418203"></a><a name="p19210418203"></a>osSemaphoreGetCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1724420202"><a name="p1724420202"></a><a name="p1724420202"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1721940202"><a name="p1721940202"></a><a name="p1721940202"></a>获取信号量的计数值</p>
</td>
</tr>
<tr id="row542615752015"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p204274714202"><a name="p204274714202"></a><a name="p204274714202"></a>osSemaphoreDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p184270772012"><a name="p184270772012"></a><a name="p184270772012"></a>信号量类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p204271472203"><a name="p204271472203"></a><a name="p204271472203"></a>删除信号量</p>
</td>
</tr>
<tr id="row184981711182012"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p880713912213"><a name="p880713912213"></a><a name="p880713912213"></a>osMessageQueueNew</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p10279202635913"><a name="p10279202635913"></a><a name="p10279202635913"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1149871112016"><a name="p1149871112016"></a><a name="p1149871112016"></a>创建消息队列</p>
</td>
</tr>
<tr id="row663560132214"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p663520042216"><a name="p663520042216"></a><a name="p663520042216"></a>osMessageQueueGetName</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p13649182585910"><a name="p13649182585910"></a><a name="p13649182585910"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p263580172219"><a name="p263580172219"></a><a name="p263580172219"></a>获取消息队列名称（目前固定返回NULL）</p>
</td>
</tr>
<tr id="row737914416227"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p133791746226"><a name="p133791746226"></a><a name="p133791746226"></a>osMessageQueuePut</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p149431924145916"><a name="p149431924145916"></a><a name="p149431924145916"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1537916414229"><a name="p1537916414229"></a><a name="p1537916414229"></a>往消息队列里放入消息</p>
</td>
</tr>
<tr id="row865377162217"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p365310714226"><a name="p365310714226"></a><a name="p365310714226"></a>osMessageQueueGet</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p32866243598"><a name="p32866243598"></a><a name="p32866243598"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p196532079229"><a name="p196532079229"></a><a name="p196532079229"></a>从消息队列里获取消息</p>
</td>
</tr>
<tr id="row33612114227"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p12361511112218"><a name="p12361511112218"></a><a name="p12361511112218"></a>osMessageQueueGetCapacity</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p9186142211592"><a name="p9186142211592"></a><a name="p9186142211592"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p183610117224"><a name="p183610117224"></a><a name="p183610117224"></a>获取消息队列节点数量</p>
</td>
</tr>
<tr id="row722652417229"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p9226132420228"><a name="p9226132420228"></a><a name="p9226132420228"></a>osMessageQueueGetMsgSize</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p3582132195913"><a name="p3582132195913"></a><a name="p3582132195913"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p92264240226"><a name="p92264240226"></a><a name="p92264240226"></a>获取消息队列节点大小</p>
</td>
</tr>
<tr id="row017554516589"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p1317519456583"><a name="p1317519456583"></a><a name="p1317519456583"></a>osMessageQueueGetCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p1131821115914"><a name="p1131821115914"></a><a name="p1131821115914"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p1617615458582"><a name="p1617615458582"></a><a name="p1617615458582"></a>获取当前消息队列里的消息数量</p>
</td>
</tr>
<tr id="row10862048135815"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p15862104814583"><a name="p15862104814583"></a><a name="p15862104814583"></a>osMessageQueueGetSpace</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p10343120205910"><a name="p10343120205910"></a><a name="p10343120205910"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p786214483582"><a name="p786214483582"></a><a name="p786214483582"></a>获取当前消息队列里的剩余消息数量</p>
</td>
</tr>
<tr id="row99908516587"><td class="cellrowborder" valign="top" width="23.97239723972397%" headers="mcps1.1.4.1.1 "><p id="p16990155145813"><a name="p16990155145813"></a><a name="p16990155145813"></a>osMessageQueueDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.21222122212221%" headers="mcps1.1.4.1.2 "><p id="p7990105135810"><a name="p7990105135810"></a><a name="p7990105135810"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.815381538153815%" headers="mcps1.1.4.1.3 "><p id="p169906510580"><a name="p169906510580"></a><a name="p169906510580"></a>删除消息队列</p>
</td>
</tr>
</tbody>
</table>

<h4 id="CMSIS-v2-0不支持接口">CMSIS v2.0不支持接口</h4>

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="24.062406240624064%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="22.732273227322732%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.205320532053214%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p2034116332198"><a name="p2034116332198"></a><a name="p2034116332198"></a>osKernelSuspend</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p43417338191"><a name="p43417338191"></a><a name="p43417338191"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>挂起内核阻止调度，一般用于低功耗处理，目前Huawei LiteOS已提供Tickless、Runstop等低功耗机制，暂未适配该接口</p>
</td>
</tr>
<tr id="row1597219341198"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p870814282913"><a name="p870814282913"></a><a name="p870814282913"></a>osKernelResume</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p1468155104211"><a name="p1468155104211"></a><a name="p1468155104211"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p16341163311918"><a name="p16341163311918"></a><a name="p16341163311918"></a>同上</p>
</td>
</tr>
<tr id="row169721334161913"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p33411833151910"><a name="p33411833151910"></a><a name="p33411833151910"></a>osThreadJoin</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p897315560427"><a name="p897315560427"></a><a name="p897315560427"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p113411833141914"><a name="p113411833141914"></a><a name="p113411833141914"></a>目前通过oSThreadNew创建的任务是相互解耦的，暂未适配该接口</p>
</td>
</tr>
<tr id="row13972133416199"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p11341233141911"><a name="p11341233141911"></a><a name="p11341233141911"></a>osThreadExit</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p19691258124218"><a name="p19691258124218"></a><a name="p19691258124218"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p183411233201914"><a name="p183411233201914"></a><a name="p183411233201914"></a>Huawei LiteOS任务支持自删除，任务退出前不需要调用osThreadExit</p>
</td>
</tr>
<tr id="row1497243491910"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p145171355897"><a name="p145171355897"></a><a name="p145171355897"></a>osThreadEnumerate</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p1223110252484"><a name="p1223110252484"></a><a name="p1223110252484"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p1341933181918"><a name="p1341933181918"></a><a name="p1341933181918"></a>获取已创建的任务列表，目前未适配该接口，用户可以调用Huawei LiteOS的LOS_TaskInfoGet等维测接口获取任务状态</p>
</td>
</tr>
<tr id="row597213415193"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p467111208105"><a name="p467111208105"></a><a name="p467111208105"></a>osMemoryPoolAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p159758266482"><a name="p159758266482"></a><a name="p159758266482"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p23421133131915"><a name="p23421133131915"></a><a name="p23421133131915"></a>接口需要支持超时时间内申请内存块，目前Huawei LiteOS暂未提供这种机制</p>
</td>
</tr>
<tr id="row197263401911"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p834223371919"><a name="p834223371919"></a><a name="p834223371919"></a>osMemoryPoolFree</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p1429685181115"><a name="p1429685181115"></a><a name="p1429685181115"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p14342163315198"><a name="p14342163315198"></a><a name="p14342163315198"></a>由于osMemoryPoolAlloc未实现，剩余osMemPool类接口暂不实现</p>
</td>
</tr>
<tr id="row2097217345193"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p4342183318199"><a name="p4342183318199"></a><a name="p4342183318199"></a>osMemoryPoolGetCapacity</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p634273371916"><a name="p634273371916"></a><a name="p634273371916"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p18342123316192"><a name="p18342123316192"></a><a name="p18342123316192"></a>同上</p>
</td>
</tr>
<tr id="row182806364130"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p4280103681311"><a name="p4280103681311"></a><a name="p4280103681311"></a>osMemoryPoolGetBlockSize</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p9280173631318"><a name="p9280173631318"></a><a name="p9280173631318"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p19280203612138"><a name="p19280203612138"></a><a name="p19280203612138"></a>同上</p>
</td>
</tr>
<tr id="row9703193914132"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p9703193917132"><a name="p9703193917132"></a><a name="p9703193917132"></a>osMemoryPoolGetCount</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p127031939181318"><a name="p127031939181318"></a><a name="p127031939181318"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p19703143913135"><a name="p19703143913135"></a><a name="p19703143913135"></a>同上</p>
</td>
</tr>
<tr id="row20863114681312"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p686313463133"><a name="p686313463133"></a><a name="p686313463133"></a>osMemoryPoolGetSpace</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p168631446131312"><a name="p168631446131312"></a><a name="p168631446131312"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p6863134616132"><a name="p6863134616132"></a><a name="p6863134616132"></a>同上</p>
</td>
</tr>
<tr id="row11551114316133"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p855117436132"><a name="p855117436132"></a><a name="p855117436132"></a>osMemoryPoolDelete</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p855184311135"><a name="p855184311135"></a><a name="p855184311135"></a>块状内存类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p14551843101314"><a name="p14551843101314"></a><a name="p14551843101314"></a>同上</p>
</td>
</tr>
<tr id="row397219346193"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.4.1.1 "><p id="p118111629205016"><a name="p118111629205016"></a><a name="p118111629205016"></a>osMessageQueueReset</p>
</td>
<td class="cellrowborder" valign="top" width="22.732273227322732%" headers="mcps1.1.4.1.2 "><p id="p1934243314192"><a name="p1934243314192"></a><a name="p1934243314192"></a>消息队列类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.205320532053214%" headers="mcps1.1.4.1.3 "><p id="p534219338193"><a name="p534219338193"></a><a name="p534219338193"></a>操作系统暂不提供清空队列接口，由用户对队列内容进行操作，避免资源泄漏或其他异常</p>
</td>
</tr>
</tbody>
</table>

<h4 id="CMSIS-v2-0标准接口适配差异">CMSIS v2.0标准接口适配差异</h4>

考虑接口的易用性和Huawei LiteOS内部机制与CMSIS标准接口的差异，在适配CMSIS v2.0接口时，对部分接口进行了修改，详见下表：

<a name="table2444612171913"></a>
<table><thead align="left"><tr id="row124441412151917"><th class="cellrowborder" valign="top" width="23.442344234423445%" id="mcps1.1.4.1.1"><p id="p399611168192"><a name="p399611168192"></a><a name="p399611168192"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="23.092309230923092%" id="mcps1.1.4.1.2"><p id="p179961216161916"><a name="p179961216161916"></a><a name="p179961216161916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="53.465346534653456%" id="mcps1.1.4.1.3"><p id="p18996816141914"><a name="p18996816141914"></a><a name="p18996816141914"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row8972173416196"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p2034116332198"><a name="p2034116332198"></a><a name="p2034116332198"></a>osKernelGetTickCount</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p43417338191"><a name="p43417338191"></a><a name="p43417338191"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p934183351915"><a name="p934183351915"></a><a name="p934183351915"></a>标准接口返回类型uint32_t，Huawei LiteOS适配为uint64_t</p>
</td>
</tr>
<tr id="row1597219341198"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p13341143314191"><a name="p13341143314191"></a><a name="p13341143314191"></a>osKernelGetTick2ms</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p1468155104211"><a name="p1468155104211"></a><a name="p1468155104211"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p16341163311918"><a name="p16341163311918"></a><a name="p16341163311918"></a>新增接口，用于获取系统启动后时间（单位：ms）</p>
</td>
</tr>
<tr id="row169721334161913"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p33411833151910"><a name="p33411833151910"></a><a name="p33411833151910"></a>osMs2Tick</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p897315560427"><a name="p897315560427"></a><a name="p897315560427"></a>内核类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p113411833141914"><a name="p113411833141914"></a><a name="p113411833141914"></a>新增接口，用于毫秒与系统Tick转换</p>
</td>
</tr>
<tr id="row1381482963912"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p1081452919398"><a name="p1081452919398"></a><a name="p1081452919398"></a>osThreadNew</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p28147291397"><a name="p28147291397"></a><a name="p28147291397"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p7814172903914"><a name="p7814172903914"></a><a name="p7814172903914"></a>优先级范围仅支持[osPriorityLow3, osPriorityHigh]</p>
</td>
</tr>
<tr id="row15887183874020"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p788753811401"><a name="p788753811401"></a><a name="p788753811401"></a>osThreadSetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p198871938194014"><a name="p198871938194014"></a><a name="p198871938194014"></a>任务/线程类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p198871538124010"><a name="p198871538124010"></a><a name="p198871538124010"></a>同上</p>
</td>
</tr>
<tr id="row13972133416199"><td class="cellrowborder" valign="top" width="23.442344234423445%" headers="mcps1.1.4.1.1 "><p id="p11341233141911"><a name="p11341233141911"></a><a name="p11341233141911"></a>osDelayUntil</p>
</td>
<td class="cellrowborder" valign="top" width="23.092309230923092%" headers="mcps1.1.4.1.2 "><p id="p19691258124218"><a name="p19691258124218"></a><a name="p19691258124218"></a>延时类接口</p>
</td>
<td class="cellrowborder" valign="top" width="53.465346534653456%" headers="mcps1.1.4.1.3 "><p id="p183411233201914"><a name="p183411233201914"></a><a name="p183411233201914"></a>标准接口入参类型uint32_t，Huawei LiteOS适配为uint64_t</p>
</td>
</tr>
</tbody>
</table>

