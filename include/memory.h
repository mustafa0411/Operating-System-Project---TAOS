#ifndef MPX_MEMORY_H
#define MPX_MEMORY_H

#include <stddef.h>

/**
 @file memory.h
 @brief MPX-specific dynamic memory functions.
*/

/**
 Allocate dynamic memory.
 @param size The amount of memory, in bytes, to allocate
 @return NULL on error, otherwise the address of the newly allocated memory
*/
void *sys_alloc_mem(size_t size);

/**
 Free dynamic memory.
 @param ptr The address of dynamically allocated memory to free
 @return 0 on success, non-zero on error
*/
int sys_free_mem(void *ptr);

/**
 Installs user-supplied heap management functions.
 @param alloc_fn A function that dynamically allocates memory
 @param free_fn A function that frees dynamically allocated memory
*/
void sys_set_heap_functions(void * (*alloc_fn)(size_t), int (*free_fn)(void *));

#endif
