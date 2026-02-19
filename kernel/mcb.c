#include <mcb.h>
#include <mpx/panic.h>
#include <mpx/vm.h>
#include <string.h>
#include <stdlib.h>

mcb *mem_head = NULL;
static mcb *mem_tail = NULL;
static size_t total_heap_size = 0;
mcb *free_list = NULL;
mcb *allocated_list = NULL;
static int total_blocks = 0;

void initialize_heap(size_t heap_size)
{
    void *heap_memory = kmalloc(heap_size, 0, NULL);
    if (!heap_memory) {
        kpanic("Failed to allocate heap memory");
    }

    total_heap_size = heap_size;

    mcb *initial = (mcb *)heap_memory;
    initial->start_address = (void *)((char *)heap_memory + sizeof(mcb));
    initial->size = heap_size - sizeof(mcb);
    initial->next = NULL;
    initial->prev = NULL;
    initial->is_free = 1;

    mem_head = initial;
    mem_tail = initial;
    total_blocks = 1;
    
    // Set the lists to point to main list
    free_list = mem_head;
    allocated_list = NULL;
}

void update_list_pointers(void)
{
    // Simply set pointers to the main list - user commands can traverse and filter
    free_list = mem_head;
    allocated_list = mem_head;
}

void *allocate_memory(size_t size)
{
    if (size == 0) return NULL;

    // Find a free block that's large enough
    mcb* mem_ptr = mem_head;
    while (mem_ptr != NULL) {
        if (mem_ptr->is_free && mem_ptr->size >= size) {
            break;
        }
        mem_ptr = mem_ptr->next;
    }

    if (mem_ptr == NULL) {
        return NULL;
    }
    
    // If perfect fit or can't split (too small remainder)
    if (mem_ptr->size <= size + sizeof(mcb) + 1) {
        mem_ptr->is_free = 0;
        return mem_ptr->start_address;
    }
    
    // Split the block
    size_t remaining_size = mem_ptr->size - size - sizeof(mcb);
    mem_ptr->size = size;
    mem_ptr->is_free = 0;
    
    // Create the split block
    mcb* split_mcb = (mcb*)((char*)mem_ptr->start_address + size);
    split_mcb->start_address = (void*)((char*)split_mcb + sizeof(mcb));
    split_mcb->size = remaining_size;
    split_mcb->is_free = 1;
    split_mcb->next = mem_ptr->next;
    split_mcb->prev = mem_ptr;
    
    if (mem_ptr->next) {
        mem_ptr->next->prev = split_mcb;
    } else {
        mem_tail = split_mcb;
    }
    
    mem_ptr->next = split_mcb;
    total_blocks++;
    
    return mem_ptr->start_address;
}

int free_memory(void *ptr)
{
    if (ptr == NULL) {
        return -1;
    }

    // Find the block to free
    mcb *block = mem_head;
    while (block != NULL) {
        if (ptr == block->start_address) {
            break;
        }
        block = block->next;
    }

    if (block == NULL || block->is_free) {
        return -1;
    }

    block->is_free = 1;

    // Merge block with following freespace
    if (block->next != NULL && block->next->is_free) {
        mcb *next = block->next;
        block->size += (next->size + sizeof(mcb));
        block->next = next->next;
        if (next->next != NULL) {
            next->next->prev = block;
        } else {
            mem_tail = block;
        }
    }

    // Merge block with leading freespace
    if (block->prev != NULL && block->prev->is_free) {
        mcb *prev = block->prev;
        prev->size += (block->size + sizeof(mcb));
        prev->next = block->next;
        if (block->next != NULL) {
            block->next->prev = prev;
        } else {
            mem_tail = prev;
        }
        total_blocks--;
    }

    update_list_pointers();
    return 0;
}
