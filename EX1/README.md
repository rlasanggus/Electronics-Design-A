# GPIO  
## <pre>ex1-1 
>100ms delay를 만들고, delay를 이용하여 100ms 마다 8bit레지스터의 값을 1씩증가시키고, 레지스터의 변화값을 LED8개로 출력하여라.  
  
##### <code>PORT I/O</code>  
+ input  
	inputport 없음
+ output  
	LED8개 출력을 위한 8개의 output port가 필요.  
    ![image](https://user-images.githubusercontent.com/43701183/48459061-39192180-e80c-11e8-8995-69e23f386d7f.png)
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
PORTB에 counter 8bit 값을 
counter 값을 100ms 마다 1씩 증가  


    
          
## <pre>ex1-2</pre>
>toggle 스위치 입력 2개와 8개의 LED출력을 이용하여 LED불빛이 1칸씩 이동하도록 만드시오. toggle 스위치 하나가 on/off될땐 불빛의 방향이, 또 다른하나의 toggle 스위치가 on/off될때마다 이동시간을 100ms/200ms가 되도록 만드시오.  
###### <code>PORT I/O</code>  
+ input  
	toggle sw 입력을 위한 2개의 input port 필요.
+ output  
	LED8개 출력을 위한 8개의 output port가 필요.
