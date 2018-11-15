# CCP(CAPTURE, COMPARE, PWM) 
## <pre>ex3-1</pre>  
>push button 2개와 toggle sw 1개를 이용하여 pwm dutyratio 를 조정하여 led 불빛의 밝기를 10단계로 조정하여라. toggle sw on/off에 따라 led를 선택  
  
##### <code>PORT I/O</code>  
+ input  
	toggle sw 입력을 위한 PA0(RA0), push button 2개 입력을 위한 PA1(RA1), PA2(RA2)  
	![image](https://user-images.githubusercontent.com/43701183/48491386-cf7c3000-e86a-11e8-9049-a2cfd1f894b4.png)    
	
+ output  
	PWM 신호 2개를 출력할 PC1(RC1), PC2(RC2) (PC1, PC2선정이유 CCP1, CCP2 signal을 출력할 수 있는 pin 이기때문)  
	![image](https://user-images.githubusercontent.com/43701183/48488750-d56f1280-e864-11e8-8605-a4a21fde6730.png)    
+ 변수설정  
	 없음  
+ 함수  
  void init() : 초기치 설정  

##### <code>code 해석</code>  
```c
#include <htc.h>
```
코딩에 필요한 헤더파일 참조  
```c
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
```  
PORTC는 PWM된 신호 출력위해 output mode로 설정, PORTA의 0,1,2번 port는 button과 sw 입력받기위해 input mode 설정  
PWM 모드를 위해 CCP1CON 레지스터와 CCP2CON레지스터 설정  
pulse-width-modulation 위한 주기와 duty ratio설정 (주기가 PR2값, duty ratio는 CCP1L/PR2)  
>레지스터 참고 : [REG](https://google.com)  
```c
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
```  
toggle sw가 on 이면서 push button이 눌리지 않으면, LED1의 duty ratio 증가(250을 넘어갈 순 없으므로 250이라면 0으로 초기화)  
toggle sw가 on 이면서 push button이 눌리지 않으면, LED2의 duty ratio 감소(0보다 작을 순 없으므로 0 이라면 250으로 초기화)  
이후 코드는 위와 toggle sw의 on/off만 달라짐(LED선택)  




* * *          
* * *  

## <pre>ex3-2</pre>
>Timer1을 이용하여 100ms 마다 pwm dutyratio를 조절하여 1초에 10단계로 밝기가 변하도록하시오.  
##### <code>PORT I/O</code>  
+ input  
  없음  
  
+ output  
	PWM 신호 1개를 출력할 PC1(RC1)  
  ![image](https://user-images.githubusercontent.com/43701183/48488902-241cac80-e865-11e8-9b15-036d52dfc34e.png)  
+ 변수설정  
	 없음  
+ 함수  
  void init() : 초기치 설정  
  void interrupt isr() : 인터럽트 발생시  

##### <code>code 해석</code>  
```c
#include <htc.h>
```
코딩에 필요한 헤더파일 참조  
```c
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
```  
100ms 만들기위해 Timer1 사용, T1CON 설정  
pwm mode를 이용하기위한 CCP1CON 설정  
pwm mode에서 주기와 duty ratio는 Timer2의 PR2와 CCP1L값으로 조절가능  
```c
void interrupt isr(){
	if((TMR1IE)&&(TMR1IF)){
		if(CCPR1L == 250){
			CCPR1L = 0;
		}else{
			CCPR1L = CCPR1L + 25;
		}
		TMR1IF = 0;
	}
```
Timer1 interrupt 발생시(100ms마다) duty ratio 증가  
duty ratio는 1을 넘을 수 없으므로, PR2(250)>=CCPR1L 이어야 함으로 CCPR1L = 250이라면 0으로 초기화  
```c
void main(){
	TMR1L = 0b10101111;
	TMR1H = 0b00111100;
	init();
	ei();
	while(1){
	}
}
```
ei()를 이용 interrupt on
timer1의 초기값 설정 TMR1H = 0b00111100,  TMR1L = 0b10101111  
  (1:1 prescale에선 1us에 1씩증가, 1:2 설정했으므로 2us마다 1씩증가 => 1111111111111111(2) - 100000/2)  
다른 동작 필요없으므로 main에선 무한루프 while(1)  


* * *          
* * *  


## <pre>사용장치</pre>  
>LED  
![image](https://user-images.githubusercontent.com/43701183/48461808-e2feab00-e818-11e8-901e-c0aab3782127.png)  
## <pre>PIC16F876A</pre>  
![image](https://user-images.githubusercontent.com/43701183/48489587-bf625180-e866-11e8-8557-e988bb1c4b54.png)  
출저 : 39582C-PIC16F87XA datasheet
