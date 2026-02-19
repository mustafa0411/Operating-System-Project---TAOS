#ifndef MPX_VM_H
#define MPX_VM_H

/**
 @file mpx/vm.h
 @brief Kernel functions for virtual memory and primitive allocation
*/

#include <stddef.h>

/**
 Allocates memory from a primitive heap.
 @param size The size of memory to allocate
 @param align If non-zero, align the allocation to a page boundary
 @param phys_addr If non-NULL, a pointer to a pointer that will
                  hold the physical address of the new memory
 @return The newly allocated memory
 */
void *kmalloc(size_t size, int align, void **phys_addr);

/**
 Initializes the kernel page directory and initial kernel heap area.
 Performs identity mapping of the kernel frames such that the virtual
 addresses are equivalent to the physical addresses.
*/
void vm_init(void);

#endif
