#include<sys/defs.h>
#include<sys/kprintf.h>

#ifndef __IOPORT_H
#define __IOPORT_H


//void writeToPort(uint16_t port, uint8_t value);
void writeToPort( unsigned short port, unsigned char val );
unsigned char inb( unsigned short port );
#endif

