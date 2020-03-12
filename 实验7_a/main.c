/*********************************************************************************
* 【编写时间】： 2018年04月20日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ (直销店)
* 【实验平台】： QX-MCS51 单片机开发板 & QX-A51智能小车
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio4
* 【程序功能】：QX-A51智能小车WIFI控制		   			            			    
* 【使用说明】：请看资料内“QX-51智能小车wifi控制使用指南”文档
* 【注意事项】：请看资料内“QX-51智能小车wifi控制使用指南”文档
**********************************************************************************/
#include <reg52.h> //51头文件
#include <intrins.h>   //包含nop等系统函数
#include <QXA51.h>//QX-A51智能小车配置文件
unsigned char pwm_left_val = 30;//左电机占空比值 取值范围0-170，0最快
unsigned char pwm_right_val = 30;//右电机占空比值取值范围0-170 ,0最快
unsigned char pwm_t;//周期
unsigned char control=0X01;//车运动控制全局变量，默认开机为停车状态

/*****************ESPWIFI部分********************/
unsigned char ReceivedCount = 0;//串口数据接收计数
unsigned char ReadData; 	//读串口SBUF
bit ESP_EN = 0;//接收ESPwifi模块数据允许位
/***********************************************/

/******************************
超声波定义
*******************************/
sbit RX = P2^0;//ECHO超声波模块回响端
sbit TX = P2^1;//TRIG超声波模块触发端
unsigned int  time = 0;//传输时间
unsigned long S = 0;//距离
bit      flag = 0;//超出测量范围标志位

void Delay10us(unsigned char i)    	//10us延时函数 启动超声波模块时使用
{ 
   unsigned char j; 
	do{ 
		j = 10; 
		do{ 
			_nop_(); 
		}while(--j); 
	}while(--i); 
}

void delay(unsigned int z)//毫秒级延时
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}
	
/*小车前进*/
void forward()
{
	pwm_left_val = 120;//左电机占空比值 取值范围0-170，0最快
	pwm_right_val = 140	;
	left_motor_go; //左电机前进
	right_motor_go; //右电机前进
}
/*小车左转*/
void left_run()
{
	pwm_left_val = 140;//左电机占空比值 取值范围0-170，0最快
	pwm_right_val = 140	;
	left_motor_stops; //左电机停止
	right_motor_go; //右电机前进	
}
/*小车右转*/
void right_run()
{
	pwm_left_val = 140;//左电机占空比值 取值范围0-170，0最快
	pwm_right_val = 140	;
	right_motor_stops;//右电机停止
	left_motor_go;    //左电机前进
}

/*PWM控制使能 小车高速右转*/
void right_rapidly()
{
	pwm_left_val = 10;//左电机占空比值 取值范围0-170，0最快
	pwm_right_val = 150	;
	right_motor_go;
	left_motor_go;	
}
/*小车高速左转*/
void left_rapidly()
{
//	right_motor_go;
//	left_motor_back;
		pwm_left_val = 150;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 10	;
		right_motor_go;
		left_motor_go;
}


/*PWM控制使能 小车后退*/
void backward()
{
	left_motor_back; //左电机后退
	right_motor_back; //右电机后退	
}
void stop()
{
		left_motor_stops; //左电机后退
		right_motor_stops; //右电机后退	
}

/**************************************************
超声波程序
***************************************************/
void  StartModule() 		         //启动超声波模块
{
	  TX=1;			                     //启动一次模块
      Delay10us(2);
	  TX=0;
}

/*计算超声波所测距离*/
void Conut(void)
{
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(float)(time*1.085)*0.17;     //算出来是MM
	if((S>=7000)||flag==1) //超出测量范围
	{	 
		flag=0;
	}
}

/*超声波避障*/
void	Avoid()
{
	if(S < 400)//设置避障距离 ，单位毫米	刹车距离
	{
		stop();//停车
		backward();//后退
		delay(100);//后退时间越长、距离越远。后退是为了留出车辆转向的空间
		do{
			left_rapidly();//高速左转
			delay(90);//时间越长 转向角度越大，与实际行驶环境有关
			stop();//停车
			delay(100);//时间越长 停止时间越久长

			StartModule();	//启动模块测距，再次判断是否
			while(!RX);		//当RX（ECHO信号回响）为零时等待
			TR0=1;			    //开启计数
			while(RX);			//当RX为1计数并等待
			TR0=0;				//关闭计数
			Conut();			//计算距离
			}while(S < 280);//判断前面障碍物距离
	}
	else
	{
		forward();//前进
	}	
}


//黑线寻迹
void BlackLine()
{
		//为0 没有识别到黑线 为1识别到黑线
	if(left_led1 == 1 && right_led1 == 1)//左右寻迹探头识别到黑线
	{
		forward();//前进
	}
	else
	{
		if(left_led1 == 1 && right_led1 == 0)//小车右边出线，左转修正
		{
			left_run();//左转
		}
		if(left_led1 == 0 && right_led1 == 1)//小车左边出线，右转修正
		{
			right_run();//右转
		}
		if(left_led1 == 0 && right_led1 == 0)//左右寻迹探头都没识别到黑线
		{
			backward();//后退
		}		
	}	
}

//红外避障
void IRAvoid()
{
				//为0 识别障碍物 为1没有识别到障碍物
	if(left_led2 == 1 && right_led2 == 1)//左右都没识别到障碍物
	{
		pwm_left_val = 140;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 150;//右电机占空比值取值范围0-170 ,0最快
		forward();//前进
	}
	if(left_led2 == 1 && right_led2 == 0)//小车右侧识别到障碍物，左转躲避
	{
		pwm_left_val = 180;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 110;//右电机占空比值取值范围0-170 ,0最快
		left_run();//左转
		delay(40);//左转40毫秒（实现左小弯转）
	}
	if(left_led2 == 0 && right_led2 == 1)//小车左侧识别到障碍物，右转躲避
	{
		pwm_left_val = 100;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 180;//右电机占空比值取值范围0-170 ,0最快
		right_run();//右转
		delay(40);//右转40毫秒（实现右小弯转）
	}
	if(left_led2 == 0 && right_led2 == 0) //小车左右两侧都识别到障碍物，后退掉头
	{
		pwm_left_val = 150;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 160;//右电机占空比值取值范围0-170 ,0最快	
		backward();//后退
		delay(100);//后退的时间影响后退的距离。后退时间越长、后退距离越远。
		pwm_left_val = 140;//左电机占空比值 取值范围0-170，0最快
		pwm_right_val = 150;//右电机占空比值取值范围0-170 ,0最快
		right_rapidly();//高速右转
		delay(180);//延时时间越长，转向角度越大。	
	}		
}

//超声波避障
void Ultrasonic()
{
	StartModule();	//启动模块测距
	while(!RX);		//当RX（ECHO信号回响）为零时等待
	TR0=1;			    //开启计数
	while(RX);			//当RX为1计数并等待
	TR0=0;				//关闭计数
	Conut();			//计算距离
	Avoid();			//避障
	delay(65);			//测试周期不低于60MS	
}

//红外物体跟随
void IRTracking()
{
		//为0 识别障碍物 为1没有识别到障碍物
	if(left_led2 == 0 && right_led2 == 0)//左右识别到障碍物，前进跟随
	{
		forward();//前进
	}
	if(left_led2 == 1 && right_led2 == 0)//小车右侧识别到障碍物，右转跟随
	{
		right_run();//右转
	}
	if(left_led2 == 0 && right_led2 == 1)//小车左侧识别到障碍物，左转跟随
	{
		left_run();//左转
	}
}

//初始化
void Init(void)
{
   	EA = 1;	    //开总中断

   	SCON |= 0x50; 	// SCON: 模式1, 8-bit UART, 使能接收
	T2CON |= 0x34; //设置定时器2为串口波特率发生器并启动定时器2
	TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //设置波特率	    自定义波特率与串口通信波特率相同
	TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
	ES= 1; 			//打开串口中断

	TMOD |= 0x01;//定时器0工作模块1,16位定时模式。T0用测ECH0脉冲长度
	TH0 = 0;
	TL0 = 0;//T0,16位定时计数用于记录ECHO高电平时间
	ET0 = 1;//允许定时器0中断

	TMOD |= 0x20;	//定时器1，8位自动重装模块
	TH1 = 220;     
    TL1 = 220;	   //11.0592M晶振下占空比最大比值是256,输出100HZ
	TR1 = 1;//启动定时器1
	ET1 = 1;//允许定时器1中断
}


void main()
{
	unsigned char *CIPMUX;	 //设置多连接
	unsigned char *CIPSERVER;//设置端口
	unsigned char *CIPSTO;//设置超时链接
	delay(2000);//延时2秒
	Init();//定时器、串口初始化

	do
	{
		CIPMUX = "AT+CIPMUX=1\r\n";//设置多连接	
		while(*CIPMUX != '\0')
		{
			SBUF=*(CIPMUX++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//延时1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	do
	{
		CIPSERVER = "AT+CIPSERVER=1,8080\r\n";//设置端口	
		while(*CIPSERVER != '\0')
		{
			SBUF=*(CIPSERVER++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//延时1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	do
	{
		CIPSTO = "AT+CIPSTO=0\r\n";//串口发送指令AT+CIPSTO=0，设置服务超时时间，0即永远不超时	
		while(*CIPSTO != '\0')
		{
			SBUF=*(CIPSTO++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//延时1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	beep = 0;
	delay(100);
	beep = 1;

	while(1)
	{
		if(control > 0x17)//如果成立，则表示接收的命令不在运行命令内
	   {
	   	stop();	//  停车 
	   }
		switch(control)
		{
			case 0x02:	forward();	break;	//前进
			case 0x03:	backward();	break;	//后退
			case 0x04:	left_run();	break;	//左转
			case 0x05:	right_run();	break;//右转
			case 0x01:	stop();		break;//停车
			case 0x06:	left_rapidly();	break;//左旋转
			case 0x07:	right_rapidly();	break;//右旋转
			case 0x08:	beep = 0;	break;//鸣笛
			case 0x09:	beep = 1;	break;//停止鸣笛

			case 0X11:	Ultrasonic();		break;//超声波测距
			case 0X12:	BlackLine();		break;//黑线寻迹
			case 0X13:	IRAvoid();			break;//红外避障

			case 0X15:	IRTracking();		break;//红外-跟随物体
		}
	}
}

//定时器0中断
void timer0() interrupt 1
{
	flag=1;							 //中断溢出标志					 
}

void timer1() interrupt 3 		 //T1中断用来计数器溢出
{
    pwm_t++;//周期计时加
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)//左电机占空比	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)//右电机占空比
		EN2 = 1;			 
}

 /******************************************************************/
/* 串口中断程序*/
/******************************************************************/
void UART_SER () interrupt 4
{
	if(RI) 		//判断是接收中断产生
	{
		RI = 0; 	//标志位清零
		ReadData = SBUF; //读入缓冲区的值
	}
	if(ReadData == '+') //读到包头
	{
		ESP_EN = 1;
	}
	if(ESP_EN == 1)
	{
		ReceivedCount++;
	}
	if(ReceivedCount == 10)
	{
		control = SBUF; //读入缓冲区的值
		ReceivedCount = 0;
		ESP_EN = 0;	
		if((control >= 51) && (control <= 150))//左电机调速0~100个档位 手机端软件进行调节
			pwm_left_val = 170-((control-50)*1.7);	
		if((control >= 151) && (control <= 250))	  //右电机调速0~100个档位 手机端软件进行调节
			pwm_right_val = 170-((control-150)*1.7);
	}
		

}