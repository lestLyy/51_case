#include "reg51.h"

#define SBM   0x80                //ʶ����
#define m_9    (65536-9000) 	      //Լ9mS
#define m_45  (65536-4500) 	      //Լ4.5mS
#define m_68  (65536-1680) 	      //Լ1.68mS
#define m_56  (65536-560) 	      //Լ0.56mS
#define m_40   (65536-40000)       //Լ40mS

typedef unsigned char uchar;
typedef unsigned int uint; 
uint key_val;
sbit IR = P3^0;				  //���巢�����ţ���PNP�����ܻ�����
uint code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void delay(uchar ms)
{
	while(ms--);
}

void key_scan()
{
	int a=0;
	P1=0x0f;
	if(P1!=0x0f)  //��������
	{
		delay(1000);
		 if(P1!=0x0f)
		 {
		 	P1=0x0f;
			switch(P1)
			{
				case 0x0d:key_val=0;break;
				case 0x0b:key_val=1;break;
				case 0x07:key_val=2;break;
			}
			delay(10);
			P1=0xf0;
			switch(P1)
			{
				case 0xe0:key_val+=0;break;
				case 0xd0:key_val+=3;break;
				case 0xb0:key_val+=6;break;
				case 0x70:key_val+=9;break;
			}
		}

	}

}

void T0_delay(bit BT,uint x)
{
  TH0 = x>>8;	            //����T0��ʼֵ
  TL0 = x;
  TF0=0;			        //��0
  TR0=1;			        //������ʱ��0
  if(BT == 0) while(!TF0);	//BT=0ʱ������38KHz����ֻ��ʱ��BT=1����38KHz��������ʱ��
  else while(1)			    //38KHz���壬ռ�ձ�5:26
         {
		  IR = 0;
		  if(TF0)break;
 	      if(TF0)break;
		  IR = 1;
  	      if(TF0)break;
   	      if(TF0)break;
   	      if(TF0)break;
		  if(TF0)break;
   	      if(TF0)break;
   	      if(TF0)break;
   	      if(TF0)break;
   	      if(TF0)break;
   	      if(TF0)break;
		  if(TF0)break;
		 }
  TR0=0;			        //�رն�ʱ��0
  TF0=0;			        //��־λ�������0

  IR =1;			        //����ֹͣ�󣬷���˿ڳ�̬Ϊ�ߵ�ƽ
}

void send_one_byte(uchar data1)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		 T0_delay(1,m_56);    //�ߵ�ƽ0.56ms
		 if(data1&0x01)
		 	T0_delay(0,m_68);	 // 1
		 else
		 	T0_delay(0,m_56);	 // 0
		 data1>>=1;
	}
}

void send_msg(uchar data1)
{
	T0_delay(1,m_9);
	T0_delay(0,m_45);      //������

	send_one_byte(SBM);
	send_one_byte(~SBM);
	send_one_byte(data1);
	send_one_byte(~data1);

	T0_delay(1,m_56);	  //������
	T0_delay(0,m_40);

}


int main()
{

	uint p_val;
	TMOD = 0x01;         //T0 16λ������ʽ
	

	while(1)
	{
		key_scan();
		IR=1;				//����˿ڳ�̬Ϊ�ߵ�ƽ
		if(p_val!=key_val)
		{

			p_val=key_val;	
			P2=smg[key_val];
			delay(20);
			send_msg(key_val);
		}
						   
		
		
	}
}


