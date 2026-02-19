#ifndef MPX_INTERRUPTS_H
#define MPX_INTERRUPTS_H

/**
 @file mpx/interrupts.h
 @brief Kernel functions related to software and hardware interrupts
*/

/** Disable interrupts */
#define cli() __asm__ volatile ("cli")

/** Enable interrupts */
#define sti() __asm__ volatile ("sti")

/**
 Installs the initial interrupt handlers for the first 32 IRQ lines. Most do a
 panic for now.
*/
void irq_init(void);

/**
 Initializes the programmable interrupt controllers and performs the necessary
 remapping of IRQs. Leaves interrupts turned off.
*/
void pic_init(void);

/** Creates and installs the Interrupt Descriptor Table. */
void idt_init(void);

/** Installs an interrupt handler */
void idt_install(int vector, void (*handler)(void *));

#endif
