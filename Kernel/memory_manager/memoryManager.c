#ifdef FREE_MM

// LAS FUNCIONES SON BASADAS DEL LIBRO (EL LENGUJAE DE PROGRAMACION C DE K&R)

#include <memoryManager.h>
#include <syscalls.h>

#define NULL 0
#define NALLOC 1024 /* minimo # de unidades por requerir */
//#define sizeof(type) (char *)(&type+1)-(char*)(&type)

typedef long Align;     /* para alineamiento al limite mayor */

union header     /* encabezado del bloque */
{
      struct
      {
            union header *ptr;     /* siguiente bloque si esta en la lista libre */
            unsigned size;      /* tamano de este bloque */
      } s;
      Align x;      /* obliga a la alineacion de bloques */
};

typedef union header Header;

static Header base;     /* lista vacia para iniciar */
static Header *freep = NULL;      /* inicio de una lista libre */

unsigned long totalUnits;
unsigned long fbr = 0;

void memInit(char *memBase, unsigned long memSize)
{
      // Initially its all a very large block
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      freep = base = (Header *)memBase;
      freep->s.size = totalUnits;
      freep->s.ptr = freep;
      fbr = freep->s.size;  //cuando toda la memoria esta libre
}



/* malloc: asignador de almacenamiento de proposito general */
void *malloc(unsigned long nbytes)
{
     if (nbytes == 0)
            return NULL;

      Header *p, *prevp;
      // Header *morecore(unsigned);
      unsigned long nunits;

      nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
      if ((prevp = freep) == NULL)       /* no hay lista libre aun */
      { 
            base.s.ptr = freep = prevp = &base;
            base.s.size = 0;
      }
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
            if (p == freep)    /* dio la vuelta a !a lista libre */
                  /*if ((p = morecore(nunits)) = = NULL)*/
                        return NULL; /* nada libre */
                        
      }
      fbr -= p->s.size;
}

/* morecore: solicita mas memoria al sistema */
// static Header *morecore(unsigned nu)
// {
//       char *cp, *sbrk(int);
//       Header *up;
      
//       if (nu < NALLOC)
//             nu = NALLOC;
//       cp = sbrk(nu * sizeof(Header));
//       if (cp == (char *) -1) /* no hay nada de espacio */
//             return NULL;
//       up = (Header *)cp;
//       up->s.size = nu;
//       free((void *)(up + 1));
//       return freep;
// }

/* free: coloca el bloque ap en la lista vacia */
void free(void *ap)
{

    if (ap == NULL || (((long)ap - (long)base) % sizeof(Header)) != 0)
            return;

      Header *bp, *p;
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

void printMemState()
{
      char *buffer[20];
      sysWrite(2, (uint64_t) "Total Mem: ", 11, 0, 0);
      intToHexa( totalUnits * sizeof(Header), buffer, 8);
      sysWrite(2, buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Used Mem: ", 10, 0, 0);
      intToHexa( (totalUnits * sizeof(Header)) - fbr, buffer, 8);
      sysWrite(2, buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);

      sysWrite(2, (uint64_t) "Free Mem: ", 10, 0, 0);
      intToHexa( fbr, buffer, 8);
      sysWrite(2, buffer, 20, 0, 0);
      sysWrite(2, (uint64_t) " Bytes\n", 8, 0, 0);
}

#endif