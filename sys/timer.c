#include<sys/ioPort.h>
#include<sys/defs.h>
#include<sys/kprintf.h>

#define COMMAND_REGISTER 0x43
#define CHANNEL_0_DATA_PORT 0x40

#define PIT_FREQUENCY 1193182
#define CHANNEL_0_MODE_2_COMMAND 0x36

uint32_t tick = 0;
int secondsElapsed=0;
void setTimer(uint32_t frequency)
{
	
	writeToPort(COMMAND_REGISTER,CHANNEL_0_MODE_2_COMMAND);
	uint16_t numberOfTicks = 1193182/ frequency;
	
	uint8_t lowerBits = (numberOfTicks) & 0xFF;
	uint8_t upperBits =(numberOfTicks >>8) & 0xFF;
	
	writeToPort(CHANNEL_0_DATA_PORT,lowerBits);
	writeToPort(CHANNEL_0_DATA_PORT,upperBits);
}
void timer_callback(){
	tick++;
	if((tick%100)==0){
		secondsElapsed++;
		int hours=secondsElapsed/3600;
		int minutes=(secondsElapsed/60)%60;
		int seconds=secondsElapsed%60;
		writeTimeElapsed(hours,minutes,seconds);
		tick=0;
	}
}
