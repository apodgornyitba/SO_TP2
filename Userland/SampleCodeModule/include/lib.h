#ifndef LIB_H
#define LIB_H

#define TIMER_STOP 0
#define TIMER_START 1
#define NULL (void *) 0

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
int my_yield();
int my_getpid();
int my_create_process(void(*entryPoint)(int, char **), int argc, char *argv[], int fg, int * fd);
int my_nice(int pid, int newPrio);
int my_kill(int pid);
int my_block(int pid);
int my_unblock(int pid);
int my_wait(int pid);
int my_print_pcs();
int my_sem_open(int sem_id, int initialValue);
int my_sem_wait(int sem_id);
int my_sem_post(int sem_id);
int my_sem_close(int sem_id);
int my_print_sems();
int my_pipe_open(int pipe_id);
int my_pipe_write(int pipe_id, char * str);
int my_pipe_read(int pipe_id);
int my_pipe_close(int pipe_id);
int my_print_pipes();
void * my_create_shm(unsigned int shm_id);


#endif