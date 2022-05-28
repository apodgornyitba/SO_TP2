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
uint64_t sysGetpid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysCreateProcess(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t fg, uint64_t fd);
uint64_t sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysBlock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysUnblock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysYield(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysNice(uint64_t pid, uint64_t newPrio, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysWait(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPrintPCS(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysOpenSem(uint64_t sem_id, uint64_t initialValue, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysCloseSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysWaitSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPostSem(uint64_t sem_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPrintSems(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPipeOpen(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPipeClose(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPipeRead(uint64_t pipe_id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPipeWrite(uint64_t pipe_id, uint64_t str, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sysPrintPipe(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);


uint64_t *getRegs();

#endif