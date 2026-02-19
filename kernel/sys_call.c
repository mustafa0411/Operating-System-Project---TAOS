#include <sys_call.h>
#include <stddef.h>
#include <pcb.h>
#include <sys_req.h>
#include <queue.h>
#include <memory.h>
#include <string.h>
#include <commhand.h>

static context* saved_context = NULL;
static pcb* current_pcb = NULL;

context* sys_call(context* current_process) {   
    uint32_t op_code = current_process->eax;

    // Only handle IDLE and EXIT
    if (op_code != IDLE && op_code != EXIT) {
        current_process->eax = -1;
        return current_process;
    }

    if (op_code == IDLE) {
        // Save the kernel context ONLY if we're currently in kernel (current_pcb == NULL)
        if (current_pcb == NULL && saved_context == NULL) {
            saved_context = current_process;
        }

        // Get next ready, non-suspended PCB (dequeues)
        pcb* next_pcb = get_ready_next();

        // Save current PCB context and re-queue it
        if (current_pcb) {
            current_pcb->stack = (void*)current_process;
            pcb_insert(current_pcb);
        }

        // If no ready process, return to original kernel context
        if (!next_pcb) {
            current_pcb = NULL;
            return saved_context;
        }

        // Dispatch the selected PCB
        current_pcb = next_pcb;
        context* next_process = (context*)next_pcb->stack;
        next_process->eax = 0;
        return next_process;

    } else if (op_code == EXIT) {
        // Delete the current PCB
        if (current_pcb) {
            pcb_free(current_pcb);
            current_pcb = NULL;
        }
        
        // Get next ready, non-suspended PCB
        pcb* next_pcb = get_ready_next();

        // If no ready process, return to original context and reset
        if (!next_pcb) {
            context* temp = saved_context;
            saved_context = NULL;   // Reset for next yield
            current_pcb = NULL;
            return temp;
        }

        // Dispatch the selected PCB
        current_pcb = next_pcb;
        context* next_process = (context*)next_pcb->stack;
        next_process->eax = 0;
        return next_process;
    }
    
    return current_process;
}
