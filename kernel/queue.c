#include "pcb.h"
#include <memory.h>
#include <stdlib.h>
#include <stddef.h>
#include <queue.h>
#include "pcb.h"
#include <memory.h>
#include <stddef.h>
#include <queue.h>

/**
 * @brief Create a queue of items
 * 
 * @return queue* the created queue
 */
queue* create_queue(void)
{
    queue* q = (queue*)sys_alloc_mem(sizeof(queue));
    if(q == NULL){
        printf("Could not allocate memory for queue\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
    return q;
}

/**
 * @brief add item into the queue
 * 
 * @param q the queue to add the item in
 * @param p pointer to the object in the queue
 */
void enqueue(queue* q, pcb* p)
{
    if(q == NULL || p == NULL){
        printf("Queue does not exist\n");
        return;
    }
    
    p->next = NULL;

    // Insert at tail
    if(q->tail == NULL){
        q->head = p;
        q->tail = p;
    } else {
        q->tail->next = p;
        q->tail = p;
    }
    q->count++;
}

/**
 * @brief remove item from queue
 * 
 * @param q queue for the item to remove
 * @return pcb*  pointer to the item you remove
 */
pcb* dequeue(queue* q)
{
    if(q == NULL){
        printf("Queue does not exist\n");
        return NULL;
    }
    if(is_empty(q))
    {
        printf("Queue is empty\n");
        return NULL;
    }

    pcb* p = q->head;
    if (p) {
        q->head = p->next;
        if (q->head == NULL) {
            q->tail = NULL;
        }
        q->count--;
        p->next = NULL;
    }
    return p;
}

/**
 * @brief Check if the queue is empty
 * 
 * @param q the queue to check
 * @return int 1 if empty, 0 if not, -1 if queue is NULL
 */
int is_empty(queue* q)
{
    if(q == NULL){
        printf("Queue does not exist\n");
        return -1;
    }
    return q->head == NULL;
}
