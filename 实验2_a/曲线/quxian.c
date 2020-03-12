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

	TMOD = 0x02;   //��ʱ��0������ģʽ2��8λ��ʱģʽ
 	TH0=210;     //д��Ԥ�ó�ֵ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TL0=210;     //д��Ԥ��ֵ ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TR0=1;       //������ʱ��
	ET0=1;       //����ʱ��0�ж�
	EA=1;        //�������ж�

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