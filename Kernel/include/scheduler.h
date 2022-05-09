#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include <lib.h>
#include <memoryManager.h>
#include <stddef.h>
#include <syscalls.h>
#include <interrupts.h>

#define STACK_SIZE (4 * 1024)
#define BACKGROUND_PRIORITY 1
#define FOREGROUND_PRIORITY 2
#define MAX_PRIORITY 50

typedef enum
{
      READY,
      BLOCKED,
      KILLED
} State;

typedef struct
{
      uint64_t pid;
      uint64_t ppid;
      int foreground;
      int fd[2]; // 0 -> in - 1 -> out
      char name[30];
      void *rsp;
      void *rbp;
      int priority;
      int argc;
      char **argv;
} PCB;

typedef struct
{
      uint64_t gs;
      uint64_t fs;
      uint64_t r15;
      uint64_t r14;
      uint64_t r13;
      uint64_t r12;
      uint64_t r11;
      uint64_t r10;
      uint64_t r9;
      uint64_t r8;
      uint64_t rsi;
      uint64_t rdi;
      uint64_t rbp;
      uint64_t rdx;
      uint64_t rcx;
      uint64_t rbx;
      uint64_t rax;

      uint64_t rip;
      uint64_t cs;
      uint64_t eflags;
      uint64_t rsp;
      uint64_t ss;
      uint64_t base;
} StackFrame;

typedef struct
{
      PCB pcb;
      State state;
      struct Process *next;
} Process;

typedef struct
{
      uint32_t size;
      uint32_t readySize;
      Process *first;
      Process *last;
} ProcessList;

void initScheduler();
void *scheduler(void *oldRSP);
int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
uint64_t killProcess(uint64_t pid);
uint64_t blockProcess(uint64_t pid);
uint64_t unblockProcess(uint64_t pid);
int getCurrPID();
void processDisplay();
void setNewCycle(uint64_t pid, int priority);
void killFgProcess();
void yield();
int currentReadsFrom();
int currentWritesTo();
int currentProcessFg();
void waitForPID(uint64_t pid);

#endif