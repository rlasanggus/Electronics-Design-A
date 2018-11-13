#include <htc.h>

void init(){
	TRISC = 0x00;
	TRISA = 0b00000111; // RA0 = toggle, RA2, 3 push button
	ADCON1 = 0b00000111;

	T2CON = 0b00000100;
	CCP1CON = 0b00001111;
	CCP2CON = 0b00001111;
	CCPR1L = 0;
	CCPR2L = 0;
	PR2 = 250;

}

void main(){
	init();

	while(1){
		if(RA0 == 0){
			if(RA1 == 0){
				if(CCPR1L == 250){
					CCPR1L = 0;
				}else{
					CCPR1L = CCPR1L + 25;
				}
			}
			if(RA2 == 0){
				if(CCPR1L == 0){
					CCPR1L = 250;
				}else{
					CCPR1L = CCPR1L - 25;
				}
			}
		}
		if(RA1 == 1){
			if(RA1 == 0){
				if(CCPR2L == 250){
					CCPR2L = 0;
				}else{
					CCPR2L = CCPR2L + 25;
				}
			}
			if(RA2 == 0){
				if(CCPR2L == 0){
					CCPR2L = 250;
				}else{
					CCPR2L = CCPR2L - 25;
				}
			}
		}
	}
}