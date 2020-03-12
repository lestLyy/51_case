   
#include <AT89x51.H>		//器件配置文件
#include <intrins.h>
#define  RX  P2_0
#define  TX  P2_1
sbit DU  = P2^6;   //数码管段选
sbit WE  = P2^7;   //数码管位选

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit enL=P1^4;
sbit enR=P1^5;
sbit rightFor=P1^6;
sbit rightBack=P1^7;

unsigned int  time=0;
unsigned int  timer=0;
unsigned char posit=0;
unsigned long S=0;
bit      flag =0;
unsigned char const discode[] ={ 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,0x6F,0x40,0x00/*-*/};
unsigned char const positon[4]={ 0xfe,0xfd,0xfb,0xf7};
unsigned char disbuff[4]	   ={ 0,0,0,0,};
void go()
{
	enL=1;
	enR=1;
	leftBack=0;
	rightBack=0;
	leftFor=1;
	rightFor=1;
}
/********************************************************/
    void Display(void)				 //扫描数码管
	{
	DU = 0;	
	 if(posit==0)
	 {P0=(discode[disbuff[posit]]);}
	 else
	 {P0=discode[disbuff[posit]];}
	 DU = 1;
	 DU = 0;

	 WE = 0;
	  P0=positon[posit];
	  WE=1;
	  WE=0;
	  if(++posit>=4)
	  posit=0;
	}
/********************************************************/
    void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(float)(time*1.085)*0.17;     //算出来是MM
	 
	 if((S>=7000)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
	  disbuff[0]=10;	   //“-”
	  disbuff[1]=10;	   //“-”
	  disbuff[2]=10;	   //“-”
	  disbuff[3]=10;	   //“-”
	 }
	 if(S<450)
	 {	  
	 	  enL=0;
		  enR=0;
		  disbuff[0]=S/1000;
		  disbuff[1]=S%1000/100;
		  disbuff[2]=S%100/10;
		  disbuff[3]=S%10;
	 }	 
	 else
	 	go();
	}
/********************************************************/
     void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
  {
    flag=1;							 //中断溢出标志
  }
/********************************************************/
   void  zd3()  interrupt 3 		 //T1中断用来扫描数码管和计800MS启动模块
  {
	 TH1=0xf8;
	 TL1=0x30;
	 Display();
	 timer++;
	 if(timer>=400)
	 {
	  timer=0;
	  TX=1;			                //800MS  启动一次模块
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TX=0;
	 } 
  }
/*********************************************************/

	void  main(  void  )

  {  
    TMOD=0x11;		   //设T0,T1为方式1定时模式，GATE=1；
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2MS定时
	TL1=0x30;
	ET0=1;             //允许T0中断
	ET1=1;			   //允许T1中断
	TR1=1;			   //开启定时器1
	EA=1;			   //开启总中断

	while(1)
	{
	 while(!RX);		//当RX（ECHO信号回响）为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
     Conut();			//计算
	}

  }
                