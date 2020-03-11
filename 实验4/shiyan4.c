#include"reg51.h"
#define uint unsigned int
#define uchar unsigned char


sbit LSA=P2^1;
sbit LSB=P2^2;
sbit LSC=P2^3;


uint num=0;
uint pre_num=0;

uint code smg[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x38,0x37,0x73}; 
void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<930;j++);
}

void scan()
{
	uchar tmp;
	P1=0x0F;   //预先控制P1 0~3端口为低电平
	P3=0xE0;   //预先控制P3 0~5端口为低电平
	delay(1);
	tmp=P1^0x0f;
	switch(tmp)
	{
		case 1:num=1;break;
		case 2:num=2;break;
		case 4:num=3;break;
		case 8:num=4;break;
	}
	P1=0xF0;	 //控制P1 4~7为高电平
	P3=0x1F;	 //控制P3 0~5为高电平
	delay(1);
	tmp=P3^0x1F;
	switch(tmp)
	{
		case 1:num+=0;break;
		case 2:num+=4;break;
		case 4:num+=8;break;
		case 8:num+=12;break;
		case 16:num+=16;break;
		default:break;
	}
}
void main()
{
	LSA=0;
	LSB=0;
	LSC=0;
	P0=0x00;

	while(1)
	{
		scan();
		delay(1);
		if(pre_num!=num)
		{
			P0=smg[num-1];
			pre_num=num;	
		}
		delay(10);
		
	}
}