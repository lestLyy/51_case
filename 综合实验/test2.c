#include<reg52.h>
#include<intrins.h>  
sbit IR=P3^2;           
unsigned char a[4];    //储存用户码、用户反码与键数据码、键数据反码

sbit led=P2^0;
sbit beep=P2^1;
sbit motor=P2^2;




unsigned int LowTime,HighTime; 
unsigned int code smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
bit DeCode(void)        
{
    
   unsigned char  i,j;
	unsigned char temp;    
	for(i=0;i<4;i++)      
	  {
		 for(j=0;j<8;j++)  //每个码有8位数字
			 {
	         temp=temp>>1;  									
			   TH0=0;         
			   TL0=0;         
			   TR0=1;         
		      while(IR==0);   //低电平就等待	      
		  	   TR0=0;         
			   LowTime=TH0*256+TL0;    //保存低电平宽度
			   TH0=0;         
			   TL0=0;         
			   TR0=1;         
			  while(IR==1);   //高电平就等待		   
			   TR0=0;        
			   HighTime=TH0*256+TL0;   //保存高电平宽度
			   if((LowTime<360)||(LowTime>680))   
			  		    return 0;       		
			   if((HighTime>400)&&(HighTime<680))   
			           temp=temp&0x7f;       //(520-100=420, 520+100=620)，则该位是0
			   if((HighTime>1400)&&(HighTime<1850)) 
			           temp=temp|0x80;       //(1550-250=1300,1550+250=1800),则该位是1
		     }  			            
	   a[i]=temp;																						 
    }  				 		 
  if(a[2]=~a[3])  
	 return 1;     //解码正确，返回1
}
void init(void)
{

	EA=1;        //开启总中断
  
   ET0=1;       //定时器T0中断允许
   IT0=1;       //外中断的下降沿触发  
    TMOD=0x01;   //使用定时器T0的模式1	
	TR0=0;       //定时器T0关闭
  EX0=1;       //开外中断0
  
}




void function_()
{
	switch(a[2])
	{
		case 1:led=0;motor=1;beep=1;break;
		case 2:beep=0;led=1;motor=1;break;
		case 3:led=1;beep=1;motor=0;break;
		default: led=1;motor=1;beep=1;
	}	
}


void main(void)
{		
    init();
	while(1)
	{ 
    }
 
}


/************************************************************
函数功能：红外线触发的外中断处理函数
*************************************************************/
void Int0(void) interrupt 0
  {	 
     EX0=0;      //关闭外中断0，不再接收二次红外信号的中断，只解码当前红外信号
	  TH0=0;      
	 TL0=0;     
	  TR0=1;	     
	  while(IR==0);        
	  TR0=0;                   
	  LowTime=TH0*256+TL0;  
	  if(((LowTime>8500)&&(LowTime<9500))!=1) {EX0=1;return;} 
	  TH0=0;      
	  TL0=0;      
      TR0=1;	   
 while(IR==1);  
	  
	  
	  TR0=0;        
	  HighTime=TH0*256+TL0;	//保存引导码的高电平长度
  if((HighTime>4000)&&(HighTime<5000))
		{	 
                	DeCode();  
					P1=smg[a[2]]; 
					function_();
		 
          }
		  
	EX0=1; 
 
  }