#include <com_hist.h>
#include <string.h>
#include <memory.h>

void initialize_comhist(void)
{
    com_hist = sys_alloc_mem(sizeof(char*) * (hist_max_size + 1));
}

void hist_add(char* command) {
    if (hist_size < hist_max_size) {
        hist_size++;
    } else {
        sys_free_mem(com_hist[hist_size]);
    }
        
    for (int i = hist_size; i > 1; i--)
    {
        com_hist[i] = com_hist[i - 1];
    }
    hist_set(1, command);
    com_hist[0] = NULL;
}

char* hist_next(void) {
    if (hist_index < hist_size)
        hist_index++;

    return com_hist[hist_index];
}

char* hist_previous(void) {
    if (hist_index > 0)
        hist_index--;

    return com_hist[hist_index];
}

char* hist_get(int i) {
    return com_hist[i];
}

void hist_set(int i, char* command) {
    if (command == NULL) {
        com_hist[i] = NULL;
        return;
    }

    size_t n = strlen(command);
    char* new_command = (char*)sys_alloc_mem(n + 1);
    strncpy(new_command, command, n);
    new_command[n] = '\0';
    com_hist[i] = new_command;
}

void hist_index_set(int i) {
    hist_index = i;
}

int hist_get_index(void) {
    return hist_index;
}
