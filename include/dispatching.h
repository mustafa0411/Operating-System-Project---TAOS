#ifndef DISPATCHING_H
#define DISPATCHING_H


/**
 * @brief Cause Command Handler to yield the CPU
 * If any processes are in queue, they will execute
 * 
 */
void yield(void);


/**
 * @brief Loads the R3 test processes from <processes.h>
 * Each process (one per function) is loaded and queued in 
 * a non-suspended ready state, with a name and priority 
 * of your choosing
 * 
 */
void loadr3(void);

/**
 * @brief Loads all R3 processes but sets the status to ready suspended
 * 
 */
void loadr3_suspended(void);

/**
 * Loads a single process giving one of the process names proc1 -> proc5
 * The process will be loaded as a ready non-suspended process
 */
int load_process(const char* name);

/**
 * @brief resumes all ready suspended processes
 * 
 */
void resume_all(void);

/**
 * @brief Creates system processes (command handler and idle process)
 */
void create_system_processes(void);


/**
 * @brief Create the alarm process object with the user input
 * 
 * @param time the time the alarm will go off
 * @param message the message that will display when the alarm goes off
 * @return int 
 */
int create_alarm_process(const char* time, const char* message);

/**
 * @brief Alarm process that waits until a specified time to display a message
 */
void alarm_process(void);


// Structure to hold alarm data for each process

typedef struct {
    char target_time[9];     // HH:MM:SS
    char target_date[11];    // YYYY/MM/DD
    char message[200];
} process_alarm_data;


#endif

