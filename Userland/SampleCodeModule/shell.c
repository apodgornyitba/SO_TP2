// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// #include <shell.h>
// #include <lib.h>
// #include <libc.h>
// #include <commands.h>

// //?--> CAMBIARON TODA LA IMPLEMENTACION PARA QUE SE HAGA CON PROCESOS (PIPES Y SEMAFOROS)
// #define MAX_MEMORY (96*1024*1024)

// //QUE TANTOS COMANDO DEL TP HACE FALTA MOSTRAR ?--> LOS REVISAN
// void shellMain(char *command, char *param, int * esc) {
//         if (strcmp(command,"HELP") == 0){
//             getHelp();
//         }
//         else if (strcmp(command,"DATETIME")==0){
//             printDateTime();
//         }
//         else if(strcmp(command,"CLEAR")==0){
//             scClear();
//         }
//         else if (strcmp(command, "PRINTMEM") == 0)
//         {
//             if (param[0]=='0'&& param[1]=='x')
//                 printMem(param);
//             else printf("Ingrese un parametro valido en hexa.\n");
//         }
//         else if (strcmp(command, "INFOREG") == 0){
//             printRegisters();
//         }
//         else if (strcmp(command,"DIVEX") == 0){
//             divExc();
//         }
//         else if(strcmp(command,"OPEX")==0){
//             opCodeExc();
//         }
//         // else if (strcmp(command,"GAMES")==0){ //VUELVA
//         //     initGames();
//         // }
//         else if (strcmp(command,"MEM")==0){
//             print_mm();
//         }
//         else if(strcmp(command, "TESTMM") == 0){
//             test_mm(1, (char**) MAX_MEMORY); //RECIBE ARGUMENTOS
//         }
//         else if(strcmp(command, "TESTPCS") == 0){
//             test_processes(1, (char **)10); //RECIBE ARGUMENTOS
//         }
//         else if(strcmp(command, "TESTPCS") == 0){
//             test_prio(); 
//         }
//         else if (strcmp(command, "EXIT") == 0){
//             *esc = 1;
//             return;
//         }
//         else printf("Comando invalido.\n Escriba HELP para mas informacion.\n");  
//     return;
//     //COMANDOS QUE FALTAN: VER EN PRINT DE HELP
// }
#include <commands.h>
#include <libc.h>
#include <tests.h>
#include <phylo.h>
#include <shell.h>
#include <stdint.h>
#include <lib.h>

#define USERLAND_INIT_PID 1

static int getCommandArgs(char *userInput, char *argv[MAX_ARGUMENTS]);
static void shellExecute();
static int getCommandIdx(char *command);
static void changeUser(int argc, char **argv);
static void help(int argc, char **argv);
static void helpTest(int argc, char **argv);
static void helpShell(int argc, char **argv);
static void printHelpTable();
static void printHelpTestTable();

static int findPipe(int argc, char **argv);
static void initializePipe(int pipeIndex, int argc, char **argv);
static int handlePipe(int pipeIndex, int argc, char **argv);
static int runPipeCmd(int argc, char **argv, int fdin, int fdout, int foreground);

static int pipeId = 70;

void startShell(int argc, char **argv) {
  scClear();
  printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP \n");
  my_kill(USERLAND_INIT_PID);
  shellExecute();
}

static void shellExecute() {
  char input[BUFFER_SIZE] = {0};
  int foreground;

  while (1) {
    int argc;
    int pipeIndex;
    input[0] = 0;
    char *argv[MAX_ARGUMENTS] = {0};
    foreground = 1;

    if (scanf("%s", input) >= BUFFER_SIZE) {
      printf("\nMaximo de caracteres permitidos para input: %d\n\n", BUFFER_SIZE - 1);
      continue;
    }

    argc = getCommandArgs(input, argv);

    if (argc == -1) {
      printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos" "permitida es: %d.\n\n",MAX_ARGUMENTS);
    }

    pipeIndex = findPipe(argc, argv);

    if (pipeIndex >= 0) {
      initializePipe(pipeIndex, argc, argv);
      continue;
    }

    if (argv[argc - 1][0] == '&') {
      foreground = BACKGROUND;
      argc--;
    }

    int commandIdx = getCommandIdx(argv[0]);

    if (commandIdx >= 0) {
      newProcess(
          (void (*)(int, char **))shellData.commands[commandIdx].commandFn,
          argc, (char **)argv, foreground, NULL);
    } else {
      printf("\nComando invalido: use /help\n\n");
    }
  }
}

static int getCommandArgs(char *userInput, char **argv) {
  int argc = 0;

  if (*userInput != ' ' && *userInput != '\0') {
    argv[argc++] = userInput;
  }

  while (*userInput != 0) {
    if (*userInput == ' ') {
      *userInput = 0;
      if ((*(userInput + 1) != ' ') && (*(userInput + 1) != 0)) {
        if (argc >= MAX_ARGUMENTS) {
          return -1;
        }
        argv[argc++] = userInput + 1;
      }
    }
    userInput++;
  }
  return argc;
}

static int findPipe(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "|") == 0) {
      return i;
    }
  }
  return -1;
}

static void initializePipe(int pipeIndex, int argc, char **argv) {
  if (pipeIndex == 0 || pipeIndex == argc - 1) {
    printf("\nPipe (|) debe ser usado entre dos comandos.\n\n");
    return;
  }
  int pipe = handlePipe(pipeIndex, argc, argv);
  if (pipe == -1) {
    printf("\nUno de los comandos es invalido. Use /help.\n\n");
    return;
  }
}

static int handlePipe(int pipeIndex, int argc, char **argv) {
  char *currentArgv[MAX_ARGUMENTS];
  int currentArgc = 0;
  int pids[2];

  int pipe = pipeOpen(pipeId++);
  if (pipe == -1) {
    printf("\nError creating pipe.\n");
    return -2;
  }

  for (int i = pipeIndex + 1, j = 0; i < argc; i++, j++) {
    currentArgv[j] = argv[i];
    currentArgc++;
  }

  pids[0] = runPipeCmd(currentArgc, currentArgv, pipe, 1, BACKGROUND);
  if (pids[0] == -1) {
    pipeClose(pipe);
    return -1;
  }
  
  currentArgc = 0;
  for (int i = 0; i < pipeIndex; i++) {
    currentArgv[i] = argv[i];
    currentArgc++;
  }
  
  pids[1] = runPipeCmd(currentArgc, currentArgv, 0, pipe, FOREGROUND);
  if (pids[1] == -1) {
    pipeClose(pipe);
    return -1;
  }
  
  int endOfFile = EOF;
  pipeWrite(pipe, (char *)&endOfFile);

  pipeClose(pipe);
  putChar('\n');
  return 1;
}

static int runPipeCmd(int argc, char **argv, int fdin, int fdout, int foreground) {
  int fd[2];
  int commandIdx = getCommandIdx(argv[0]);
  if (commandIdx == -1) {
    return -1;
  }

  fd[0] = fdin;
  fd[1] = fdout;

  return newProcess(shellData.commands[commandIdx].commandFn, argc, argv,
                    foreground, fd);
}

static void printHelpTable() {
  printDivider(1, C1_WIDTH, C2_WIDTH);
  setBGC(WHITE);
  setFTC(BLACK);
  printCenteredHeading(1, "Lista de comandos");
  setFTC(WHITE);
  setBGC(BLACK);

  printDivider(0, C1_WIDTH, C2_WIDTH);
  for (int i = 0; i < TEST_COMMAND_START; i++) {
    printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }
  printDivider(0, C1_WIDTH, C2_WIDTH);
  setBGC(WHITE);
  setFTC(BLACK);

  printCenteredHeading(1, "Ejemplos de uso:  /c1 | /c2   /c1 &   /c1 arg1 ...");
  printCenteredHeading(1,
                       "Use /helptest para obtener informacion de los tests");
  printCenteredHeading(1,
                       "Use /helpshell para obtener informacion de la shell");
  setFTC(WHITE);
  setBGC(BLACK);

  printDivider(1, C1_WIDTH, C2_WIDTH);
}

static void printHelpTestTable() {
  printDivider(1, C1_WIDTH, C2_WIDTH);
  printCenteredHeading(1, "Lista de tests");
  printDivider(0, C1_WIDTH, C2_WIDTH);
  for (int i = TEST_COMMAND_START; i < COMMAND_COUNT - 2; i++) {
    printRow(shellData.commands[i].name, shellData.commands[i].description, 1);
  }

  printDivider(0, C1_WIDTH, C2_WIDTH);
}

static int getCommandIdx(char *command) {
  for (int i = 0; i < COMMAND_COUNT; i++) {
    if ((strcmp(shellData.commands[i].name, command)) == 0) {
      return i;
    }
  }
  return -1;
}

static void help(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }

  printHelpTable();
}

static void helpShell(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }

  printf("\n");
  setBGC(WHITE);
  setFTC(BLACK);
  printCenteredHeading(0, "Instructivo para manejo de la shell");
  setFTC(WHITE);
  setBGC(BLACK);
  printf("\nUse Ctrl + TAB para cambiar de pantalla.\n");
  printf("Use Ctrl + C para terminar el proceso actual.\n");
  printf("Use Ctrl + R para capturar el valor de los registros\n");
  printf(
      "Use Ctrl + D para obtener resultados en comandos como \n/wc o "
      "/filter\n");

  printf("\n");
  setBGC(WHITE);
  setFTC(BLACK);
  printCenteredHeading(0, "Tabla de colores");
  setFTC(WHITE);

  setBGC(BLACK);
  printf("\n");

  printc(WHITE, "                      Blanco   1\n");
  printf("                      ");
  setBGC(WHITE);
  printc(BLACK, "Negro    2\n");
  setBGC(BLACK);
  printc(RED, "                      Rojo     3\n");
  printc(GREEN, "                      Verde    4\n");
  printc(BLUE, "                      Azul     5\n");
  printf("\n");
}

static void helpTest(int argc, char **argv) {
  if (checkArgcWrapper(argc, 1) == -1) {
    return;
  }
  printHelpTestTable();
}

static void changeUser(int argc, char **argv) {
  if (checkArgcWrapper(argc, 2) == -1) {
    return;
  }
  if (strlen(argv[1]) > USER_SIZE - 1) {
    printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n",
           USER_SIZE - 1);
    return;
  }
  strcpy(shellData.userName, argv[1]);
  setFirstChange(1);
}