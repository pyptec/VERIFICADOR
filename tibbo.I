
#line 1 "tibbo.c" /0
  
#line 1 "C:\Keil_v5\C51\Inc\reg51.h" /0






 
 
 
 
 
 
 sfr P0   = 0x80;
 sfr P1   = 0x90;
 sfr P2   = 0xA0;
 sfr P3   = 0xB0;
 sfr PSW  = 0xD0;
 sfr ACC  = 0xE0;
 sfr B    = 0xF0;
 sfr SP   = 0x81;
 sfr DPL  = 0x82;
 sfr DPH  = 0x83;
 sfr PCON = 0x87;
 sfr TCON = 0x88;
 sfr TMOD = 0x89;
 sfr TL0  = 0x8A;
 sfr TL1  = 0x8B;
 sfr TH0  = 0x8C;
 sfr TH1  = 0x8D;
 sfr IE   = 0xA8;
 sfr IP   = 0xB8;
 sfr SCON = 0x98;
 sfr SBUF = 0x99;
 
 
 
 
 sbit CY   = 0xD7;
 sbit AC   = 0xD6;
 sbit F0   = 0xD5;
 sbit RS1  = 0xD4;
 sbit RS0  = 0xD3;
 sbit OV   = 0xD2;
 sbit P    = 0xD0;
 
 
 sbit TF1  = 0x8F;
 sbit TR1  = 0x8E;
 sbit TF0  = 0x8D;
 sbit TR0  = 0x8C;
 sbit IE1  = 0x8B;
 sbit IT1  = 0x8A;
 sbit IE0  = 0x89;
 sbit IT0  = 0x88;
 
 
 sbit EA   = 0xAF;
 sbit ES   = 0xAC;
 sbit ET1  = 0xAB;
 sbit EX1  = 0xAA;
 sbit ET0  = 0xA9;
 sbit EX0  = 0xA8;
 
 
 sbit PS   = 0xBC;
 sbit PT1  = 0xBB;
 sbit PX1  = 0xBA;
 sbit PT0  = 0xB9;
 sbit PX0  = 0xB8;
 
 
 sbit RD   = 0xB7;
 sbit WR   = 0xB6;
 sbit T1   = 0xB5;
 sbit T0   = 0xB4;
 sbit INT1 = 0xB3;
 sbit INT0 = 0xB2;
 sbit TXD  = 0xB1;
 sbit RXD  = 0xB0;
 
 
 sbit SM0  = 0x9F;
 sbit SM1  = 0x9E;
 sbit SM2  = 0x9D;
 sbit REN  = 0x9C;
 sbit TB8  = 0x9B;
 sbit RB8  = 0x9A;
 sbit TI   = 0x99;
 sbit RI   = 0x98;
 
 
#line 1 "tibbo.c" /0
 
  
#line 1 "tibbo.h" /0



 



 
 
 
 
 
 
 
 
 void time_bit();
 void time_mbit(void);
 void tx_aux(unsigned char caracter);
 void Debug_txt_Tibbo(unsigned char * str);
 void Debug_chr_Tibbo(unsigned char Dat);
 void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
 unsigned char rx_Data(void);
 void cond_ini_tibbo(void);
 void Debug_HexDec(unsigned char xfc);
 void Debug_Fecha_actual(unsigned char *buffer);
 void Debug_Dividir_texto();
 
 
 
#line 2 "tibbo.c" /0
 
 
 
 extern void Delay_20us(unsigned int cnt);
 extern void          _nop_     (void);
 extern unsigned char hex_bcd (unsigned char byte);
 
 
 extern unsigned char Debug_Tibbo;
 
 sbit rx_ip = P0^0;					 
 sbit txd2 = P1^0;					 
 
 

 
 void time_bit()
 {
 unsigned char j;
 
 for (j=0; j<=7; j++) 				 
 {
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 
 
 
 }
 }

 
 void time_mbit(void)
 {
 unsigned char j;
 for (j=0; j<=4; j++)
 {
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 _nop_();
 }
 
 }
 


 
 void tx_aux(unsigned char caracter)
 {
 unsigned char j, temporal, bitTX;
 
 EA=0;
 txd2=0;
 time_bit();
 _nop_();
 _nop_();
 temporal=caracter;
 bitTX=caracter&0x01;
 if (bitTX==0x00)
 {
 txd2=0;
 }
 else
 {
 txd2=1;
 }
 time_bit();
 for (j=1; j<=7; j++)
 {
 temporal>>=1;
 bitTX=temporal&(0x01);
 if (bitTX==0x00)
 {
 txd2=0;
 }
 else
 {
 txd2=1;
 }
 time_bit();
 }
 txd2=1;
 time_bit();
 time_bit(); 
 
 EA=1;
 }
 
 
 void Debug_HexDec(unsigned char xfc)
 {
 unsigned int valor;
 unsigned char centena, decena, unidad;
 valor=0;
 
 centena=0;
 decena=0;
 unidad=0;
 if (Debug_Tibbo==1)
 {	 
 while (xfc>=0x064) 				 
 {
 xfc=xfc-0x64;
 centena=centena+1;
 }
 while (xfc>=0x0a)				 
 {
 xfc=xfc-0x0a;
 decena=decena+1;
 }
 unidad=xfc;
 tx_aux(centena|0x30);
 tx_aux(decena|0x30);
 tx_aux(unidad|0x30);
 
 }
 }
 


 
 void Debug_chr_Tibbo(unsigned char Dat)
 {
 unsigned char temp;
 if (Debug_Tibbo==1)
 {
 temp=(Dat&0xf0)>>4;
 (temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
 
 tx_aux(temp);
 
 temp=(Dat&0x0f);
 (temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
 tx_aux(temp);
 tx_aux(' ');
 
 }
 }
 




 
 void DebugBufferMF(unsigned char *str,unsigned char num_char,char io)
 {
 unsigned char j;
 
 
 if (Debug_Tibbo==1)
 {
 if(io!=0)
 {
 Debug_txt_Tibbo((unsigned char *) "Datos MF respuesta: ");
 }else Debug_txt_Tibbo((unsigned char *) "Datos MF Enviados: ");
 for (j=0; j<num_char; j++)
 {
 Debug_chr_Tibbo(*str);
 str++;
 }
 tx_aux('\r');
 tx_aux('\n');
 }
 
 }
 


 
 void Debug_txt_Tibbo(unsigned char * str)
 {
 unsigned char i;
 i=0;
 
 if (Debug_Tibbo==1)
 {
 for (i=0; str[i] != '\0'; i++)
 {
 tx_aux(str[i]);
 }
 
 }
 }
 
 void Debug_Dividir_texto()
 {
 Debug_txt_Tibbo((unsigned char *) "/*---------------------------------------*/\r\n");
 }
 


 
 unsigned char rx_Data(void)
 {
 unsigned char temporal;
 
 temporal=0xff;
 time_mbit();
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 temporal>>=1;
 
 time_bit();
 temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
 
 time_bit();
 while (rx_ip==0)
 {
 }
 
 return temporal; 
 
 }	
 void Debug_Fecha_actual(unsigned char *buffer)
 {
 Debug_txt_Tibbo((unsigned char *) "Fecha Actual en Board: ");
 Debug_chr_Tibbo(hex_bcd(*buffer));														 
 tx_aux('/');
 Debug_chr_Tibbo(hex_bcd(*(buffer+1)));												 
 tx_aux('/');
 Debug_chr_Tibbo(hex_bcd(*(buffer+2)));												 
 tx_aux(' ');
 Debug_chr_Tibbo(hex_bcd(*(buffer+3)));												 
 tx_aux(':');
 Debug_chr_Tibbo(hex_bcd(*(buffer+4)));												 
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 }


 
 void cond_ini_tibbo(void)
 {
 
 txd2=1;
 rx_ip=1;
 }

 










































































 
