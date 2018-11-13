#include <htc.h>

#define _XTAL_FREQ 4000000

void init(){
	TRISA = 0xff;
	TRISB = 0x00;
	TRISC = 0x00;

	ADCON0 = 0b01000001;
	ADCON1 = 0b10000111;	
}

void main(){
	GO = 1;
	_delay(20);
	while(1){
		while(GO){
		}
		PORTC = ADRESL;
		PORTB = ADRESH;
		GO = 1;
	}
}