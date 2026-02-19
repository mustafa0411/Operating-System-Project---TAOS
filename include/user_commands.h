#ifndef MPX_USER_COMMANDS_H
#define MPX_USER_COMMANDS_H

/**
 * @file user_commands.h
 * @brief Functions for executing commands access
 *
 */

/**
 * @brief Prints the current system version
 */
void version(void);

/**
 * @brief Get the date in the RTC
 */
void get_date(void);

/**
 * @brief Set the date in the RTC
 *
 * @param date_string
 */
void set_date( char * date_string);

/**
 * @brief Get the time from the RTC
 */
void get_time(void);

/**
 * @brief Set the date in the RTC
 *
 * @param time_string
 */
void set_time(char* time_string);

int shutdown(void);

#endif
