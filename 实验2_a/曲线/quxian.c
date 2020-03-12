#include"reg51.h"

#define uint unsigned int
#define uchar unsigned char

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit rightFor=P1^6;
sbit rightBack=P1^7;
sbit enL=P1^4;
sbit enR=P1^5;

uint left_zkb,right_zkb,l=0,r=0;

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<50;j++);
}

void initTime()
{

	TMOD = 0x02;   //定时器0，工作模式2，8位定时模式
 	TH0=210;     //写入预置初值（取值1-255，数越大PWM频率越高）
	TL0=210;     //写入预置值 （取值1-255，数越大PWM频率越高）
	TR0=1;       //启动定时器
	ET0=1;       //允许定时器0中断
	EA=1;        //允许总中断

}

void init()
{
	leftBack=0;
	rightBack=0;
	leftFor=0;
	rightFor=0;
}

void yuan1(uint n)
{
	init();
	left_zkb=27;
	right_zkb=95;
	leftFor=1;
	rightFor=1;
	delay(n);
}	

void yuan2(uint n)
{
	init();
	left_zkb=95;
	right_zkb=27;
	leftFor=1;
	rightFor=1;
	delay(n);
}

void go(uint n)
{
	init();
	left_zkb=70;
	right_zkb=70;
	leftFor=1;
	rightFor=1;
	delay(n);
}

void left(uint n)
{
	init();
	left_zkb=40;
	right_zkb=50;
	leftBack=1;
	rightFor=1;
	delay(n);
}

void right(uint n)
{
	init();
	left_zkb=50;
	right_zkb=40;
	leftFor=1;
	rightBack=1;
	delay(n);
}

void run()
{

	yuan1(145);
	init();
	delay(80);
	go(40);
	init();
	delay(80);
	right(22);
	init();
	delay(80);
	go(40);
	init();
	delay(80);
	right(22);
	init();
	delay(80);
	go(40);
	init();
	delay(40);
	left(22);
	go(50);
	init();
	delay(100);
	
	left(35);
	init();
	delay(50);
	go(50);
	init();
	delay(80);
	right(22);
	init();
	delay(80);
	go(40);
	init();
	delay(50);
	left(22);
	init();
	delay(80);
	go(50);
	init();
	delay(80);
	left(22);
	init();
	delay(50);
	go(50);
	init();
	delay(80);
	yuan2(145);
	init();
	delay(80);

	
	
	

}
void main()
{
	initTime();
	run();
	while(1);
}

init0()interrupt 1
{
	l++;
	r++;
	if(l<left_zkb)
		enL=1;
	else
		enL=0;
	if(l==100)
		l=0;

	if(r<right_zkb)
		enR=1;
	else
		enR=0;
	if(r==100)
		r=0;	
}