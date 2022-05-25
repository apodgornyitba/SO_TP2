// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <lib.h>
#include <libc.h>
#include <commands.h>

//?--> CAMBIARON TODA LA IMPLEMENTACION PARA QUE SE HAGA CON PROCESOS (PIPES Y SEMAFOROS)
#define MAX_MEMORY (96*1024*1024)

void initialize() {
    scClear();
    printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP \n");
}

//QUE TANTOS COMANDO DEL TP HACE FALTA MOSTRAR ?--> LOS REVISAN
void shellMain(char *command, char *param, int * esc) {
        if (strcmp(command,"HELP") == 0){
            getHelp();
        }
        else if (strcmp(command,"DATETIME")==0){
            printDateTime();
        }
        else if(strcmp(command,"CLEAR")==0){
            scClear();
        }
        else if (strcmp(command, "PRINTMEM") == 0)
        {
            if (param[0]=='0'&& param[1]=='x')
                printMem(param);
            else printf("Ingrese un parametro valido en hexa.\n");
        }
        else if (strcmp(command, "INFOREG") == 0){
            printRegisters();
        }
        else if (strcmp(command,"DIVEX") == 0){
            divExc();
        }
        else if(strcmp(command,"OPEX")==0){
            opCodeExc();
        }
        // else if (strcmp(command,"GAMES")==0){ //VUELVA
        //     initGames();
        // }
        else if (strcmp(command,"MEM")==0){
            print_mm();
        }
        else if(strcmp(command, "TESTMM") == 0){
            test_mm(1, (char**) MAX_MEMORY); //RECIBE ARGUMENTOS
        }
        else if(strcmp(command, "TESTPCS") == 0){
            test_processes(1, (char **)10); //RECIBE ARGUMENTOS
        }
        else if(strcmp(command, "TESTPCS") == 0){
            test_prio(); 
        }
        else if (strcmp(command, "EXIT") == 0){
            *esc = 1;
            return;
        }
        else printf("Comando invalido.\n Escriba HELP para mas informacion.\n");  
    return;
    //COMANDOS QUE FALTAN: VER EN PRINT DE HELP
}
