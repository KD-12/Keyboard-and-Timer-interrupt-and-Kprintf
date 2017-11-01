#include <sys/kprintf.h>
#include <stdarg.h>
#include <sys/keyboard.h>
//#include "timer.h"
#define WIDTH 80
#define HEIGHT 24

#define INTEGER_CODE 1
#define HEX_CODE 2
#define STRING_CODE 3
#define CHAR_CODE 4
#define POINTER_CODE 5

#define TAB_CODE 101

extern void irq0();
int presentColumn=0;
int presentRow=0;
char *screenBasePointer=(char *)0xb8000;
void copyString(char *input, char *output){
        while((*(output++)=*(input++))!='\0');
}
int isWhiteSpace(char c){
        if(c==' '||c=='\t'||c=='\n'|| c=='\r'||c=='\v'||c=='\f'){
        return 1;
        }
        return 0;
}

int specialSequenceCode(const char *fmt){
	if(*fmt=='%' && *(fmt+1))
	{
		switch(*(fmt+1)){
			case 'd':
			return INTEGER_CODE;
			case 'x':
			return HEX_CODE;
			case 's':
			return STRING_CODE;
			case 'c':
			return CHAR_CODE;
			case 'p':
			return POINTER_CODE;
		}
	}
	else{
		switch(*fmt){
			case '\t':
			return TAB_CODE;
		}
	}
	return 0;
}
 void  num_conversion_unsigned(unsigned long  n, int b ,char *input)
 {
   char print_from[20] ={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','\0'};
 char storage[100];
 char * p =&storage[99];
   *p='\0';
   if(n==0){
	 p--;
     *p='0';		
   }
   while(n != 0)
   {
     p--;
     *p=print_from[n%b];
      n=n/b ;

   }
  copyString(p,input); 
 }

void num_conversion(long n,int b,char *input){
 int negNumber=n<0;
 if(negNumber){
  *input='-';
   input++; 
   n*=-1;
 }
 
 num_conversion_unsigned(n,b,input);
}
void writeTimeElapsed(unsigned int hours,unsigned int minutes,unsigned int seconds){
        int yCordinate=HEIGHT;
        int xCordinate=WIDTH/2;
        char *message="Time elapsed since boot :";
	for(int i=xCordinate;i<WIDTH;i++)
	{
	*(screenBasePointer+160*yCordinate+2*i)=' ';
	}
        while(*message)
        {
                 *(screenBasePointer+160*yCordinate+2*xCordinate)=*message;
                xCordinate++;
                message++;
        }
        char hoursS[5];
        char minutesS[5];
        char secondsS[5];
        num_conversion_unsigned(hours,10,hoursS);
        num_conversion_unsigned(minutes,10,minutesS);
         num_conversion_unsigned(seconds,10,secondsS);
        for(int i=0;hoursS[i];i++)
        {
                *(screenBasePointer+160*yCordinate+2*xCordinate)=hoursS[i];
                xCordinate++;
        }
	*(screenBasePointer+160*yCordinate+2*xCordinate)='h';
         xCordinate++;
        *(screenBasePointer+160*yCordinate+2*xCordinate)=':';
         xCordinate++;
        for(int i=0;minutesS[i];i++)
        {
                *(screenBasePointer+160*yCordinate+2*xCordinate)=minutesS[i];
                xCordinate++;
        }
	*(screenBasePointer+160*yCordinate+2*xCordinate)='m';
         xCordinate++;
        *(screenBasePointer+160*yCordinate+2*xCordinate)=':';
         xCordinate++;
         for(int i=0;secondsS[i];i++)
        {
                *(screenBasePointer+160*yCordinate+2*xCordinate)=secondsS[i];
                xCordinate++;
        }
	*(screenBasePointer+160*yCordinate+2*xCordinate)='s';
         xCordinate++;


}

void writeKeyPressed(char c){
	int yCordinate=HEIGHT-1;
        int xCordinate=(WIDTH/2);
	char *message="Last Key Pressed is:";
	for(int i=xCordinate;i<WIDTH;i++)
        {
        	*(screenBasePointer+160*yCordinate+2*i)=' ';
        }
        while(*message)
        {
                *(screenBasePointer+160*yCordinate+2*xCordinate)=*message;
                xCordinate++;
                message++;
        }
	if(controlPressed){
		*(screenBasePointer+160*yCordinate+2*xCordinate)= '^';
		xCordinate++;
		controlPressed = 0;
	}
	*(screenBasePointer+160*yCordinate+2*xCordinate)= c;
	xCordinate++;
}

void kprintf(const char *fmt, ...)
{
	va_list arguments;
        int integerInput=0;
	char* stringInput;
	char charInput[2]={'\0','\0'};
	va_start(arguments,fmt);
	long longArgument=0;
	while(*(fmt)){
		char buffer[500];
		int i=0;
		int specialSequence=0;
		while(*(fmt) && !(specialSequence=specialSequenceCode(fmt))){
			buffer[i]=*(fmt);
			i++;
			fmt++;
		}
		buffer[i]='\0';
		putOnScreen(buffer);
		char specialCharBuffer[100];
		if(specialSequence){
			switch(specialSequence){
				case INTEGER_CODE:
					integerInput=va_arg(arguments,int);
					num_conversion(integerInput,10,specialCharBuffer);
					putOnScreen(specialCharBuffer);
					fmt++;
					fmt++;
				break;
				case HEX_CODE:
					integerInput=va_arg(arguments,int);
					num_conversion(integerInput,16,specialCharBuffer);
                              	 	putOnScreen(specialCharBuffer);
                               		fmt++;
                                	fmt++;
				break;
				case STRING_CODE:
					stringInput=va_arg(arguments,char *);
					putOnScreen(stringInput);
					fmt++;
					fmt++;
				break;		
				case CHAR_CODE:
					integerInput=va_arg(arguments,int);
					charInput[0]=integerInput;
					putOnScreen(charInput);
                                        fmt++;
                                        fmt++;
				break;
				case POINTER_CODE:
					longArgument=(unsigned long)va_arg(arguments,long);
					num_conversion_unsigned(longArgument,16,specialCharBuffer);
					putOnScreen("0x");
					putOnScreen(specialCharBuffer);
					fmt++;
					fmt++;
				break;
				case TAB_CODE:
					putOnScreen("        ");
					fmt++;
				break;

			}
		}
	}

}

int populateWhiteSpaces(int *whiteSpaces,char * input){
	int length=0;
	int i=0;
	for(;*(input);input++,i++){
		if(isWhiteSpace(*input)){
			whiteSpaces[length]=i;
			length++;
		}
	}
	whiteSpaces[length]=i;
	return length;
}
void shiftRowsUp(int count){
        if(count>HEIGHT)
        {
                count=HEIGHT;
        }
        for(int i=0;i<HEIGHT-count;i++)
        {
             for(int j=0;j<WIDTH;j++){
                 *(screenBasePointer+160*i+2*j)= *(screenBasePointer+160*(i+count)+2*j);
             }
        }
        for(int i=HEIGHT-count;i<HEIGHT;i++){
                for(int j=0;j<WIDTH;j++){
                 *(screenBasePointer+160*i+2*j)= 0;
             }
        }
        presentRow-=count;
        if(presentRow<0)
        {
                presentRow=0;
        }
}
void putOnScreen(char * input){
	
	int lastWasSpace=0;
	while(*input){
                if(*input=='\n')
		{
		    presentColumn=0;
                    presentRow++; 
		    lastWasSpace=0;
		}
	        else if(*input=='\r')
                {
                    presentColumn=0;
                    lastWasSpace=0;
                }
		else if(lastWasSpace && *input!=' '){
			int size=0;
			while(*(input+size) && *(input+size)!=' ' && *(input +size)!='\n')
			{
				size++;
			}
			if(presentColumn+size>WIDTH && size<=80){
				presentColumn=0;
				presentRow++;
                                if(presentRow==HEIGHT){
                                        shiftRowsUp(1);
                                  //      presentRow--;
                                }

			}
			*(screenBasePointer+160*presentRow+2*presentColumn)=*input;
			presentColumn++;
			lastWasSpace=0;
		}
		else{
			*(screenBasePointer+160*presentRow+2*presentColumn)=*input;
               	 	presentColumn++;
              		if(presentColumn==WIDTH){
                        	presentColumn=0;
                        	presentRow++;
                		if(presentRow==HEIGHT){
					shiftRowsUp(1);
				//	presentRow--;
				}
			}
		}
		if(*input==' ')
		{
			lastWasSpace=1;
		}
                input++;
        }
}

