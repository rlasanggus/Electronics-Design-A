#include <htc.h>

#define _XTAL_FREQ 4000000

void init(){
	TRISB = 0b00000000;
}

char counter;

void main(){
	counter = 0;
	init();
	while(1){
		PORTB = counter;
		_delay(100000);
		//__delay_ms(100);
		//__delat_us(100000);
		counter ++;
	}
}