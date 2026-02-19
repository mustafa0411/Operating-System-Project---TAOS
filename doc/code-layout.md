# MPX Code Layout

This file describes the contents of the repository as it is provided to you at
the beginning of the semester, with some guidance on how to direct your changes
as the semester goes on.

## Headers

Headers for C code are located in the `include` directory.

### Standard Headers

At the top level of the `include` directory are headers containing declarations
for the handful of provided functions based on the C standard library. These
are:

- `ctype.h` -- Character operations
- `stdlib.h` -- The `atoi()` function
- `string.h` -- Various memory and string functions

You may want to add declarations to these files if you implement additional
functions from the standard library. You should *not* remove or modify any
of the existing contents of these files.

### MPX Headers

MPX-specific headers include those that should only be directly referenced by
kernel code, under the `include/mpx` directory, and those that may also be
needed by user-space code. There is no reason to modify most of these headers.
You will likely want to add new headers to the `include` (for user-space) and
`include/mpx` directories as you develop your MPX. The provided files are:

- `processes.h` -- Declarations for functions that will be loaded as processes
- `sys_req.h` -- The interface between user-space and kernel-space
- `mpx/device.h` -- Constants such as `COM1`
- `mpx/gdt.h` -- Functions for the Global Descriptor Table
- `mpx/interrupts.h` -- Functions for dealing with interrupts
- `mpx/io.h` -- Macros for performing hardware I/O
- `mpx/panic.h` -- The kernel panic function
- `mpx/vm.h` -- Virtual memory functions

Two headers you may want to add to (but not remove or modify existing contents)
are:

- `memory.h` -- The MPX-specific heap manager
- `mpx/serial.h` -- Functions interacting with serial ports

## Source Files

Source files are located in the `kernel`, `lib`, and `user` directories,
according to their use.

### Kernel Files

Files in the `kernel` directory are part of the kernel. They execute in
kernel-mode, interact directly with hardware, and manage system resources.
Provided files are:

- `core-asm.s` -- Required assembly routines to load the system. Should not be
modified.
- `core-c.c -- Required C functions to initialize the system. Should not be
modified.
- `kmain.c` -- Contains the `kmain()` function that controls overall system
functionality. You will need to modify this function in several modules.
- `serial.c` -- Contains functions for interacting with serial ports. You will
need to modify this file in modules R1 and R6.
- `sys_call_isr.s` -- Contains the Interrupt Service Routine linking
kernel-space and user-space. You will need to modify this in module R3.

### Library Files

Files in the `lib` directory contain functionality that can be used by both
kernel-space and user-space code. Provided files are:

- `core.c` -- Functions required to be present by the compiler along with the
basic MPX heap manager. Should not be modified.
- `ctype.c` -- Functions declared in `<ctype.h>`.
- `stdlib.c` -- Functions declared in `<stdlib.h>`.
- `string.c` -- Functions declared in `<string.h>`.

### User Files

Files in the `user` directory form the MPX user-space. These are the Command
Handler (which you must design and implement) along with its various commands,
along with several processes used in testing your module implementations
and the `sys_req()` function, which links user-space to kernel-space. The
only provided file is:

- `core.c` -- Contains `sys_req()` and functions for processes needed in R3 and
R4. Should not be modified.

## Build Files

MPX uses the POSIX standard `make` utility to automate compilation and linking.
While in the top level directory of your repository, you can build MPX by
running:

    make

You can remove build artifacts and force the next build to recompile all source
files by running:

    make clean

The build process is controlled by several files. Several of these should not be changed:

- `Makefile` -- The primary configuration file.
- `make/CFLAGS` -- Options passed to the compiler to ensure proper creation of
a standalone 32-bit x86 kernel.
- `make/LDFLAGS` -- Options passed to the linker to ensure proper creation of
a standalone 32-bit x86 kernel.
- `make/link.ld` -- A script providing additional information beyond what can
be included in `make/LDFLAGS` directly necessary for proper creation of a
standalone 32-bit x86 kernel.
- `make/deps.sh` -- Generates the lists of object files and dependencies (below).

Other files *will* require updating as you add and modify your own source files:

- `make/kernel.mk` -- Kernel objects and their dependencies.
- `make/lib.mk` -- Library objects and their dependencies.
- `make/user.mk` -- User objects and their dependencies.

To update these files, run:

    make deps

## Other Files

The remaining files are not source code or used directly in the build process.
They are:

- `doc/` -- (Directory) Contains documentation. You will add your own
documentation to this directory.
- `.github/workflows/makefile.yml` - Contains a GitHub action that attempts to
compile your code whenever you push it to GitHub. If it doesn't compile on
GitHub, it is not likely to compile in the test environment (barring
compilation failures due to network issues). This file should not be modified.
- `.gitignore` -- Instructs Git to ignore object files and your compiled
kernel when considering what has changed in your repository. You may want to
add lines to this file if your team's workflow involves additional artifacts
that should not be in the repository.
- `mpx.sh` -- A POSIX shell script that executes MPX in a QEMU virtual
machine with appropriate options for serial I/O and attaching a debugger,
and resetting the terminal afterward. Should not be changed.
