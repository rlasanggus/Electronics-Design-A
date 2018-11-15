# Analog/Digital CONVERTER 
## <pre>ex4-1</pre>  
>외부로부터 Analog signal(Vtg 변환기)를 하나 입력받아 A/D converter 후 LED로 출력하시오  

##### <code>PORT I/O</code>  
+ input  
	Analog 신호를 받을 pin1개 필요(channel0 사용했음으로 PA0(RA0))  
  ![image](https://user-images.githubusercontent.com/43701183/48490911-c76fc080-e869-11e8-9013-ce041c22b5ec.png)  
+ output  
  A/D 변환된 값이 저장되는 레지스터는 10bit이므로 10bit출력을 위한 10개의 LED output port가 필요.  
  ![image](https://user-images.githubusercontent.com/43701183/48490674-44e70100-e869-11e8-9b1f-be83f632016d.png)  
  ![image](https://user-images.githubusercontent.com/43701183/48490807-9099aa80-e869-11e8-8e76-5b4dfb687d4b.png)  
+ 변수설정  
  없음  
+ 함수  
  void init() : 초기치 설정  
  
##### <code>code 해석</code> 
```c   
 #include <htc.h>
 #define _XTAL_FREQ 4000000
```
코딩에 필요한 헤더파일과 PIC16F876A의 clock frequency인 4MHZ 를 FREQ 값으로 define  

```c
void init(){
	TRISA = 0xff;
	TRISB = 0x00;
	TRISC = 0x00;

	ADCON0 = 0b01000001;
	ADCON1 = 0b10000111;	
}
```  
A/D 변환을 사용하기 위한 ADCON 설정 자세한 내용은 하단 설정 참조
>설정 참고 : [ADCON](https://google.com)  
```c
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
```
A/D 변환기를 on 했을 때 필요한 초기시간 delay(20)  
GO=0가 되면 변환 완료. 변환된 값을 PORT로 출력  
GO=1 setting 다시 변환시작  
  
  
  
    
* * *          
* * * 

## <pre>ex4-2</pre>
>LM35로부터 온도 정보를 받아 7seg 로 온도를 출력하시오  
+ input  
  Analog 신호를 받을(LM35로부터 온도정보를 받을) pin1개 필요(channel0 사용했음으로 PA0(RA0))  
  ![image](https://user-images.githubusercontent.com/43701183/48522221-2c9fd200-e8bb-11e8-8dbf-d7dd28f9deb4.png)  

+ output  
  7seg 출력위치를 결정할 4개의 output port 필요  
  ![image](https://user-images.githubusercontent.com/43701183/48522297-80122000-e8bb-11e8-9f34-5e7748d09a3a.png)  
  7seg 출력을 위한 8개의 output port 필요  
  ![image](https://user-images.githubusercontent.com/43701183/48462123-3fae9580-e81a-11e8-8be9-f5e52fe2027a.png)  
+ 변수설정  
  int count : 시간을 만들기위한 counter  
  int disp_count : disp digit 위치를 주기적으로 분기하기위한 counter  
  unsigned char disp1 : digit1의 값을 저장할 변수  
  unsigned char disp2 : digit2의 값을 저장할 변수  
  unsigned char disp3 : digit3의 값을 저장할 변수  
  unsigned char disp4 : digit4의 값을 저장할 변수  
  volatile const unsigned char digit[10] : 7seg에 각 숫자에 대한 정보 저장  
  int temperature = 0 : A/D변환된 온도정보를 16bit에 저장  
+ 함수  
  void init() : 초기치 설정  
  void display(unsigned char dis, unsigned char pos) : 7seg display를 위한 함수  
  void interrupt isr() : 인터럽트 발생시  
  
##### <code>code 해석</code> 
```c   
 #include <htc.h>
 #define _XTAL_FREQ 4000000
```
코딩에 필요한 헤더파일과 PIC16F876A의 clock frequency인 4MHZ 를 FREQ 값으로 define  
```c
void init(void){
	TRISC = 0x00;
	TRISB = 0x00;
	TRISA = 0xff;		//RA0/AN0, channel0
	OPTION_REG = 0b10000010; //timer0 set 1:8
	ADCON0 = 0b01000001;	//A/D converter set FOSC/8, channel0
	ADCON1 = 0b10001010;	//A/D converter set right justified, Vref, Vss
	TMR0IE = 1;				//timer0 start
}
```
각 설정은 주석과 레지스터 참조  
>설정 참고 : [ADCON](https://google.com)  
```c
void display(unsigned char dis, unsigned char pos){
	PORTB = 0x00;
	PORTC = digit[dis];
	if(pos == 0x04){
		RC0=0;
		}
	PORTB = pos;
}
```  
7seg의 digit 위치값인 pos 값과 7seg의 disp 값인 dis 값을 받아 pos을 선택하는 PORTB에 disp값을 출력해주는 PORTC에 입력  
온도를 예를들어 023.5 로 출력하기위해 3번째자리에(0x04)일때 dot을 on  
```c
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
``` 
disp_count를 4로 나누어 4가지 case로 분주. timer0 interrupt는 2ms마다 발생함으로, 각 disp를 2ms마다 돌아가며 실행  
```c
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
```
temperature 변수에 A/D 변환된 레지스터 값을 입력하고,  
각 disp에 (disp1는 100의자리, 2는 10의 자리, 3은 1의자리, 4는 소수점 첫번째자리)에 값을 할당  


* * *          
* * *  
  
## <pre>사용장치</pre>   
>7seg  
![image](https://user-images.githubusercontent.com/43701183/48464575-61f8e100-e823-11e8-91fd-a09e429b48dc.png)  
>LM35  
![image](https://user-images.githubusercontent.com/43701183/48522727-198e0180-e8bd-11e8-82c1-555b191956fd.png)
