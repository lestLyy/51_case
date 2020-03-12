#include "reg51.h"
#define uint unsigned int
#define uchar unsigned char

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit rightFor=P1^6;
sbit rightBack=P1^7;
sbit enL=P1^4;
sbit enR=P1^5;

sbit led=P0^0;

sbit left_red=P3^3;
sbit right_red=P3^2;

uint left_zkb,i=0;
uint right_zkb,j=0;

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<930;j++);
}

void initTime()
{
	left_red=0;
	right_red=0;
	TMOD = 0x02;   //��ʱ��0������ģʽ2��8λ��ʱģʽ
 	TH0=210;     //д��Ԥ�ó�ֵ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TL0=210;     //д��Ԥ��ֵ ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TR0=1;       //������ʱ��
	ET0=1;       //����ʱ��0�ж�
	EA=1;        //�������ж�
	//P1=0xff; 	 //��ʼ��P1������˿�

}

void init()
{
	enR=0;
	enL=0;
	leftBack=0;
	leftFor=0;
	rightBack=0;
	rightFor=0;
}

void go()
{
	leftFor = 1	;
	rightFor = 1; 
	left_zkb=10;
	right_zkb=10;
	

}

void left()
{
	left_zkb=15;
	right_zkb=11;

	leftFor=1;
		
}

void right()
{
	right_zkb=15;
	left_zkb=11;

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

void search()
{
	if(left_red==0&&right_red==0)
		go();
	if(left_red==1)
		left();
	if(right_red==1)
		right();
}

void main()
{
	initTime();
	init();
	enR=1                                                                      ;

	while(1)
		search();
}

init0 ()interrupt 1
{
	TH0=210;     //д��Ԥ�ó�ֵ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TL0=210;     //д��Ԥ��ֵ ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	i++;j++;
	if(i<left_zkb)
		enL=1;
	else
		enL=0;

	if(i==40)
	{
		i=0;
	}

	if(j<right_zkb)
		enR=1;
	else
		enR=0;

	if(j==40)
	{
		j=0;
	}
	

}
