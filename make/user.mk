.POSIX:

user/core.o: user/core.c include/string.h include/mpx/serial.h \
  include/mpx/device.h include/processes.h include/sys_req.h

user/user_commands.o: user/user_commands.c

user/help_commands.o: user/help_commands.c

user/pcb_user_commands.o: user/pcb_user_commands.c include/mpx/io.h \
	include/user_commands.h include/stdlib.h include/sys_req.h \
	include/string.h include/pcb.h include/pcb_user_commands.h

user/commhand.o: user/commhand.c include/commhand.h include/sys_req.h \
  include/mpx/device.h include/string.h include/user_commands.h \
  include/pcb_user_commands.h include/pcb.h include/shaw.h

user/dispatching.o: user/dispatching.c include/dispatching.h include/sys_req.h \
  include/processes.h include/sys_call.h include/string.h include/pcb.h \
  include/queue.h

user/shaw.o: user/shaw.c include/shaw.h

user/mcb_user_commands.o: user/mcb_user_commands.c include/mcb_user_commands.h \
	include/mcb.h include/mpx/io.h include/string.h include/stdlib.h


USER_OBJECTS=\
	user/core.o \
	user/user_commands.o \
	user/commhand.o \
	user/pcb_user_commands.o \
	user/help_commands.o \
	user/dispatching.o \
	user/shaw.o \
	user/mcb_user_commands.o
