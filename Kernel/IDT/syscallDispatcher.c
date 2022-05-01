#include <stdint.h>
#include <syscalls.h>
#include <syscallDispatcher.h>

static uint64_t (*systemCalls[])(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) = {
    sysRead,       //0
    sysWrite,      //1
    sysClear,      //2
    sysGetMem,     //3
    sysGetReg,     //4
    sysDateTime,   //5
    sysSeparateScreen, //6  
    sysWriteAtPos, //7
    sysTicks //8
    };

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
    return (*systemCalls[rdi])(rsi, rdx, rcx,r8,r9);
}