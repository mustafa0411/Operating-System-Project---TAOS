#ifndef MPX_PCB_USER_COMMANDS_H
#define MPX_PCB_USER_COMMANDS_H

/**
 * @brief Create a pcb object
 * 
 * @param name name of the pcb
 * @param processor_class class of pcb(0 = user, 1 = system)
 * @param priority priority of the pcb(0-9)
 */
int create_pcb(const char *name, int processor_class, int priority);

/**
 * @brief delete the selected pcb using its name
 * 
 * @param name name of the pcb to delete
 */
int delete_pcb(const char *name);

/**
 * @brief set the pcb to the blocked state
 * 
 * @param name pcb to block
 */
int block_pcb(const char* name);

/**
 * @brief unblock the selected pcb
 * 
 * @param name name of pcb to unblock
 */
int unblock_pcb(const char* name);

/**
 * @brief show the selected pcb
 * 
 * @param name pcb to show
 */
void show_pcb(const char *name);

int resume_pcb(const char* name);

/**
 * @brief set the pcb to the suspended state
 * 
 * @param name pcb to suspend
 */
int suspend_pcb(const char* name);

/**
 * @brief gives selected process new priority
 * 
 * @param name name of pcb to change priority
 * @param new_priority new priority to set(0-9)
 */
int set_pcb_priority(const char* name, int new_priority);

/**
 * @brief Show all processes in the Ready state
 */
void show_ready_pcb(void);

/**
 * @brief Show all processes in the Blocked state
 */
void show_blocked_pcb(void);

/**
 * @brief Show all processes in any state
 */
void show_all_pcb(void);
#endif
