#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
int atoi(const char *s);

/**
 * @brief Converts an integer to a string
 * 
 * @param i The integer to convert
 * @param str The string that will contain the converted integer
 * @return char* The string
 */
char *itoa(int i, char* str);

/**
 * @brief Converts an integer into Binary Coded Decimal
 * 
 * @param i 
 * @return int 
 */
unsigned int i_to_bcd(int num);

/**
 * @brief Converts a Binary Coded Decimal to an integer
 * 
 * @param i 
 * @return int 
 */
int bcd_to_i(unsigned int bcd);

/**
 * @brief Simple printf with format specifiers
 * 
 * @param string 
 * @param ... 
 */
void printf(const char *string, ...);

char *itohex(unsigned int n, char *str, int uppercase);

#endif
