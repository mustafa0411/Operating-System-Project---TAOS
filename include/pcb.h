#ifndef PCB_H
#define PCB_H
#include <stddef.h>
#include <stdlib.h>

#define PCB_STATE_NEW 0
#define DEFAULT_STACK_SIZE 1024  // 4 KB, adjust as needed
#define MAX_NAME_LENGTH 32 // Maximum length for process name
#define QID_SUSPENDED 0
#define QID_READY 1
#define QID_BLOCKED 2

/**
 * @brief pcb struct for all of the information that a PCB requires
 * 
 */
typedef struct pcb {
    char name[32];
    int state; // 1 = Ready, 2 = Blocked
    int suspended; // 0 = Not suspended, 1 = Suspended
    int priority; // 0 highest priority, 9 lowest priority
    int processor_class; // 1 = User, 0 = System
    void* stack;
    void* stack_base;
    struct pcb *next;
    size_t stack_size;
} pcb;

/**
 * @brief simple queue structure
 * 
 */
typedef struct queue{
    pcb *head;
    pcb *tail;
    int count;
} queue;

// PCB Queues
extern queue ready_queue;
extern queue blocked_queue;
extern queue suspended_queue;

/**
 * @brief Allocate memory for a new PCB, including the stack, 
 * and perform reasonable initialization
 * 
 * @return pcb* the allocated PCB
 */
pcb* pcb_allocate(void);

/**
 * @brief free the pcb using sys_free_mem for cleanup
 * 
 * @param p pointer for process
 * @return pcb* 
 */
int pcb_free(pcb* p);

/**
 * @brief Allocates a new PCB, initializes it with data, and sets the state to 
 * Ready, Not-Suspended.
 * 
 * @param name name of the process
 * @param processor_class process processor_class
 * @param priority priority of the process
 * @return pcb* 
 */
pcb* pcb_setup(const char *name, int processor_class, int priority);

/**
 * @brief Searches all process queues for a process with the provided name
 * 
 * @param name process name
 * @return pcb* pointer to the found pcb
 */
pcb* pcb_find(const char* name);

/**
 * @brief Inserts a PCB into the appropriate queue based on state and priority
 * 
 * For Ready state processes, insertion is priority-based (0 highest, 9 lowest)
 * For other states, insertion is at the tail of the respective queue
 * 
 * @param p Pointer to the PCB to insert
 */
void pcb_insert(pcb* p);

/**
 * @brief Removes a PCB from its current queue without freeing memory
 * 
 * @param p Pointer to the PCB to remove
 * @return int Success (0) or error code (-1 for invalid input, -2 for PCB not found)
 */
int pcb_remove(pcb* p);

/**
 * @brief Get the next ready pcb from the ready queue
 * 
 * @return pcb* pcb to dequeue
 */
void add_to_ready_queue(pcb* p);

void add_to_suspended_queue(pcb* p);

void add_to_blocked_queue(pcb* p);

int is_queue_empty(int qid);

/**
 * @brief 
 * 
 * @param p pointer to the pcb to add to the ready queue
 */
pcb* get_ready_next(void);

pcb* get_suspended_next(void);

/**
 * @brief Peek at the next ready process without removing it
 * 
 * @return char* name of next process or NULL if queue empty
 */
char* peek_ready_next(void);

#endif
