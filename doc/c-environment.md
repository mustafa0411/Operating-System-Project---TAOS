# The MPX C Environment

MPX is implemented in a *Freestanding C* environment. Importantly, this means
that you do *not* have access to the bulk of the standard library. What you do
have access to is primarily constants and type definitions specific to the
architecture. Specifically, the Freestanding implementation gives you:

- `<float.h>`  
 Constants relating to how floating-point math works

- `<iso646.h>`  
 Alternate spellings for some operators (e.g. `and` for `&&`)

- `<limits.h>`  
 Constants describing the limits of primitive types (e.g. `INT_MAX`)

- `<stdalign.h>`  
 Macros for determining the memory alignment of types

- `<stdarg.h>`  
 Macros for implementing variadic functions (similar to how `printf()` works)

- `<stdbool.h>`  
 Definitions for `true` and `false`

- `<stddef.h>`  
 Basic constants and types like `NULL` and `size_t`

- `<stdint.h>`  
 Fixed-width integer type definitions (e.g. `uint32_t`)

- `<stdnoreturn.h>`  
 Allows defining a function that never returns

## Limited Library

MPX does provide some functions from the standard library for your use. You are
free to implement additional functions to aid in the development of MPX
(suggestions include `sprintf()` (or at least something to convert from an
integer to a NUL-terminated string) and `strcpy()`).

### `<stdlib.h>`

- `int atoi(const char *s)`  
 Converts the NUL-terminated string `s` to an integer

### `<ctype.h>`

- `int isspace(int c)`  
 Determines whether the character `c` is a white-space character

### `<string.h>`

- `void *memcpy(void *dst, void *src, size_t n)`  
 Copies `n` bytes of memory from `src` to `dst`

- `void *memset(void *ptr, char val, size_t n)`  
 Fills `n` bytes of memory starting at `ptr` with the value `val`

- `int strcmp(const char *s1, const char *s2)`  
 Compares two strings

- `size_t strlen(const char *s)`  
 Count the number of characters in the string `s`

- `char *strtok(char *s, const char *delimiters)`  
 Split the string `s` into tokens based on `delimiters`
