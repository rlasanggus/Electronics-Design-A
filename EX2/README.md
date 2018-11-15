# INTERRUPT, TIMER/COUNTER  
## <pre>ex2-1</pre>  
>Timer0 를 이용하여 60sec 짜리 초시계 만들기  
##### <code>PORT I/O</code>  
+ input  
  inputport 없음
+ output  
	2자리의 7seg dig출력을 위한 2개의 output port  
  ![image](https://user-images.githubusercontent.com/43701183/48462095-20176d00-e81a-11e8-9c39-c3937461bc2a.png)  
  7seg 출력을 위한 8개의 output port  
  ![image](https://user-images.githubusercontent.com/43701183/48462123-3fae9580-e81a-11e8-8be9-f5e52fe2027a.png)  
>>7seg에 대한 자료는 하단 참조
+ 변수설정  
  int count : 시간을 만들기위한 counter  
  int disp_count : disp digit 위치를 주기적으로 분기하기위한 counter  
  unsigned char disp1 : digit1의 값을 저장할 변수  
  unsigned char disp2 : digit2의 값을 저장할 변수  
  volatile const unsigned char digit[10] : 7seg에 각 숫자에 대한 정보 저장  
+ 함수  
  void init() : 초기치 설정  
  void display(unsigned char dis, unsigned char pos) : 7seg display를 위한 함수  
  void interrupt isr() : 인터럽트 발생시  
	
##### <code>code 해석</code>
```c   
 #include <htc.h>
```
코딩에 필요한 헤더파일 참조
```c
int count = 0;
int disp_count = 0;

unsigned char disp1 = 0;
unsigned char disp2 = 0;

volatile const unsigned char digit[10]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1B, 0x01, 0x09};
```
변수들의 초기값으로 0을 지정, 7seg에 0~9의 숫자를 표시하기위한 정보 저장
```c
void init(){
	TRISC = 0x00;
	TRISB = 0x00;
	TRISA = 0x00;
	ADCON1 = 0b00000111;

	OPTION_REG = 0b10000010;
	PEIE = 1;
	TMR0IE = 1;	
}
```
PORTC, PORTB, PORTA를 output mode로  
OPTION_REG 설정 Timer0를 1:8분주로  
인터럽트 on, timer0 start  
>레지스터 참고 : [REG](https://google.com)

```c
void display(unsigned char dis, unsigned char pos){
	PORTB = 0;
	PORTC = digit[dis];
	PORTB = pos;
}
```
7seg의 digit 위치값인 pos 값과 7seg의 disp 값인 dis 값을 받아 pos을 선택하는 PORTB에 disp값을 출력해주는 PORTC에 입력  
```c
void interrupt isr(){
	if((TMR0IE)&&(TMR0IF)){
  	disp_count ++;
		switch(disp_count%2){
			case 0: display(disp1, 0x01); break;
			case 1:	display(disp2, 0x02); count++; break;
		}
		TMR0IF = 0;
	}
}
```
TMR0IE&&TMR0IF => timer0가 enable 이고 flag bit 즉 인터럽트가 발생했다면(2ms마다 발생함) disp_count를 증가시키고 2로 나누어 분기. disp_count를 나눈값은 0 또는 1 이므로 2ms마다 두 자리를 반복 출력하게됨  
flagbit 초기화(매우중요)  
```c
void main(){
	init();
	ei();
	while(1){
		if(count == 250){
			count = 0;
			disp1 ++;
			if(disp1 == 10){
				disp1 = 0;
				disp2 ++;
				if(disp2 == 6){
					disp2 = 0;
				}
			}
		}
	}
}
```
2ms 마다 인터럽트가 발생하고, 인터럽트가 두번 발생했을 때 count가 1씩올라가기 때문에 count는 4ms 마다 1씩 증가하게 되고, count값이 250이라면 1초가 된다. disp1은 1초단위, disp2는 10초단위를 나타낸다.
  
  
  
  
    
* * *          
* * *  
## <pre>ex2-2</pre>
>Timer0 를 이용하여 60sec 짜리 초시계를 만들고, 외부 인터럽트(INTE)를 이용하여 인터럽트 발생시 시계를 초기화시켜라. 또한 Timer1을 이용하여 500ms마다 LED1을 toggle시키고 Timer2를 이용하여 1sec마다 LED2를 toggle 시켜라
##### <code>PORT I/O</code>  
+ input  
  외부 인터럽트를 입력받기위한 RB0 핀  
  ![image](https://user-images.githubusercontent.com/43701183/48463948-2230fa00-e821-11e8-9151-57ddd12e3478.png)  
+ output  
  2자리의 7seg dig출력을 위한 2개의 output port  
  ![image](https://user-images.githubusercontent.com/43701183/48463948-2230fa00-e821-11e8-9151-57ddd12e3478.png)  
  7seg 출력을 위한 8개의 output port  
  ![image](https://user-images.githubusercontent.com/43701183/48462123-3fae9580-e81a-11e8-8be9-f5e52fe2027a.png)  
  LED2개 output  
  ![image](https://user-images.githubusercontent.com/43701183/48464714-cae05900-e823-11e8-8e61-b298c36ddd91.png)  

  
>>7seg에 대한 자료는 하단 참조
+ 변수설정  
  int count : 시간을 만들기위한 counter  
  int disp_count : disp digit 위치를 주기적으로 분기하기위한 counter  
  unsigned char disp1 : digit1의 값을 저장할 변수  
  unsigned char disp2 : digit2의 값을 저장할 변수  
  volatile const unsigned char digit[10] : 7seg에 각 숫자에 대한 정보 저장  
+ 함수  
  void init() : 초기치 설정  
  void display(unsigned char dis, unsigned char pos) : 7seg display를 위한 함수  
  void interrupt isr() : 인터럽트 발생시  

##### <code>code 해석</code>
1번에서 보충하는 형식입니다. 다른 필요한 내용은 ex2-1에서 찾아주세요  
```c
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
```
PB0 핀을 외부인터럽트를 받기위해 input 설정  
timer0, timer1, tiemr2를 사용하기위한 REG 설정 timer1는 1:8 prescale 설정, timer2는 1:16 prescale 설정  
```c
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
  ```  
  timer0는 ex2-1참고, 외부 인터럽트 발생시 시계 초기화, timer1을 이용 500ms timer, timer2를 이용 1sec timer 만듬. timer1은 다른 timer와는 다르게 16bit로 이루어짐.(나머진 8bit)  
 ```c
 void main(){
	init();
	ei();
	TMR1H = 0b00001011;
	TMR1L = 0b11011011;
	RA1 = 0;
	RA2 = 0;
	PR2 = 250;
  ```
  LED로 출력되는 초기값 0 (RA1, RA2, 끄고시작)
  timer1의 초기값 설정 TMR1H = 0b00001011,  TMR1L = 0b11011011  
  (1:1 prescale에선 1us에 1씩증가, 1:8 설정했으므로 8us마다 1씩증가 => 1111111111111111(2) - 500000/8)  
  timer2의 초기값 PR2 = 250으로 설정(timer2는 PR2값이 자동을 업데이트 된다고 생각해도됨)  



* * *          
* * *  
  
## <pre>사용장치</pre>  
>LED  
![image](https://user-images.githubusercontent.com/43701183/48461808-e2feab00-e818-11e8-901e-c0aab3782127.png)  
>7seg  
![image](https://user-images.githubusercontent.com/43701183/48464575-61f8e100-e823-11e8-91fd-a09e429b48dc.png)
