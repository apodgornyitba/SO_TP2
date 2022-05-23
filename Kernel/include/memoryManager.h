/***************************************************
  memoryManager.h
****************************************************/

#ifndef _MM_
#define _MM_

#include <syscalls.h>
#include <lib.h>

void memInit(char *memBase, unsigned long memSize);
void * my_malloc(unsigned long nbytes);
void my_free(void* ap);
void printMemState();

#endif