#include<reg51.h>
sbit P1_0=P1^0;
unsigned char num[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88};
unsigned char wei[4]={0x01,0x02,0x04,0x08};
unsigned int count=0;


void DelayMS(unsigned int ms)
{

	unsigned char i;
	while(ms--) 
		for(i=0;i<120;i++);
}
void main()
{
	unsigned char i=0;
	unsigned char dspcode[4];
	unsigned long t;
	P3=num[i];
	ET0=1;
	EA=1;
	TH0=0x3c;
	TL0=0xb0;
	TMOD=0x01;
while(1)
{
     if(P1_0==0)

     {
         TR0=1;
         i++;
		 P2=0x01;
         P0=num[i];

         while(P1_0==0);
         TR0=0;
         if(i==10)
         {
             t=((TH0<<8)+TL0)/1000;

             t=(t+50*count)/10;

             dspcode[0]=num[(t/1000)%10]^0x80;
	         dspcode[1]=num[(t/100)%10];
             dspcode[2]=num[(t/10)%10];
             dspcode[3]=num[t%10];
             break;

         }

     }
}

while(1)
{
     for(i=0;i<4;i++)
     {
         P2=wei[i];
         P0=dspcode[i];
         DelayMS(5);
     }
}

}
void int1() interrupt 1
{
	count++;	 //50ms
	TL0=0xb0;	
	TH0=0x3c;

}