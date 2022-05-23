#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>

#define MAX_BLOCKED_PROCESSES 20
#define NULL ((void *)0)

typedef struct Semaphore {
  uint32_t id;
  uint64_t value;
  int blockedProcesses[MAX_BLOCKED_PROCESSES];
  uint16_t blockedProcessesAmount;
  uint16_t listeningProcesses;
  struct Semaphore *next;
  int lock;
} Semaphore;

int semOpen(uint32_t id, uint64_t initialValue);
int semWait(uint32_t id);
int semPost(uint32_t id);
int semClose(uint32_t id);
void semStatus();

#endif