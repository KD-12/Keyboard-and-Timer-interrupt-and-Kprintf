#include<sys/ioPort.h>
#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/keyboard.h>

static int shiftPressed = 0;
int controlPressed = 0;
unsigned char scanCodeToASCII[128] =
{0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e',
'r','t','y','u','i','o','p','[',']','\n','^','a','s','d','f','g','h','j','k',
'l','\'','`',0,';','\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,0,'!','@',
'#','$','%','^','&','*','(',')','_','+',0,0,0,0,0,0,'-',0,0,0,'+',0,0,0,0,0,0,0,0,
0,0,0};		


void convertScanCode(unsigned char code){
	if(!(code & 0x80)){
		//kprintf("CODE: %c, MAP: %c, INT: %d",code,scanCodeToASCII[code],(int)code);
		if(shiftPressed && code > 12){
			writeKeyPressed(scanCodeToASCII[code]-32);
			shiftPressed = 0;
		}
		else if(shiftPressed && (code > 1 && code < 12)){
			writeKeyPressed(scanCodeToASCII[(int)(code+58)]);
			shiftPressed = 0;
		}
		else if((int)code == 42){
                        shiftPressed = 1;
                }
		else if((int)code == 29){
			controlPressed = 1;
		}
		else if(!shiftPressed){
			writeKeyPressed(scanCodeToASCII[code]);
		}
		
	}

}

void keyPressCallback(){
	unsigned char scancode;
	scancode = inb(0x60); 
	convertScanCode(scancode);
	//kprintf("Scancode: %c");
}

