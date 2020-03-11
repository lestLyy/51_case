#include"reg51.h"
#define uint unsigned int
#define uchar unsigned char

sbit led=P1^0;
uint count=0,t=0;

sbit du=P2^6;
sbit wei=P2^7;

uint code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<112;j++);
}
void initTime()
{

	TMOD = 0x01;   
 	TH0=0xd8;     //��ʱ10ms
	TL0=0xf0;     
	TR0=1;       //������ʱ��
	ET0=1;       //����ʱ��0�ж�
	EA=1;        //�������ж�
	//P1=0xff; 	 //��ʼ��P1������˿�
}

void display()
{
	uint count1,count2;
	count1=count/10;
	count2=count%10;

	P0=0xff;
	if(count<10)
	{
		wei=1;
		P0=0x02;
		wei=0;
		delay(1);
		du=1;
		P0=smg[count];
		du=0;	
	}

	if(count>=10)
	{
				
		wei=1;
		P0=0x02;
		
		
		wei=0;
		delay(1);
		P0=0xff;
		du=1;
		P0=smg[count1];
		du=0;

		wei=1;
		P0=0x01;
		wei=0;
		delay(1);
		P0=0xff;
		du=1;
		P0=smg[count2];
		du=0;	
	}
	
}
void main()
{
	initTime();
	while(1)
		display();

}

init0()interrupt 1
{
	TH0=0xd8;     //��ʱ10ms
	TL0=0xf0;
	t++;
	if(t==100)	   
	{
		led=~led;
		count++;
		if(count>99)
			count=0;
		while(t--);
		t=0;	
	}
}
