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

int getHelp() {
    printf("\n~ help: Informacion de comandos.\n");
    printf("~ datetime: Se imprime el tiempo y la fecha actual.\n");
    printf("~ clear: Se limpia la pantalla.\n");
    printf("~ inforeg: Se imprimen los valores de los registros. Utilizar la tecla CTRL para actualizar los valores\n");
    printf("~ divex: Para lanzar una excepcion por division por cero.\n");
    printf("~ opex: Para lanzar una excepcion por operador de codigo invalido.\n");
    //COMANDOS NUEVOS
    printf("~ printmm: Imprime memoria total, libre y ocupada.\n");
    printf("~ ps: Imprime la lista de todos los proceso\n");
    printf("~ loop: Proceso loop que muestra current ID y un saludo\n");
    printf("~ kill: Mata un proceso dado su ID\n");
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
    printf("~ testmm: Testeo de Memory Manager\n");
    printf("~ testprio: Testeo de prioridades\n");
    printf("~ testpcs: Testeo de procesos, scheduler y cambio de contexto\n");
    printf("~ testsync: Testeo de sincronizacion de procesos\n");
    printf("~ exit: Abortar la ejecucion.\n");
    return 0;

}

int printDateTime() {
   printf("%d:%d:%d\n%d\\%d\\%d\n", DateTime(2), DateTime(1), DateTime(0),DateTime(3),DateTime(4),DateTime(5));
   return 0;
}

int clear() {
    scClear();
    return 0;
}

int printRegisters(){
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
    return 0;
}

int divExc(){
    int a = 1, b = 0;
    a = a/b;
    return 0;
}

int opExc() {
    opCodeExc();
    return 0;
}

int printMem(){
    print_mm();
    return 0;
}

int printPCS(){
    my_print_pcs();
    return 0;
}

int loop() {
    int t = 0;
    long pid = my_getpid();

    while(1){
        t = ticks();
        if(t % 18 == 0 ) {
            printf("PID: %d\n", pid);
        }
    }
    return 0;
}
 
int killPCS(int argc, char ** argv){
    if (argc != 2){
        printf("Invalid amount of arguments. Use: kill <PID>\n");
        return -1;
    }
    //ARREGLAR STRTOINT PARA QUE RECIBA UN SOLO PARAMETRO
    int pid = strToInt(argv[1]);
    my_kill(pid);
    return 0;
}

int nicePCS(int argc, char ** argv){
    if(argc != 3){
        printf("Invalid amount of arguments. Use: nice <PID> <PRIORITY>\n");
        return -1;
    }
    int pid = strToInt(argv[1]);
    int prio = strToInt(argv[2]);
    my_nice(pid, prio);
    return 0;
}
//VER COMANDO
int blockPCS(int argc, char ** argv){
    if(argc != 2){
        printf("Invalid amount of arguments. Use: block <PID>\n");
        return -1;
    }
    int pid = strToInt(argv[1]);
    my_block(pid);
    return 0;
}

int unblockPCS(int argc, char ** argv){
    if(argc != 2){
        printf("Invalid amount of arguments. Use: unblock <PID>\n");
        return -1;
    }
    int pid = strToInt(argv[1]);
    my_unblock(pid);
    return 0;
}

int printSems(){
    my_print_sems();
    return 0;
}

int cat(int argc, char ** argv){

    if(argc != 1){
        printf("Invalid amount of arguments. Use: cat\n");
        return -1;
    }
    
    int c;

    while((c=getChar()) != -1){
        putChar(c);
    }
    return 0;
}

int wc(int argc, char ** argv){
    
    if(argc != 1){
        printf("Invalid amount of arguments. Use: wc\n");
        return -1;
    }
    
    int c;
    int count = 1;

    while((c=getChar()) != -1){
        putChar(c);
        if((char) c == '\n')
            count++;
    }
    printf("\n Cantidad de lineas: %d\n", count);
    return 0;
}

int filter(int argc, char ** argv){
    
    if(argc != 1){
        printf("Invalid amount of arguments. Use: filter\n");
        return -1;
    }
    
    int c;
    while((c=getChar()) != -1){
        if(isVowel(c)){
            putChar(c);
        }
    }
    return 0;
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

int printPipes(){
    my_print_pipes();
    return 0;
}


int philosopher(int argc, char ** argv){
    start_philosopher(argc, argv);
    return 0;
}

int testMM(int argc, char ** argv){
    test_mm(1, (char**) MAX_MEMORY); //RECIBE ARGUMENTOS
    return 0;
}

int testPrio(int argc, char ** argv){
    test_prio();
    return 0;
}

int testPCS(int argc, char ** argv){
    test_processes(1, (char **)10); //RECIBE ARGUMENTOS
    return 0;
}

int testSync(int argc, char ** argv){
    test_sync(argc, argv);
    return 0;
}


