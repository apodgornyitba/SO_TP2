#ifndef TESTS_H
#define TESTS_H

#include "test_util.h"

uint64_t test_mm();
void test_prio();
int64_t test_processes(uint64_t argc, char *argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);

#endif
