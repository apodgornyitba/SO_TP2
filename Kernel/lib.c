// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <rtc.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int abs(int num){
    return num < 0? -num : num;
}

char *intToHexa(long long num, char *str, int bytes) 
{ 
    int i = 0;
    long long n = abs(num);

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (n == 0) 
    {
        str[i++] = '0';
        str[i++] = 'x';

        while (i < bytes*2 + 2) {
            str[i++] = '0';
        }

        str[i] = '\0'; 
        return str; 
    } 
  
    // Process individual digits 
    while (i < bytes*2 && n != 0) 
    { 
        int rem = n % 16; 
        str[i++] = (rem >= 10)? (rem - 10) + 65 : rem + 48; 
        n = n/16; 
    } 

    while (i < bytes*2) {
        str[i++] = '0';
    }

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    return reverse(str, 0, i-1); 
} 

int strlength(const char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

// Iterative function to implement itoa() function in C
char *intToStr(int value, char *buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;

    // consider absolute value of number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10)
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;

        n = n / base;
    }

    // if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

char *strcopy(char *destination, const char *source)
{

    char *ptr = destination;

    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0';
    return ptr;
 }

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}



