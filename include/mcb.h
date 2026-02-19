#ifndef MCB_H
#define MCB_H

#include <stddef.h>

// Memory Control Block structure
typedef struct mcb {
    void *start_address;   // Base address of usable memory (first byte after MCB)
    size_t size;          // Size of the block in bytes (not including the MCB)
    struct mcb *next;     // Pointer to next MCB in the list
    struct mcb *prev;     // Pointer to previous MCB in the list
    int is_free;          // Flag: 1 if free, 0 if allocated
} mcb;

// Global pointers to the free and allocated lists
extern mcb *free_list;
extern mcb *allocated_list;
extern mcb *mem_head;

/**
 * @brief Initializes the heap memory manager
 * 
 * Allocates all memory available to the memory manager as a single, large
 * free block using kmalloc(). Creates an MCB for this block and places it
 * on the free list. Initializes the allocated list to be empty.
 * 
 * @param heap_size The total size of the heap in bytes (does not include 
 *                  the size of the initial MCB)
 * @return None
 */
void initialize_heap(size_t heap_size);

/**
 * @brief Allocates memory from the heap
 * 
 * Allocates memory from the heap using a first-fit algorithm. Splits a free
 * memory block in two if necessary, initializing and/or updating the 
 * corresponding MCBs. Places the allocated block on the allocated list.
 * 
 * @param size The size, in bytes, of the requested allocation
 * @return A pointer to the start address of the newly allocated block 
 *         (not the MCB address), or NULL on error
 */
void *allocate_memory(size_t size);

/**
 * @brief Frees allocated memory
 * 
 * Frees allocated memory, placing the associated block on the free list.
 * If the freed block is adjacent to any other free blocks, they are merged
 * into a single free block to prevent fragmentation.
 * 
 * @param ptr A pointer to the start address (not the MCB address) of an 
 *            allocated block
 * @return 0 on success, non-zero on error
 */
int free_memory(void *ptr);

#endif
