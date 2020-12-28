include $(LITEOSTOPDIR)/config.mk

MODULE_NAME     := $(notdir $(CURDIR))

LOCAL_SRCS_y    := los_bitmap.c los_err.c los_hwi.c los_lockdep.c los_misc.c \
                   los_mp.c los_percpu.c los_sortlink.c los_stackinfo.c \
                   los_task.c los_tick.c los_exc.c los_printf.c

LOCAL_SRCS_$(LOSCFG_BASE_IPC_QUEUE) += los_queue.c
LOCAL_SRCS_$(LOSCFG_BASE_IPC_EVENT) += los_event.c
LOCAL_SRCS_$(LOSCFG_BASE_IPC_MUX) += los_mux.c
LOCAL_SRCS_$(LOSCFG_BASE_IPC_SEM) += los_sem.c
LOCAL_SRCS_$(LOSCFG_BASE_CORE_SWTMR) += los_swtmr.c

#ringbuf
LOCAL_SRCS_$(LOSCFG_KERNEL_RINGBUF) += los_ringbuf.c

# debug
LOCAL_SRCS_$(LOSCFG_DEBUG_QUEUE) += debug/los_queue_debug.c
LOCAL_SRCS_$(LOSCFG_DEBUG_MUTEX) += debug/los_mux_debug.c
LOCAL_SRCS_$(LOSCFG_DEBUG_MUTEX_DEADLOCK) += debug/los_mux_deadlock.c
LOCAL_SRCS_$(LOSCFG_DEBUG_SEMAPHORE) += debug/los_sem_debug.c
LOCAL_SRCS_$(LOSCFG_DEBUG_SCHED_STATISTICS) += debug/los_sched_debug.c

# Schedulers
LOCAL_DIRS_$(LOSCFG_SCHED_SQ) += sched/sched_sq
LOCAL_DIRS_$(LOSCFG_SCHED_MQ) += sched/sched_mq

# Memory Algorithms
LOCAL_DIRS_$(LOSCFG_KERNEL_MEM_BESTFIT) += mem/bestfit
LOCAL_DIRS_$(LOSCFG_KERNEL_MEM_BESTFIT_LITTLE) += mem/bestfit_little

# Memory Extention
LOCAL_DIRS_$(LOSCFG_KERNEL_MEM_SLAB_EXTENTION) += mem/slab
LOCAL_DIRS_$(LOSCFG_MEM_TASK_STAT) += mem/common/memstat

# Membox
LOCAL_SRCS_$(LOSCFG_KERNEL_MEMBOX_STATIC) += mem/membox/los_membox.c
LOCAL_SRCS_$(LOSCFG_KERNEL_MEMBOX_DYNAMIC) += mem/membox/los_membox_dyn.c

# Memory Debug
LOCAL_DIRS_$(LOSCFG_MEM_RECORDINFO) += mem/common/memrecord
LOCAL_DIRS_$(LOSCFG_MEM_MUL_POOL) += mem/common/multipool

# Shellcmd
LOCAL_DIRS_$(LOSCFG_SHELL) += shellcmd

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_INCLUDE   := \
                -I $(LITEOSTOPDIR)/kernel/extended/include \
                -I $(LITEOSTOPDIR)/kernel/extended/include/lowpower \
                -I $(LITEOSTOPDIR)/kernel/base/mem

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
