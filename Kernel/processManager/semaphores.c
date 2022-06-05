// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphores.h>

//LINK DE APOYO: https://eng.libretexts.org/Bookshelves/Computer_Science/Operating_Systems/Book%3A_Think_OS_-_A_Brief_Introduction_to_Operating_Systems_(Downey)/11%3A_Semaphores_in_C/11.03%3A_Make_your_own_semaphores

Semaphore *semaphores;

static Semaphore *getSemaphore(uint32_t id);
static Semaphore *createSemaphore(uint32_t id, uint64_t initialValue);
static void addSemaphoreToList(Semaphore *newSem);
static void removeSemaphore(Semaphore *sem);
static void blockedProcessesDump(int *blockedProcesses, uint16_t blockedProcessesAmount);
static void unblockSem(Semaphore *sem);

void acquire(int *lock)
{
  while (_xchg(lock, 1) != 0)
    ;
}

void release(int *lock)
{
  _xchg(lock, 0);
}

int semOpen(uint32_t id, uint64_t initialValue) {
  Semaphore *semaphore = getSemaphore(id);

  if (semaphore == NULL) {
    semaphore = createSemaphore(id, initialValue);
    if (semaphore == NULL) {
      return -1;
    }
  }

  if (semaphore->listeningProcesses >= MAX_BLOCKED_PROCESSES) {
    return -1;
  }

  semaphore->listeningProcesses++;
  return id;
}

int semWait(uint32_t id) {
  Semaphore *sem;
  if ((sem = getSemaphore(id)) == NULL) {
    return -1;
  }

  acquire(&(sem->lock));
  if (sem->value > 0) {
    sem->value--;
    release(&(sem->lock));
  } else {
    int currentPID = getCurrPID();
    sem->blockedProcesses[sem->blockedProcessesAmount++] = currentPID;
    release(&(sem->lock));
    blockProcess(currentPID);
  }
  return 0;
}

int semPost(uint32_t id) {
  Semaphore *sem;
  if ((sem = getSemaphore(id)) == NULL) {
    return -1;
  }

  acquire(&(sem->lock));
  if (sem->blockedProcessesAmount > 0) {
    unblockSem(sem);
  } else {
    sem->value++;
  }

  release(&(sem->lock));
  return 0;
}

int semClose(uint32_t id) {
  Semaphore *sem;
  if ((sem = getSemaphore(id)) == NULL) {
    return -1;
  }

  if (sem->listeningProcesses > 0) {
    sem->listeningProcesses--;
  }

  if (sem->listeningProcesses == 0) {
    removeSemaphore(sem);
  }

  return 0;
}

void semStatus()
{
  sysWrite(2, (uint64_t) "\n", 2, 0, 0);
  print("%s", "Active Semaphore Status");
  Semaphore *sem = semaphores;
  while (sem)
  {
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    print("%s %d", "     Semaphore ID:", sem->id);
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    print("%s %d", "     Value: ", sem->value);
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    print("%s %d", "     Number of attached processes: ", sem->listeningProcesses);
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    print("%s %d", "     Number of blocked processes: ", sem->blockedProcessesAmount);
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    print("%s", "     Blocked processes:");
    sysWrite(2, (uint64_t) "\n", 2, 0, 0);
    blockedProcessesDump(sem->blockedProcesses, sem->blockedProcessesAmount);
    sem = sem->next;
  }
}

static void blockedProcessesDump(int * blockedProcesses, uint16_t blockedProcessesAmount) {
  char tmpBuffer[20];
  for (int i = 0; i < blockedProcessesAmount; i++) {
    sysWrite(2, (uint64_t)"         PID: ",15,0,0);
    sysWrite(2, (uint64_t) blockedProcesses[i], strlength(intToStr(blockedProcesses[i], tmpBuffer, 10)),0,0);
  }
  sysWrite(2,(uint64_t) "\n", 2, 0, 0);
}

static void removeSemaphore(Semaphore *sem) {
  Semaphore *semListAux = semaphores;
  if (sem == semListAux) {
    semaphores = semListAux->next;
  } else {
    while (semListAux->next != sem) {
      semListAux = semListAux->next;
    }
    semListAux->next = sem->next;
  }
  my_free(sem);
}

static Semaphore *createSemaphore(uint32_t id, uint64_t initialValue) {
  Semaphore *newSem = my_malloc(sizeof(Semaphore));
  if (newSem != NULL) {
    newSem->id = id;
    newSem->value = initialValue;
    newSem->blockedProcessesAmount = 0;
    newSem->listeningProcesses = 0;
    newSem->lock = 0;
    newSem->next = NULL;
    addSemaphoreToList(newSem);
  }
  return newSem;
}

static void addSemaphoreToList(Semaphore *newSem) {
  Semaphore *tailSem = semaphores;
  if (tailSem == NULL) {
    semaphores = newSem;
  } else {
    while (tailSem->next != NULL) {
      tailSem = tailSem->next;
    }
    tailSem->next = newSem;
  }
}

static Semaphore *getSemaphore(uint32_t id) {
  Semaphore *result = semaphores;
  while (result) {
    if (result->id == id) {
      return result;
    }
    result = result->next;
  }
  return NULL;
}

static void unblockSem(Semaphore *sem) {
  int readyPID = sem->blockedProcesses[0];
  for (int i = 0; i < sem->blockedProcessesAmount - 1; i++) {
    sem->blockedProcesses[i] = sem->blockedProcesses[i + 1];
  }
  sem->blockedProcessesAmount--;
  setNewState(readyPID, READY);
}