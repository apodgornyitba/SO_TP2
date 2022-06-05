// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <libc.h>
#include "test_util.h"
#include "lib.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT      10000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1 //TODO: Change as required
#define MEDIUM 2 //TODO: Change as required
#define HIGHEST 3 //TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(){
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless_loop_print"};
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++){
    pids[i] = my_create_process(&endless_loop_print, 2, argv, 0, NULL);
  }

  bussy_wait( TOTAL_PROCESSES * WAIT);

  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    my_nice(pids[i], prio[i]);
  }

  bussy_wait(TOTAL_PROCESSES * WAIT);

  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait( TOTAL_PROCESSES * WAIT);
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
  return;
}
