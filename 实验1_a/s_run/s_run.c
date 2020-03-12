
#include "reg52.h"
#define uchar unsigned char
#define uint  unsigned int


sbit LB=P1^2;
//sbit LB=P2^1;
sbit LF=P1^3;
sbit RF=P1^6;
sbit RB=P1^7;
sbit EL=P1^4;  
sbit ER=P1^5;


void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 930; y > 0 ; y--);
}



void MF (){

	LF = 1;
	RF = 1;
	
}

void TA (){
	LB = 1;
	RF = 1;
		
	

}

void init(){

	EL = 1;
	ER = 1;
	LB = 0;
	RB = 0;
	LF = 0;
	RF = 0;

}

void stop(){
	EL = 0;
	ER = 0;
	LB = 0;
	LF = 0;
	RB = 0;
	RF = 0;
}

void main(){
	//MF
	init();
	MF();
	delay(200)	;
	stop();
	delay(150)	;  

	//TA
	
	
	init();
	TA();
	delay(40);



	stop();
	delay(100);	
	

	//MF
	init();
	MF();
	delay(200);
	stop();
	delay(10000);
}