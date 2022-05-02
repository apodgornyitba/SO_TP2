#ifndef LIB_H
#define LIB_H

#define TIMER_STOP 0
#define TIMER_START 1

int write(int fd, char* buffer, int length);
int read(int fd, char *buffer, int length);
void scClear();
int DateTime(int selector);
int getMem(unsigned char *buff, unsigned long long address, int bytes);
int getRegs(unsigned long long* arr);
void separateScreen();
void opCodeExc();
int writeAtPos(int fd, char *buffer, int length,int x, int y);
int ticks();

//FALTA DEFINIR EN C LAS SYSCALLS NUEVAS

#endif