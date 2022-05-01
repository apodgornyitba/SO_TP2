#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */


DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt() {

  _cli();
  
  setup_IDT_entry(0x00, (uint64_t)&_exception0Handler); //Div por cero
  setup_IDT_entry(0x06, (uint64_t)&_exception6Handler); //Op invalido

  setup_IDT_entry (0x21, (uint64_t)&_irq01Handler); //Teclado
  setup_IDT_entry(0x20, (uint64_t)&_irq00Handler); //tick

  setup_IDT_entry(0x80, (uint64_t)&_syscallHandler); 
  
  // A common mistake is that people reload the mask with 0xFE when they want to add timer,
  // but doing this actually enables only the timer and disables the keyboard (bit #1 of 0xFE is set!)
  // The correct value for enabling both keyboard and timer is 0xFC.
	picMasterMask(0xFC); 
	picSlaveMask(0xFF);
        
	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}
