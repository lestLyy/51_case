/*********************************************************************************
* ����дʱ�䡿�� 2018��04��20��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ�������� & QX-A51����С��
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio4
* �������ܡ���QX-A51����С��WIFI����		   			            			    
* ��ʹ��˵�������뿴�����ڡ�QX-51����С��wifi����ʹ��ָ�ϡ��ĵ�
* ��ע��������뿴�����ڡ�QX-51����С��wifi����ʹ��ָ�ϡ��ĵ�
**********************************************************************************/
#include <reg52.h> //51ͷ�ļ�
#include <intrins.h>   //����nop��ϵͳ����
#include <QXA51.h>//QX-A51����С�������ļ�
unsigned char pwm_left_val = 30;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
unsigned char pwm_right_val = 30;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���
unsigned char pwm_t;//����
unsigned char control=0X01;//���˶�����ȫ�ֱ�����Ĭ�Ͽ���Ϊͣ��״̬

/*****************ESPWIFI����********************/
unsigned char ReceivedCount = 0;//�������ݽ��ռ���
unsigned char ReadData; 	//������SBUF
bit ESP_EN = 0;//����ESPwifiģ����������λ
/***********************************************/

/******************************
����������
*******************************/
sbit RX = P2^0;//ECHO������ģ������
sbit TX = P2^1;//TRIG������ģ�鴥����
unsigned int  time = 0;//����ʱ��
unsigned long S = 0;//����
bit      flag = 0;//����������Χ��־λ

void Delay10us(unsigned char i)    	//10us��ʱ���� ����������ģ��ʱʹ��
{ 
   unsigned char j; 
	do{ 
		j = 10; 
		do{ 
			_nop_(); 
		}while(--j); 
	}while(--i); 
}

void delay(unsigned int z)//���뼶��ʱ
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}
	
/*С��ǰ��*/
void forward()
{
	pwm_left_val = 120;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
	pwm_right_val = 140	;
	left_motor_go; //����ǰ��
	right_motor_go; //�ҵ��ǰ��
}
/*С����ת*/
void left_run()
{
	pwm_left_val = 140;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
	pwm_right_val = 140	;
	left_motor_stops; //����ֹͣ
	right_motor_go; //�ҵ��ǰ��	
}
/*С����ת*/
void right_run()
{
	pwm_left_val = 140;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
	pwm_right_val = 140	;
	right_motor_stops;//�ҵ��ֹͣ
	left_motor_go;    //����ǰ��
}

/*PWM����ʹ�� С��������ת*/
void right_rapidly()
{
	pwm_left_val = 10;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
	pwm_right_val = 150	;
	right_motor_go;
	left_motor_go;	
}
/*С��������ת*/
void left_rapidly()
{
//	right_motor_go;
//	left_motor_back;
		pwm_left_val = 150;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 10	;
		right_motor_go;
		left_motor_go;
}


/*PWM����ʹ�� С������*/
void backward()
{
	left_motor_back; //��������
	right_motor_back; //�ҵ������	
}
void stop()
{
		left_motor_stops; //��������
		right_motor_stops; //�ҵ������	
}

/**************************************************
����������
***************************************************/
void  StartModule() 		         //����������ģ��
{
	  TX=1;			                     //����һ��ģ��
      Delay10us(2);
	  TX=0;
}

/*���㳬�����������*/
void Conut(void)
{
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(float)(time*1.085)*0.17;     //�������MM
	if((S>=7000)||flag==1) //����������Χ
	{	 
		flag=0;
	}
}

/*����������*/
void	Avoid()
{
	if(S < 400)//���ñ��Ͼ��� ����λ����	ɲ������
	{
		stop();//ͣ��
		backward();//����
		delay(100);//����ʱ��Խ��������ԽԶ��������Ϊ����������ת��Ŀռ�
		do{
			left_rapidly();//������ת
			delay(90);//ʱ��Խ�� ת��Ƕ�Խ����ʵ����ʻ�����й�
			stop();//ͣ��
			delay(100);//ʱ��Խ�� ֹͣʱ��Խ�ó�

			StartModule();	//����ģ���࣬�ٴ��ж��Ƿ�
			while(!RX);		//��RX��ECHO�źŻ��죩Ϊ��ʱ�ȴ�
			TR0=1;			    //��������
			while(RX);			//��RXΪ1�������ȴ�
			TR0=0;				//�رռ���
			Conut();			//�������
			}while(S < 280);//�ж�ǰ���ϰ������
	}
	else
	{
		forward();//ǰ��
	}	
}


//����Ѱ��
void BlackLine()
{
		//Ϊ0 û��ʶ�𵽺��� Ϊ1ʶ�𵽺���
	if(left_led1 == 1 && right_led1 == 1)//����Ѱ��̽ͷʶ�𵽺���
	{
		forward();//ǰ��
	}
	else
	{
		if(left_led1 == 1 && right_led1 == 0)//С���ұ߳��ߣ���ת����
		{
			left_run();//��ת
		}
		if(left_led1 == 0 && right_led1 == 1)//С����߳��ߣ���ת����
		{
			right_run();//��ת
		}
		if(left_led1 == 0 && right_led1 == 0)//����Ѱ��̽ͷ��ûʶ�𵽺���
		{
			backward();//����
		}		
	}	
}

//�������
void IRAvoid()
{
				//Ϊ0 ʶ���ϰ��� Ϊ1û��ʶ���ϰ���
	if(left_led2 == 1 && right_led2 == 1)//���Ҷ�ûʶ���ϰ���
	{
		pwm_left_val = 140;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 150;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���
		forward();//ǰ��
	}
	if(left_led2 == 1 && right_led2 == 0)//С���Ҳ�ʶ���ϰ����ת���
	{
		pwm_left_val = 180;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 110;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���
		left_run();//��ת
		delay(40);//��ת40���루ʵ����С��ת��
	}
	if(left_led2 == 0 && right_led2 == 1)//С�����ʶ���ϰ����ת���
	{
		pwm_left_val = 100;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 180;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���
		right_run();//��ת
		delay(40);//��ת40���루ʵ����С��ת��
	}
	if(left_led2 == 0 && right_led2 == 0) //С���������඼ʶ���ϰ�����˵�ͷ
	{
		pwm_left_val = 150;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 160;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���	
		backward();//����
		delay(100);//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ�������˾���ԽԶ��
		pwm_left_val = 140;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
		pwm_right_val = 150;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���
		right_rapidly();//������ת
		delay(180);//��ʱʱ��Խ����ת��Ƕ�Խ��	
	}		
}

//����������
void Ultrasonic()
{
	StartModule();	//����ģ����
	while(!RX);		//��RX��ECHO�źŻ��죩Ϊ��ʱ�ȴ�
	TR0=1;			    //��������
	while(RX);			//��RXΪ1�������ȴ�
	TR0=0;				//�رռ���
	Conut();			//�������
	Avoid();			//����
	delay(65);			//�������ڲ�����60MS	
}

//�����������
void IRTracking()
{
		//Ϊ0 ʶ���ϰ��� Ϊ1û��ʶ���ϰ���
	if(left_led2 == 0 && right_led2 == 0)//����ʶ���ϰ��ǰ������
	{
		forward();//ǰ��
	}
	if(left_led2 == 1 && right_led2 == 0)//С���Ҳ�ʶ���ϰ����ת����
	{
		right_run();//��ת
	}
	if(left_led2 == 0 && right_led2 == 1)//С�����ʶ���ϰ����ת����
	{
		left_run();//��ת
	}
}

//��ʼ��
void Init(void)
{
   	EA = 1;	    //�����ж�

   	SCON |= 0x50; 	// SCON: ģʽ1, 8-bit UART, ʹ�ܽ���
	T2CON |= 0x34; //���ö�ʱ��2Ϊ���ڲ����ʷ�������������ʱ��2
	TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //���ò�����	    �Զ��岨�����봮��ͨ�Ų�������ͬ
	TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
	ES= 1; 			//�򿪴����ж�

	TMOD |= 0x01;//��ʱ��0����ģ��1,16λ��ʱģʽ��T0�ò�ECH0���峤��
	TH0 = 0;
	TL0 = 0;//T0,16λ��ʱ�������ڼ�¼ECHO�ߵ�ƽʱ��
	ET0 = 1;//����ʱ��0�ж�

	TMOD |= 0x20;	//��ʱ��1��8λ�Զ���װģ��
	TH1 = 220;     
    TL1 = 220;	   //11.0592M������ռ�ձ�����ֵ��256,���100HZ
	TR1 = 1;//������ʱ��1
	ET1 = 1;//����ʱ��1�ж�
}


void main()
{
	unsigned char *CIPMUX;	 //���ö�����
	unsigned char *CIPSERVER;//���ö˿�
	unsigned char *CIPSTO;//���ó�ʱ����
	delay(2000);//��ʱ2��
	Init();//��ʱ�������ڳ�ʼ��

	do
	{
		CIPMUX = "AT+CIPMUX=1\r\n";//���ö�����	
		while(*CIPMUX != '\0')
		{
			SBUF=*(CIPMUX++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//��ʱ1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	do
	{
		CIPSERVER = "AT+CIPSERVER=1,8080\r\n";//���ö˿�	
		while(*CIPSERVER != '\0')
		{
			SBUF=*(CIPSERVER++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//��ʱ1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	do
	{
		CIPSTO = "AT+CIPSTO=0\r\n";//���ڷ���ָ��AT+CIPSTO=0�����÷���ʱʱ�䣬0����Զ����ʱ	
		while(*CIPSTO != '\0')
		{
			SBUF=*(CIPSTO++);		
			while(TI==0);		
			TI=0;						
		}
		delay(1000);//��ʱ1000ms
	}while(ReadData != '\n');
	ReadData = '0';

	beep = 0;
	delay(100);
	beep = 1;

	while(1)
	{
		if(control > 0x17)//������������ʾ���յ����������������
	   {
	   	stop();	//  ͣ�� 
	   }
		switch(control)
		{
			case 0x02:	forward();	break;	//ǰ��
			case 0x03:	backward();	break;	//����
			case 0x04:	left_run();	break;	//��ת
			case 0x05:	right_run();	break;//��ת
			case 0x01:	stop();		break;//ͣ��
			case 0x06:	left_rapidly();	break;//����ת
			case 0x07:	right_rapidly();	break;//����ת
			case 0x08:	beep = 0;	break;//����
			case 0x09:	beep = 1;	break;//ֹͣ����

			case 0X11:	Ultrasonic();		break;//���������
			case 0X12:	BlackLine();		break;//����Ѱ��
			case 0X13:	IRAvoid();			break;//�������

			case 0X15:	IRTracking();		break;//����-��������
		}
	}
}

//��ʱ��0�ж�
void timer0() interrupt 1
{
	flag=1;							 //�ж������־					 
}

void timer1() interrupt 3 		 //T1�ж��������������
{
    pwm_t++;//���ڼ�ʱ��
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)//����ռ�ձ�	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)//�ҵ��ռ�ձ�
		EN2 = 1;			 
}

 /******************************************************************/
/* �����жϳ���*/
/******************************************************************/
void UART_SER () interrupt 4
{
	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI = 0; 	//��־λ����
		ReadData = SBUF; //���뻺������ֵ
	}
	if(ReadData == '+') //������ͷ
	{
		ESP_EN = 1;
	}
	if(ESP_EN == 1)
	{
		ReceivedCount++;
	}
	if(ReceivedCount == 10)
	{
		control = SBUF; //���뻺������ֵ
		ReceivedCount = 0;
		ESP_EN = 0;	
		if((control >= 51) && (control <= 150))//��������0~100����λ �ֻ���������е���
			pwm_left_val = 170-((control-50)*1.7);	
		if((control >= 151) && (control <= 250))	  //�ҵ������0~100����λ �ֻ���������е���
			pwm_right_val = 170-((control-150)*1.7);
	}
		

}