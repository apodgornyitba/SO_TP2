#include <stdint.h>
#include <tests.h>
#include <syscall.h>

#define CMD_COUNT 23

typedef struct t_command t_command;

typedef struct t_command{
	void(*command)(int, char**);
	const char* name;
} Command;

void getHelp();
void printDateTime();
void clear();
void printRegisters();
void divExc();
void opExc();
void printMem();
void printPCS();
void loop();
void killPCS();
void nicePCS();
void blockPCS();
void unblockPCS();
void printSems();
void cat();
void wc();
void filter();
void printPipes();
void philosopher();
void testMM();
void testPrio();
void testPCS();
void testSync();
void writeSHM();
void readSHM();



static Command commands[] = {
	{&getHelp, "help"},
	{&printDateTime, "time"},
	{&clear, "clear"},
	{&printRegisters, "inforeg"},
	{&divExc, "divex"},
	{&opExc, "opex"},
	{&printMem,"mem"},
	{&printPCS,"ps"},
	{&loop, "loop"},
	{&killPCS, "kill"},
	{&nicePCS, "nice"},
	{&blockPCS, "block", },
	{&unblockPCS, "unblock"},
	{&printSems, "sem"},
	{&cat, "cat"},
	{&wc, "wc"},
	{&filter, "filter"},
	{&printPipes, "pipe"},
	{&philosopher, "phylo"},
	{&writeSHM, "writeSHM"},
	{&readSHM, "readSHM"},
    
	{&testMM, "test_mm"},
	{&testPrio, "test_prio"},
	{&testPCS, "test_processes"},
	{&testSync, "test_sync"},
};