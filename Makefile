.POSIX:

########################################################################
# Makefile for MPX - there should be no need to modify this file
########################################################################

all: kernel.bin

include make/kernel.mk
include make/lib.mk
include make/user.mk

AS	= nasm
ASFLAGS = -f elf -g

CC	= clang
CFLAGS  = @make/CFLAGS

ifeq ($(shell uname), Darwin)
LD	= i686-elf-ld
else
LD      = i686-linux-gnu-ld
endif
LDFLAGS = @make/LDFLAGS

OBJFILES = $(KERNEL_OBJECTS) $(LIB_OBJECTS) $(USER_OBJECTS)

kernel.bin: $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $(OBJFILES)

deps:
	sh make/deps.sh

clean:
	rm -f $(OBJFILES) kernel.bin
