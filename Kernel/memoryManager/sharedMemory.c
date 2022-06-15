#include <sharedMemory.h>
#include <memoryManager.h>

#define MAX_SIZE 1024


void * array[10] = {0};

void * createSHM(uint64_t id){
    if( id < 0 || id >= 10)
        return (void *) 0;

    if(array[id] == 0){
        void * mem = my_malloc(MAX_SIZE);
        array[id] = mem;
    }
    return array[id];
}