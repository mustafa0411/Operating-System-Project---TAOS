#ifndef MCB_USER_COMMANDS_H
#define MCB_USER_COMMANDS_H

/**
 * @brief Allocates heap memory and displays the result
 * 
 * Allocates heap memory by calling allocate_memory() and prints (in
 * hexadecimal) the address of the newly allocated block (not the MCB
 * address), or an error message if allocation fails.
 * 
 * @param args The size of the allocation request (in decimal)
 */
void usr_allocate_memory(const char *args);

/**
 * @brief Frees heap memory
 * 
 * Frees heap memory by calling free_memory() and prints a success message
 * or an error message if freeing fails.
 * 
 * @param args The address of the memory block (not MCB) to free (in hexadecimal)
 */
void usr_free_memory(const char *args);

/**
 * @brief Displays all allocated memory blocks
 * 
 * Walks through the memory list, printing information for each allocated
 * block of memory. Information includes the start address of the block 
 * (not the MCB address) in hexadecimal and the size of the block in decimal.
 * 
 * @param args Should be NULL or empty (command takes no arguments)
 */
void usr_show_allocated(const char *args);

/**
 * @brief Displays all free memory blocks
 * 
 * Walks through the memory list, printing information for each free
 * block of memory. Information includes the start address of the block 
 * (not the MCB address) in hexadecimal and the size of the block in decimal.
 * 
 * @param args Should be NULL or empty (command takes no arguments)
 */
void usr_show_free(const char *args);


#endif
