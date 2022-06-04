#include <scheduler.h>
#include <lib.h>

//LINK DE APOYO: https://www.geeksforgeeks.org/program-round-robin-scheduling-set-1/

static uint64_t currentPID = 0;
static ProcessList *processes;
static Process *currentProcess;
static uint64_t cyclesLeft;
static Process *baseProcess;

static int queueIsEmpty();
static uint64_t getNewPID();
static void freeProcess(Process *process);

static void processQueue(Process * newProcess)
{
      if (queueIsEmpty())
      {
            processes->first = newProcess;
            processes->last = processes->first;
      }
      else
      {
            processes->last->next = (struct Process *) newProcess;
            newProcess->next = NULL;
            processes->last = newProcess;
      }

      if (newProcess->state == READY)
            processes->readySize++;

      processes->size++;
}

static Process *processDequeue()
{
      if (queueIsEmpty())
            return NULL;

      Process *p = processes->first;
      processes->first = (Process *) processes->first->next;
      processes->size--;

      if (p->state == READY)
            processes->readySize--;

      return p;
}

static int queueIsEmpty()
{
      return processes->size == 0;
}

static Process *getProcessOfPID(uint64_t pid)
{
      if (currentProcess != NULL && currentProcess->pcb.pid == pid)
            return currentProcess;

      for (Process *p = processes->first; p != NULL; p = (Process *) p->next)
            if (p->pcb.pid == pid)
                  return p;

      return NULL;
}

uint64_t setNewState(uint64_t pid, State newState)
{

      Process *process = getProcessOfPID(pid);

      if (process == NULL || process->state == KILLED)
            return -1;

      if (process == currentProcess)
      {
            process->state = newState;
            return process->pcb.pid;
      }

      if (process->state != READY && newState == READY)
            processes->readySize++;

      if (process->state == READY && newState != READY)
            processes->readySize--;

      process->state = newState;

      return process->pcb.pid;
}

uint64_t killProcess(uint64_t pid)
{
      if (pid <= 2)
            return -1;

      int aux = setNewState(pid, KILLED);

      if (pid == currentProcess->pcb.pid)
            callTimerTick();

      return aux;
}

uint64_t blockProcess(uint64_t pid)
{
      int aux = setNewState(pid, BLOCKED);

      if (pid == currentProcess->pcb.pid)
            callTimerTick();
      return aux;
}

uint64_t unblockProcess(uint64_t pid)
{
      return setNewState(pid, READY);
}

static void exit()
{
      killProcess(currentProcess->pcb.pid);
      callTimerTick();
}


static void haltFunc(int argc, char **argv)
{
      while (1)
            _hlt();
}

static int argsCopy(char **buffer, char **argv, int argc)
{
      for (int i = 0; i < argc; i++)
      {
            buffer[i] = my_malloc(sizeof(char) * (strlength(argv[i]) + 1));
            strcopy(buffer[i],argv[i]);
      }
      return 1;
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv)
{
      entryPoint(argc, argv);
      exit();
}

static void setStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *rbp)
{
      StackFrame *frame = (StackFrame *)rbp - 1;
      frame->gs = 0x001;
      frame->fs = 0x002;
      frame->r15 = 0x003;
      frame->r14 = 0x004;
      frame->r13 = 0x005;
      frame->r12 = 0x006;
      frame->r11 = 0x007;
      frame->r10 = 0x008;
      frame->r9 = 0x009;
      frame->r8 = 0x00A;
      frame->rsi = (uint64_t)argc;
      frame->rdi = (uint64_t)entryPoint;
      frame->rbp = 0x00D;
      frame->rdx = (uint64_t)argv;
      frame->rcx = 0x00F;
      frame->rbx = 0x010;
      frame->rax = 0x011;
      frame->rip = (uint64_t)wrapper;
      frame->cs = 0x008;
      frame->eflags = 0x202;
      frame->rsp = (uint64_t)(&frame->base);
      frame->ss = 0x000;
      frame->base = 0x000;
}

static int createPCB(PCB *process, char *name, int fg, int *fd)
{
      strcopy(process->name, name);
      process->pid = getNewPID();

      process->ppid = currentProcess == NULL ? 0 : currentProcess->pcb.pid;
      if (fg > 1 || fg < 0)
            return -1;
      // If i have a parent and he is not in the foreground, then I can not be in the foreground either
      process->foreground = currentProcess == NULL ? fg : (currentProcess->pcb.foreground ? fg : 0);
      process->rbp = my_malloc(STACK_SIZE);
      process->priority = process->foreground ? FOREGROUND_PRIORITY : BACKGROUND_PRIORITY;
      process->fd[0] = fd ? fd[0] : 0;
      process->fd[1] = fd ? fd[1] : 1;

      if (process->rbp == NULL)
            return -1;

      process->rbp = (void *)((char *)process->rbp + STACK_SIZE - 1);
      process->rsp = (void *)((StackFrame *)process->rbp - 1);
      return 0;
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int foreground, int *fd)
{
      if (entryPoint == NULL)
            return -1;

      Process *newProcess = my_malloc(sizeof(Process));

      if (newProcess == NULL){
            return -1;
      }

      if (createPCB(&newProcess->pcb, argv[0], foreground, fd) == -1)
      {
            my_free(newProcess);

            return -1;
      }

      char **argvCopy = my_malloc(sizeof(char *) * argc);
      if (argvCopy == 0){
            return -1;
      }
      argsCopy(argvCopy, argv, argc);

      newProcess->pcb.argc = argc;
      newProcess->pcb.argv = argvCopy;

      setStackFrame(entryPoint, argc, argvCopy, newProcess->pcb.rbp);

      newProcess->state = READY;
      processQueue(newProcess);
      if (newProcess->pcb.foreground && newProcess->pcb.ppid){
            blockProcess(newProcess->pcb.ppid);
      }
      return newProcess->pcb.pid;
}

void initScheduler()
{
      processes = my_malloc(sizeof(ProcessList));

      if (processes == NULL)
            return;

      processes->first = NULL;
      processes->last = processes->first;
      processes->size = 0;
      processes->readySize = 0;

      char *argv[] = {"Halt Process"};
      addProcess(&haltFunc, 1, argv, 0, 0);
      baseProcess = processDequeue();
}

void *scheduler(void *oldRSP)
{
      if (currentProcess)
      {
            if (currentProcess->state == READY && cyclesLeft > 0)
            {
                  cyclesLeft--;
                  return oldRSP;
            }

            currentProcess->pcb.rsp = oldRSP;

            if (currentProcess->pcb.pid != baseProcess->pcb.pid) //baseProcess should never be pushed into the queue
            {
                  if (currentProcess->state == KILLED)
                  {
                        Process *parent = getProcessOfPID(currentProcess->pcb.ppid);
                        if (parent != NULL && currentProcess->pcb.foreground && parent->state == BLOCKED)
                        {
                              unblockProcess(parent->pcb.pid);
                        }
                        freeProcess(currentProcess);
                  }
                  else
                        processQueue(currentProcess);
            }
      }

      if (processes->readySize > 0)
      {
            currentProcess = processDequeue();
            while (currentProcess->state != READY)
            {

                  if (currentProcess->state == KILLED)
                  {
                        freeProcess(currentProcess);
                  }
                  if (currentProcess->state == BLOCKED)
                  {
                        processQueue(currentProcess);
                  }
                  currentProcess = processDequeue();
            }
      }
      else{
            currentProcess = baseProcess;
      }
      cyclesLeft = currentProcess->pcb.priority;
      return currentProcess->pcb.rsp;
}

static uint64_t getNewPID()
{
      return currentPID++;
}

static void freeProcess(Process *process)
{
      for (int i = 0; i < process->pcb.argc; i++)
            my_free(process->pcb.argv[i]);
      my_free(process->pcb.argv);
      my_free((void *)((char *)process->pcb.rbp - STACK_SIZE + 1));
      my_free((void *)process);
}

char *stateToStr(State state)
{
      switch (state)
      {
      case READY:
            return "R";
            break;
      case BLOCKED:
            return "B";
      default:
            return "K";
            break;
      };
}

void printProcess(Process *process)
{

      if (process != NULL)
      {
            sysWrite(2, (uint64_t) "\n", 2, 0, 0);
      }
      print("\n%d        %d        %x        %x        %s            %s\n", process->pcb.pid, (int)process->pcb.foreground,
            (uint64_t)process->pcb.rsp, (uint64_t)process->pcb.rbp, stateToStr(process->state), process->pcb.name);
}

void processDisplay()
{
      const char * message = "PID      FG       RSP              RBP              STATE        NAME";
      const int length = strlength(message); 
      
      sysWrite(2, (uint64_t)message, length, 0, 0);


      if (currentProcess != NULL){
            printProcess(currentProcess);
      }
           

      Process *curr = processes->first;
      while (curr)
      {
            printProcess(curr);
            curr = (Process *) curr->next;
      }
}

int getCurrPID()
{
      return currentProcess ? currentProcess->pcb.pid : -1;
}

void setPriority(uint64_t pid, int priority)
{

      if (priority < 0)
            priority = 0;
      if (priority > MAX_PRIORITY)
            priority = MAX_PRIORITY;

      Process *p = getProcessOfPID(pid);

      if (p != NULL)
            p->pcb.priority = priority;
}

void killFgProcess()
{
      if (currentProcess != NULL && currentProcess->pcb.foreground && currentProcess->state == READY)
      {
            killProcess(currentProcess->pcb.pid);
            return;
      }
}

void yield()
{
      cyclesLeft = 0;
      callTimerTick();
}

int currentReadsFrom()
{
      if (currentProcess)
      {
            return currentProcess->pcb.fd[0];
      }
      return -1;
}

int currentWritesTo()
{
      if (currentProcess)
            return currentProcess->pcb.fd[1];
      return -1;
}

int currentProcessFg()
{
      if (currentProcess)
            return currentProcess->pcb.foreground;
      return -1;
}

void waitForPID(uint64_t pid)
{
      Process *process = getProcessOfPID(pid);
      if (process)
      {
            process->pcb.foreground = 1;
            blockProcess(currentProcess->pcb.pid);
      }
}