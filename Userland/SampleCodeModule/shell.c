// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <libc.h>
#include <tests.h>
#include <phylo.h>
#include <shell.h>
#include <stdint.h>
#include <lib.h>

#define USERLAND_INIT_PID 1

static int getCommandArgs(char *userInput, char *argv[MAX_ARGUMENTS]);
static int getCommandIdx(char *command);
static int findPipe(int argc, char **argv);
static void initializePipe(int pipeIndex, int argc, char **argv);
static int handlePipe(int pipeIndex, int argc, char **argv);
static int runPipeCmd(int argc, char **argv, int fdin, int fdout, int foreground);

static int pipeId = 70;

void startShell(int argc, char **argv) {
  scClear();
  printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP \n");
  shellExecute();
}

void shellExecute() {
  char input[BUFFER_SIZE] = {0};
  int foreground;

  while (1) {
    printf("$ >   ");
    int argc = 0;
    int pipeIndex;
    input[0] = 0;
    char *argv[MAX_ARGUMENTS] = {0};
    foreground = 1;

    if (scanf("%s", input) >= BUFFER_SIZE) {
      printf("\nMaximo de caracteres permitidos para input: %d\n", BUFFER_SIZE - 1);
      continue;
    }
    argc = getCommandArgs(input, argv);
    
    if (argc == -1) {
      printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n",MAX_ARGUMENTS);
    }

    pipeIndex = findPipe(argc, argv);

    if (pipeIndex >= 0) {
      initializePipe(pipeIndex, argc, argv);
      continue;
    }

    if (argv[argc - 1][0] == '&') {
      foreground = 0;
      argc--;
    }

    int commandIdx = getCommandIdx(argv[0]);

    if (commandIdx >= 0) {
      my_create_process(commands[commandIdx].command, argc, (char **)argv, foreground, NULL);
    } else {
      printf("\nComando invalido: use help\n");
    }
  }
}

static int getCommandArgs(char *input, char **argv)
{
  int argc = 0;

  if (*input != ' ' && *input != '\0')
  {
    argv[argc++] = input;
  }

  while (*input != '\0')
  {
    if (*input == ' ')
    {
      *input = 0;
      if ((*(input + 1) != ' ') && (*(input + 1) != '\0'))
      {
        if (argc >= MAX_ARGUMENTS)
        {
          return argc;
        }
        argv[argc++] = input + 1;
      }
    }
    input++;
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
    printf("\nPipe (|) debe ser usado entre dos comandos.\n");
    return;
  }
  int pipe = handlePipe(pipeIndex, argc, argv);
  if (pipe == -1) {
    printf("\nUno de los comandos es invalido: use help.\n");
    return;
  }
}

static int handlePipe(int pipeIndex, int argc, char **argv) {
  char *currentArgv[MAX_ARGUMENTS];
  int currentArgc = 0;
  int pids[2];

  int pipe = my_pipe_open(pipeId++);
  if (pipe == -1) {
    printf("\nError creating pipe.\n");
    return -2;
  }

  for (int i = pipeIndex + 1, j = 0; i < argc; i++, j++) {
    currentArgv[j] = argv[i];
    currentArgc++;
  }

  pids[0] = runPipeCmd(currentArgc, currentArgv, pipe, 1, 0);
  if (pids[0] == -1) {
    my_pipe_close(pipe);
    return -1;
  }
  
  currentArgc = 0;
  for (int i = 0; i < pipeIndex; i++) {
    currentArgv[i] = argv[i];
    currentArgc++;
  }
  
  pids[1] = runPipeCmd(currentArgc, currentArgv, 0, pipe, 1);
  if (pids[1] == -1) {
    my_pipe_close(pipe);
    return -1;
  }
  
  int endOfFile = -1;
  my_pipe_write(pipe, (char *)&endOfFile);
  my_pipe_close(pipe);
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

  return my_create_process(commands[commandIdx].command, argc, argv,foreground, fd);
}

static int getCommandIdx(char *command) {
  for (int i = 0; i < CMD_COUNT; i++) {
    if ((strcmp(commands[i].name, command)) == 0) {
      return i;
    }
  }
  return -1;
}