#include <mcb.h>
#include <mcb_user_commands.h>
#include <mpx/io.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Helper function to parse hexadecimal string to unsigned int
 * 
 * @param hex_str Hexadecimal string to parse
 * @param result Pointer to store the parsed result
 * @return int 1 on success, 0 on failure
 */
static int parse_hex(const char *hex_str, unsigned int *result) {
    if (!hex_str || !result) {
        return 0;
    }
    
    *result = 0;
    int i = 0;
    
    // Skip "0x" prefix if present
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        i = 2;
    }
    
    while (hex_str[i] != '\0') {
        char c = hex_str[i];
        if (c >= '0' && c <= '9') {
            *result = *result * 16 + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            *result = *result * 16 + (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            *result = *result * 16 + (c - 'A' + 10);
        } else {
            return 0; // Invalid character
        }
        i++;
    }
    
    return 1; // Success
}

void usr_allocate_memory(const char *args) {
    if (!args || strlen(args) == 0) {
        printf("Usage: allocmem <size>\n");
        return;
    }

    int size = atoi(args);
    if (size <= 0) {
        printf("Error: Size must be positive\n");
        return;
    }

    void *ptr = allocate_memory(size);
    if (ptr) {
        printf("Allocated %d bytes at address: 0x%08X\n", size, (unsigned int)ptr);
    } else {
        printf("Error: Could not allocate %d bytes\n", size);
    }
}

void usr_free_memory(const char *args) {
    if (!args || strlen(args) == 0) {
        printf("Usage: freemem <address>\n");
        return;
    }

    unsigned int addr;
    if (!parse_hex(args, &addr)) {
        printf("Error: Invalid address format (use hexadecimal)\n");
        return;
    }

    void *ptr = (void *)addr;
    if (free_memory(ptr) == 0) {
        printf("Successfully freed memory at address: 0x%08X\n", addr);
    } else {
        printf("Error: Could not free memory at address: 0x%08X\n", addr);
    }
}

void usr_show_allocated(const char *args) {
    if (args && strlen(args) > 0) {
        printf("Error: 'showalloc' command does not take any arguments\n");
        return;
    }

    printf("Allocated Memory Blocks:\n");

    // Check if mem_head is valid
    if (mem_head == NULL) {
        printf("No allocated blocks.\n");
        return;
    }

    int count = 0;
    int found_any = 0;
    mcb *current = mem_head;
    
    // Traverse with safety checks
    while (current != NULL && count < 100) {  // Increased limit for safety
        // Validate the mcb pointer before dereferencing
        if ((void*)current < (void*)0x100000) {  // Sanity check: should be in valid memory
            printf("ERROR: Invalid MCB pointer detected at 0x%08X\n", (unsigned int)current);
            break;
        }
        
        if (!current->is_free) {
            printf("Address: 0x%08X, Size: %u bytes\n", 
                   (unsigned int)current->start_address, 
                   (unsigned int)current->size);
            found_any = 1;
        }
        count++;
        current = current->next;
    }

    if (!found_any) {
        printf("No allocated blocks.\n");
    }
}

void usr_show_free(const char *args) {
    if (args && strlen(args) > 0) {
        printf("Error: 'showfree' command does not take any arguments\n");
        return;
    }

    printf("Free Memory Blocks:\n");

    // Check if mem_head is valid
    if (mem_head == NULL) {
        printf("No free blocks.\n");
        return;
    }

    int count = 0;
    int found_any = 0;
    mcb *current = mem_head;
    
    // Traverse with safety checks
    while (current != NULL && count < 100) {  // Increased limit for safety
        // Validate the mcb pointer before dereferencing
        if ((void*)current < (void*)0x100000) {  // Sanity check: should be in valid memory
            printf("ERROR: Invalid MCB pointer detected at 0x%08X\n", (unsigned int)current);
            break;
        }
        
        if (current->is_free) {
            printf("Address: 0x%08X, Size: %u bytes\n", 
                   (unsigned int)current->start_address, 
                   (unsigned int)current->size);
            found_any = 1;
        }
        count++;
        current = current->next;
    }

    if (!found_any) {
        printf("No free blocks.\n");
    }
}
