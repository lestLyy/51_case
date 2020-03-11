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
	P1=0x0F;   //Ԥ�ȿ���P1 0~3�˿�Ϊ�͵�ƽ
	P3=0xE0;   //Ԥ�ȿ���P3 0~5�˿�Ϊ�͵�ƽ
	delay(1);
	tmp=P1^0x0f;
	switch(tmp)
	{
		case 1:num=1;break;
		case 2:num=2;break;
		case 4:num=3;break;
		case 8:num=4;break;
	}
	P1=0xF0;	 //����P1 4~7Ϊ�ߵ�ƽ
	P3=0x1F;	 //����P3 0~5Ϊ�ߵ�ƽ
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