# Common Errors

Here are some of the most common errors people encounter when starting to work
on MPX, along with their solutions.

## Implicit Declaration

    $ make
    nasm -f elf -g  -o kernel/core-asm.o kernel/core-asm.s
    nasm -f elf -g  -o kernel/sys_call_isr.o kernel/sys_call_isr.s
    clang @make/CFLAGS   -c -o kernel/core-c.o kernel/core-c.c
    clang @make/CFLAGS   -c -o kernel/kmain.o kernel/kmain.c
    kernel/kmain.c:91:2: error: implicit declaration of function 'commhand' is invalid in C99 [-Werror,-Wimplicit-function-declaration]
            commhand();
            ^
    1 error generated.
    make: *** [<builtin>: kernel/kmain.o] Error 1

This is a compiler error (technically a warning, but MPX has instructed the
compiler to treat warnings as errors to force you to deal with them) that you
are attempting to call a function, or refer to a variable, which hasn't been
declared. First, check the spelling of the variable or function name. If you're
sure that's correct, the most likely cause is that the variable or function
is defined in a different source file, and you forgot to either (1) add the
appropriate declaration to the corresponding header file; or (2) `#include`
that header file in the file that is generating the error.

## Undefined Reference

    $ make
    nasm -f elf -g  -o kernel/core-asm.o kernel/core-asm.s
    nasm -f elf -g  -o kernel/sys_call_isr.o kernel/sys_call_isr.s
    clang @make/CFLAGS   -c -o kernel/core-c.o kernel/core-c.c
    clang @make/CFLAGS   -c -o kernel/kmain.o kernel/kmain.c
    clang @make/CFLAGS   -c -o kernel/serial.o kernel/serial.c
    clang @make/CFLAGS   -c -o lib/core.o lib/core.c
    clang @make/CFLAGS   -c -o lib/ctype.o lib/ctype.c
    clang @make/CFLAGS   -c -o lib/stdlib.o lib/stdlib.c
    clang @make/CFLAGS   -c -o lib/string.o lib/string.c
    clang @make/CFLAGS   -c -o user/core.o user/core.c
    i686-linux-gnu-ld @make/LDFLAGS -o kernel.bin kernel/core-asm.o kernel/sys_call_isr.o kernel/core-c.o kernel/kmain.o kernel/serial.o lib/core.o lib/ctype.o lib/stdlib.o lib/string.o user/core.o
    i686-linux-gnu-ld: kernel/kmain.o: in function `kmain':
    /home/jkk00010/cs450/mpx/kernel/kmain.c:92: undefined reference to `commhand'
    make: *** [Makefile:29: kernel.bin] Error 1

This is a linker error that there is a refernce to a function or variable in
at least one object file that is not defined in *any* object file in MPX. The
most likely cause is you have created a new source (.c) file, but didn't add
its object file to the list of objects to be built. See the Build section of
`doc/code-layout.md` for instructions on how to add object files to the build.

## Multiple Definitions

    $ make
    nasm -f elf -g  -o kernel/core-asm.o kernel/core-asm.s
    nasm -f elf -g  -o kernel/sys_call_isr.o kernel/sys_call_isr.s
    clang @make/CFLAGS   -c -o kernel/core-c.o kernel/core-c.c
    clang @make/CFLAGS   -c -o kernel/kmain.o kernel/kmain.c
    clang @make/CFLAGS   -c -o kernel/serial.o kernel/serial.c
    clang @make/CFLAGS   -c -o lib/core.o lib/core.c
    clang @make/CFLAGS   -c -o lib/ctype.o lib/ctype.c
    clang @make/CFLAGS   -c -o lib/stdlib.o lib/stdlib.c
    clang @make/CFLAGS   -c -o lib/string.o lib/string.c
    clang @make/CFLAGS   -c -o user/core.o user/core.c
    i686-linux-gnu-ld @make/LDFLAGS -o kernel.bin kernel/core-asm.o kernel/sys_call_isr.o kernel/core-c.o kernel/kmain.o kernel/serial.o lib/core.o lib/ctype.o lib/stdlib.o lib/string.o user/core.o
    i686-linux-gnu-ld: kernel/kmain.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: multiple definition of `a_global_variable'; kernel/core-c.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: first defined here
    i686-linux-gnu-ld: kernel/serial.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: multiple definition of `a_global_variable'; kernel/core-c.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: first defined here
    i686-linux-gnu-ld: user/core.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: multiple definition of `a_global_variable'; kernel/core-c.o:/home/jkk00010/cs450/mpx/include/sys_req.h:31: first defined here
    make: *** [Makefile:29: kernel.bin] Error 1

This is a linker error telling you that you have two (or more) functions and/or
variables with the same name at global scope in different object files. The
most likely source of this error is an improper definition of a global
variable. There is an important distinction between a variable *declaration*
and a variable *definition* in C programs, especially at global scope. When you
create a variable, like:

    int some_variable = 0;

This is a *definition*. The name is reserved, as well as space in the object
file to store its value. Contrast:

    extern int some_variable;

This is a *declaration*. The `extern` keyword instructs the compiler to reserve
the name, but not any space in the object file - that space will be allocated
elsewhere.

When placed in a header file, a *definition* attempts to reserve both the
variable name *and* its space in every object file, resulting in the multiple
definition error. In contrast, a *declaration* in a header file just allows you
to access that variable, and can be `#include`d in as many files as you want.

What you want to do for global variables is put the *declaration* (i.e. using
the `extern` keyword, without an initial value) in your header file, then place
the matching definition (without `extern`, and as good programming practice,
with an explicit initial value) in exactly one source file.
