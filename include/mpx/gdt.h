#ifndef MPX_GDT_H
#define MPX_GDT_H

/**
 @file mpx/gdt.h
 @brief Kernel functions to initialize the Global Descriptor Table
*/

/** Creates and installs the Global Descriptor Table. */
void gdt_init(void);

#endif
