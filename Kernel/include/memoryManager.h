/***************************************************
  memoryManager.h
****************************************************/

#ifndef _MM_
#define _MM_

#include <syscalls.h>
#include <lib.h>

void memInit(char *memBase, unsigned long memSize);
void * malloc(unsigned long nbytes);
void free(void* ap);
void printMemState();

#endif