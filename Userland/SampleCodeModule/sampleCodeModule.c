// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <shell.h>
#include <commands.h>
#include <lib.h>
#include <stdio.h>

int main() {
    // scClear();
	// write(2, "\nHOLAFROMSAMPLECODE", 20);
    // printf("\nHOLAFROMSAMPLECODE");

    // scClear();
    printf("\n");
    printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP \n");

    shellExecute();

    // char *argv[] = {"Shell"};
    // my_create_process(startShell, 1, argv, 1, NULL);
    
    printPCS();
	return 0;
}