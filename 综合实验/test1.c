#include "reg51.h"

#define SBM   0x80                //识别码
#define m_9    (65536-9000) 	      //约9mS
#define m_45  (65536-4500) 	      //约4.5mS
#define m_68  (65536-1680) 	      //约1.68mS
#define m_56  (65536-560) 	      //约0.56mS
#define m_40   (65536-40000)       //约40mS

typedef unsigned char uchar;
typedef unsigned int uint; 
uint key_val;
sbit IR = P3^0;				  //定义发射引脚（接PNP三极管基极）
uint code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void delay(uchar ms)
{
	while(ms--);
}

void key_scan()
{
	int a=0;
	P1=0x0f;
	if(P1!=0x0f)  //按键按下
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
  TH0 = x>>8;	            //输入T0初始值
  TL0 = x;
  TF0=0;			        //清0
  TR0=1;			        //启动定时器0
  if(BT == 0) while(!TF0);	//BT=0时不发射38KHz脉冲只延时；BT=1发射38KHz脉冲且延时；
  else while(1)			    //38KHz脉冲，占空比5:26
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
  TR0=0;			        //关闭定时器0
  TF0=0;			        //标志位溢出则清0

  IR =1;			        //脉冲停止后，发射端口常态为高电平
}

void send_one_byte(uchar data1)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		 T0_delay(1,m_56);    //高电平0.56ms
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
	T0_delay(0,m_45);      //引导吗

	send_one_byte(SBM);
	send_one_byte(~SBM);
	send_one_byte(data1);
	send_one_byte(~data1);

	T0_delay(1,m_56);	  //结束吗
	T0_delay(0,m_40);

}


int main()
{

	uint p_val;
	TMOD = 0x01;         //T0 16位工作方式
	

	while(1)
	{
		key_scan();
		IR=1;				//发射端口常态为高电平
		if(p_val!=key_val)
		{

			p_val=key_val;	
			P2=smg[key_val];
			delay(20);
			send_msg(key_val);
		}
						   
		
		
	}
}


