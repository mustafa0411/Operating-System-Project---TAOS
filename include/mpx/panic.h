#ifndef MPX_PANIC_H
#define MPX_PANIC_H

#include <stdnoreturn.h>

/**
@file mpx/panic.h
@brief Common system functions and definitions
*/

/**
 Kernel panic. Prints an error message and halts.
 @param msg A message to display before halting
*/
/*
 non-standard attribute is required for clang < 15
 */
noreturn __attribute__((no_caller_saved_registers)) void kpanic(const char *msg);

#endif
