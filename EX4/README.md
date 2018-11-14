# Analog/Digital CONVERTER 
### <pre>ex4-1</pre>  
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

### <pre>ex4-2</pre>
>LM35로부터 온도 정보를 받아 7seg 로 온도를 출력하시오
