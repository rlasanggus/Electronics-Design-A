#include <htc.h>

#define _XTAL_FREQ 4000000

int disp_count = 0;
int count = 0;

unsigned char disp_4 = 0;
unsigned char disp_3 = 0;
unsigned char disp_2 = 0;
unsigned char disp_1 = 0;

volatile const unsigned char digit[10]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1B, 0x01, 0x09};

void init(void){
	TRISC = 0x00;
	TRISB = 0x00;
	TRISA = 0xff;		//RA0/AN0, channel0
	OPTION_REG = 0b10000010; //timer0 set 1:8
	ADCON0 = 0b01000001;	//A/D converter set FOSC/8, channel0
	ADCON1 = 0b10001010;	//A/D converter set right justified, Vref, Vss
	TMR0IE = 1;				//timer0 start
}

void display(unsigned char dis, unsigned char pos){
	PORTB = 0x00;
	PORTC = digit[dis];
	if(pos == 0x04){
		RC0=0;
		}
	PORTB = pos;
}

void interrupt isr(){
	if(TMR0IF&&TMR0IE){
		disp_count ++;
		switch(disp_count%4){
			case 0:
				display(disp_1, 0x01);
				break;
			case 1:
				display(disp_2, 0x02);
				count++;
				break;
			case 2:
				display(disp_3, 0x04);
				break;
			case 3:
				display(disp_4, 0x08);
				count++;
				break;
		}

		TMR0IF = 0;
	}
}

void main(){
	init();
	ei();		//start timer0
	GO = 1;		//start A/D converter
	int temperature = 0;
	_delay(20);
	while(1){
		if(count == 125){
			count = 0;

			temperature = ADRESH;
			temperature = temperature<<8;
			temperature = temperature + ADRESL;

			disp_1 = temperature/1000;
			disp_2 = (temperature-disp_1*1000)/100;
			disp_3 = (temperature-disp_1*1000-disp_2*100)/10;
			disp_4 = temperature%10;

			GO = 1;
		}
	}
}