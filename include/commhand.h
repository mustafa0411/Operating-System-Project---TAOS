#ifndef COMMHAND_H
#define COMMHAND_H

#include <stdarg.h>

extern int shutting_down;

/**
 * @file commhand.h
 * @brief Functions for requesting and handling user commands
 */

/**
 * @brief The command handler loop. Recieves input from the user which is then executed as a command
 *
 */
void commhand(void);

/**
 * @brief Executes a given command by calling it's respective function
 *
 * @param command Un-tokenized command string to execute
 */
void execute_command(char *command);

/**
 * @brief Process entry point wrapper for command handler
 */
void commhand_process(void);

#endif
