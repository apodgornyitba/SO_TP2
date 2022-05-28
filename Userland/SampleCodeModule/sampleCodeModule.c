// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <shell.h>
#include <libc.h>

int main() {

    char *argv[] = {"Shell"};
    my_create_process(startShell, 1, argv, 1, NULL);
    
	return 0;
}