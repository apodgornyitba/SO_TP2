// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <rtc.h>
#include <screenDriver.h>
#include <graphicRenderer.h>
#include <syscalls.h>
#include <kbDriver.h>
#include <font.h>
#include <time.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <pipes.h>
#include <semaphores.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

static int fontColour = 0xFFFFFF;

static uint64_t regs[17] = {0};

uint64_t *getRegs()
{
  return regs;
}

uint64_t sysGetReg(uint64_t buffer, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  long long *array = (long long *)buffer;
  for (int i = 0; i < 15; i++)
  {
    array[i] = regs[i];
  }
  return 0;
}

uint64_t sysWrite(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t r8, uint64_t r9)
{
  char *buff = (char *)buffer;
  int inserted = 0;

  while (length > 0)
  {
    if (*buff == '\0' || *buff == -1)
      break;
    if (fd == STDIN)
      loadKey(*buff);
    else
      putChar(*buff, fontColour);
    buff++;
    inserted++;
    length--;
  }
  return inserted;
}

uint64_t sysWriteAtPos(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y)
{
  char *buff = (char *)buffer;
  if (fd != STDOUT)
    return -1;
  int inserted = 0;
  for (int i = 0; i < length; i++)
  {
    if (*buff == '\0' || *buff == -1)
      break;
    putCharAtPos(*buff, fontColour, x + i * ABS_WIDTH, y); // Y si me pase de linea? Mala suerte! No se imprime
    buff++;
    inserted++;
  }
  return inserted;
}

uint64_t sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  cleanScreen();
  return 0;
}

uint64_t sysDateTime(uint64_t selector, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return DateTime(selector);
}

uint64_t sysGetMem(uint64_t buffer, uint64_t address, uint64_t bytes, uint64_t r8, uint64_t r9)
{
  unsigned char *array = (unsigned char *)buffer;
  for (unsigned char i = 0; i < bytes; i++)
  {
    array[i] = (unsigned char)getMem(address + i);
  }
  return 0;
}

uint64_t sysRead(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t r8, uint64_t r9)
{
  if (fd != STDIN)
  {
    return -1;
  }
  char *buff = (char *)buffer;
  return dumpBuffer(buff, length);
}

uint64_t sysSeparateScreen(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  separateMainScreen();
  return 0;
}

uint64_t sysTicks(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return ticks_elapsed();
}

uint64_t sysMalloc(uint64_t nbytes, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return (uint64_t) my_malloc((unsigned long)nbytes);
}

uint64_t sysFree(uint64_t ap, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  my_free((void *)ap);
  return 0;
}

uint64_t sysPrintMM(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  printMemState();
  return 0;
}

uint64_t sysGetpid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return getCurrPID();
}

uint64_t sysCreateProcess(uint64_t name, uint64_t argc, uint64_t argv, uint64_t fg, uint64_t fd)
{
  return addProcess((void (*)(int, char **))name, (int)argc, (char **)argv, (int)fg, (int *)fd);
}

uint64_t sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return killProcess(pid);
}

uint64_t sysBlock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return blockProcess(pid);
}

uint64_t sysUnblock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return unblockProcess(pid);
}

uint64_t sysYield(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  yield();
  return 0;
}

uint64_t sysNice(uint64_t pid, uint64_t newPrio, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  setPriority(pid, (int) newPrio);
  return 0;
}

uint64_t sysWait(uint64_t pid, uint64_t newPrio, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  waitForPID(pid);
  return 0;
}

uint64_t sysPrintPCS(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  processDisplay();
  return 0;
}

uint64_t sysOpenSem(uint64_t sem_id, uint64_t initialValue, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return semOpen( (uint32_t) sem_id, initialValue);
  
}

uint64_t sysCloseSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return semClose( (uint32_t) sem_id );
}

uint64_t sysWaitSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return semWait( (uint32_t) sem_id);
}

uint64_t sysPostSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return semPost( (uint32_t) sem_id);
}

uint64_t sysPrintSems(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  semStatus();
  return 0;
}

uint64_t sysPipeOpen(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return pipeOpen(pipe_id);
}

uint64_t sysPipeClose(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return pipeClose(pipe_id);
}

uint64_t sysPipeRead(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return pipeRead(pipe_id);
}

uint64_t sysPipeWrite(uint64_t pipe_id, uint64_t str, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  return pipeWrite(pipe_id, (char *) str);
}

uint64_t sysPrintPipe(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  pipeStatus();
  return 0;
}