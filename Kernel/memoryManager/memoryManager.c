#ifdef FREE_MM

// LAS FUNCIONES SON BASADAS DEL LIBRO (EL LENGUJAE DE PROGRAMACION C DE K&R)

#include <memoryManager.h>

#define NULL 0
#define NALLOC 1024 /* minimo # de unidades por requerir */

typedef long Align;     /* para alineamiento al limite mayor */
typedef union header Header;

union header     /* encabezado del bloque */
{
      struct
      {
            union header *ptr;     /* siguiente bloque si esta en la lista libre */
            unsigned size;      /* tamano de este bloque */
      } s;
      Align x;      /* obliga a la alineacion de bloques */
};

static Header *base;     /* lista vacia para iniciar */
static Header *freep = NULL;      /* inicio de una lista libre */

unsigned long totalUnits;
unsigned long fbr = 0;

void memInit(char *memBase, unsigned long memSize)
{
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      freep = base = (Header *)memBase;
      freep->s.size = totalUnits;
      freep->s.ptr = freep;
      fbr = freep->s.size;  //cuando toda la memoria esta libre
}

/* malloc: asignador de almacenamiento de proposito general */
void *my_malloc(unsigned long nbytes)
{
     if (nbytes == 0)
            return NULL;

      Header *p, *prevp;
      prevp = freep;
      unsigned long nunits;

      nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
      for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
      {
            if (p->s.size >= nunits)
            {                                 /* suficientemente grande */
                  if (p->s.size == nunits) /* exacto */
                        prevp->s.ptr = p->s.ptr;
                  else                 /* asigna la parte final */
                  { 
                        p->s.size -= nunits;
                        p += p->s.size;
                        p->s.size = nunits;
                  }
                  freep = prevp;
                  return (void *)(p + 1);
            }
            if (p == freep)    /* dio la vuelta a la lista libre */
                  return NULL; /* nada libre */
                        
      }
      fbr -= p->s.size;
}

/* free: coloca el bloque ap en la lista vacia */
void my_free(void* ap)
{

    if (ap == NULL || (((long)ap - (long)base) % sizeof(Header)) != 0)
            return;

      Header *bp, *p;
      p=freep;
      bp = (Header *)ap - 1; /* apunta al encabezador de un bloque */
      
      if (bp < base || bp >= (base + totalUnits * sizeof(Header)))
            return;

      fbr += p->s.size;
      
      for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr){
            if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)){
                  break;            /* libera bloque al inicio o al final */
            }
                  
      }
      if (bp + bp->s.size == p->s.ptr) /* une al nbr superior */
      { 
            bp->s.size += p->s.ptr->s.size;
            bp->s.ptr = p->s.ptr->s.ptr;
      }
      else
            bp->s.ptr = p-> s.ptr;
      if (p + p->s.size == bp)     /* une al nbr inferior */
      { 
            p->s.size += bp->s.size;
            p->s.ptr = bp->s.ptr;
      }
      else
            p->s.ptr = bp;
      freep = p;
}

void printMemState() //CAMBIAR A PASAJE DE ESTRUCTURA E IMPRESION EN EL USERLAND
{
      char buffer[20];
      sysWrite(2, (uint64_t) "Total Mem: ", 11, 0, 0);
      intToHexa( totalUnits * sizeof(Header), buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Used Mem: ", 10, 0, 0);
      intToHexa( (totalUnits * sizeof(Header)) - fbr, buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Free Mem: ", 10, 0, 0);
      intToHexa( fbr, buffer, 8);
      sysWrite(2, (uint64_t) buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);
}

#endif