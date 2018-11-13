#include <htc.h>

#define _XTAL_FREQ 4000000

void init(){
	TRISA = 0b00000011;
	TRISC = 0b00000000;
	ADCON1 = 0b00000111;
}

void main(){
	init();
	PORTC = 1;
	__delay_ms(200);
	while(1){
		if(RA1 == 0){
			if(RA0 == 0){
				__delay_ms(200);
			}else{
				__delay_ms(100);
			}

			PORTC = PORTC<<1;
			if(PORTC == 0){
				PORTC = 1;
			}
		}else{
			if(RA0 == 0){
				__delay_ms(200);
			}else{
				__delay_ms(100);
			}
			PORTC = PORTC>>1;
			if(PORTC == 0){
				PORTC = 128;
			}
		}
	}
}