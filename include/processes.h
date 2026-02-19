#ifndef MPX_PROCESSES_H
#define MPX_PROCESSES_H

/**
 @file processes.h
 @brief Provided system process and user processes for testing
*/

/* **********************************************************************
 The following functions are needed for Module R3.
********************************************************************** */

/**
 A test process that prints a message then yields, exiting after 1 iteration.
*/
void proc1(void);

/**
 A test process that prints a message then yields, exiting after 2 iterations.
*/
void proc2(void);

/**
 A test process that prints a message then yields, exiting after 3 iterations.
*/
void proc3(void);

/**
 A test process that prints a message then yields, exiting after 4 iterations.
*/
void proc4(void);

/**
 A test process that prints a message then yields, exiting after 5 iterations.
*/
void proc5(void);

/* **********************************************************************
 The following function is needed for Module R4.
********************************************************************** */

/**
 System idle process. Used in dispatching. It will be dispatched if NO other
 processes are available to execute. Must be a system process.
*/
void sys_idle_process(void);

#endif
