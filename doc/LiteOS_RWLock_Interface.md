<h3>读写锁</h3>

### 概述

#### 基本概念

读写锁实际是一种特殊的锁，它把对共享资源的访问者划分成读者和写者，读者对共享资源进行读访问，写者对共享资源进行写操作。

这种锁允许同时有多个读者来访问共享资源，而写者是排他性的。一个读写锁同时只能有一个写者或多个读者，但不能同时既有读者又有写者。

Huawei LiteOS提供的读写锁具有如下特点：

- 考虑到嵌入式领域绝大部分是读多写少的场景，采用写者优先的策略。

#### 运作机制

**读写锁运作原理**

用读写锁处理公共资源的访问时，如果有写者访问该资源，则读写锁为写加锁状态。此时其他任务如果想读写这个公共资源则会被阻塞，直到写锁被持有该锁的任务释放后，其他任务才能重新访问该公共资源，由于采用写锁优先的策略，这时候其他写锁会优先访问该公共资源。如果此时没有其他写者阻塞，那么就唤醒所有阻塞的读者进行并发读操作。

如果一个或多个读者访问该资源，则读写锁为读加锁状态。此时如果没有写者阻塞，那么新来的读者可以获取该读锁。如果已有写者阻塞，那么新来的读者和写者都是阻塞的。只有当所有持有该读锁的读者都释放后，才会真正释放该锁。

### 开发指导

#### 使用场景

多任务环境下会存在多个任务访问同一个公共资源的场景，该公共资源是允许被同时读的，并且为了保证一致性，不允许同时写或同时读写的。这种情况下，可以考虑使用读写锁。

#### 功能

Huawei LiteOS 系统中的读写锁模块为用户提供下面几种功能。

| 功能分类           | 接口名            | 描述             |
| ------------------ | ----------------- | ---------------- |
| 读写锁的创建和删除 | LOS_RWLockCreate  | 创建读写锁       |
|                    | LOS_RWLockDelete  | 删除指定的读写锁 |
| 读锁的申请和释放   | LOS_RWReadLock    | 申请指定的读锁   |
|                    | LOS_RWReadUnLock  | 释放指定的读锁   |
| 写锁的申请和释放   | LOS_RWWriteLock   | 申请指定的写锁   |
|                    | LOS_RWWriteUnLock | 释放指定的写锁   |

#### 开发流程

读写锁典型场景的开发流程：

1. 创建读写锁LOS_RWLockCreate。

2. 申请读写锁LOS_RWReadLock或LOS_RWWriteLock。

   - 申请读锁时：如果该锁没有被持有，则申请成功；如果该锁被任意读者持有而且没有写者阻塞，则申请成功。
   - 申请写锁时：如果该锁没有被持有或者已被自己持有，则申请成功。

   申请模式有三种：无阻塞模式、永久阻塞模式、定时阻塞模式。

   - 无阻塞模式：任务需要申请读写锁，若该读写锁当前没有任务持有，或者持有该读写锁的任务和申请该读写锁的任务为同一个任务，则申请成功。

   - 永久阻塞模式：任务需要申请读写锁，若该读写锁当前没有被占用，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，直到有其他任务释放该读写锁，阻塞任务才会重新得以执行。

   - 定时阻塞模式：任务需要申请读写锁，若该读写锁当前没有被占用，则申请成功。否则该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，指定时间超时前有其他任务释放该读写锁，或者用户指定时间超时后，阻塞任务才会重新得以执行。

3. 释放读写锁LOS_RWReadUnLock或LOS_RWWriteUnLock。

   - 释放读锁时：如果还有其他的读者在持有该读锁，那么剩余的读者继续持有；如果没有任何读者在持有该读锁，并且有写者阻塞，那么开始唤醒写者。

   - 释放写锁时：如果有其他写者阻塞，那么开始唤醒写者；如果无任何写者阻塞，那么同时唤醒所有阻塞的读者。

4. 删除读写锁LOS_RWLockDelete。

#### 读写锁错误码

对读写锁存在失败的可能性操作，包括读写锁创建，读写锁删除，读写锁申请，读写锁释放。

| 序号 | 定义                          | 实际数值   | 描述                                       | 参考解决方案                         |
| ---- | ----------------------------- | ---------- | ------------------------------------------ | ------------------------------------ |
| 1    | LOS_ERRNO_RWLOCK_NO_MEMORY    | 0x02001f00 | 内存请求失败                               | 减少读写锁限制数量的上限             |
| 2    | LOS_ERRNO_RWLOCK_INVALID      | 0x02001f01 | 读写锁不可用                               | 传入有效的读写锁的ID                 |
| 3    | LOS_ERRNO_RWLOCK_PTR_NULL     | 0x02001f02 | 入参为空                                   | 确保入参可用                         |
| 4    | LOS_ERRNO_RWLOCK_ALL_BUSY     | 0x02001f03 | 没有读写锁可用                             | 增加读写锁限制数量的上限             |
| 5    | LOS_ERRNO_RWLOCK_UNAVAILABLE  | 0x02001f04 | 锁失败，因为锁被其他线程使用               | 等待其他线程解锁或者设置等待时间     |
| 6    | LOS_ERRNO_RWLOCK_PEND_INTERR  | 0x02001f05 | 在中断中使用读写锁                         | 在中断中禁止调用此接口               |
| 7    | LOS_ERRNO_RWLOCK_PEND_IN_LOCK | 0x02001f06 | 任务调度没有使能，线程等待另一个线程释放锁 | 设置PEND为非阻塞模式或者使能任务调度 |
| 8    | LOS_ERRNO_RWLOCK_TIMEOUT      | 0x02001f07 | 读写锁PEND超时                             | 增加等待时间或者设置一直等待模式     |
| 9    | LOS_ERRNO_RWLOCK_OVERFLOW     | 0x02001f08 | 暂未使用，待扩展                           | 无                                   |
| 10   | LOS_ERRNO_RWLOCK_PENDED       | 0x02001f09 | 删除正在使用的锁                           | 等待解锁再删除锁                     |
| 11   | LOS_ERRNO_RWLOCK_MAXNUM_ZERO  | 0x02001f0a | 锁的数量限制为0                            | 将锁的数量限制改为非零值             |

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下

```
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \
(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
LOS_ERRTYPE_ERROR：Define critical OS errors
LOS_ERRNO_OS_ID：OS error code flag
LOS_MOD_RWLOCK：RWLock module ID
MID：OS_MOUDLE_ID
ERRNO：error ID number
```

例如：
```
LOS_ERRNO_RWLOCK_TIMEOUT           LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x07)
```

#### 平台差异性
无。

### 注意事项

- 读写锁不能在中断服务程序中使用。

- Huawei LiteOS作为实时操作系统需要保证任务调度的实时性，尽量避免任务的长时间阻塞，因此在获得读写锁之后，应该尽快释放读写锁。

- 持有读写锁的过程中，不得再调用LOS\_TaskPriSet等接口更改持有读写锁任务的优先级。

### 编程实例

#### 实例描述

本实例实现如下流程。

1.  任务Example_RWLock创建一个读写锁，锁任务调度，创建四个任务Example_RWLockTask1、Example_RWLockTask2,Example_RWLockTask3和Example_RWLockTask4，优先级排序为：Example_RWLockTask2> Example_RWLockTask1 >Example_RWLockTask3 = Example_RWLockTask4，解锁任务调度。
2.  Example_RWLockTask2被调度，申请读锁，然后任务休眠20Tick，Example_RWLockTask2挂起，Example_RWLockTask1被唤醒。
3.  Example_RWLockTask1申请读锁，等待时间为50Tick，因读写锁被Example_RWLockTask2读者持有，Example_RWLockTask1拿到读锁，Example_RWLockTask1在执行过程中会进行20次长度为10Tick的任务休眠。
4.  Example_RWLockTask1挂起后，调度Example_RWLockTask3和Example_RWLockTask4，这两个写者均阻塞。
5.  Example_RWLockTask1和Example_RWLockTask2并行访问，然后Example_RWLockTask2解读锁，Example_RWLockTask1继续执行。
6.  Example_RWLockTask1解锁，Example_RWLockTask3获取写锁，开始执行。
7.  Example_RWLockTask3解锁，Example_RWLockTask4获取写锁，开始执行。
8.  Example_RWLockTask4解锁，最后删除读写锁。

#### 编程示例

前提条件：

- 在los\_config.h中，将LOSCFG\_BASE\_IPC\_RWLOCK配置项打开。

- 配好LOSCFG\_BASE\_IPC\_RWLOCK\_LIMIT最大的读写锁个数。

代码实现如下：
```
/* rwlock handle id */
static UINT32 g_TestRWLock01;

/* task pid */
static UINT32 g_TestTaskID01;
static UINT32 g_TestTaskID02;
static UINT32 g_TestTaskID03;
static UINT32 g_TestTaskID04;


static VOID Example_RWLockTask1(VOID)
{
    UINT32 uwRet = LOS_OK;
    UINT32 count =20;

    dprintf("task1 try to get rwlock, wait 50 Tick. \r\n");
    /* get rwlock */
    uwRet = LOS_RWReadLock(g_TestRWLock01, 50);

    if (uwRet == LOS_OK)
    {
        dprintf("task1 get rwlock g_TestRWLock01. \r\n");
    }
    else if (uwRet == LOS_ERRNO_RWLOCK_TIMEOUT)
    {
        dprintf("task1 timeout and try to get  rwlock, wait forever. \r\n");
        /* LOS_WAIT_FOREVER type get rwlock, LOS_RWWriteLock return until has been get rwlock */
        uwRet = LOS_RWReadLock(g_TestRWLock01, LOS_WAIT_FOREVER);
        if (uwRet == LOS_OK)
        {
            dprintf("task1 wait forever,got g_TestRWLock01 success. \r\n");
        }
    }
    while(count--)
    {
        dprintf("Task 1 r\r\n");
        LOS_TaskDelay(10);
    }
    dprintf("task1 unlock the wlock  \r\n");
    LOS_RWReadUnLock(g_TestRWLock01);
    return;
}

static VOID Example_RWLockTask2(VOID)
{
    UINT32 uwRet = LOS_OK;
    UINT32 count=10;

    dprintf("task2 try to get rlock, wait forever. \r\n");
    /* get rwlock */
    uwRet = LOS_RWReadLock(g_TestRWLock01, LOS_WAIT_FOREVER);
    if(uwRet != LOS_OK)
    {
        dprintf("task2 LOS_RWWriteLock failed . \r\n");
        return;
    }

    dprintf("task2 get  g_TestRWLock01.  \r\n");

    /* task delay 20 ticks */
    LOS_TaskDelay(20);

    while(count--)
    {
        dprintf("Task 2 r\r\n");
        LOS_TaskDelay(10);
    }

    dprintf("task2 unlock the rlock \r\n");
    /* release rwlock */
    LOS_RWReadUnLock(g_TestRWLock01);
    return;
}

static VOID Example_RWLockTask3(VOID)
{
    UINT32 uwRet;
    UINT32 count=30;

    dprintf("task3 try to get rwlock. \r\n");
    /* get rwlock */
    uwRet = LOS_RWWriteLock(g_TestRWLock01, LOS_WAIT_FOREVER);
    if(uwRet != LOS_OK)
    {
        dprintf("task3 LOS_RWWriteLock failed . \r\n");
        return;
    }

    dprintf("task3 get rwlock g_TestRWLock01 . \r\n");

    /* task delay 20 ticks */
    LOS_TaskDelay(20);

    while(count--)
    {
        dprintf("Task 3 w\r\n");
        LOS_TaskDelay(10);
    }
    dprintf("task3 resumed and post the g_TestRWLock01 \r\n");
    LOS_RWWriteUnLock(g_TestRWLock01);
    return;
}

static VOID Example_RWLockTask4(VOID)
{
    UINT32 uwRet;
    UINT32 count=30;

    dprintf("task4 try to get rwlock. \r\n");
    /* get rwlock */
    uwRet = LOS_RWReadLock(g_TestRWLock01, LOS_WAIT_FOREVER);
    if(uwRet != LOS_OK)
    {
        dprintf("task4 LOS_RWReadLock failed . \r\n");
        return;
    }

    dprintf("task4 get rwlock g_TestRWLock01 . \r\n");

    /* task delay 20 ticks */
    LOS_TaskDelay(20);

    while(count--)
    {
        dprintf("Task 4 w\r\n");
        LOS_TaskDelay(10);
    }
    dprintf("task4 unlock the rlock \r\n");
    LOS_RWReadUnLock(g_TestRWLock01);
    return;
}


UINT32 Example_RWLock(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;
    TSK_INIT_PARAM_S stTask3;
    TSK_INIT_PARAM_S stTask4;

    /* create rwlock */
    LOS_RWLockCreate(&g_TestRWLock01);

    /* lock task schedue */
    LOS_TaskLock();

    /* create task1 */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_RWLockTask1;
    stTask1.pcName       = "RWLockTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if (uwRet != LOS_OK)
    {
        dprintf("task1 create failed . \r\n");
        return LOS_NOK;
    }

    /* create task2 */
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_RWLockTask2;
    stTask2.pcName       = "RWLockTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = 4;
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if (uwRet != LOS_OK)
    {
        dprintf("task2 create failed . \r\n");
        return LOS_NOK;
    }

    /* create task3 */
    memset(&stTask3, 0, sizeof(TSK_INIT_PARAM_S));
    stTask3.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_RWLockTask3;
    stTask3.pcName       = "RWLockTsk3";
    stTask3.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask3.usTaskPrio   = 6;
    uwRet = LOS_TaskCreate(&g_TestTaskID03, &stTask3);
    if (uwRet != LOS_OK)
    {
        dprintf("task3 create failed . \r\n");
        return LOS_NOK;
    }

    /* create task4 */
    memset(&stTask4, 0, sizeof(TSK_INIT_PARAM_S));
    stTask4.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_RWLockTask4;
    stTask4.pcName       = "RWLockTsk4";
    stTask4.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask4.usTaskPrio   = 6;
    uwRet = LOS_TaskCreate(&g_TestTaskID04, &stTask4);
    if (uwRet != LOS_OK)
    {
        dprintf("task4 create failed . \r\n");
        return LOS_NOK;
    }

    /* unlock task schedue */
    LOS_TaskUnlock();
    /* task delay 300 ticks */
    LOS_TaskDelay(300);

    /* delete rwlock */
    LOS_RWLockDelete(g_TestRWLock01);
    return LOS_OK;
}
```

#### 结果验证

编译运行得到的结果为：

    task2 try to get rlock, wait forever.
    task2 get  g_TestRWLock01.
    task1 try to get rwlock, wait 50 Tick.
    task1 get rwlock g_TestRWLock01.
    Task 1 r
    task3 try to get rwlock.
    task4 try to get rwlock.
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    Task 2 r
    Task 1 r
    task2 unlock the rlock
    Task 1 r
    Task 1 r
    Task 1 r
    Task 1 r
    Task 1 r
    Task 1 r
    Task 1 r
    Task 1 r
    task1 unlock the wlock
    task3 get rwlock g_TestRWLock01 .
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    Task 3 w
    task3 resumed and post the g_TestRWLock01
    task4 get rwlock g_TestRWLock01 .
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    Task 4 w
    task4 unlock the rlock
