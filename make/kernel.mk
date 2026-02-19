.POSIX:

kernel/kmain.o: kernel/kmain.c include/mpx/gdt.h include/mpx/interrupts.h \
  include/mpx/serial.h include/mpx/device.h include/mpx/vm.h \
  include/sys_req.h include/string.h include/memory.h include/commhand.h \
  include/queue.h include/pcb.h

kernel/serial.o: kernel/serial.c include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h

kernel/pcb.o: kernel/pcb.c include/pcb.h include/memory.h \
  include/string.h include/queue.h

kernel/queue.o: kernel/queue.c include/queue.h include/pcb.h

kernel/core-c.o: kernel/core-c.c include/mpx/gdt.h include/mpx/panic.h \
  include/mpx/interrupts.h include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h include/string.h \
  include/mpx/vm.h

kernel/rtc.o: kernel/rtc.c include/mpx/io.h include/stdlib.h

kernel/com_hist.o: kernel/com_hist.c 

kernel/sys_call.o: kernel/sys_call.c include/sys_call.h

kernel/mcb.o: kernel/mcb.c include/mcb.h include/mpx/panic.h \
  include/mpx/vm.h include/string.h include/stdlib.h \
  

KERNEL_OBJECTS=\
	kernel/sys_call_isr.o\
	kernel/core-asm.o\
	kernel/kmain.o\
	kernel/serial.o\
	kernel/core-c.o\
  kernel/rtc.o\
  kernel/pcb.o \
  kernel/queue.o\
  kernel/com_hist.o\
  kernel/sys_call.o\
  kernel/mcb.o
