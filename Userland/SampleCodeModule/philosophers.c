// https://github.com/pchapin/tutorialpthread/blob/master/philosophers.c

#include <phylo.h>

#define MAX_PHILOS 8
#define BASE_PHILOS 4
#define MUTEX_ID 999
#define BASE_SEM_ID 1000

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
  printf("\n%d\n", idx);
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
  my_sem_wait(tableMutex);
  philos[i]->state = HUNGRY;
  test(i);
  my_sem_post(tableMutex);
  my_sem_wait(philos[i]->sem);
}

void placeForks(int i)
{
  my_sem_wait(tableMutex);
  philos[i]->state = THINKING;
  test(LEFT(i));
  test(RIGHT(i));
  my_sem_post(tableMutex);
}

void test(int i)
{
  if (philos[i]->state == HUNGRY && philos[LEFT(i)]->state != EATING && philos[RIGHT(i)]->state != EATING)
  {
    philos[i]->state = EATING;
    my_sem_post(philos[i]->sem);
  }
}

void start_philosopher(int argc, char *argv[])
{
  problemRunning = 1;
  tableMutex = my_sem_open(MUTEX_ID, 1);
  printf("Bienvenido al problema de los filosofos\n");
  printf("El problema empieza con 4 filosofos y hay un maximo de 8 filosofos\n");
  printf("Puedes agregarlos usando la tecla \'a\', sacarlos con la tecla \'d\' y salir con la tecla \'q\'.\n");
  printf("El estado de cada uno se muestra como E (Comiendo o Eating) o . (Hambriento o Hungry)\n\n");

  printf("Esperando por los filosofos...\n\n");

  // printf("me rompo aca1\n");

  sleep(5);

  // printf("me rompo aca2\n");

  for (int i = 0; i < BASE_PHILOS; i++){
    // printf("me rompo aca1\n");

    addPhilosopher();
    
    // printf("me rompo aca2\n");

  }
  // printf("me rompo aca3\n");

  char *args[] = {"PrintTable"};
  int printTablePid = my_create_process(&printTable, 1, args, 0, NULL);
  while (problemRunning)
  {

    char key = getChar();
    switch (key)
    {
    case 'a':
      if (addPhilosopher() == -1)
        printf("No se puede agregar otro filosofo. Hay un maximo de 8 filosofos.\n");
      else
        printf("Se unio un nuevo filosofo\n");
      break;
    case 'd':
      if (removePhilosopher() == -1)
        printf("No se puede sacar otro filosofo. Hay un minimo de 4 filosofos.\n");
      else
        printf("Se saco un filosofo\n");
      break;
    case 'q':
      printf("Programa terminado\n");
      problemRunning = 0;
      break;
    default:
      break;
    }
  }

  for (int i = 0; i < actualPhilosopherCount; i++)
  {
    my_sem_close(philos[i]->sem);
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

  my_sem_wait(tableMutex);
  Philosopher *auxPhilo = my_malloc(sizeof(Philosopher));
  if (auxPhilo == NULL)
    return -1;
  auxPhilo->state = THINKING;
  auxPhilo->sem = my_sem_open(BASE_SEM_ID + actualPhilosopherCount, 1);
  char buffer[3];
  char *name[] = {"philosopher", intToStr(actualPhilosopherCount, buffer, 10)};
  auxPhilo->pid = my_create_process(&philo, 2, name, 0, NULL);
  philos[actualPhilosopherCount++] = auxPhilo;
  my_sem_post(tableMutex);
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
  my_sem_close(chosenPhilo->sem);
  my_kill(chosenPhilo->pid);
  my_free(chosenPhilo);
  my_sem_post(tableMutex);

  return 0;
}

void printTable(int argc, char *argv[])
{
  while (problemRunning)
  {
    my_sem_wait(tableMutex);
    for (int i = 0; i < actualPhilosopherCount; i++)
    {
      philos[i]->state == EATING ? putChar('E') : putChar('.');
      putChar(' ');
    }
    putChar('\n');
    my_sem_post(tableMutex);
    my_yield();
  }
}