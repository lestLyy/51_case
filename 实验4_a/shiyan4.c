/*
* ¡¾±àÐ´Ê±¼ä¡¿£º 2017Äê06ÔÂ11ÈÕ
* ¡¾×÷    Õß¡¿£º ÇåÏèµç×Ó:03
* ¡¾°æ    ±¾¡¿£º 1.6
* ¡¾Íø    Õ¾¡¿£º http://www.qxmcu.com/ 
* ¡¾ÌÔ±¦µêÆÌ¡¿£º http://qxmcu.taobao.com/ (Ö±Ïúµê)
* ¡¾ÊµÑéÆ½Ì¨¡¿£º QX-MCS51 µ¥Æ¬»ú¿ª·¢°å & QX-A51ÖÇÄÜÐ¡³µ
* ¡¾Íâ²¿¾§Õñ¡¿£º 11.0592mhz	
* ¡¾Ö÷¿ØÐ¾Æ¬¡¿£º STC89C52
* ¡¾±àÒë»·¾³¡¿£º Keil ¦ÌVisio4
* ********************************¡¾½ÓÏßËµÃ÷¡¿********************************
             ÒÔÏÂ"A_"±íÊ¾ÖÇÄÜÐ¡³µµ×°å~~~"B_"±íÊ¾¿ª·¢°å     
*¿ª·¢°å¹©µçÏß  £ºA_J5-VCC~~~B_VCC»ò5V0    A_J6-GND~~~B_GND £¨Ò»¹²Ê¹ÓÃ2¸ù¶Å°îÏß£©
*µç»ú¿ØÖÆÏß    £ºA_J10-P1.2ÖÁP1.7 ¶ÔÓ¦½Óµ½B_P1.2ÖÁP1.7 £¨Ò»¹²Ê¹ÓÃ6¸ù¶Å°îÏß£©
*³¬Éù²¨Ä£¿é·´À¡Ïß£ºA_J2-P20~~~B_P20    A_J2-P21~~~B_P21 £¨Ò»¹²Ê¹ÓÃ2¸ù¶Å°îÏß£©
*±ÜÕÏÑ°¼£·´À¡Ïß£ºA_J11-P3.2ÖÁP3.5 ¶ÔÓ¦½Óµ½B_P3.2ÖÁP3.5 £¨Ò»¹²Ê¹ÓÃ4¸ù¶Å°îÏß£©
******************************************************************************
* ¡¾³ÌÐò¹¦ÄÜ¡¿£ºQX-A51ÖÇÄÜÐ¡³µÑ°¼£±ÜÕÏ¶þºÏÒ»£¨ÔÚºÚÏßÑ°¼£µÄ¹ý³ÌÖÐÈçÓöÕÏ°­ÎïÔò×Ô¶¯µôÍ·£©		   			            			    
* ¡¾Ê¹ÓÃËµÃ÷¡¿£º½ÓÏßÎÞÎóºó£¬ÉÕÐ´³ÌÐò´ò¿ªµçÔ´¿ª¡¢°´ÏÂS2°´¼üºó·äÃùÆ÷·¢³öÌáÊ¾Òô1ÃëºóÆô¶¯Ð¡³µ
* ¡¾×¢ÒâÊÂÏî¡¿£º±ÜÃâÐ¡³µ×²ÏòÕÏ°­Îï»òÐ¡³µÂÖ×Ó¶Â×ª¡£
				Ð¡³µµçÑ¹²»ÄÜµÍÓÚ6V.
				´Ë³ÌÐòÖ»×ö²Î¿¼£¬Êµ¼ÊÔËÐÐÐ§¹ûÐè¸ù¾Ý²»Í¬ÊµÑé³¡µØ½øÐÐ²»Í¬µ÷ÊÔ
**********************************************************************************/
#include <reg52.h>//51Í·ÎÄ¼þ
#include <intrins.h>   //°üº¬nopµÈÏµÍ³º¯Êý
#include <QXA51.h>//QX-A51ÖÇÄÜÐ¡³µÅäÖÃÎÄ¼þ


sbit RX = P2^0;//ECHO³¬Éù²¨Ä£¿é»ØÏì¶Ë
sbit TX = P2^1;//TRIG³¬Éù²¨Ä£¿é´¥·¢¶Ë

unsigned char pwm_left_val = 180;//×óµç»úÕ¼¿Õ±ÈÖµ È¡Öµ·¶Î§0-170£¬0×î¿ì
unsigned char pwm_right_val = 180;//ÓÒµç»úÕ¼¿Õ±ÈÖµÈ¡Öµ·¶Î§0-170 ,0×î¿ì
unsigned char pwm_t;//ÖÜÆÚ
unsigned int  time = 0;//´«ÊäÊ±¼ä
unsigned long S = 0;//¾àÀë
bit      flag = 0;//³¬³ö²âÁ¿·¶Î§±êÖ¾Î»


void delay(unsigned int z)//ºÁÃë¼¶ÑÓÊ±
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}
void Delay10us(unsigned char i)    	//10usÑÓÊ±º¯Êý Æô¶¯³¬Éù²¨Ä£¿éÊ±Ê¹ÓÃ
{ 
   unsigned char j; 
	do{ 
		j = 10; 
		do{ 
			_nop_(); 
		}while(--j); 
	}while(--i); 
}
/*Ð¡³µÇ°½ø*/
void forward()
{
	left_motor_go; //×óµç»úÇ°½ø
	right_motor_go; //ÓÒµç»úÇ°½ø
}
/*Ð¡³µ×ó×ª*/
void left_run()
{
	left_motor_stops; //×óµç»úÍ£Ö¹
	right_motor_go; //ÓÒµç»úÇ°½ø	
}
/*Ð¡³µÓÒ×ª*/
void right_run()
{
	right_motor_stops;//ÓÒµç»úÍ£Ö¹
	left_motor_go;    //×óµç»úÇ°½ø
}


/*PWM¿ØÖÆÊ¹ÄÜ Ð¡³µÔ­µØ×ó×ª*/
//void left_rapidly()
//{
//	left_motor_back;
//	right_motor_go;	
//}

/*Ð¡³µºóÍË*/
void backward()
{
	left_motor_back; //×óµç»úºóÍË
	right_motor_back; //ÓÒµç»úºóÍË	
}
/*Ð¡³µÍ£Ö¹*/
void stop()
{
	right_motor_stops;//ÓÒµç»úÍ£Ö¹
	left_motor_stops; //×óµç»úÍ£Ö¹	
}


/*¶¨Ê±Æ÷1ÖÐ¶ÏÊä³öPWMÐÅºÅ*/
void timer1() interrupt 3
{
	pwm_t++;//ÖÜÆÚ¼ÆÊ±¼Ó
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)//×óµç»úÕ¼¿Õ±È	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)//ÓÒµç»úÕ¼¿Õ±È
		EN2 = 1;			 
}


/*ÅÐ¶ÏS2ÊÇ·ñ±»°´ÏÂ*/
//void keyscan()
//{
//	for(;;)	//ËÀÑ­»·
//	{
//		if(key_s2 == 0)// ÊµÊ±¼ì²âS2°´¼üÊÇ·ñ±»°´ÏÂ
//		{
//			delay(5); //Èí¼þÏû¶¶
//			if(key_s2 == 0)//ÔÙ¼ì²âS2ÊÇ·ñ±»°´ÏÂ
//			{
//				while(!key_s2);//ËÉÊÖ¼ì²â
//				beep = 0;	//Ê¹ÄÜÓÐÔ´·äÃùÆ÷
//				delay(200);//200ºÁÃëÑÓÊ±
//				beep = 1;	//¹Ø±ÕÓÐÔ´·äÃùÆ÷
//				break;		//ÍË³öFORËÀÑ­»·
//			}
//		}
//	}	
//}

/*¶¨Ê±Æ÷0ÖÐ¶Ï*/
void timer0() interrupt 1	//T0ÖÐ¶ÏÓÃÀ´¼ÆÊýÆ÷Òç³ö,³¬¹ý²â¾à·¶Î§
{
	flag=1;							 //ÖÐ¶ÏÒç³ö±êÖ¾			 
}
//void  StartModule() 		         //Æô¶¯³¬Éù²¨Ä£¿é
//{
//	  TX=1;			                     //Æô¶¯Ò»´ÎÄ£¿é
//      Delay10us(2);
//	  TX=0;
//}

///*¼ÆËã³¬Éù²¨Ëù²â¾àÀë²¢ÏÔÊ¾*/
//void Conut()
//{
//	time=TH0*256+TL0;
//	TH0=0;
//	TL0=0;
//	
//	S=(float)(time*1.085)*0.17;     //Ëã³öÀ´ÊÇMM
//	if((S>=7000)||flag==1) //³¬³ö²âÁ¿·¶Î§
//	{	 
//		flag=0;
//
//	}
//}

//Ñ°¼£
void tracking()
{
		//Îª0 Ã»ÓÐÊ¶±ðµ½ºÚÏß Îª1Ê¶±ðµ½ºÚÏß
	if(left_led1 == 1 && right_led1 == 1)//×óÓÒÑ°¼£Ì½Í·Ê¶±ðµ½ºÚÏß
	{
		forward();//Ç°½ø
	}
	else
	{
		if(left_led1 == 1 && right_led1 == 0)//Ð¡³µÓÒ±ß³öÏß£¬×ó×ªÐÞÕý
		{
			left_run();//×ó×ª
		}
		if(left_led1 == 0 && right_led1 == 1)//Ð¡³µ×ó±ß³öÏß£¬ÓÒ×ªÐÞÕý
		{
			right_run();//ÓÒ×ª
		}
		if(left_led1 == 0 && right_led1 == 0)//×óÓÒÑ°¼£Ì½Í·¶¼Ã»Ê¶±ðµ½ºÚÏß
		{
			backward();//ºóÍË
		}		
	}	
}


void	Avoid()
{
	
	if(left_led2==0||right_led2==0)
	{
		backward();
		delay(150);

//		stop();
//		delay(80);


		left_run();
		delay(420);

		forward();
		delay(120);

		stop();
		delay(50);

		right_run();
		delay(370);

		forward();
		delay(200);

		stop();
		delay(80);

		left_run();
		delay(200);

		forward();
		delay(150);
	}

}
	
void main()
{
	unsigned int i;
//	keyscan();//µÈ´ý°´¼ü°´ÏÂÆô¶¯
	delay(1000);//ÑÓÊ±1Ãë
	TMOD |= 0x20;//¶¨Ê±Æ÷1¹¤×÷Ä£Ê½2,8Î»×Ô¶¯ÖØ×°¡£ÓÃÓÚ²úÉúPWM
	TMOD |= 0x01;//¶¨Ê±Æ÷0¹¤×÷Ä£¿é1,16Î»¶¨Ê±Ä£Ê½¡£T0ÓÃ²âECH0Âö³å³¤¶È
	TH1 = 220; //
	TL1 = 220; //100HZ T1
	TH0	= 0;
    TL0	= 0;//T0,16Î»¶¨Ê±¼ÆÊýÓÃÓÚ¼ÇÂ¼ECHO¸ßµçÆ½Ê±¼ä         
    ET1	= 1;//ÔÊÐíT1ÖÐ¶Ï
	ET0 = 1;//ÔÊÐíT0ÖÐ¶Ï
	TR1 = 1;//Æô¶¯¶¨Ê±Æ÷1
	EA  = 1;//Æô¶¯×ÜÖÐ¶Ï
	while(1)
	{		
		 Avoid();			//±ÜÕÏ
		 for(i=0; i<1800; i++)  //³¬Éù²¨Ã¿´Î²â¾à¼ä¸ô²»µÍÓÚ65ms
			tracking();	//Ñ°£	
	}
}