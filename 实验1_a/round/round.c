//实现转圈

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
	TMOD = 0x02;   //定时器0，工作模式2，8位定时模式
	TH0=210;     //写入预置初值（取值1-255，数越大PWM频率越高）
	TL0=210;     //写入预置值 （取值1-255，数越大PWM频率越高）
	TR0=1;       //启动定时器
	ET0=1;       //允许定时器0中断
	EA=1;        //允许总中断
	P1=0xff; 	 //初始化P1，输出端口

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