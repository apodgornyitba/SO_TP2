#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void * memorySet(void * b, int c, int len);
int64_t satoi(char* str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(int argc, char *argv[]);

#endif
