#include <pcb.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>
#include <queue.h>
queue ready_queue = {NULL, NULL, 0};
queue blocked_queue = {NULL, NULL, 0};
queue suspended_queue = {NULL, NULL, 0};

pcb* pcb_allocate(void) {
    pcb* new_pcb = (pcb*)sys_alloc_mem(sizeof(pcb));
    if (!new_pcb) return NULL;

    new_pcb->state = PCB_STATE_NEW; //Default
    new_pcb->priority = 0; //Default
    new_pcb->stack_size = DEFAULT_STACK_SIZE; //Default
    new_pcb->stack_base = sys_alloc_mem(new_pcb->stack_size);
    if (!new_pcb->stack_base) {
        //Free PCB if stack allocation fails
        sys_free_mem(new_pcb);
        return NULL;
    }
    new_pcb->stack = new_pcb->stack_base;

    return new_pcb;
}

int pcb_free(pcb* p) {
    if (!p) return -1;
    

    if(p->stack) {
        sys_free_mem(p->stack_base);
        p->stack_base = NULL;
        p->stack = NULL;
    }

    sys_free_mem(p);

    return 0;
}

pcb* pcb_setup(const char *name, int processor_class, int priority) {
    //Validate params
    if (!name || processor_class < 0 || priority < 0 || priority > 9 || (processor_class != 0 && processor_class != 1)) {
        return NULL; //Invalid params
    }

    //Allocate new PCB
    pcb* new_pcb = pcb_allocate();
    if (!new_pcb) {
        return NULL; //Allocation failed
    }

    //Initialize PCB fields
    new_pcb->state = 1; // Ready state
    new_pcb->priority = priority;
    new_pcb->processor_class = processor_class; //Set process processor_class (User or System)

    //Copy process name
    strncpy(new_pcb->name, name, MAX_NAME_LENGTH - 1);
    new_pcb->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    //Set suspension status (if applicable)
    new_pcb->suspended = 0; // Not suspended

    return new_pcb;
}

/**
 * @brief Helper function for pcb_find to search through different queues.
 * 
 * @param q queue to look through
 * @param name procesor name
 * @return pcb* 
 */
static pcb* search_queue(queue *q, const char *name) {
    if (!q || !name) return NULL;

    pcb *curr = q->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

pcb* pcb_find(const char* name) {
    if (!name) return NULL;

    pcb *found = NULL;

    // look for pcb in ready_queue
    found = search_queue(&ready_queue, name);
    if (found) return found;

    // look for pcb in blocked queue
    found = search_queue(&blocked_queue, name);
    if (found) return found;

    // look for pcb in suspended queue
    found = search_queue(&suspended_queue, name);
    if (found) return found;

    // Not found in any queue
    return NULL;
}


void pcb_insert(pcb* p) {
    if (!p) return;

    queue* target_queue = NULL;

    // Determine target queue based on state
    if (p->suspended) { // Ready state
        target_queue = &suspended_queue;
    } else if (p->state == 1) { // Blocked state
        target_queue = &ready_queue;
    } else if (p->state == 2) { // Suspended state
        target_queue = &blocked_queue;
    } else {
        return; // Invalid state
    }

    // If queue is empty, insert as first element
    if (!target_queue->head) {
        target_queue->head = p;
        target_queue->tail = p;
        p->next = NULL;
        target_queue->count++;
        return;
    }

    // For Ready state, insert based on priority
    if (p->state == 1) {
        pcb* curr = target_queue->head;
        pcb* prev = NULL;

        // Find insertion point based on priority (0 highest, 9 lowest)
        while (curr && curr->priority <= p->priority) {
            prev = curr;
            curr = curr->next;
        }

        // Insert at head
        if (!prev) {
            p->next = target_queue->head;
            target_queue->head = p;
        }
        // Insert at tail using enqueue
        else if (!curr) {
            enqueue(target_queue, p);
            return;
        }
        // Insert in middle
        else {
            prev->next = p;
            p->next = curr;
        }
        target_queue->count++;
    }
    // Blocked or Suspedned: just enqueue to tail
    else {
        enqueue(target_queue, p);
    }
}

/**
 * @brief Removes a PCB from its current queue without freeing memory
 * 
 * @param p Pointer to the PCB to remove
 * @return int Success (0) or error code (-1 for invalid input, -2 for PCB not found)
 */
int pcb_remove(pcb* p) {
    if (!p) return -1;  // Invalid input

    queue* queues[] = {&ready_queue, &blocked_queue, &suspended_queue};
    
    for (int i = 0; i < 3; i++) {
        queue* current_queue = queues[i];
        if (is_empty(current_queue)) continue;  // Skip empty queue

        pcb* curr = current_queue->head;
        pcb* prev = NULL;

        while (curr) {
            if (curr == p) {
                // Remove from head
                if (!prev) {
                    current_queue->head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                // Remove from tail
                if (curr == current_queue->tail) {
                    current_queue->tail = prev;
                }
                current_queue->count--;
                curr->next = NULL;
                return 0;  // Success
            }
            prev = curr;
            curr = curr->next;
        }
    }

    return -2;  // PCB not found in any queue
}

pcb* get_ready_next(void) {
    return dequeue(&ready_queue);
}

pcb* get_suspended_next(void) {
    return dequeue(&suspended_queue);
}

void add_to_ready_queue(pcb* p) {
    if (!p) return;
    enqueue(&ready_queue, p);
}

void add_to_suspended_queue(pcb* p) {
    if (!p) return;
    enqueue(&suspended_queue, p);
}

void add_to_blocked_queue(pcb* p) {
    if (!p) return;
    enqueue(&blocked_queue, p);
}

char* peek_ready_next(void) {
    if (is_empty(&ready_queue)) {
        return NULL;
    }
    // returns the name of the first item in the queue
    return ready_queue.head->name; 
}

int is_queue_empty(int qid) {
    switch (qid) {
        case QID_SUSPENDED:
            return is_empty(&suspended_queue);
        case QID_READY:
            return is_empty(&ready_queue);
        case QID_BLOCKED:
            return is_empty(&blocked_queue);
        default:
            printf("QID %d does not exist", qid);
            return -1;
    }
}
