#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

uint64_t sysGetReg(uint64_t buffer, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysWrite(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t r8, uint64_t r9);
uint64_t sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysGetMem(uint64_t buffer, uint64_t address, uint64_t bytes, uint64_t r8, uint64_t r9);
uint64_t sysRead(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t r8, uint64_t r9);
uint64_t sysDateTime(uint64_t selector, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysSeparateScreen(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysWriteAtPos(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y);
uint64_t sysTicks(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysMalloc (uint64_t nbytes, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysFree (uint64_t ap, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPrintMM (uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysGetpid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysCreateProcess(uint64_t name, uint64_t argc, uint64_t argv, uint64_t fg, uint64_t fd);
int64_t sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysBlock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysUnblock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysYield(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysNice(uint64_t pid, uint64_t newPrio, uint64_t rcx, uint64_t r8, uint64_t r9);
int64_t sysWait(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

uint64_t *getRegs();

/*
int64_t my_sem_open(char *sem_id, uint64_t initialValue);
int64_t my_sem_wait(char *sem_id);
int64_t my_sem_post(char *sem_id);
int64_t my_sem_close(char *sem_id);
*/

#endif