#ifdef BUDDY_MM
// LINK DE APOYO: https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

#include <memoryManager.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ALLOC_LOG2 4 // The minimum allocation size is 16 bytes.
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)
#define MAX_ALLOC_LOG2 31 // The maximum allocation size is currently set to 2gb.
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1) //Allocations are done in powers of two starting from MIN_ALLOC and ending at MAX_ALLOC inclusive.

typedef struct list_t {
      unsigned int bucket;
      unsigned int free;
      struct list_t *prev, *next;
} list_t;

static list_t buckets[BUCKET_COUNT];
static size_t bucket_limit;
static list_t *base;
static unsigned int buckets_amount;
static unsigned long used_memory = 0;

static void list_init(list_t *list) {
  list->prev = list;
  list->next = list;
}

static void list_push(list_t *list, list_t *entry) {
  list_t *prev = list->prev;
  entry->prev = prev;
  entry->next = list;
  prev->next = entry;
  list->prev = entry;
}

static void list_remove(list_t *entry) {
  list_t *prev = entry->prev;
  list_t *next = entry->next;
  prev->next = next;
  next->prev = prev;
}

static list_t *list_pop(list_t *list) {
  list_t *back = list->prev;
  if (back == list) 
      return NULL;
  list_remove(back);
  return back;
}

int is_empty(list_t *list)
{
    return list->prev == list;
}

static void addNodeToBucket(list_t *bucketList, list_t *node, uint8_t bucketLevel) {
  node->bucket = bucketLevel;
  node->free = 1;
  list_push(bucketList, node);
}

static size_t getIdealBucket(size_t request) {
  size_t requestLog2 = log2(request);

  if (requestLog2 < MIN_ALLOC_LOG2) {
    return 0;
  }

  requestLog2 -= MIN_ALLOC_LOG2;

  if (request && !(request & (request - 1))) {
    return requestLog2;
  }

  return requestLog2 + 1;
}

static int getAvailableBucket(uint8_t minBucketRequired) {
  uint8_t availableBucket;

  for (availableBucket = minBucketRequired; availableBucket < buckets_amount && is_empty(&buckets[availableBucket]); availableBucket++)
    ;

  if (availableBucket > buckets_amount) {
    return -1;
  }

  return availableBucket;
}

static list_t *getBuddy(list_t *node) {
  uint8_t bucket = node->bucket;
  uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
  uintptr_t newOffset = currentOffset ^ (1 << (MIN_ALLOC_LOG2 + bucket));

  return (list_t *)((uintptr_t)base + newOffset);
}

static list_t *getAddress(list_t *node) {
  uint8_t bucket = node->bucket;
  uintptr_t mask = (1 << (MIN_ALLOC_LOG2 + bucket));
  mask = ~mask;

  uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
  uintptr_t newOffset = currentOffset & mask;

  return (list_t *)(newOffset + (uintptr_t)base);
}

void memInit(char *memBase, unsigned long memSize) {
  if (memBase == NULL) {
    return;
  }

  base = (list_t *)memBase;
  bucket_limit = memSize;

  buckets_amount = (int)log2(memSize) - MIN_ALLOC_LOG2 + 1;

  if (buckets_amount > BUCKET_COUNT) {
    buckets_amount = BUCKET_COUNT;
  }

  for (int i = 0; i < buckets_amount; i++) {
    list_init(&buckets[i]);
    buckets[i].free = 0;
    buckets[i].bucket = i;
  }

  addNodeToBucket(&buckets[buckets_amount - 1], base, buckets_amount - 1);
}

void *malloc(unsigned long nbytes) {
  size_t bytesNeeded = nbytes + sizeof(list_t);

  if (nbytes == 0 || bytesNeeded > bucket_limit + 1) {
    return NULL;
  }

  uint8_t idealBucket = getIdealBucket(bytesNeeded);
  int availableBucket = getAvailableBucket(idealBucket);

  if (availableBucket == -1) {
    return NULL;
  }

  used_memory += bytesNeeded;

  list_t *resultNode = list_pop(&buckets[availableBucket]);

  for (; idealBucket < availableBucket; availableBucket--) {
    resultNode->bucket--;
    addNodeToBucket(&buckets[availableBucket - 1], getBuddy(resultNode), availableBucket - 1);
  }

  resultNode->free = 0;

  return (void *)(resultNode + 1);
}

void free(void *block) {
  if (block == NULL) {
    return;
  }

  //?--> PREGUNTAR: Agregar un used_memory -= cantidad de memoria liberada

  list_t *freeNode = (list_t *)block - 1;

  freeNode->free = 1;

  list_t *freeBuddy = getBuddy(freeNode);

  while (freeNode->bucket != buckets_amount - 1 && freeBuddy->bucket == freeNode->bucket && freeBuddy->free) {
    list_remove(freeBuddy);
    freeNode = getAddress(freeNode);
    freeNode->bucket++;
    freeBuddy = getBuddy(freeNode);
  }

  list_push(&buckets[freeNode->bucket], freeNode);
}

void printMemState()
{
      char buffer[20];
      sysWrite(2, (uint64_t) "Total Mem: ", 11, 0, 0);
      intToHexa( MAX_ALLOC, buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Used Mem: ", 10, 0, 0);
      intToHexa( used_memory, buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Free Mem: ", 10, 0, 0);
      intToHexa( MAX_ALLOC - used_memory, buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);
}

#endif