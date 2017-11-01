#include<sys/timer.h>
#include<sys/ioPort.h>
#include<sys/kprintf.h>
#include<sys/keyboard.h>

extern int counter;

struct idt_entry_struct{
	uint16_t addrLow;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t addrMid;
	uint32_t addrHi;
	uint32_t reserved;
}__attribute__((packed));

struct idt_ptr_struct {
	uint16_t limit;
	uint64_t idtEntryArray ;
}__attribute__((packed));

struct idt_entry_struct idt_array[256];
struct idt_ptr_struct idt_ptr;

void handleTimerInterrupt(){
	timer_callback();	
	writeToPort(0x20,0x20);
}
void timerInterrupt(){
		__asm__ __volatile__(
                        "    pushq %%rax;\n" 
                        "    pushq %%rdi;\n" 
                        "    pushq %%rsi;\n" 
                        "    pushq %%rdx;\n" 
                        "    pushq %%r8;\n" 
                        "    pushq %%r9;\n" 
                        "    pushq %%r10;\n"
			"    movq  %%rsp,%%rdi;\n"
                        "    call handleTimerInterrupt;\n"	
                        "    popq %%r10;\n" 
                        "    popq %%r9;\n" 
                        "    popq %%r8;\n" 
                        "    popq %%rdx;\n" 
                        "    popq %%rsi;\n" 
                     "    popq %%rdi;\n"
			"    popq %%rax;\n"
          "iretq;":::"rax","rdi","rsi","rdx","r8","r9","r10");
}

void handleKeyboardInterrupt(){
	keyPressCallback();
	writeToPort(0x20,0x20);

}

void KeyboardInterrupt(){
                __asm__ (
              "movq $0, %%rax"
             :::"rax" );
                __asm__ __volatile__(
                        "    pushq %%rax;\n"
                        "    pushq %%rdi;\n"
                        "    pushq %%rsi;\n"
                        "    pushq %%rdx;\n"
                        "    pushq %%r8;\n"
                        "    pushq %%r9;\n"
                        "    pushq %%r10;\n"
                        "    movq  %%rsp,%%rdi;\n"
                        "    call handleKeyboardInterrupt;\n"
                        "    popq %%r10;\n"
                        "    popq %%r9;\n"
                        "    popq %%r8;\n"
                        "    popq %%rdx;\n"
                        "    popq %%rsi;\n"
                     "    popq %%rdi;\n"
                        "    popq %%rax;\n"
          "iretq;":::"rax","rdi","rsi","rdx","r8","r9","r10");
}

void initializeInterrupt(uint64_t functionAddress,int interruptNumber){
	idt_array[interruptNumber].selector=0x08;
	idt_array[interruptNumber].flags=0x8E;
	idt_array[interruptNumber].zero=0;
	
	idt_array[interruptNumber].addrLow=functionAddress & 0xFFFF;
        idt_array[interruptNumber].addrMid=(functionAddress>>16) & 0xFFFF;
        idt_array[interruptNumber].addrHi=(functionAddress>>32) & 0xFFFFFFFF;
}

void initializeIdtArray(){
	idt_ptr.limit=sizeof(struct idt_ptr_struct)*256-1;
	idt_ptr.idtEntryArray=(uint64_t)&idt_array;
	for(int i=0;i<256;i++)
	initializeInterrupt((uint64_t)&timerInterrupt,32);
	initializeInterrupt((uint64_t)&KeyboardInterrupt,33);
	__asm__(
                "lidt (%0);\n"
                :
                :"r"(&idt_ptr)
       );
}

