#include <AT89x51.H>		//器件配置文件
#include <intrins.h>
#define  RX  P2_0
#define  TX  P2_1

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit enL=P1^4;
sbit enR=P1^5;
sbit rightFor=P1^6;
sbit rightBack=P1^7;
sbit Servo = P2^7;//舵机接口
unsigned int  time=0;
unsigned int  timer=0;
unsigned char posit=0;
unsigned long S=0;
bit      flag =0;


sbit echo=P2^0;
sbit trig=P2^1;

void delay(unsigned int z)//毫秒级延时
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}
/*舵机方波延时朝向小车正前方*/
void Delay1550us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 3;
	j = 196;
	do
	{
		while (--j);
	} while (--i);
}

void Delay18450us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 34;
	j = 16;
	do
	{
		while (--j);
	} while (--i);
}
/*舵机方波延时向小车右方*/
void Delay600us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}
void Delay19400us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 35;
	j = 197;
	do
	{
		while (--j);
	} while (--i);
}
/*舵机方波延时朝向小车左方*/
void Delay17500us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 32;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}
void Delay2500us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 5;
	j = 120;
	do
	{
		while (--j);
	} while (--i);
}
void go()
{
	enL=1;
	enR=1;
	leftBack=0;
	rightBack=0;
	leftFor=1;
	rightFor=1;
}

void stop()
{
	enL=0;
	enR=0;
}

void rightTurn()
{
	enL=1;
	enR=1;
	rightFor=0;
	leftBack=0;
	leftFor=1;
	rightBack=0;
}
void leftTurn()
{
	enL=1;
	enR=1;
	rightBack=0;
	leftFor=0;
	rightFor=1;
	leftBack=0;
}
unsigned long count()
{
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
	while(!RX);		//当RX（ECHO信号回响）为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
	time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 return (float)(time*1.085)*0.17;     //算出来是MM
}
void select()
	{
	unsigned char i,s0,s1,s2;

	for(i=0;i<10;i++)			 //0°
	{	
		Servo = 1;
		Delay1550us();
		Servo = 0;
		Delay18450us();
	}

	 delay(50);
	 s0=count();
	 delay(10);
	 while(s0>160)
		{
			
			go();
			delay(30);
			stop();
			s0=count();
			delay(120);
		}

	 stop();
	 delay(100);
	 for(i=0;i<10;i++)			   //-90°
		{	
			Servo = 1;
			Delay2500us();
			Servo = 0;
			Delay17500us();
		}
		
	   s1=count();
	   delay(100);
		for(i=0;i<10;i++)		  //90°
		{	
			Servo = 1;
			Delay600us();
			Servo = 0;
			Delay19400us();
		}
		s2=count();
		delay(100);
		if(s1>s2)
		{
			leftTurn();
			delay(270);
			stop();
			delay(200);
			do
			{
				s0=count();
				delay(150);
				go();
				delay(20);
				stop();
				s0=count();
			}while(s0<220);

			stop();
			delay(100);
			rightTurn();
			delay(250);
			go();
			delay(300);
			stop();
		}
		else
		{
			rightTurn();
			delay(270);
			stop();
			delay(200);
			do
			{
				s0=count();
				delay(150);
				go();
				delay(20);
				stop();
				s0=count();
			}while(s0<220);

			stop();
			delay(100);
			leftTurn();
			delay(250);
			go();
			delay(300);
			stop();
		}

	 
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
	 timer++;
	 if(timer>=250)
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
  	unsigned char i;
    TMOD=0x11;		   //设T0,T1为方式1定时模式，GATE=1；
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2MS定时
	TL1=0x30;
	ET0=1;             //允许T0中断
	ET1=1;			   //允许T1中断
	TR1=1;			   //开启定时器1
	EA=1;			   //开启总中断
	

     select();			//计算
	while(1);
  }
                