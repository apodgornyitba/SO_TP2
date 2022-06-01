// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <lib.h>
#include <libc.h>
#include <phylo.h>
#include <tests.h>

// #ifdef FREE_LIST
#define MAX_MEMORY (96 * 1024 * 1024)
static int isVowel(char c);
// #else
// #define MAX_MEMORY (32 * 1024 * 1024)
// #endif

static const char *registers[] = {"RAX:", "RBX:", "RCX:", "RDX:", "RBP:", "RDI:", "RSI:", "R8 :", "R9 :", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:"};

void getHelp() {
    printf("\n~ help: Informacion de comandos.\n");
    printf("~ time: Se imprime el tiempo y la fecha actual.\n");
    printf("~ clear: Se limpia la pantalla.\n");
    printf("~ inforeg: Se imprimen los valores de los registros. Utilizar la tecla CTRL + s para actualizar los valores de los registros\n");
    printf("~ divex: Para lanzar una excepcion por division por cero.\n");
    printf("~ opex: Para lanzar una excepcion por operador de codigo invalido.\n");
    //COMANDOS NUEVOS
    printf("~ mem: Imprime memoria total, libre y ocupada.\n"); //AREGLAR EL PRINT
    printf("~ ps: Imprime la lista de todos los proceso\n");
    printf("~ loop: Proceso loop que muestra current ID y un saludo\n");
    printf("~ kill: Mata un proceso dado su ID. Usar CTRL + c para terminar un proceso\n");
    printf("~ nice: Cambia la prioridad de un proceso, dado su ID y el nuevo nivel de prioridad\n");
    printf("~ block: Bloquea un proceso dado su ID\n");
    printf("~ unblock: Desbloquea un proceso dado su ID\n");
    printf("~ sem: Imprime la lista de todos los semaforos\n");
    printf("~ cat: Imprime standard input en standard output \n");
    printf("~ wc: Cuenta la cantidad de lineas de input\n");
    printf("~ filter: Filtra las vocales del input\n");
    printf("~ pipe: Imprime la lista de todos los pipes\n");
    printf("~ phylo: Problema de los filosofos comensales.\n");
    printf("APARTADO DE TESTEOS:\n");
    printf("~ test_mm: Testeo de Memory Manager. Metodo de uso: test_mm <MAX_MEMORY>\n");
    printf("~ test_prio: Testeo de prioridades\n");
    printf("~ test_processes: Testeo de procesos, scheduler y cambio de contexto\n");
    printf("~ test_sync: Testeo de sincronizacion de procesos\n");

}

void printDateTime() {
   printf("%d:%d:%d\n%d\\%d\\%d\n", DateTime(2), DateTime(1), DateTime(0),DateTime(3),DateTime(4),DateTime(5));
}

void clear() {
    scClear();
}

void printRegisters(){
    unsigned long long buff[17];
    char hexa[20];
    printf("\n");
    getRegs(buff);
    for (int i = 0; i <= 14; i++){
        intToHexa((long long)buff[14-i],hexa,8);
        printf("%s",registers[i]);
        printf("%s\n",hexa);
    }
    printf("\n");
    
}

void divExc(){
    int a = 1, b = 0;
    a = a/b;
}

void opExc() {
    opCodeExc();
}

void printMem(){
    print_mm();
    printf("\n\n");
}

void printPCS(){
    my_print_pcs();
    printf("\n\n");
}

void loop() {
    int t = 0;
    long pid = my_getpid();

    while(1){
        t = ticks();
        if(t % 18 == 0 ) {
            printf("PID: %d\n", pid);
     
        }  
    }
}
 
void killPCS(int argc, char ** argv){
    if (argc != 2){
        printf("Cantidad de parametros invalida. Metodo de uso: kill <PID>\n");
        return;
    }
    int pid = strToInt(argv[1]);
    // printf("ARGV EN KILL DE KERNEL: %c\n", *argv);
    // printf("PID EN KILL DEL KERNEL: %d\n", pid);
    my_kill(pid);
}

void nicePCS(int argc, char ** argv){
    if(argc != 3){
        printf("Cantidad de parametros invalida. Metodo de uso: nice <PID> <PRIORITY> \n");
        return;
    }
    int pid = strToInt(argv[1]);
    int prio = strToInt(argv[2]);
    my_nice(pid, prio);
}
//VER COMANDO
void blockPCS(int argc, char ** argv){
    if(argc != 2){
        printf("Cantidad de parametros invalida. Metodo de uso: block <PID>\n");
        return;
    }
    int pid = strToInt(argv[1]);
    my_block(pid);

}

void unblockPCS(int argc, char ** argv){
    if(argc != 2){
        printf("Cantidad de parametros invalida. Metodo de uso: unblock <PID>\n");
        return;
    }
    int pid = strToInt(argv[1]);
    my_unblock(pid);
}

void printSems(){
    // if((my_sem_open(8, 1)) == -1){
    //     return;
    // }
    my_print_sems();
    printf("\n\n");
}

void cat(int argc, char ** argv){
    

    if(argc != 1){
        printf("Cantidad de parametros invalida. Metodo de uso: cat\n");
        return;
    }
    
    char c[2] = {0};
    while(1){
        c[0] = getChar();
        // printf("HOLA\n");
        putChar(c[0]);
        c[0] = 0;
    }
}

void wc(int argc, char **argv)
{

    if (argc != 1)
    {
        printf("Cantidad de parametros invalida. Metodo de uso: wc\n");
        return;
    }

    int count = 1;
    char c[2] = {0};

    while (1)
    {
        c[0] = getChar();
        putChar(c[0]);
        if (c[0] == '\n')
        {
            count++;
        }
        c[0] = 0;
    }
    printf("Cantidad de lineas: %d \n", count);
}

void filter(int argc, char ** argv){
    
    if(argc != 1){
        printf("Cantidad de parametros invalida. Metodo de uso: filter\n");
        return;
    }
    
    char c[2] = {0};
    while(1){
        c[0] = getChar();
        if(!isVowel(c[0])){
            putChar(c[0]);
        }
        c[0] = 0;
    }
}

static int isVowel(char c){
    if(c >= 'A' && c <= 'Z'){
        c += 'a' - 'A';
    }
    if( c== 'a' || c == 'e' || c == 'i' || c== 'o' || c == 'u') {
        return 1;
    }
    return 0;
}

void printPipes(){
    // printf("Se abre y cierra un pipe con id 70 en forma de prueba\n\n");
    // if(my_pipe_open(70) == -1){
    //     return;
    // }

    my_print_pipes();
    printf("\n\n");

    // if(my_pipe_close(70) == -1){
    //     return;
    // }
}


void philosopher(int argc, char ** argv){
    start_philosopher(argc, argv);
}

void testMM(int argc, char ** argv){
    if(argc != 2){
        printf("Cantidad de parametros invalida. Metodo de uso: test_mm <MAX_MEMORY>\n");
        return;
    }
    test_mm(1, argv);  //RECIBE ARGUMENTOS
}

void testPrio(int argc, char ** argv){
    test_prio();
}

void testPCS(int argc, char ** argv){
    test_processes(1, (char **)10); //RECIBE ARGUMENTOS
}
void testSync(int argc, char ** argv){
    test_sync(argc, argv);
}


