# GPIO  
## <pre>ex1-1</pre>
>100ms delay를 만들고, delay를 이용하여 100ms 마다 8bit레지스터의 값을 1씩증가시키고, 레지스터의 변화값을 LED8개로 출력하여라.  
  
##### <code>PORT I/O</code>  
+ input  
	inputport 없음
+ output  
	LED8개 출력을 위한 8개의 output port가 필요.  
	![image](https://user-images.githubusercontent.com/43701183/48460173-f1e15f80-e810-11e8-9ff7-38fe81ceef45.png)  
+ 변수설정  
	char counter : counter를 계수할 char형 8bit 변수
	
##### <code>code 해석</code> 
```c   
 #include <htc.h>
 #define _XTAL_FREQ 4000000
```
코딩에 필요한 헤더파일과 PIC16F876A의 clock frequency인 4MHZ 를 FREQ 값으로 define
```c
void init(){
TRISB = 0b00000000;
}

char counter;
```
PORTB의 8개 pin을 모두 output mode로 설정 필요한 변수 설정
```c
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
```
PORTB에 counter 8bit 값을입력  
counter 값을 100ms 마다 1씩 증가 
>__delay_ms(T) : Tms delay  
>__delay_us(T) : Tus delay
  
  
  
  
    
* * *          
* * *
## <pre>ex1-2</pre>
>toggle 스위치 입력 2개와 8개의 LED출력을 이용하여 LED불빛이 1칸씩 이동하도록 만드시오. toggle 스위치 하나가 on/off될땐 불빛의 방향이, 또 다른하나의 toggle 스위치가 on/off될때마다 이동시간을 100ms/200ms가 되도록 만드시오.  
###### <code>PORT I/O</code>  
+ input  
	toggle sw 입력을 위한 2개의 input port 필요.  
	![image](https://user-images.githubusercontent.com/43701183/48461634-f4938300-e817-11e8-97f3-cebd09de9c8b.png)  
+ output  
	LED8개 출력을 위한 8개의 output port가 필요.  
	![image](https://user-images.githubusercontent.com/43701183/48460072-68ca2880-e810-11e8-89e9-a6beffff5021.png)  
+ 변수설정  
##### <code>code 해석</code>  
```c
#include <htc.h>

#define _XTAL_FREQ 4000000
```
코딩에 필요한 헤더파일과 PIC16F876A의 clock frequency인 4MHZ 를 FREQ 값으로 define  
```c
void init(){
	TRISA = 0b00000011;
	TRISC = 0b00000000;
	ADCON1 = 0b00000111;
}
```  
toggle sw의 입력을 받을 PA0, PA1을 input mode 로 설정  
LED8개 출력을 위한 PORTC 8bit를 모두 output mode 로 설정  
PORTA의 Digital I/O를 위해 ADCON1 설정  
```c
void main(){
	init();
	PORTC = 1;
	__delay_ms(200);
	while(1){
```
main에서 기본설정 init() 호출, PORTC에 기본값 1 설정
```c
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
```
RA1에 연결된 toggle sw가 on 일때(on일때 0, off일때 1), RA0에 연결된 toggle sw가 on 이면 200ms delay, off이면 100msdealy  
이후 PORTC를 shift => LED불빛도 옮겨짐  
PORTC가 0 라면 최상위 비트를 지나 carry가 발생한 상태. 따라서 PORTC는 0으로 모든 비트가 0이므로 LED불빛이 안들어오게됨. 따라서 다시 초기값 1을 세팅
```c
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
```
위의 코드와 비슷하나 오른쪽으로 shift 시켰으므로 0이 되었다면 1이 최상위 비트로 옮겨져아함으로 128
