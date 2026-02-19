#ifndef SYS_CALL_H
#define SYS_CALL_H

#include<stdint.h>


/**
 * @brief context struct with the required registers for our architecture.
 * 
 */
typedef struct {
    // Segment registers (we push manually, CS is pushed by CPU)
    uint32_t ds, es, fs, gs, ss;

    // General purpose registers
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp;
    
    // Interrupt frame (pushed automatically by CPU)
    uint32_t eip, cs, eflags;

    uint32_t esp;
} context;


/**
 * @brief System call handler for process context switching
 * 
 * Handles IDLE and EXIT operation codes from sys_req(). Manages process
 * dispatching by saving the current process context, selecting the next
 * ready process based on priority, and returning the appropriate context
 * to load.
 * 
 * @param current_process Pointer to the context struct of the currently executing process
 * @return context* Pointer to the context of the process to be loaded
 */
context* sys_call(context* current_process);

#endif
