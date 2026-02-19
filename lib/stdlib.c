#include <stdlib.h>
#include <ctype.h>
#include <sys_req.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

static void kputs(const char *c);


// Convert unsigned int to hexadecimal string
char *itohex(unsigned int n, char *str, int uppercase)
{
    if (str == NULL) return NULL;
    
    if (n == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    const char *hex_chars;
    if (uppercase) {
        hex_chars = "0123456789ABCDEF";
    } else {
        hex_chars = "0123456789abcdef";
    }
    int len = 0;
    
    // Write the hex digits into the string right to left
    while (n != 0)
    {
        str[len] = hex_chars[n % 16];
        n /= 16;
        len++;
    }

    // Reverse the string
    for (int i = 0; i < len/2; i++)
    {
        char c = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = c;
    }

    // Add null terminator
    str[len] = '\0';

    return str;
}

int atoi(const char *s)
{
	int res = 0;
	char sign = ' ';
	int valid = 0;

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while(*s) {
		if ('0' <= *s && *s <= '9') {
			res = res * 10 + (*s - '0');
			valid = 1;
		}else {
			return -1;
		}
			s++;
	}

	if (!valid) return -1;

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

char *itoa(int n, char *str)
{
	if (str == NULL) return NULL;
	
	if (n == 0)
	{
		str = "0\0";
		return str;
	}

	int is_neg = 0;
	if (n < 0)
	{
		is_neg = 1;
		n = -n;
	}

	char c;
	int len = 0;
	// Write the digits into the string right to left
	while (n != 0)
	{
		// Get the corresponding char from the least significant digit
		str[len] = '0' + n % 10;
		// Integer divide by 10 will effectively move one place to the right
		// ex. 158 / 10 = 15.8, .8 is truncated, final result is 15
		n /= 10;
		len++;
	}

	// Add negative symbol if applicable
	if (is_neg == 1)
		str[len++] = '-';

	// Reverse the string
	for (int i = 0; i < len/2; i++)
	{
		c = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = c;
	}

	// Add null terminator
	str[len] = '\0';

	return str;
}

// Convert integer to BCD
unsigned int i_to_bcd(int num) //untested
{
    unsigned int bcd = 0;
    int shift = 0;

    if (num < 0) {
        return 0xFFFFFFFF; // indicate error
    }

    while (num > 0) {
        int digit = num % 10;
        bcd |= (digit << (shift * 4));  // pack 4-bit nibbles
        num /= 10;
        shift++;
    }

    return bcd;
}

// Convert BCD to integer
int bcd_to_i(unsigned int bcd) //untested
{
    int num = 0;
    int multiplier = 1;

    while (bcd > 0) {
        int digit = bcd & 0xF;  // take low nibble
        if (digit > 9) {
            // invalid BCD digit
            return -1;
        }
        num += digit * multiplier;
        multiplier *= 10;
        bcd >>= 4; // move to next nibble
    }

    return num;
}

void kputc(char c) {
	sys_req(WRITE, COM1, &c, 1);
}

static void kputs(const char *c) {
	sys_req(WRITE, COM1, c, strlen(c));
}

void printf(const char *string, ...) {
    // open the argument list
    va_list args;
    va_start(args, string);

    char buffer[64]; // placeholder value

    // formatting flags
    int align = 0;
    int format_end = 0;
    int pad_with_zero = 0;
    int pad_num = 0;

    while (*string) {
        if (*string == '%') {
            format_end = 0;
            pad_num = 0;
            pad_with_zero = 0;
            align = 0;
            while (format_end == 0)
            {
                string++;
                /*Format Specifiers: %c, %d, %s, %x, %X, %u*/
                switch (*string) {
                    case '-': {
                        align = 1;
                        break;
                    }
                    case '0': {
                        // If there is no padding yet specified, zero indicates to pad with zeroes 
                        if (pad_num == 0) {
                            pad_with_zero = 1;
                            break;
                        }
                        // Do not break otherwise, treat like regular number
                    } 
                    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
                        pad_num *= 10;
                        pad_num += *string - '0';
                        break;
                    }
                    case 'd': {
                        int n = va_arg(args, int);
                        char* buf = itoa(n, buffer); //use itoa to convert int to string
                        pad_num -= strlen(buf);
                        if (align == 1) kputs(buf);
                        while (pad_num-- > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputs(buf);
                        format_end = 1;
                        break;
                    }
                    case 'u': {
                        unsigned int n = va_arg(args, unsigned int);
                        // Convert unsigned int to string manually
                        char temp[32];
                        int len = 0;
                        if (n == 0) {
                            temp[0] = '0';
                            temp[1] = '\0';
                            len = 1;
                        } else {
                            unsigned int copy = n;
                            while (copy > 0) {
                                temp[len++] = '0' + (copy % 10);
                                copy /= 10;
                            }
                            // Reverse
                            for (int i = 0; i < len/2; i++) {
                                char c = temp[i];
                                temp[i] = temp[len - 1 - i];
                                temp[len - 1 - i] = c;
                            }
                            temp[len] = '\0';
                        }
                        pad_num -= len;
                        if (align == 1) kputs(temp);
                        while (pad_num-- > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputs(temp);
                        format_end = 1;
                        break;
                    }
                    case 'x': {
                        unsigned int n = va_arg(args, unsigned int);
                        char* buf = itohex(n, buffer, 0); // lowercase hex
                        pad_num -= strlen(buf);
                        if (align == 1) kputs(buf);
                        while (pad_num-- > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputs(buf);
                        format_end = 1;
                        break;
                    }
                    case 'X': {
                        unsigned int n = va_arg(args, unsigned int);
                        char* buf = itohex(n, buffer, 1); // uppercase hex
                        pad_num -= strlen(buf);
                        if (align == 1) kputs(buf);
                        while (pad_num-- > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputs(buf);
                        format_end = 1;
                        break;
                    }
                    // Adjust for single characters
                    case 'c': {
                        char c = (char)va_arg(args, int);
                        if (align == 1) kputc(c);		
                        while (--pad_num > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputc(c);
                        format_end = 1;
                        break;
                    }
                    // Fetch string and print it
                    case 's': {
                        char *s = va_arg(args, char*);
                        pad_num -= strlen(s);
                        if (align == 1) kputs(s);
                        while (pad_num-- > 0) kputc(pad_with_zero ? '0' : ' ');
                        if (align == 0) kputs(s);
                        format_end = 1;
                        break;
                    }
                    case '%': {
                        kputc('%');
                        format_end = 1;
                        break;
                    }
                    default:
                        // Unknown specifiers, eg. %y
                        kputc('%');
                        kputc(*string);
                        format_end = 1;
                        break;
                }
            }
        } else {
            kputc(*string); // No format specifier, gets printed anyway.
        }
        string++;
    }
    // close arg list for cleanup.
    va_end(args);
}
