#ifndef _IDT_LOADER_
#define _IDT_LOADER_

#include <stdint.h>

void load_idt();
void irqDispatcher(uint64_t irq, uint64_t rsp);

#endif