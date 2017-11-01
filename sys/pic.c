#include<sys/ioPort.h>
#include<sys/kprintf.h>

// To wait while I/O is happening
void io_wait( void )
{
    __asm__ __volatile__ ( "jmp 1f\n\t"
                  "1:jmp 2f\n\t"
                  "2:" );
}

void initializePIC(){
	
	

	uint8_t masterMask;
	uint8_t slaveMask;
	masterMask=inb(0x21);
	slaveMask=inb(0xA1);

	// instruct the PIC to start initialization
	writeToPort(0x20,0x11);
	io_wait();	
	writeToPort(0xA0,0x11);
	io_wait();
	
	//set the offset to 32 and 39
	writeToPort(0x21,0x20);
        io_wait();
	writeToPort(0xA1,0x28);
        io_wait();
	// setup the master slave config
	 writeToPort(0x21,0x04);
        io_wait();
        writeToPort(0xA1,0x02);
        io_wait();
	//setup 8086 mode
	 writeToPort(0x21,0x01);
        io_wait();
        writeToPort(0xA1,0x01);
        io_wait();
	
	writeToPort(0x21,masterMask);
        writeToPort(0xA1,slaveMask);
	
}
