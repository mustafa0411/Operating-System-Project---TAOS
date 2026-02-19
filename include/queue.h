#ifndef QUEUE_H
#define QUEUE_H
#include "pcb.h"

//Define a simple queue structure for managing PCBs /
typedef struct queue_node {
    pcb *process;
    struct queue_node *next;

}node;

/**
 * @brief Create a queue of items
 * 
 * @return queue* the created queue
 */
queue* create_queue(void);

/**
 * @brief add item into the queue
 * 
 * @param q the queue to add the item in
 * @param p pointer to the object in the queue
 */
void enqueue(queue* q, pcb* p);
/**
 * @brief remove item from queue
 * 
 * @param q queue for the item to remove
 * @return pcb*  pointer to the item you remove
 */
pcb* dequeue(queue* q);

/**
 * @brief Check if the queue is empty
 * 
 * @param q the queue to check
 * @return int 1 if empty, 0 if not, -1 if queue is NULL
 */
int is_empty(queue* q);

#endif // QUEUE_H
