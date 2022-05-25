// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <syscalls.h>
#include <lib.h>
#include <rtc.h>
#include <scheduler.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

static void zero_division();
static void invalid_op_code();

static const char *registers[] = {"RAX:", "RBX:", "RCX:", "RDX:", "RBP:", "RDI:", "RSI:", "R8 :", "R9 :", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:"};
void printRegisters(uint64_t rsp);

void exceptionDispatcher(int exception,uint64_t rsp){
    if (exception == ZERO_EXCEPTION_ID){
        zero_division();
    }
    if (exception == INVALID_OP_CODE)
        invalid_op_code();
    printRegisters(rsp);
    // hold(10);
    sysWrite(2,(uint64_t)"\n",1,0,0);
    killProcess(getCurrPID());
    return;
}

static void zero_division() {
	sysWrite(2,(uint64_t)"Excepcion 0: Division por cero\n",29,0,0);
}

static void invalid_op_code() {
	sysWrite(2,(uint64_t)"Excepcion 6: Operacion invalida\n",30,0,0);
}


void printRegisters(uint64_t rsp)
{
    char hexa[20];
    sysWrite(2,(uint64_t)"RSP: ",6,0,0);
    intToHexa((long long)(rsp + 3*8),hexa,8);
    sysWrite(2,(uint64_t)hexa,20,0,0);
    sysWrite(2,(uint64_t)"\n",1,0,0);
    sysWrite(2,(uint64_t)"RIP: ",6,0,0);
    intToHexa((long long)(rsp),hexa,8);
    sysWrite(2,(uint64_t)hexa,20,0,0);
    sysWrite(2,(uint64_t)"\n",1,0,0);

    for (int i = 14; i >= 0; i--){
        intToHexa((long long)getReg(i),hexa,8);
        sysWrite(2,(uint64_t)registers[14-i] ,3,0,0);
        sysWrite(2,(uint64_t)": " ,2,0,0);
        sysWrite(2,(uint64_t)hexa,20,0,0);
        sysWrite(2,(uint64_t)"\n",1,0,0);
    }
    sysWrite(2,(uint64_t)"\n",1,0,0);
}

