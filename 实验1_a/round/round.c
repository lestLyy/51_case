//ʵ��תȦ

#include <reg51.h>

typedef unsigned char u8;
typedef unsigned int  u16;

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit enL=P1^4;
sbit enR=P1^5;
sbit rightFor=P1^6;
sbit rightBack=P1^7;

void delay_1ms(u16 t)
{
	u16 x,y;
	for(x=t;x>0;x--)
		for(y=120;y>0;y--);
}

u8 t;
u8 time;

void init()
{
	leftBack=0;
	leftFor=0;
	rightBack=0;
	rightFor=0;
}

void initTime()
{
	TMOD = 0x02;   //��ʱ��0������ģʽ2��8λ��ʱģʽ
	TH0=210;     //д��Ԥ�ó�ֵ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TL0=210;     //д��Ԥ��ֵ ��ȡֵ1-255����Խ��PWMƵ��Խ�ߣ�
	TR0=1;       //������ʱ��
	ET0=1;       //����ʱ��0�ж�
	EA=1;        //�������ж�
	P1=0xff; 	 //��ʼ��P1������˿�

}

int main()
{
	initTime();
	while(1);
	
	
}

time0() interrupt 1
{
	t++;
	if(t==400)
	{
		enL=0;
		enR=0;
		t = 0;
	}
	if(t==35)
	{
		enL=0;
		enR=1;
		rightBack=0;
		rightFor=1;
	}
	
	if(t==200)
	{

		enL=1;
		leftBack=0;
		leftFor=1;
		enR=1;
		rightBack=0;
		rightFor=1;
	}

/*	if(t>30)
	{
		enL=1;
		leftBack=0;
		leftFor=0;
		enR=1;
		rightBack=0;
		rightFor=0;
	}	*/

/*	if(t==250)
	{
		enL=0;
		enR=0;
	}
	if(t==55)
	{
		enL=0;
		enR=1;
		rightBack=0;
		rightFor=1;
	}
	
	if(t==150)
	{
		enL=1;
		leftBack=0;
		leftFor=1;
		enR=1;
		rightBack=0;
		rightFor=1;
		t=0;
	}		*/
}