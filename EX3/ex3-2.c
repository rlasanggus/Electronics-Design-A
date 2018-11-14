#include <htc.h>

void init(){
	TRISC = 0x00;
	TRISA = 0x01;
	ADCON1 = 0b00000111;

	T1CON = 0b00010001;
	PEIE = 1;
	TMR1IE = 1;

	T2CON = 0b00000100;
	CCP1CON= 0b00001111;
	CCPR1L = 0;
	PR2 = 250;
}

void interrupt isr(){
	if((TMR1IE)&&(TMR1IF)){
		if(CCPR1L == 250){
			CCPR1L = 0;
		}else{
			CCPR1L = CCPR1L + 25;
		}
		TMR1IF = 0;
	}
}

void main(){
	TMR1L = 0b10101111;
	TMR1H = 0b00111100;
	init();
	ei();
	while(1){
	}
}
