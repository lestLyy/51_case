#include "reg51.h"
#include <intrins.h>   //包含nop等系统函数
typedef  unsigned int uint;
typedef  unsigned char uchar;

sbit leftBack=P1^2;
sbit leftFor=P1^3;
sbit enL=P1^4;
sbit enR=P1^5;
sbit rightFor=P1^6;
sbit rightBack=P1^7;
sbit Servo = P2^7;//舵机接口
sbit echo=P2^0;		  //超声波
sbit trig=P2^1;
sbit led_left=P3^2;	   //左前红外线
sbit led_right=P3^3;   //右前红外线

uchar left_pwm,right_pwm,pwm_t;
bit flag; uint S;
void delay(unsigned int z)//毫秒级延时
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

void delay10us(unsigned char i)    	//10us延时函数 启动超声波模块时使用
{ 
   unsigned char j; 
	do{ 
		j = 10; 
		do{ 
			_nop_(); 
		}while(--j); 
	}while(--i); 
}

void motor_init()
{
	leftBack=0;
	rightBack=0;
	leftFor=0;
	rightFor=0;
	
}

void go()
{
//	 left_pwm=100;
//	 right_pwm=100;
	 enL=1;
	 enR=1;
	 motor_init();
	 leftFor=1;
	 rightFor=1;
}
void back(uint ms)											 
{
//	 left_pwm=100;
//	 right_pwm=100;
	 enL=1;
	 enR=1;
	 motor_init();
	 leftBack=1;
	 rightBack=1;
	 delay(ms);
}

void left_turn(uint ms)
{
//	left_pwm=190;
//	right_pwm=60;
	enL=1;
	enR=1;
	motor_init();
	rightFor=1;
	leftBack=1;
	delay(ms);
}

void right_turn(uint ms)
{
//	left_pwm=60;
//	right_pwm=200;
	enL=1;
	enR=1;
	motor_init();
	leftFor=1;
	rightBack=1;
	delay(ms);
}

void stop()
{
	left_pwm=251;
	right_pwm=251;
	enL=0;
	enR=0;
}
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

uint Count_s()
{
	uint time;
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(float)(time*1.085)*0.17;     //算出来是MM
	if((S>=7000)||flag==1) //超出测量范围
	{	 
		flag=0;
	}
	return S;
}

void StartModule() 		         //启动超声波模块
{
	  trig=1;			                     //启动一次模块
      delay10us(2);
	  trig=0;
}

uint servo_left()	 //舵机0°
{
	uchar i,s;
 	for(i=0;i<10;i++)
	{	
		Servo = 1;
		Delay1550us();
		Servo = 0;
		Delay18450us();
	}

	delay(2000);
	StartModule();
	while(!echo);
	TR0=1;
	while(echo);
	TR0=0;
	s=Count_s();
	return s;
}

uint servo_right()     //  舵机90°
{
	uchar i,s;
	 for(i=0;i<10;i++)		  //90°
	{	
		Servo = 1;
		Delay600us();
		Servo = 0;
		Delay19400us();
	}

	delay(2000);
	StartModule();
	while(!echo);
	TR0=1;
	while(echo);
	TR0=0;
	s=Count_s();	
	return s;
}


void avoid()
{
	uint s1,s2;
	//为0 识别障碍物 为1没有识别到障碍物
	if(led_left==0 || led_right==0)
	{
		stop();
		delay(100);
	   	s2=servo_right();
		delay(1000);
		s1=servo_left();
		delay(1000);
		back(80);
		if(s1>s2)
		{			
			right_turn(150);
			
		}
		else
		{
			left_turn(150);
		}
	}
	else
	{
		go();
	}
}

void init(void)
{
   	EA = 1;	    //开总中断
	TMOD |= 0x01;//定时器0工作模块1,16位定时模式。T0用测ECH0脉冲长度
	TH0 = 0;
	TL0 = 0;//T0,16位定时计数用于记录ECHO高电平时间
	ET0 = 1;//允许定时器0中断

	TMOD |= 0x20;	//定时器1，8位自动重装模块
	TH1 = 220;     
    TL1 = 220;	   //11.0592M晶振下占空比最大比值是256,输出100HZ
	TR1 = 1;//启动定时器1
//	ET1 = 1;//允许定时器1中断
}

void servo_init()
{
	Servo = 0;
	Servo = 1;
	Delay1550us();
	Servo = 0;
	Delay18450us();
	Servo = 1;
	delay(1000);
}
void time_0() interrupt 1
{
	flag=1;
}
//void time_pwm() interrupt 3
//{
//	pwm_t++;
//	if(pwm_t==250)
//		pwm_t=enL=enR=0;
//	if(left_pwm== pwm_t)
//		enL=1;
//	if(right_pwm==pwm_t)
//		enR=1;
//}

void main()
{
	servo_init();
	init();
	
	while(1)
	{		
		avoid();
	
	}
}
