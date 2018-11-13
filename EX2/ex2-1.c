#include <htc.h>

int count = 0;
int disp_count = 0;

unsigned char disp1 = 0;
unsigned char disp2 = 0;
unsigned char disp3 = 0;
unsigned char disp4 = 0;

volatile const unsigned char digit[10]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1B, 0x01, 0x09};

void init(){
	TRISC = 0x00;
	TRISB = 0x00;
	TRISA = 0x00;
	ADCON1 = 0b00000111;

	OPTION_REG = 0b10000010;
	PEIE = 1;
	TMR0IE = 1;	
}

void display(unsigned char dis, unsigned char pos){
	PORTB = 0;
	PORTC = digit[dis];
	PORTB = pos;
}

void interrupt isr(){
	disp_count ++;
	if((TMR0IE)&&(TMR0IF)){
		switch(disp_count%4){
			case 0: display(disp4, 0x01); break;
			case 1:	display(disp3, 0x02); count++; break;
			case 2: display(disp2, 0x04); break;
			case 3: display(disp1, 0x08); count++; break;
		}
		TMR0IF = 0;
	}
}

void main(){
	init();
	ei();
	while(1){
		if(count == 250){
			count = 0;
			disp4 ++;
			if(disp4 == 10){
				disp4 = 0;
				disp3 ++;
				if(disp3 == 6){
					disp3 = 0;
					disp2 ++;
					if(disp2 == 10){
						disp2 = 0;
						disp1 ++;
						if(disp1 == 6){
							disp1 = 0;
						}
					}
				}
			}
		}
	}
}