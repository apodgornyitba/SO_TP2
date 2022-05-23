#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphores.h>

Semaphore *semaphores;

static Semaphore *getSemaphore(uint32_t id);
static Semaphore *createSemaphore(uint32_t id, uint64_t initialValue);
static void addSemaphoreToList(Semaphore *newSem);
static void unblockSemProcess(Semaphore *sem);
static void removeSemaphore(Semaphore *sem);
static void blockedProcessesDump(int *blockedProcesses, uint16_t blockedProcessesAmount);

void acquire(int *lock) {
  while (_xchg(lock, 1) != 0)
    ;
}

void release(int *lock) { _xchg(lock, 0); }

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

void semStatus() {
  sysWrite(2,"Active Semaphore Status\n",25,0,0);
  Semaphore *sem = semaphores; 
  while (sem) {
    sysWrite(2, (uint64_t)"Semaphore ID: ", 15, 0, 0);
    sysWrite(2, (uint64_t) sem->id, strlength(sem->id), 0, 0);
    sysWrite(2, (uint64_t)"\n     Value: ", 14, 0, 0);
    sysWrite(2, (uint64_t) sem->value, strlength(sem->value), 0, 0);
    sysWrite(2, (uint64_t)"\n     Attached processes amount: ", 34, 0, 0);
    sysWrite(2, (uint64_t) sem->listeningProcesses, strlength(sem->listeningProcesses), 0, 0);
    sysWrite(2, (uint64_t)"\n     Blocked processes amount: ", 33, 0, 0);
    sysWrite(2, (uint64_t) sem->blockedProcessesAmount, strlength(sem->blockedProcessesAmount), 0, 0);
    sysWrite(2, (uint64_t)"\n     Blocked processes: ", 26, 0, 0);
    blockedProcessesDump(sem->blockedProcesses, sem->blockedProcessesAmount);
    sem = sem->next;
  }
}

static void blockedProcessesDump(int *blockedProcesses, uint16_t blockedProcessesAmount) {
  for (int i = 0; i < blockedProcessesAmount; i++) {
    sysWrite(2, (uint64_t)"         PID: ",15,0,0);
    sysWrite(2, (uint64_t) blockedProcesses[i]), strlength(blockedProcesses[i]),0,0;
  }
  sysWrite(2,"\n",1,0,0);
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
  free(sem);
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