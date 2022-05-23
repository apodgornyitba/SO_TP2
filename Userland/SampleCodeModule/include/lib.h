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
//NUEVAS FUNCIONES
void * my_malloc(unsigned int nbytes);
void my_free(void * ap);
void print_mm();
int64_t my_yield();
int64_t my_getpid();
int64_t my_create_process(char * name, uint64_t argc, char *argv[], int fg, int fd);
int64_t my_nice(uint64_t pid, uint64_t newPrio);
int64_t my_kill(uint64_t pid);
int64_t my_block(uint64_t pid);
int64_t my_unblock(uint64_t pid);
int64_t my_wait(int64_t pid);


#endif