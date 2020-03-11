    #ifndef _lcd_H_
    #define _lcd_H_
 	#define LCD P0  /*LCD端口*/
	#define uint unsigned int 
    #define uchar	unsigned char
extern	uchar code WUYOU[2][32];
extern	uchar code XIAN[32];
extern	uchar code HUAN[32];
extern	uchar code JING[32];
extern 	uchar code JIAN[32];
extern	uchar code CE[32];
extern  uchar code WEN[32];
extern  uchar code DU[32];
extern  uchar code MAOHAO[32];
extern  uchar code LIANG[32];
extern  uchar code DUHAO[32];
extern  uchar code SHUZI[10][32];

   

	/*控制信号宏定义*/
    sbit CS1  = P2^1;
	sbit CS2  = P2^0;
	sbit E    = P2^3;
	sbit RW   = P2^4;
	sbit RS   = P2^5;
	
	sbit busy_bit =P0^7;
   	extern void lcd_mwc( uchar i);
	extern void lcd_mwd( uchar i );
	extern void lcd_clear(void);
    extern void lcd_init(void);
    extern void dispm_zi_up( uchar code *zi);
	extern void dispm_zi_down( uchar code *zi);
  
	
	#endif	