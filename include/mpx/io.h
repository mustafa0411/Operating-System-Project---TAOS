#ifndef MPX_IO_H
#define MPX_IO_H

/**
 @file mpx/io.h
 @brief Kernel macros to read and write I/O ports
*/

/**
 Write one byte to an I/O port
 @param port The port to write to
 @param data The byte to write to the port
*/
#define outb(port, data)						\
	__asm__ volatile ("outb %%al, %%dx" :: "a" (data), "d" (port))

/**
 Read one byte from an I/O port
 @param port The port to read from
 @return A byte of data read from the port
*/
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#define inb(port) ({							\
      unsigned char r;							\
      __asm__ volatile ("inb %%dx, %%al" : "=a" (r) : "d" (port));	\
      r;								\
    })

#endif
