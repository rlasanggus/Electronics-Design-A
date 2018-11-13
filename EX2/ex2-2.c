#include <htc.h>

int count = 0;
int disp_count = 0;
int countt = 0;

unsigned char disp1 = 0;
unsigned char disp2 = 0;

volatile const unsigned char digit[10]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1B, 0x01, 0x09};

void init(){
	TRISC = 0x00;
	TRISB = 0x01;
	TRISA = 0x00;
	ADCON1 = 0b00000111;

	OPTION_REG = 0b10000010;
	PEIE = 1;
	INTEDG = 0;
	INTE = 1;

	TMR0IE = 1;

	T1CON = 0b00110000; //prescale 1:8
	TMR1IE = 1;
	TMR1ON = 1;

	T2CON = 0b00000011;
	TMR2IE = 1;
	TMR2ON = 1;

}

void display(unsigned char dis, unsigned char pos){
	PORTB = 0;
	PORTC = digit[dis];
	PORTB = pos;
}

void interrupt isr(){
	if((TMR0IE)&&(TMR0IF)){
		disp_count ++;
		switch(disp_count%2){
			case 0: display(disp1, 0x02); break;
			case 1: display(disp2, 0x04); count++; break;
		}

		TMR0IF = 0;
	}

	if((INTE)&&(INTF)){
		disp1 = 0;
		disp2 = 0;
		INTF = 0;
	}

	if((TMR1IE)&&(TMR1IF)){ //500ms timer
		RA1 = ~RA1;
		TMR1H = 0b00001011;
		TMR1L = 0b11011011;
		TMR1IF = 0;
	}
	if((TMR2IE)&&(TMR2IF)){	//1sec timer;
		countt ++;
		if(countt == 250){
			RA2 = ~RA2;
			countt = 0;
		}
		TMR2IF = 0;
	}
}

void main(){
	init();
	ei();
	TMR1H = 0b00001011;
	TMR1L = 0b11011011;
	RA1 = 0;
	RA2 = 0;
	PR2 = 250;
	while(1){
		if(count == 250){
			count = 0;
			disp1 ++;
			if(disp1 == 10){
				disp2 ++;
				if(disp2 == 6){
					disp2 = 0;
				}
			}
		}
	}

}