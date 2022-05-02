// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <libc.h>
#include <shell.h>
#include <lib.h>

static int esc = 0;

void handler(){
	initialize();

    char command[120] = {0};
    char param[120] = {0};

    while(!esc) {

        command[0]=0;
        param[0]=0;
        printf("$ > ");
        scanf("%s %s", command, param);
        shellMain(command, param, &esc);
    }
    scClear();
    return;
    
}