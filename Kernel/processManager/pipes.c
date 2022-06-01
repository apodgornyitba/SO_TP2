#include <pipes.h>
#include <lib.h>
#include <semaphores.h>

#define MAX_PIPES 8

Pipe pipes[MAX_PIPES];

int initialSemId = 500;

static int createPipe(int pipeId);
static int pipeWriter(char c, int idx);
static int getNewIndex();
static int getIndex(int pipeId);

int pipeOpen(int pipeId) {
  int idx;
  if ((idx = getIndex(pipeId)) == -1) {
    idx = createPipe(pipeId);
    if (idx == -1) {
      return -1;
    }
  }
  pipes[idx].totalProcesses++;
  return pipeId;
}

int pipeWrite(int pipeId, char *str) {
  int idx = getIndex(pipeId);
  if (idx == -1) {
    return -1;
  }
  int i = 0;
  while (str[i] != 0) {
    pipeWriter(str[i], idx);
    i++;
  }
  return pipeId;
}

int pipeRead(int pipeId) {
  int idx = getIndex(pipeId);

  if (idx == -1) {
    return -1;
  }

  Pipe *pipe = &(pipes[idx]);

  char c;

  semWait(pipe->readLock);

  c = pipe->buffer[pipe->readIndex];
  // circular buffer
  pipe->readIndex = (pipe->readIndex + 1) % PIPE_BUFFER_SIZE;

  semPost(pipe->writeLock);

  return c;
}

int pipeClose(int pipeId) {
  int idx = getIndex(pipeId);
  if (idx == -1) {
    return -1;
  }

  Pipe *pipe = &(pipes[idx]);

  pipe->totalProcesses--;

  if (pipe->totalProcesses > 0) {
    return 1;
  }

  semClose(pipe->readLock);
  semClose(pipe->writeLock);

  pipe->state = EMPTY;

  return 1;
}

static int pipeWriter(char c, int idx) {
  Pipe *pipe = &(pipes[idx]);

  semWait(pipe->writeLock);

  pipe->buffer[pipe->writeIndex] = c;
  // circular buffer
  pipe->writeIndex = (pipe->writeIndex + 1) % PIPE_BUFFER_SIZE;

  semPost(pipe->readLock);

  return 0;
}

static int getIndex(int pipeId) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].state == IN_USE && pipes[i].id == pipeId) {
      return i;
    }
  }
  return -1;
}

static int getNewIndex() {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].state == EMPTY) {
      return i;
    }
  }
  return -1;
}

static int createPipe(int pipeId) {
  int idx;

  if ((idx = getNewIndex()) == -1) {
    return -1;
  }

  Pipe *pipe = &(pipes[idx]);

  pipe->id = pipeId;
  pipe->state = IN_USE;
  pipe->readIndex = pipe->writeIndex = pipe->totalProcesses = 0;

  if ((pipe->readLock = semOpen(initialSemId++, 0)) == -1) {
    return -1;
  }
  if ((pipe->writeLock = semOpen(initialSemId++, PIPE_BUFFER_SIZE)) == -1) {
    return -1;
  }

  return pipeId;
}

void pipeStatus() {
  // char tmpBuffer[20];
  // sysWrite(2,(uint64_t) "Active Pipe Status\n", 20,0,0);
  print("%s", "Active Pipe Status\n");
  
  for (int i = 0; i < MAX_PIPES; i++) {
    Pipe pipe = pipes[i];
    if (pipe.state == IN_USE) {
      // sysWrite(2, (uint64_t)"\n     Pipe ID: ", 16, 0, 0);
      print("%s %d","\n     Pipe ID: ", pipe.id);
      // sysWrite(2, (uint64_t) pipe.id, strlength(intToStr(pipe.id, tmpBuffer, 10)), 0, 0);
      // sysWrite(2, (uint64_t)"\n     Amount of attached processes: ",37, 0,0);
      print("%s %d", "\n     Amount of attached processes: ", pipe.totalProcesses);
      // sysWrite(2, (uint64_t) pipe.totalProcesses, strlength(intToStr(pipe.totalProcesses, tmpBuffer, 10)), 0, 0);
      // sysWrite(2, (uint64_t)"\n     Read semaphore: ", 23,0,0);
      print("%s %d", "\n     Read semaphore: ", pipe.readLock);
      // // sysWrite(2, (uint64_t) pipe.readLock, strlength(intToStr(pipe.readLock, tmpBuffer, 10)), 0, 0);
      // sysWrite(2, (uint64_t)"\n     Write semaphore: ", 23,0,0);
      print("%s %d", "\n     Write semaphore: ", pipe.writeLock);
      // sysWrite(2, (uint64_t)  pipe.writeLock, strlength(intToStr(pipe.writeLock, tmpBuffer, 10)), 0, 0);
      // sysWrite(2, (uint64_t)"\n     Pipe buffer content: ",27,0,0);
      print("%s", "\n     Pipe buffer content: ");
      for (int i = pipe.readIndex; i != pipe.writeIndex;
           i = (i + 1) % PIPE_BUFFER_SIZE) {
             print("%c",  pipe.buffer[i]);
            //  sysWrite(2, (uint64_t) pipe.buffer[i], 1, 0, 0);
      }
    }
  }
  sysWrite(2,(uint64_t)"\n",2,0,0);
}