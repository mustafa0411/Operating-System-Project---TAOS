#include <mpx/io.h>
#include <user_commands.h>
#include <stdlib.h>
#include <sys_req.h>
#include <string.h>
#include <pcb.h>
#include <queue.h>
#include <style.h>
#include <pcb_user_commands.h>

int create_pcb(const char *name, int processor_class, int priority) {
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return -1;
    }

    if (pcb_find(name)) {
        printf("%sA process with the name '%s' already exists.\n",RED, name);
        return -1;
    }

    if(processor_class < 0 || (processor_class != 0 && processor_class != 1)) {
        printf("%sInvalid processor class\n", RED);
        return -1;
    }

    if (priority < 0 || priority > 9) {
        printf("%sInvalid priority.\n", RED);
        return -1;
    }
    pcb *new_pcb = pcb_setup(name, processor_class, priority);
    if(!new_pcb) return -1;

    pcb_insert(new_pcb);
    printf("PCB '%s' created\n", name);

    return 0;
}

int delete_pcb(const char *name) {
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return -1;
    }

    // Find the PCB to delete
    pcb *p = pcb_find(name);

    // Validation
    if(!p) {
        printf("%sProcess does not exist\n", RED);
        return -1;
    }

    // check for system process
    if(p->processor_class == 0) {
        printf("%sCannot delete a system process\n", RED);
        return -1;
    }
    // Remove PCB from its queue
    if (pcb_remove(p) != 0) {
        printf("%sCould not delete pcb.\n", RED);
        return -1;
    }

    // Free all associated memory
    pcb_free(p);

    printf("PCB '%s' deleted\n", name);
    return 0;
}

int block_pcb(const char* name) {
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return -1;
    }
    pcb *p = pcb_find(name);
    if(!p) {
        printf("%sProcess does not exist\n", RED);
        return -1;
    }

    if (p->state == 2) {
        printf("%sPCB '%s' is already blocked.\n", RED, name);
        return -1;
    }

    // Remove PCB from its current queue
    if (pcb_remove(p) != 0) {
        printf("%sCould not delete pcb.\n", RED);
        return -1;
    }

    p->state = 2; // set state to blocked

    // Insert PCB into the blocked queue
    pcb_insert(p);

    printf("PCB '%s' blocked\n", name);
    return 0;
}

int unblock_pcb(const char* name) {
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name", RED);
        return -1;
    }
    pcb *p = pcb_find(name);
    if(!p) {
        printf("%sProcess does not exist\n", RED);
        return -1;
    }
    
    if (p->state == 1) {
        printf("%sPCB '%s' is already unblocked.\n", RED, name);
        return -1;
    }

    // Remove PCB from its current queue
    if (pcb_remove(p) != 0) {
        printf("%sCould not delete pcb.\n", RED);
        return -1;
    }

    p->state = 1; // set state to ready

    // Insert PCB into the ready queue
    pcb_insert(p);

    printf("PCB '%s' unblocked\n", name);
    return 0;
}

void show_pcb(const char *name) {
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return;
    }

    // find the pcb to show
    pcb *p = pcb_find(name);
    if(!p) {
        printf("%sProcess '%s' does not exist\n",RED, name);
        return;
    }
    // print the PCB information: name, class, State, suspended, priority
    printf("PCB Information:\n");
    printf("%11s %s\n", "Name:", p->name);
    printf("%11s %s\n", "Class:", p->processor_class == 0 ? "System" : "User");
    // ternary operator to identify state and print
    printf("%11s %s\n", "State:",
        p->state == 1 ? "Ready" :
        p->state == 2 ? "Blocked" :
        p->state == 0 ? "New" : "Unknown");
    printf("%11s %s\n", "Suspended:", p->suspended ? "Yes" : "No");
    printf("%11s %d\n", "Priority:", p->priority);
}

int set_pcb_priority(const char* name, int new_priority) {
    // Validate parameters
    if (!name) {
        printf("%sInvalid name\n", RED);
        return -1; //Invalid name
    }

    if (new_priority < 0 || new_priority > 9) {
        printf("%sInvalid priority\n", RED);
        return -2; //Invalid priority
    }

    //Find the PCB to update priority with name
    pcb* process = pcb_find(name);
    if (!process) {
        printf("%sProcess does not exist\n", RED);
        return -3; //Process not found
    }

    //No change needed if same priority
    if (process->priority == new_priority) {
        printf("%sProcess is already set to current priority.\n", RED);
        return 0;
    }

    //Remove from current queue fail
    if (pcb_remove(process) != 0) {
        printf("%sProcess could not be removed from its current queue\n", RED);
        return -4; 
    }

    //Update priority
    process->priority = new_priority;

    //Reinsert into appropriate queue with new priority
    pcb_insert(process);

    printf("PCB '%s' set to priority '%d'\n", name, new_priority);
    return 0;
}

int suspend_pcb(const char* name) {
    // Validate name parameter 
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return -1;
    }

    //Find PCB
    pcb* process = pcb_find(name);
    if (!process) {
        printf("%sProcess does not exist\n", RED);
        return -2;
    }

    if (process->processor_class == 0) {
        printf("%sCannot suspend a system process\n", RED);
        return -3;
    }

    //Check if already suspended 
    if (process->suspended) {
        printf("%sPCB '%s' is already suspended.\n", RED, name);
        return 0; //Already suspended
    }

    // Remove from current queue
    if (pcb_remove(process) != 0) {
        printf("%sCould not remove pcb from its current queue\n", RED);
        return -4; //Couldnt remove from queue
    }

    //Set suspended flag
    process->suspended = 1;

    // Insert into suspended queue
    pcb_insert(process);

    printf("PCB '%s' suspended\n", name);
    return 0;
}

int resume_pcb(const char* name) {
    // Validate name parameter 
    if (!name || strlen(name) == 0) {
        printf("%sInvalid name\n", RED);
        return -1;
    }

    //Find PCB
    pcb* process = pcb_find(name);
    if (!process) {
        printf("%sProcess does not exist\n", RED);
        return -2;
    }

    // Check if already not suspended 
    if (!process->suspended) {
        printf("%sPCB '%s' is already not suspended.\n", RED, name);
        return 0; //Already not suspended
        
    }

    // Remove from current queue
    if (pcb_remove(process) != 0) {
        printf("%sCould not remove pcb from its current queue\n", RED);
        return -4; //Couldnt remove from queue
    }
    
    //Clear suspended flag
    process->suspended = 0;

    // Insert into appropriate queue based on state
    pcb_insert(process);

    printf("PCB '%s' resumed\n", name);
    return 0;
}

void show_ready_pcb(void) {
    extern queue ready_queue;
    
    printf("Ready Processes:\n");
    
    if (ready_queue.count == 0) {
        printf("No processes in Ready state.\n");
        return;
    }
    
    pcb* current = ready_queue.head;
    while (current != NULL) {
        printf("Name: %s, Class: %s, State: %s, Suspended: %s, Priority: %d\n",
            current->name,
            current->processor_class == 0 ? "System" : "User",
            current->state == 1 ? "Ready" :
            current->state == 2 ? "Blocked" :
            current->state == 0 ? "New" : "Unknown",
            current->suspended ? "Yes" : "No",
            current->priority);
        current = current->next;
    }
}

void show_blocked_pcb(void) {
    extern queue blocked_queue;
    
    printf("Blocked Processes:\n");
    
    if (blocked_queue.count == 0) {
        printf("No processes in Blocked state.\n");
        return;
    }
    
    pcb* current = blocked_queue.head;
    while (current != NULL) {
        printf("Name: %s, Class: %s, State: %s, Suspended: %s, Priority: %d\n",
            current->name,
            current->processor_class == 0 ? "System" : "User",
            current->state == 1 ? "Ready" :
            current->state == 2 ? "Blocked" :
            current->state == 0 ? "New" : "Unknown",
            current->suspended ? "Yes" : "No",
            current->priority);
        current = current->next;
    }
}

void show_all_pcb(void) {
    extern queue ready_queue;
    extern queue blocked_queue;
    extern queue suspended_queue;
    
    printf("All Processes:\n");
    
    int total_processes = ready_queue.count + blocked_queue.count + suspended_queue.count;
    
    if (total_processes == 0) {
        printf("No processes exist.\n");
        return;
    }
    
    // Show Ready processes
    pcb* current = ready_queue.head;
    while (current != NULL) {
        printf("Name: %s, Class: %s, State: %s, Suspended: %s, Priority: %d\n",
            current->name,
            current->processor_class == 0 ? "System" : "User",
            current->state == 1 ? "Ready" :
            current->state == 2 ? "Blocked" :
            current->state == 0 ? "New" : "Unknown",
            current->suspended ? "Yes" : "No",
            current->priority);
        current = current->next;
    }
    
    // Show Blocked processes
    if (blocked_queue.count > 0) {
        pcb* current = blocked_queue.head;
        while (current != NULL) {
            printf("Name: %s, Class: %s, State: %s, Suspended: %s, Priority: %d\n",
                current->name,
                current->processor_class == 0 ? "System" : "User",
                current->state == 1 ? "Ready" :
                current->state == 2 ? "Blocked" :
                current->state == 0 ? "New" : "Unknown",
                current->suspended ? "Yes" : "No",
                current->priority);
            current = current->next;
        }
    }
    
    // Show Suspended processes
    if (suspended_queue.count > 0) {
        pcb* current = suspended_queue.head;
        while (current != NULL) {
            printf("Name: %s, Class: %s, State: %s, Suspended: %s, Priority: %d\n",
                current->name,
                current->processor_class == 0 ? "System" : "User",
                current->state == 1 ? "Ready" :
                current->state == 2 ? "Blocked" :
                current->state == 0 ? "New" : "Unknown",
                current->suspended ? "Yes" : "No",
                current->priority);
            current = current->next;
        }
    }
}

