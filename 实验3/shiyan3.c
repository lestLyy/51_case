#include"reg51.h"
#define uint unsigned int
#define uchar unsigned char

sbit led0=P0^0;
sbit led1=P0^1;
sbit led2=P0^2;
sbit led3=P0^3;
sbit led4=P0^4;
sbit led5=P0^5;
sbit led6=P0^6;
sbit led7=P0^7;

sbit button=P3^0;

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<930;j++);
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
	led0=1;
	led1=1;
	led2=1;
	led3=1;
	led4=1;
	led5=1;
	led6=1;
	led7=1;
}
void run1()
{
	led0=0;
	led7=0;
	delay(150);
	init();

	led1=0;
	led6=0;
	
	delay(150);
	init();
	led2=0;
	led5=0;
	
	delay(150);
	init();
	led3=0;
	led4=0;
	delay(150);
	init();
}

void run2()
{
	led3=0;
	led4=0;
	delay(150);
	init();

	led2=0;
	led5=0;
	
	delay(150);
	init();
	led1=0;
	led6=0;
	
	delay(150);
	init();
	led0=0;
	led7=0;
	delay(150);
	init();
}


void main()
{
	initTime();
	while(1)
	{
		if(button==0)
		{
			delay(5);
			if(button==0)
			{
				run1();	
				delay(50);
				run2();
			}
	
		}		
		else
		{
			 init();
		}	
	}
}

init0()interrupt 1
{
 	TH0=210;     
	TL0=210;
	while(button!=0)
		init();
}
