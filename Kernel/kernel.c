// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <graphicRenderer.h>
#include <screenDriver.h>
#include <kbDriver.h>
#include <syscallDispatcher.h>
#include <rtc.h>
#include <memoryManager.h>
#include <scheduler.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const sampleCodeModuleHeapAddress = (void*)0x600000;

#define HEAP_MEMORY_SIZE (64*1024*1024)


typedef int (*EntryPoint)();



void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	// char buffer[10];
	

	// ncPrint("[x64BareBones]");
	// ncNewline();

	// ncPrint("CPU Vendor:");
	// ncPrint(cpuVendor(buffer));
	// ncNewline();

	// ncPrint("[Loading modules]");
	// ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	// ncPrint("[Done]");
	// ncNewline();
	// ncNewline();

	// ncPrint("[Initializing kernel's binary]");
	// ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	// ncPrint("  text: 0x");
	// ncPrintHex((uint64_t)&text);
	// ncNewline();
	// ncPrint("  rodata: 0x");
	// ncPrintHex((uint64_t)&rodata);
	// ncNewline();
	// ncPrint("  data: 0x");
	// ncPrintHex((uint64_t)&data);
	// ncNewline();
	// ncPrint("  bss: 0x");
	// ncPrintHex((uint64_t)&bss);
	// ncNewline();

	// ncPrint("[Done]");
	// ncNewline();
	// ncNewline();
	// ncClear();

	//NUESTROS INITS
	// initVideo();
	// initKb();


	return getStackBase();
}

int main()
{	
	// ncPrint("[Kernel Main]");
	// ncNewline();
	// ncPrint("  Sample code module at 0x");
	// ncPrintHex((uint64_t)sampleCodeModuleAddress);
	// ncNewline();
	// ncPrint("  Calling the sample code module returned: ");
	// ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	// ncNewline();
	// ncNewline();

	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)sampleDataModuleAddress);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)sampleDataModuleAddress);
	// ncNewline();

	// ncPrint("[Finished]");
	// ncClear();

	
	load_idt();
	initVideo();
	memInit((char *) sampleCodeModuleHeapAddress, HEAP_MEMORY_SIZE);
	initScheduler();
	initKb();

	sysWrite(2, (uint64_t)"\nHOLA3", 7, 0,0);

	char * argV[] = {"Shell init"};
	addProcess(sampleCodeModuleAddress, 1, argV, 1, 0);
	
	
	_hlt();


	sysWrite(2, (uint64_t)"\nHOLA4", 7, 0,0);


	return 0;
}
