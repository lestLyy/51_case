#include<reg52.h>
#include<intrins.h>  
sbit IR=P3^2;           
unsigned char a[4];    //�����û��롢�û�������������롢�����ݷ���

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
		 for(j=0;j<8;j++)  //ÿ������8λ����
			 {
	         temp=temp>>1;  									
			   TH0=0;         
			   TL0=0;         
			   TR0=1;         
		      while(IR==0);   //�͵�ƽ�͵ȴ�	      
		  	   TR0=0;         
			   LowTime=TH0*256+TL0;    //����͵�ƽ���
			   TH0=0;         
			   TL0=0;         
			   TR0=1;         
			  while(IR==1);   //�ߵ�ƽ�͵ȴ�		   
			   TR0=0;        
			   HighTime=TH0*256+TL0;   //����ߵ�ƽ���
			   if((LowTime<360)||(LowTime>680))   
			  		    return 0;       		
			   if((HighTime>400)&&(HighTime<680))   
			           temp=temp&0x7f;       //(520-100=420, 520+100=620)�����λ��0
			   if((HighTime>1400)&&(HighTime<1850)) 
			           temp=temp|0x80;       //(1550-250=1300,1550+250=1800),���λ��1
		     }  			            
	   a[i]=temp;																						 
    }  				 		 
  if(a[2]=~a[3])  
	 return 1;     //������ȷ������1
}
void init(void)
{

	EA=1;        //�������ж�
  
   ET0=1;       //��ʱ��T0�ж�����
   IT0=1;       //���жϵ��½��ش���  
    TMOD=0x01;   //ʹ�ö�ʱ��T0��ģʽ1	
	TR0=0;       //��ʱ��T0�ر�
  EX0=1;       //�����ж�0
  
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
�������ܣ������ߴ��������жϴ�����
*************************************************************/
void Int0(void) interrupt 0
  {	 
     EX0=0;      //�ر����ж�0�����ٽ��ն��κ����źŵ��жϣ�ֻ���뵱ǰ�����ź�
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
	  HighTime=TH0*256+TL0;	//����������ĸߵ�ƽ����
  if((HighTime>4000)&&(HighTime<5000))
		{	 
                	DeCode();  
					P1=smg[a[2]]; 
					function_();
		 
          }
		  
	EX0=1; 
 
  }