#include <stdint.h>
#include <tests.h>
#include <syscall.h>

#define CMD_COUNT 29

typedef int (*CmdHandler)(int argc, const char* argv[]);

typedef struct {
	const char* name;
	CmdHandler handler;
	uint8_t isBackground;
} Command;

int getHelp();
int printDateTime();
int clear();
int printRegisters();
int divExc();
int opExc();
int printMem();
int printPCS();
int loop();
int killPCS();
int nicePCS();
int blockPCS();
int unblockPCS();
int printSems();
int cat();
int wc();
int filter();
int printPipes();
int philosopher();
int testMM();
int testPrio();
int testPCS();
int testSync();



static Command commands[] = {
	{"help", getHelp, 0},
	{"time", printDateTime, 0},
	{"clear", clear, 0},
	{"inforeg", printRegisters,0},
	{"divex", divExc,0},
	{"opex", opExc,0},
	{"mem", printMem,0},
	{"ps", printPCS, 0},
	{"loop", loop,0},
	{"kill", killPCS,0},
	{"nice", nicePCS,0},
	{"block", blockPCS},
	{"unblock", unblockPCS},
	{"sem", printSems,0},
	{"cat", cat,0},
	{"wc", wc,0},
	{"filter", filter,0},
	{"pipe", printPipes,0},
	{"phylo", philosopher,0},
    
	{"test_mm", (CmdHandler)test_mm,0},
	{"test_prio", (CmdHandler)test_prio,0},
	{"test_processes", (CmdHandler)test_processes,0},
	{"test_sync", (CmdHandler)test_sync,0},
	// {"test_no_sync", test_no_sync,0}
};