#include "reg51.h"
/*
sbit leftFor=P1^2;
sbit leftBack=P1^3;
sbit enL=P1^4;
sbit enR=P1^5;
sbit rightBack=P1^6;
sbit rightFor=P1^7;
*/
sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit rightFor=P1^6;
sbit rightBack=P1^7;
sbit enL=P1^4;
sbit enR=P1^5;

#define uchar unsigned char
#define uint  unsigned int


void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 930; y > 0 ; y--);
}



void init()
{
	enL=1;
	enR=1;
	leftFor=0;
	leftBack=0;
	rightBack=0;
	rightFor=0;
}

void go()
{
	init();
//	left_zkb=39;
//	right_zkb=39;
	leftFor = 1	;
	rightFor = 1;
}

void left()
{
	init();
//	left_zkb=15;
//	right_zkb=39;
	leftBack=1;
	rightFor=1;
		
}
void stop()
{
	enL=0;
	enR=0;
	leftFor=0;
	rightBack=0;
	rightFor=0;
	leftBack=0;
}

void main()
{
	uint k;
	 
	for(k=0;k<4;k++)
	{
		
		go();
		delay(120);
		stop();
		delay(60);
		left();
		delay(17);
		stop();
		delay(50);

	}	
}


