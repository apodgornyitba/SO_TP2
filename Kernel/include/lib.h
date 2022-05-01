#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int getKey(int n);
uint64_t getReg(uint64_t reg);
unsigned char getMem(uint64_t address);

char *reverse(char *buffer, int i, int j);
char *intToHexa(long long num, char *str, int bytes);
int abs(int n);


#endif