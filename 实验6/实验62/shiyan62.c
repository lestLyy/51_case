#include"reg51.h"
#define uint unsigned int 
#define uchar unsigned char

uint code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

sbit LSA=P2^0;
sbit LSB=P2^1;
sbit LSC=P2^2;

void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<930;j++);
}

void init()
{
	SCON=0X50;//����ģʽ1,8λ�첽���������
	TMOD=0X20;//T1����ģʽ2,8λ�Զ�װ��
	TH1=0XFD;//���ּ׻���ͬһ������
	TL1=0XFD;
	PCON=0X00;
	RI=0;
	TR1=1;
	IE=0x90;//�������ж�
}

void main()
{
	uchar temp;
	init();
	
	TR1=1;  //����T1
		if(RI)
		{
			RI=0;
			P0=smg[SBUF];
			temp=smg[SBUF];		
			delay(10);
		}
  	 while(1)
  	{
   		if(RI)
		{
			RI=0;

			P0=smg[SBUF];
			delay(10);

			P2=temp;
			delay(10);
			
			temp=smg[SBUF];		
			
		}
	}
}