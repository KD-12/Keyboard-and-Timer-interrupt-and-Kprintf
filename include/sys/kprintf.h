#ifndef __KPRINTF_H
#define __KPRINTF_H
#include<stdarg.h>

void kprintf(const char *fmt, ...);
void putOnScreen(char * input);
void shiftRowsUp(int count);
void writeTimeElapsed(unsigned int hours,unsigned int minutes,unsigned int seconds);
void writeKeyPressed(char c);
#endif
