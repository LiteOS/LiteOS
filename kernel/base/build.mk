kernel-base-core-y = \
	core/los_priqueue.o \
	core/los_swtmr.o \
	core/los_sys.o \
	core/los_task.o \
	core/los_tick.o \
	core/los_timeslice.o

kernel-base-ipc-y = \
	ipc/los_event.o \
	ipc/los_mux.o \
	ipc/los_queue.o \
	ipc/los_sem.o

kernel-base-mem-y = \
	mem/los_membox.o \
	mem/los_memory.o \
	mem/los_memstat.o \
	mem/los_multipledlinkhead.o

kernel-base-misc-y = \
	misc/los_misc.o

kernel-base-y += \
	$(kernel-base-core-y) \
	$(kernel-base-ipc-y) \
	$(kernel-base-mem-y) \
	$(kernel-base-misc-y)
