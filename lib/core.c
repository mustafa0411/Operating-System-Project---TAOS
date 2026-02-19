/***********************************************************************
* Core library functions needed by both kernel- and user-space.
*
* You should not need to make any modifications to this file. Doing so
* may result in losing points.
************************************************************************/

#include <mpx/serial.h>
#include <mpx/vm.h>

#include <memory.h>
#include <string.h>

/* For R5: Pointers to student provided functions */
/* DO NOT SET MANUALLY, CALL sys_set_heap_functions() !!! */
static void * (*malloc_function)(size_t) = NULL;
static int (*free_function)(void *) = NULL;

/* Standard memcpy() - required because compiler may insert calls to it */
void *memcpy(void * restrict s1, const void * restrict s2, size_t n)
{
	unsigned char *dst = s1;
	const unsigned char *src = s2;
	for (size_t i = 0; i < n; i++) {
		dst[i] = src[i];
	}
	return s1;
}

/* Standard memset() - required because compiler may insert calls to it */
void *memset(void *s, int c, size_t n)
{
	unsigned char *p = s;
	for (size_t i = 0; i < n; i++) {
		p[i] = (unsigned char)c;
	}
	return s;
}

/***********************************************************************/
/* This causes R5 to go into full effect, replacing default heap functions
 * with those implemented by students. */
/***********************************************************************/
void sys_set_heap_functions(void * (*alloc_fn)(size_t), int (*free_fn)(void *))
{
	malloc_function = alloc_fn;
	free_function = free_fn;
}

/* Allocate memory using the student function if available, fallback to kmalloc(). */
void *sys_alloc_mem(size_t size)
{
	return malloc_function ? malloc_function(size) : kmalloc(size, 0, NULL);
}

/* Free memory if a student function is available, otherwise NOP. */
int sys_free_mem(void *ptr)
{
	return free_function ? free_function(ptr) : -1;
}
