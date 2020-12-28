# Agent Tiny Mqtt Demo 使用手册
## "`Agent Tiny Mqtt Demo`"使用介绍
agent tiny mqtt demo 是mqtt客户端的实现示例， 提供了ssl单向加密验证， psk加密认证和无加密等通信验证方式， 在需要与mqtt服务器创建通信的场景中， 可以根据学习及应用需求， 按照以下文档所述切换对应的客户端通信方式。    
该文档也提供了mqtt服务端与客户端demo创建通信的简易配置方法，供通信验证使用， mqtt服务器使用的是`mosquitto`, ssl单向验证证书由`openssl`生成， 开启mosquitto, openssl等服务及`liteos源码`配置和编译，均在"`ubuntu`"系统下进行。   
使用ssl单向加密验证， psk加密认证等验证和无加密通信时， mosquitto服务器需要创建相对应的配置文件，具体配置创建操作在以下文档进行描述。   
关于设备端mqtt demo的实现所使用的硬件为stm32开发板 “`野火Cloud_STM32F429IGTx_FIRE`”。   

---
## 使能设备端 “`Agent Tiny Mqtt Demo`”
### `menuconfig`配置及编译
在`LiteOS工程`目录下执行以下操作， Agent Tiny Mqtt Demo例程默认关闭， 我们先进行如下基础配置的设置：  
```c  
cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
```

然后通过"`make menuconfig`"调用以下配置来使能`Agent Tiny Mqtt Demo`。   
在`menuconfig`中， 需要使能相对应的`Components` ---> `mqtt，agenttiny mqtt，cjson, log, lwip, sal, mbedtls`等组件，    
* `mqtt`， `agenttiny mqtt` 提供了mqtt客户端通信实现。
* `cjson` 组件提供了mqtt通信的数据格式。
* `log` 组件提供了调试信息打印功能。
* `lwip`, `sal`组件提供了网络通信接口。
* `mbedtls` 组件提供了加解密算法。   

在`menuconfig`中， 使能`Demos` --->` Agent Tiny Mqtt Demo`选项时，加密通信时`encryption mode`加密方式可选择`PSK_ENCRYPT`或`CRET_ENCRYPT`，无加密时可选择`NO_ENCRYPT`， 具体设置如下。
```c
/* menuconfig 配置如下 */
Componetns --->
        Connectivity --->
				[*] Enable Connectivity
                		[*] Enable Mqtt
               			[*] Enable Agenttiny Mqtt
        Lib --->
                [*] Enable cJSON
        Log --->
                [*] Enable Log
        Network --->
                [*] Enable Network
                		[*] Enable Lwip
						    Enable Ipv4 Or Ipv6 (Enable Ipv4)  --->
							    (X) Enable Ipv4
               			[*] Enable Sal
        Security --->
                [*] Enable Security
                		[*] Enable Mbed TLS Component
Demos --->
        Agent Tiny Mqtt Demo  --->
                [*] Enable Agent Tiny Mqtt Demo
                        encryption mode (NO_ENCRYPT)  --->
                                                (X) NO_ENCRYPT
                                                ( ) PSK_ENCRYPT
                                                ( ) CRET_ENCRYPT
```
### 配置 `Agent Tiny Mqtt Demo`
Agent Tiny Mqtt Demo源码文件为<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/agenttiny_mqtt/agent_tiny_mqtt_demo.c" target="_blank">demos/agenttiny_mqtt/agent_tiny_mqtt_demo.c</a>。   

在上面`menuconfig`选择`encryption mode`加密方式为`PSK_ENCRYPT`或`CRET_ENCRYPT`以使能`agent_tiny_mqtt_demo.c`中对应的以下宏开关：

```c
#ifdef LOSCFG_DTLS_PSK_ENCRYPT
#define MQTT_DEMO_USE_PSK 1
#elif LOSCFG_DTLS_CRET_ENCRYPT
#define MQTT_DEMO_USE_CERT 1
#endif
```
* `MQTT_DEMO_USE_PSK` 置 `1` 时，  Demo 使用 psk 加密；
* `MQTT_DEMO_USE_CERT` 置 `1` 时， Demo 使用 cert 证书加密；  


#### 设置Mqtt服务端IPv4地址
该示例是在局域网中进行测试， 这里的mqtt服务器地址为开启mosquitto服务的ubuntu主机ip地址，需要根据实际情况进行设置， 这里的示例mqtt服务端ip为"`192.168.3.8`"。
```
#define DEFAULT_SERVER_IPV4 "192.168.3.8"
```

#### 设置设备端IPv4地址
在源码<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c" target="_blank">targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c</a> 中， 如下所示， 如果mosquitto服务搭建在局域网环境中，设备的ip需要与ubuntu主机在同一网段下， 这里的示例设备端ip为"`192.168.3.200`"。
```c
void net_init(void)
{
    /* IP addresses initialization */
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 3;
    IP_ADDRESS[3] = 200;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 3;
    GATEWAY_ADDRESS[3] = 1;
	......
```

#### 设置Mqtt服务端的端口

1，选择 `NO_ENCRYPT`时, 示例中port设置为：
   ```c
   #define DEFAULT_SERVER_PORT "1883"
   ```
2，选择 `PSK_ENCRYPT` 或 `CRET_ENCRYPT` 时, 示例中port设置为：
   ```c
   #define DEFAULT_SERVER_PORT "8883"
   ```

#### `cert认证`或`psk认证`时, 需要进行以下配置
1, 当选择 `PSK_ENCRYPT` 时，参考如下代码设置`AGENT_TINY_DEMO_PSK_ID`和`g_demo_psk`,这两处配置会在后面进行tls-psk认证时使用；
```c
#define AGENT_TINY_DEMO_PSK_ID "testID" //供测试的自定义数据
#define AGENT_TINY_DEMO_PSK_LEN (3)
unsigned char g_demo_psk[AGENT_TINY_DEMO_PSK_LEN] = {0xab, 0xcd, 0xef}; //供测试的自定义数据
```


2, 当选择 `CRET_ENCRYPT` 时，`g_mqtt_ca_crt[]`数组内容是由openssl生成的ca.crt，生成方式在后面`使用openssl生成mosuqitto单向认证证书步骤`中进行介绍。
```c
static char g_mqtt_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIEQjCCAyqgAwIBAgIJAPd1i7XQtMI6MA0GCSqGSIb3DQEBBQUAMHMxCzAJBgNV\r\n"
"BAYTAmNuMQswCQYDVQQIEwJ6ajELMAkGA1UEBxMCaHoxDTALBgNVBAoTBHRlc3Qx\r\n"
"DTALBgNVBAsTBHRlc3QxEDAOBgNVBAMTBzAuMC4wLjAxGjAYBgkqhkiG9w0BCQEW\r\n"
"C3Rlc3RAcXEuY29tMB4XDTIwMTAxOTAzNTQyNFoXDTMwMTAxNzAzNTQyNFowczEL\r\n"
"MAkGA1UEBhMCY24xCzAJBgNVBAgTAnpqMQswCQYDVQQHEwJoejENMAsGA1UEChME\r\n"
"dGVzdDENMAsGA1UECxMEdGVzdDEQMA4GA1UEAxMHMC4wLjAuMDEaMBgGCSqGSIb3\r\n"
"DQEJARYLdGVzdEBxcS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\r\n"
"AQC60eLKxhugNt4qW8/F+Gw2PJr6KRwJ+GtuCMRHkEEDNTKjWWBDJp91HTgBgKL/\r\n"
"1j4M0QpvFjJk9FGeLPm3jNi/TP3n9cYbd4zd92Dg28O9d20EwEvFv1AwCcRPXMrK\r\n"
"e/7sknH7kCOd8wQPGM+3UbyK1apzkZXkIA3kjvMVs4thOteBRtdjM1EJL0ZP7Me1\r\n"
"EnOfOrHBAHSJU8uHozPfQB+YJhLQADSIdjrxk2kMW+XHhDNSUZIkB9rFD7Uhbad1\r\n"
"s9dUnBVWbS/sZaWKeMM7bJpfdub5JvLKLDHvNLCtdQ51jI605ZQrn/C3JJKFL96R\r\n"
"PjPE+m1rTsOYMH06pdOoiitjAgMBAAGjgdgwgdUwHQYDVR0OBBYEFHqPmqO2OhSw\r\n"
"PpVR2ypkNNJ1m5v3MIGlBgNVHSMEgZ0wgZqAFHqPmqO2OhSwPpVR2ypkNNJ1m5v3\r\n"
"oXekdTBzMQswCQYDVQQGEwJjbjELMAkGA1UECBMCemoxCzAJBgNVBAcTAmh6MQ0w\r\n"
"CwYDVQQKEwR0ZXN0MQ0wCwYDVQQLEwR0ZXN0MRAwDgYDVQQDEwcwLjAuMC4wMRow\r\n"
"GAYJKoZIhvcNAQkBFgt0ZXN0QHFxLmNvbYIJAPd1i7XQtMI6MAwGA1UdEwQFMAMB\r\n"
"Af8wDQYJKoZIhvcNAQEFBQADggEBAAI+zigjJskLs0baSfu8blM35P6Z7Ns5sl4f\r\n"
"xXnm9YFPi79NnDV379pTzzOy80Szh9A1k5QhAp3yELnKaK5KfZsVoh15YzEHNLep\r\n"
"j97LGFmXC7M4Fbr95+8nraKf+bDg5vhvDfFNabxpm0+/ZJp7VLFsWk5y9SrfON7Y\r\n"
"wSlNtpAfG2EPYs3wGwPsp8tb4I461QiitFb6tp97TdKS92YLRqVK5u2YABYRMQu+\r\n"
"72ij6tZ3+XE8eWBBSCJeCr9NNNyOJ669ct2/oe6Bw7sxJvJ1GfsOd2cdD5RkTt5R\r\n"
"Z7Jm8H5nVFHmIEt3yilOXkjZGdn58dBzKaSk2y2IXYQjxwuQ6og=\r\n"
"-----END CERTIFICATE-----\r\n";
```
经过以上内容的操作赫和修改后，进行代码编译。 
### 编译`Agent Tiny Mqtt Demo`
在`LiteOS工程`目录下执行以下命令：
```c
make clean && make -j
```
生成的`Huawei_LiteOS.bin`固件在路径`out/Cloud_STM32F429IGTx_FIRE`中，烧录到开发板中即可。

---
## 使用openssl生成mosuqitto单向认证证书步骤
以下示例操作中， 路径为`/home/liteos/mosuqitto`, 根据实际需要选择文件路径。   
安装openssl服务
```c
sudo apt-get install openssl
```
#### 生成CA的Key和证书文件
1 生成ca的密钥(.key)
```c
openssl genrsa -des3 -out ca.key 2048
```
2, 生成ca证书(.crt)
```c
openssl req -new -x509 -days 365 -key ca.key -out ca.crt
```
#### 生成Server端证书
1, 生成server的私钥(.key)
```c
openssl genrsa -out server.key 2048
```
2, 生成server端请求文件(.csr) 
```c
openssl req -new -out server.csr -key server.key
```
3, 生成服务端的证书(.crt)
```c
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 365
```
经过以上操作，在`/home/liteos/mosuqitto`路径中，生成的文件中包含`ca.crt, server.key, server.crt`， 这几个示例配置文件供我们在后面进行`mosquitto测试单向认证时`使用。   
* 这里贴出`ca.crt, server.key, server.crt`对应的内容，供后面测试时使用；   
#### ca.crt文件内容如下:
```
-----BEGIN CERTIFICATE-----
MIIEQjCCAyqgAwIBAgIJAPd1i7XQtMI6MA0GCSqGSIb3DQEBBQUAMHMxCzAJBgNV
BAYTAmNuMQswCQYDVQQIEwJ6ajELMAkGA1UEBxMCaHoxDTALBgNVBAoTBHRlc3Qx
DTALBgNVBAsTBHRlc3QxEDAOBgNVBAMTBzAuMC4wLjAxGjAYBgkqhkiG9w0BCQEW
C3Rlc3RAcXEuY29tMB4XDTIwMTAxOTAzNTQyNFoXDTMwMTAxNzAzNTQyNFowczEL
MAkGA1UEBhMCY24xCzAJBgNVBAgTAnpqMQswCQYDVQQHEwJoejENMAsGA1UEChME
dGVzdDENMAsGA1UECxMEdGVzdDEQMA4GA1UEAxMHMC4wLjAuMDEaMBgGCSqGSIb3
DQEJARYLdGVzdEBxcS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB
AQC60eLKxhugNt4qW8/F+Gw2PJr6KRwJ+GtuCMRHkEEDNTKjWWBDJp91HTgBgKL/
1j4M0QpvFjJk9FGeLPm3jNi/TP3n9cYbd4zd92Dg28O9d20EwEvFv1AwCcRPXMrK
e/7sknH7kCOd8wQPGM+3UbyK1apzkZXkIA3kjvMVs4thOteBRtdjM1EJL0ZP7Me1
EnOfOrHBAHSJU8uHozPfQB+YJhLQADSIdjrxk2kMW+XHhDNSUZIkB9rFD7Uhbad1
s9dUnBVWbS/sZaWKeMM7bJpfdub5JvLKLDHvNLCtdQ51jI605ZQrn/C3JJKFL96R
PjPE+m1rTsOYMH06pdOoiitjAgMBAAGjgdgwgdUwHQYDVR0OBBYEFHqPmqO2OhSw
PpVR2ypkNNJ1m5v3MIGlBgNVHSMEgZ0wgZqAFHqPmqO2OhSwPpVR2ypkNNJ1m5v3
oXekdTBzMQswCQYDVQQGEwJjbjELMAkGA1UECBMCemoxCzAJBgNVBAcTAmh6MQ0w
CwYDVQQKEwR0ZXN0MQ0wCwYDVQQLEwR0ZXN0MRAwDgYDVQQDEwcwLjAuMC4wMRow
GAYJKoZIhvcNAQkBFgt0ZXN0QHFxLmNvbYIJAPd1i7XQtMI6MAwGA1UdEwQFMAMB
Af8wDQYJKoZIhvcNAQEFBQADggEBAAI+zigjJskLs0baSfu8blM35P6Z7Ns5sl4f
xXnm9YFPi79NnDV379pTzzOy80Szh9A1k5QhAp3yELnKaK5KfZsVoh15YzEHNLep
j97LGFmXC7M4Fbr95+8nraKf+bDg5vhvDfFNabxpm0+/ZJp7VLFsWk5y9SrfON7Y
wSlNtpAfG2EPYs3wGwPsp8tb4I461QiitFb6tp97TdKS92YLRqVK5u2YABYRMQu+
72ij6tZ3+XE8eWBBSCJeCr9NNNyOJ669ct2/oe6Bw7sxJvJ1GfsOd2cdD5RkTt5R
Z7Jm8H5nVFHmIEt3yilOXkjZGdn58dBzKaSk2y2IXYQjxwuQ6og=
-----END CERTIFICATE-----
```
#### server.key文件内容如下:
```
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAxFeGkopHig3KKWFTZo+zP2e8sUMQa8v7NggNTw7auvr6bDAk
jFvq0T63aCU+cQSxE4v8/Cm0PJ+e7bHt3Kc55Mnk72z7jpMSAVFbcT59r9pt6GCm
DAvbiLkWjmfDN41GMc9Q5DhNsepsh5/6pStB1wCUgElfiP3ioQ9imVJj6anst9sc
rjS+JYTggWn2ijF78CebVhNDMtaQEm+iPCsWk5dUrciK6FYT/oJ+BL6kPB/hn5ya
kD3ku3cwfUoaNA4AZcU0/rPzjCy+1oHkFO8A4DFa1nrTH3HO8ZgMZBo+Hesjv5W9
Z/ped//OhaG7Supu93ln5UnyQOCg3u/XdGgwZwIDAQABAoIBAQDAfsNsVnAvFLf4
e/glnItzhYRFEmg+F33SinBp6zldioUMnuKmTdPZlDCCxckDS0NE08GThFu1+BT8
G68F31nuSlbOuj2jmeNUMWyCiW72gyOwaVlChDmXQUexFGAbzcFdQlGffugFn70J
9znox5FEdsghQk6M48VWfNwBT/qu8XO6h9iPzdortAwgu0+bMgSlxR/F1m7QJKqU
1ehta/f+ezef/JesxclhElzjZJTDCHip6cQ50leNM5iyKPwGMlJ9PvqSTtAGwhT6
bic4/1q1XJKYNO4QAXbDQN2WmiM0aGa8byFFndhaNeOlWEDY0sGCHD7YTsik1wUQ
qTReLB1RAoGBAP/CgO2Y13DWHuQuIcXcTrdlgAnZZcPuHLv6rCEqbhPIlXBKUzFN
e3/NjRrTcrByrc0dD2ivjVZ+EftakmNUWXdulecdotkGfzj/SEDV1C9igl6LJqCS
Q6oHSKwbUv3gRzbc7rAe1QqzJ5ksirT9lNbz7Qr6lAvY+U3I4WJ2rZMfAoGBAMSG
vDpcOwyr5FGkFLE/vBjknKMpS2X5Yb7sKX711sqU1zZiMMdIKKxtkWcjUGf0Bj1Y
1Kv18oZ9ANg/bvMC6qiHz6m8IqEFLGLopM2tnA+Cy1FUusluaWoxzQo8DcDFP/S5
GOxE7hTs1WFRN6u5ThwtCo9tTL92GdA3fsXbokG5AoGAUtXCQDHihS80bh7q3VSP
QiLjp0z8AstaDyAPI+bEJDAxBl9sNmmr+Caw7uFRIgjida5V0UgadMr8M/aPtxAY
se/lKSeWGMhb6C/2TNl0kaxMISbcaeE1fRpC1Us8Q/Gh3wkLXYt+4KpJxviALEJ9
NGloesfRr0yk4iaPqP5bFW0CgYAj1OEKzvghNlH5pK31J0a3Ty9T/Fp37EErG0nx
kQcphnu6uaplGz33Oj7xjBnD6lUc6E5WUBB4/t+hjec/vX9lXOW678dsjlHqcrph
mc7DcTh6yrUHtu7XYt8ch5iGVjB0AS+PHH5AKHI7PSCmWbHpJzkur2H34qOQOB4Q
Gxx5WQKBgAIdHmZh1m+ehU92nvLP57D6MzneT12fZZMgtNB/JNVoFhss6YZKCKDH
yIIc6hKniHSzYEsutmwB7zVVkqi/5QfYkXu2cKRRdcdrZ83VZou0VF7S1KsTTINe
zgb7jiJ5fxbHy2t31pRN/8ce+5KbpYlMvbKsvxyqBbtNKs8eG775
-----END RSA PRIVATE KEY-----
```
#### server.crt文件内容如下:
```
-----BEGIN CERTIFICATE-----
MIIDZjCCAk4CCQDW0+BqOP4J5TANBgkqhkiG9w0BAQUFADBzMQswCQYDVQQGEwJj
bjELMAkGA1UECBMCemoxCzAJBgNVBAcTAmh6MQ0wCwYDVQQKEwR0ZXN0MQ0wCwYD
VQQLEwR0ZXN0MRAwDgYDVQQDEwcwLjAuMC4wMRowGAYJKoZIhvcNAQkBFgt0ZXN0
QHFxLmNvbTAeFw0yMDEwMTkwMzU1MjVaFw0zMDEwMTcwMzU1MjVaMHcxCzAJBgNV
BAYTAmNuMQswCQYDVQQIEwJ6ajELMAkGA1UEBxMCaHoxDTALBgNVBAoTBHRlc3Qx
DTALBgNVBAsTBHRlc3QxFDASBgNVBAMTCzE5Mi4xNjguMy44MRowGAYJKoZIhvcN
AQkBFgt0ZXN0QHFxLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AMRXhpKKR4oNyilhU2aPsz9nvLFDEGvL+zYIDU8O2rr6+mwwJIxb6tE+t2glPnEE
sROL/PwptDyfnu2x7dynOeTJ5O9s+46TEgFRW3E+fa/abehgpgwL24i5Fo5nwzeN
RjHPUOQ4TbHqbIef+qUrQdcAlIBJX4j94qEPYplSY+mp7LfbHK40viWE4IFp9oox
e/Anm1YTQzLWkBJvojwrFpOXVK3IiuhWE/6CfgS+pDwf4Z+cmpA95Lt3MH1KGjQO
AGXFNP6z84wsvtaB5BTvAOAxWtZ60x9xzvGYDGQaPh3rI7+VvWf6Xnf/zoWhu0rq
bvd5Z+VJ8kDgoN7v13RoMGcCAwEAATANBgkqhkiG9w0BAQUFAAOCAQEADeqSBBHP
OOST13a5O5yku+ih0DsuVFoh3CRV5rVEEGzk80gX/kNEngoeIzozN+8T8rkuXlt6
O1c/IvbbDRRz7GiBEahqbFDHLLS83lZnQFraQQA/hlLwGAdFib8fTR0woBjnEI0M
CDz9Ukl4bq3Jo+mjK3nZ1dsWcXgsdqLV4XsEdqvrN5ERKpm3qDeup/GbBqzr2qae
rBZFk/Dy/jcY/oSaA9qLwqMq6g+Va6L0Sm5iMUDW5DdQ6aDKnfESYeuRPxec+6u1
LlA52P62YmWTVKe8W+ClMhNClu8m3nSW7RfWPGdrGHd5nqN7IfHk3pVw2A6fYeAj
SQ3xd+/uZgdw8Q==
-----END CERTIFICATE-----
```
---

## 搭建及配置mosquitto服务端

*  安装mosquitto服务端
```
sudo apt-get install mosquitto
```
* 安装mosquitto客户端
```
sudo apt-get install mosquitto-clients
```

### 创建mosquitto配置文件：
以下文件所保存的路径，示例路径为 `/home/liteos/liteos/mosquitto`
#### 使用`mosquitto ssl单向认证`：
使用配置文件为mosquitto_crt.conf, 其内容如下：
```c
port 8883 #tls端口  
cafile /home/liteos/mosquitto/ca.crt
certfile /home/liteos/mosquitto/server.crt
keyfile /home/liteos/mosquitto/server.key
allow_anonymous true #允许匿名用户
require_certificate false #单向认证应该为false
use_identity_as_username false #单向认证设置为false
```

#### 使用`mosquitto tls-psk认证`：
使用配置文件为mosquitto_psk.conf, 其内容如下：
```c
port 8883
psk_hint pskHint  #自定义内容
psk_file /home/liteos/mosquitto/psk_file.txt 
```
上述配置文件中的psk_file.txt内容如下：
```c
testID:abcdef
```
* psk_file.txt 内容格式为 psk_identity : psk_key， 对应前面Demo源码内的`AGENT_TINY_DEMO_PSK_ID`和`g_demo_psk`这两个值
* psk_key 为16进制数据,没有开头的0x；
---
## 测试 Agent Tiny Mqtt Demo
###  开启mosquitto无加密服务
1， 在终端里开启mosquitto服务，命令如下：
```c
mosquitto -p 1883 -v
```
* -v 打印更多消息；

2， 设备编译烧录使用`NO_ENCRYPT`的Demo程序，上电重启。   
mosquitto服务开启后， 终端打印以下消息内容：
```c
/* mosquitto服务运行消息 */
1608192546: mosquitto version 1.6.12 starting
1608192546: Using default config.
1608192546: Opening ipv4 listen socket on port 1883.
1608192546: Opening ipv6 listen socket on port 1883.
1608192546: mosquitto version 1.6.12 running
/* 以下显示有设备连接上服务端*/
1608192761: New connection from 192.168.3.200 on port 1883.
1608192761: New client connected from 192.168.3.200 as 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (p2, c1, k100, u'54a6ba67-c023-4d26-9a1c-41adef662550').
1608192761: No will message specified.
/* 以下显示有设备发送数据到服务端*/
1608192761: Sending CONNACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (0, 0)
1608192761: Received SUBSCRIBE from 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
1608192761: 	/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json (QoS 1)
1608192761: 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 1 /huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json
1608192761: Sending SUBACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
```
```c
mosquitto_sub -t '/#' -h 192.168.3.98 -p 1883 -v
```

3, 设备端串口输出的信息如下：
```c
********Hello Huawei LiteOS********
LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 22 2020 22:40:45

**********************************
osAppInit
cpu 0 entering scheduler
lwip test init ok.
/* 以下显示设备端ip为192.168.3.200 */
netif: netmask of interface set to  192. 168.   3.   1
netif_set_ipaddr: netif address being changed
netif: added interface st IP addr  192. 168.   3. 200 netmask  255. 255. 255.   0 gw  192. 168.   3.   1
netif: setting default interface st
/* 以下显示设备成功连接mqtt服务端 */
[SOCKET][atiny_net_connect:271] try to do name resolution now...
[SOCKET][atiny_net_connect:278] do name resolution succeed
[SOCKET][atiny_net_connect:324] TCP connect to server succeed
```

4,  在终端下开启一个mosquitto client订阅服务,来查看设备发送的mqtt消息。   
```c
mosquitto_sub -t '/#' -h 192.168.3.8 -p 1883 -v
```

* -t 指定topic
* -h 指定要连接的域名或IPv4地址
* -p 服务端的端口号
* -v 打印更多消息

5， 上述订阅服务后, 终端输出信息为：
```c
/* 以下显示设备推送的topic, 及发送的json格式数据内容 */
/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/data/json {
	"msgType":	"deviceReq",
	"hasMore":	0,
	"data":	[{
			"serviceId":	"Battery",
			"serviceData":	{
				"batteryLevel":	9
			},
			"eventTime":	"20201103T114920Z"
		}]
}
```
###  开启mosquitto ssl单向认证服务
1， 在终端里开启mosquitto ssl单向认证服务，命令如下：
```c
mosquitto -c /home/liteos/mosquitto/mosquitto_crt.conf -v
```
*  -c 链接自定义配置文件；
* -v 打印更多消息；


2， 设备编译烧录使用`CRET_ENCRYPT`的Demo程序，上电重启。   
mosquitto服务开启后， 终端打印以下消息内容：    
```c
/* mosquitto服务运行消息 */
1606103977: mosquitto version 1.6.12 starting
1606103977: Config loaded from /home/liteos/mosquitto/mosquitto_crt.conf.
1606103977: Opening ipv4 listen socket on port 8883.
1606103977: Opening ipv6 listen socket on port 8883.
1606103977: mosquitto version 1.6.12 running
/* 以下显示有设备连接上服务端*/
1606103985: New connection from 192.168.3.200 on port 8883.
1606103986: New client connected from 192.168.3.200 as 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (p2, c1, k100, u'54a6ba67-c023-4d26-9a1c-41adef662550').
1606103986: No will message specified.
/* 以下显示有设备发送数据到服务端*/
1606103986: Sending CONNACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (0, 0)
1606103987: Received SUBSCRIBE from 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
/* 以下显示设备Publish的Topic, 和发送消息的字节数*/
1606103987: /huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json (QoS 1)
1606103987: 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 1 /huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json
1606103987: Sending SUBACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
1606103995: Received PUBLISH from 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (d0, q1, r0, m6, '/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/data/json', ... (170 bytes))
1606103995: Sending PUBACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (m6, rc0)
```

3, 设备端串口输出的信息如下：
```c
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 22 2020 22:40:45

**********************************
osAppInit
cpu 0 entering scheduler
lwip test init ok.
/* 以下显示设备端ip为192.168.3.200 */
netif: netmask of interface set to  192. 168.   3.   1
netif_set_ipaddr: netif address being changed
netif: added interface st IP addr  192. 168.   3. 200 netmask  255. 255. 255.   0 gw  192. 168.   3.   1
netif: setting default interface st
/* 以下显示设备端 mbedtls ssl 相关初始化操作 */
[MBEDTLS][dtls_ssl_new:142] setting up the SSL structure
[MBEDTLS][dtls_ssl_new:209] set SSL structure succeed
[MBEDTLS][dtls_shakehand:263] connecting to server
/* 以下显示设备成功连接mqtt服务端 */
[SOCKET][atiny_net_connect:271] try to do name resolution now...
[SOCKET][atiny_net_connect:278] do name resolution succeed
[SOCKET][atiny_net_connect:324] TCP connect to server succeed
/* 以下显示设备与mqtt服务端认证成功 */
[MBEDTLS][dtls_shakehand:280] performing the SSL/TLS handshake
[MBEDTLS][dtls_shakehand:322] cert verify succeed
[MBEDTLS][dtls_shakehand:327] handshake succeed
```

4,  在终端下开启一个mosquitto client订阅服务,来查看设备发送的mqtt消息。   
```c
mosquitto_sub -t '/#' -h 192.168.3.8 -p 8883 -v --cafile /home/liteos/mosquitto/ca.crt --tls-version tlsv1.2 --insecure 
```

* -t 指定topic
* -h 指定要连接的域名或IPv4地址
* -p 服务端的端口号
* -v 打印更多消息
* --cafile  指定ca证书
* --tls-version 指定tls版本号
* --insecure 忽略检查证书域名   

5， 上述订阅服务后, 终端输出信息为：
```c
/* 以下显示设备推送的topic, 及发送的json格式数据内容 */
/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/data/json {
	"msgType":	"deviceReq",
	"hasMore":	0,
	"data":	[{
			"serviceId":	"Battery",
			"serviceData":	{
				"batteryLevel":	9
			},
			"eventTime":	"20201103T114920Z"
		}]
}
```
---


### 开启mosquitto tls-psk认证服务
1，在终端里开启mosquitto tls-psk认证服务，命令如下：
```
mosquitto -c /home/liteos/mosquitto/mosquitto_psk.conf -v
```
* -c 链接自定义配置文件
* -v 打印更多消息


2, 设备编译烧录使用`PSK_ENCRYPT`的Demo程序，上电重启。  

mosquitto服务开启后， 终端打印以下消息内容：    
```c
/* mosquitto服务运行消息 */
1606111889: mosquitto version 1.6.12 starting
1606111889: Config loaded from /home/liteos/mosquitto/mosquitto_psk.conf.
1606111889: Opening ipv4 listen socket on port 8883.
1606111889: Opening ipv6 listen socket on port 8883.
1606111889: mosquitto version 1.6.12 running
/* 以下显示有设备连接上服务端*/
1606111913: New connection from 192.168.3.200 on port 8883.
1606111914: New client connected from 192.168.3.200 as 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (p2, c1, k100, u'testID').
1606111914: No will message specified.
1606111914: Sending CONNACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (0, 0)
1606111915: Received SUBSCRIBE from 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
/* 以下显示设备Publish的Topic, 和发送消息的字节数*/
1606111915: 	/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json (QoS 1)
1606111915: 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 1 /huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/command/json
1606111915: Sending SUBACK to 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103
1606111924: Received PUBLISH from 54a6ba67-c023-4d26-9a1c-41adef662550_0_0_20201103 (d0, q1, r0, m3, '/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/data/json', ... (171 bytes))
```

3, 设备端串口输出的信息：
```c
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 22 2020 22:40:45

**********************************
osAppInit
cpu 0 entering scheduler
lwip test init ok.
/* 以下显示设备端ip为192.168.3.200 */
netif: netmask of interface set to  192. 168.   3.   1
netif_set_ipaddr: netif address being changed
netif: added interface st IP addr  192. 168.   3. 200 netmask  255. 255. 255.   0 gw  192. 168.   3.   1
netif: setting default interface st
/* 以下显示设备端 mbedtls tls-psk 相关初始化操作 */
[MBEDTLS][dtls_ssl_new:142] setting up the SSL structure
[MBEDTLS][dtls_ssl_new:209] set SSL structure succeed
[MBEDTLS][dtls_shakehand:263] connecting to server
/* 以下显示设备成功连接mqtt服务端 */
[SOCKET][atiny_net_connect:271] try to do name resolution now...
[SOCKET][atiny_net_connect:278] do name resolution succeed
[SOCKET][atiny_net_connect:324] TCP connect to server succeed
[MBEDTLS][dtls_shakehand:280] performing the SSL/TLS handshake
[MBEDTLS][dtls_shakehand:322] cert verify succeed
/* 以下显示设备与mqtt服务端认证成功 */
[MBEDTLS][dtls_shakehand:327] handshake succeed
```
4,  在终端下开启一个mosquitto client订阅服务,来查看设备发送的mqtt消息。   
开启mosquitto client订阅服务，命令如下：
```c
mosquitto_sub --psk-identity testID --psk abcdef -h 192.168.3.8 -p 8883 -t '/#' -v
```
* --psk-identity 身份信息
* --psk          共享密钥
* -h 指定要连接的域名或IPv4地址
* -p 服务端的端口号
* -t 指定topic， '/#'意为订阅所有topic
* -v 打印更多消息

5， 上述订阅服务后, 终端输出信息为：
```c
/* 以下显示设备推送的topic, 及发送的json格式数据内容 */
/huawei/v1/devices/54a6ba67-c023-4d26-9a1c-41adef662550/data/json {
	"msgType":	"deviceReq",
	"hasMore":	0,
	"data":	[{
			"serviceId":	"Battery",
			"serviceData":	{
				"batteryLevel":	7
			},
			"eventTime":	"20201103T114920Z"
		}]
}
```
