#include"reg51.h"
#define uint unsigned int 
#define uchar unsigned char

uint code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<112;j++);
}
uint num;

void init()
{
	SCON=0X40;		//串口工作在方式1
	TMOD=0X20;	   //T1工作在模式2,8位自动装载
	PCON=0X00;	   //波特率不递增
	TH1=0XFD;	   //波特率9600
	TL1=0XFD;
	TI=0;
	TR1=1;
	IE=0x90;
}

void send2port(uint c)
{
	SBUF=c;
	while(TI==0);
	TI=0;	
}

void scan()
{
	uchar tmp;
	P1=0x0f;
	delay(1);
	tmp=P1^0x0f;
	switch(tmp)
	{
		case 1:num=1;break;
		case 2:num=2;break;
		case 4:num=3;break;
		case 8:num=4;break;
	}
	P1=0xf0;
	delay(1);
	tmp=P1^0xf0;
	switch(tmp)
	{
		case 16:num+=0;break;
		case 32:num+=4;break;
		case 64:num+=8;break;
		case 128:num+=12;break;
	}
}

void main()
{
	uint pre_num;
	init();
	TR1=1;
	while(1)
	{
		scan();
		delay(1);
		if(pre_num!=num)
		{
			P0=smg[num-1];
			
			send2port(num-1);
			pre_num=num;	
		}
		delay(10);		
	}
}