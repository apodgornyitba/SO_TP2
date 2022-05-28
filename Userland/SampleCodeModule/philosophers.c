// REALIZAR LA IMPLEMENTACION ?--> REALIZAR LAS MODIFICACIONES Y BUSCAR FUENTE (TODOS LO TIENEN IGUAL)

// https://github.com/pchapin/tutorialpthread/blob/master/philosophers.c

#include <phylo.h>

#define MAX_PHILOS 8
#define BASE_PHILOS 4
#define MUTEX_ID "999"
#define BASE_SEM_ID "1000"

typedef enum
{
  THINKING = 1,
  EATING = 2,
  HUNGRY = 3,
} STATE;

typedef struct Philosopher
{
  int pid;
  int sem;
  STATE state;
} Philosopher;

Philosopher *philos[MAX_PHILOS];
static int actualPhilosopherCount = 0;
static int tableMutex;
static int problemRunning;
static char *tmpBuffer;

#define RIGHT(i) ((i) + 1) % (actualPhilosopherCount)                         /* number of i’s right neighbor */
#define LEFT(i) ((i) + actualPhilosopherCount - 1) % (actualPhilosopherCount) /* number of i’s left neighbor */

void philo(int argc, char *argv[]);
void takeForks(int i);
void placeForks(int i);
void test(int i);
int addPhilosopher();
int removePhilosopher();
void printTable();

void philo(int argc, char *argv[])
{
  int idx = strToInt(argv[1]);
  while (problemRunning)
  {
    takeForks(idx);
    sleep(1);
    placeForks(idx);
    sleep(1);
  }
}

void takeForks(int i)
{
  my_sem_wait(intToStr(tableMutex, tmpBuffer, 10));
  philos[i]->state = HUNGRY;
  test(i);
  my_sem_post(intToStr(tableMutex, tmpBuffer, 10));
  my_sem_wait(intToStr(philos[i]->sem, tmpBuffer, 10));
}

void placeForks(int i)
{
  my_sem_wait(intToStr(tableMutex, tmpBuffer, 10));
  philos[i]->state = THINKING;
  test(LEFT(i));
  test(RIGHT(i));
  my_sem_post(intToStr(tableMutex, tmpBuffer, 10));
}

void test(int i)
{
  if (philos[i]->state == HUNGRY && philos[LEFT(i)]->state != EATING && philos[RIGHT(i)]->state != EATING)
  {
    philos[i]->state = EATING;
    my_sem_post(intToStr(philos[i]->sem, tmpBuffer, 10));
  }
}

void start_philosopher(int argc, char *argv[])
{
  problemRunning = 1;
  tableMutex = my_sem_open(MUTEX_ID, 1);
  printf("Welcome to the Philosophers Problem!\n");
  printf("You start with 4 philosophers and have a maximum of 8 philosophers.\n");
  printf("You can add them with \'a\', delete them with \'d\' and exit the problem with \'q\'.\n");
  printf("The state of each will be displayed as E (Eating) or . (HUNGRY)\n\n");

  printf("Waiting for phylos...\n\n");

  sleep(5);

  for (int i = 0; i < BASE_PHILOS; i++)
    addPhilosopher();
  char *args[] = {"PrintTable"};
  int printTablePid = my_create_process(&printTable, 1, args, 0, NULL);
  while (problemRunning)
  {

    char key = getChar();
    switch (key)
    {
    case 'a':
      if (addPhilosopher() == -1)
        printf("Can\'t add another philosopher. Maximum 8 philosophers.\n");
      else
        printf("A new philosopher joined!\n");
      break;
    case 'd':
      if (removePhilosopher() == -1)
        printf("Can\'t remove another philosopher. Minimum 4 philosophers.\n");
      else
        printf("One philosopher has left!\n");
      break;
    case 'q':
      printf("Program Finished!\n");
      problemRunning = 0;
      break;
    default:
      break;
    }
  }

  for (int i = 0; i < actualPhilosopherCount; i++)
  {
    my_sem_close(intToStr(philos[i]->sem, tmpBuffer, 10));
    my_kill(philos[i]->pid);
    my_free(philos[i]);
  }
  actualPhilosopherCount = 0;
  my_kill(printTablePid);
  my_sem_close(MUTEX_ID);
}

int addPhilosopher()
{
  if (actualPhilosopherCount == MAX_PHILOS)
    return -1;

  my_sem_wait(intToStr(tableMutex, tmpBuffer, 10));
  Philosopher *auxPhilo = my_malloc(sizeof(Philosopher));
  if (auxPhilo == NULL)
    return -1;
  auxPhilo->state = THINKING;
  auxPhilo->sem = my_sem_open(BASE_SEM_ID + actualPhilosopherCount, 1);
  char buffer[3];
  char *name[] = {"philosopher", intToStr(actualPhilosopherCount, buffer, 10)};
  auxPhilo->pid = my_create_process(&philo, 2, name, 0, NULL);
  philos[actualPhilosopherCount++] = auxPhilo;
  my_sem_post(intToStr(tableMutex, tmpBuffer, 10));
  return 0;
}

int removePhilosopher()
{
  if (actualPhilosopherCount == BASE_PHILOS)
  {
    return -1;
  }

  actualPhilosopherCount--;
  Philosopher *chosenPhilo = philos[actualPhilosopherCount];
  my_sem_close(intToStr(chosenPhilo->sem, tmpBuffer, 10));
  my_kill(chosenPhilo->pid);
  my_free(chosenPhilo);
  my_sem_post(intToStr(tableMutex, tmpBuffer, 10));

  return 0;
}

void printTable(int argc, char *argv[])
{
  while (problemRunning)
  {
    my_sem_wait(intToStr(tableMutex, tmpBuffer, 10));
    for (int i = 0; i < actualPhilosopherCount; i++)
    {
      philos[i]->state == EATING ? putChar('E') : putChar('.');
      putChar(' ');
    }
    putChar('\n');
    my_sem_post(intToStr(tableMutex, tmpBuffer, 10));
    my_yield();
  }
}