#include <stddef.h>

/**
 * @file include/com_hist.h
 *
 * @brief Functions for altering and retrieving information from the command history
 */

static char** com_hist;
static int hist_size = 0;
static int hist_max_size = 5;
static int hist_index = 0;

void initialize_comhist(void);

/**
 * @brief Adds a new entry to the command history. Command history is ordered by
 * insertion order.
 * 
 * @param command The string to add to the history
 */
void hist_add(char* command);

/**
 * @brief Moves to the next index in the history and returns the command
 * in history. Does not wrap to the start if at the end of the history.
 * 
 * @return char* 
 */
char* hist_next(void);

/**
 * @brief Moves to the previous index in the history and returns the
 * command in history. Does not wrap to the end if at the start of
 * the history.
 * 
 * @return char* 
 */
char* hist_previous(void);

/**
 * @brief Gets the command at the given index in history.
 * 
 * @param i 
 * @return char* 
 */
char* hist_get(int i);

/**
 * @brief Sets the command at the given index in history.
 * 
 * @param i 
 * @param command 
 */
void hist_set(int i, char* command);

/**
 * @brief Sets the current index of the command history.
 * 
 * @param i 
 */
void hist_index_set(int i);

/**
 * @brief Get the current index of the command history.
 * 
 * @return int 
 */
int hist_get_index(void);
