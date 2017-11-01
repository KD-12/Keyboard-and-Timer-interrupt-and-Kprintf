#ifndef _IDT_H
#define _IDT_H
#include<sys/defs.h>

void init_idt();
void idt_set_gate(u8int irqNumber, u32int address, u16int segmentSelector, u8int flags);

#endif
