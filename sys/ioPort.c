#include<sys/defs.h>
#include<sys/kprintf.h>

void writeToPort( unsigned short port, unsigned char val )
{
    __asm__ __volatile__ ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

unsigned char inb( unsigned short port )
	{
	    unsigned char ret;
	    __asm__ __volatile__ ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	    return ret;
	}
