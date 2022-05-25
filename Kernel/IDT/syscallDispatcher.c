// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
    sysTicks, //8
    //NUEVAS FUNCIONES
    sysMalloc, //9
    sysFree, //10
    sysPrintMM, //11
    sysGetpid, //12
    sysCreateProcess, //13
    sysKill, //14
    sysBlock, //15
    sysUnblock, //16
    sysYield, //17
    sysNice, //18
    sysWait, //19
    sysPrintPCS, //20
    sysOpenSem, //21
    sysCloseSem, //22
    sysWaitSem, //23
    sysPostSem, //24
    sysPrintSems, //25
    sysPipeOpen, //26
    sysPipeClose, //27
    sysPipeRead, //28
    sysPipeWrite, //29
    sysPrintPipe, //30
    };

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
    return (*systemCalls[rdi])(rsi, rdx, rcx,r8,r9);
}