
#line 1 "uart.c" /0




 
 
  
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
 
 
#line 7 "uart.c" /0
 
  
#line 1 "uart.h" /0



 



 
 
 
 
 
 void com_initialize (void);
 void com_baudrate ();
 char com_putchar (unsigned char c);
 int com_getchar (void);
 unsigned char com_rbuflen (void);
 unsigned char com_tbuflen (void);
 void clean_tx();
 
 
 
 
 
 
 
 
#line 8 "uart.c" /0
 
 
 
 
 extern idata unsigned char tbuf [];
 extern idata unsigned char rbuf [];
 extern unsigned char g_cEstadoComSoft;
 extern unsigned char xdata Buffer_Rta_Lintech[];
 extern	unsigned char g_cContByteRx;
 extern unsigned char ValTimeOutCom;
 extern unsigned char cont_trama;
 
 extern const unsigned  char ACK;
 extern const unsigned  char ETX;
 extern const unsigned  char STX_LINTECH;
 
 
 extern bit buffer_ready;










 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
















 

 
 
 
 
 static data unsigned char t_in = 0;
 static data unsigned char t_out = 0;
 
 static data unsigned char r_in = 0;
 static data unsigned char r_out = 0;
 
 static bit ti_restart = 0;   
 
 

 
 static void com_isr (void) interrupt 4
 {
 static xdata unsigned char cDatoRx;
 xdata unsigned char k,bcc;
 static xdata unsigned char num_datos;


 
 if (RI != 0)
 {
 
 RI = 0;
 cDatoRx=SBUF;
 if (((r_in - r_out) & ~(8-1)) == 0)
 {
 rbuf [r_in & (8-1)] = cDatoRx;		 
 r_in++;
 
 }
 if(g_cContByteRx>50)
 {
 g_cEstadoComSoft=0;
 }
 switch (g_cEstadoComSoft)
 {


 
 case 0:
 
 
 if(cDatoRx==ACK)  							 
 {
 g_cContByteRx=0;r_in=0;r_out=0;	
 g_cEstadoComSoft=1;
 
 }
 break;


 
 case 1:
 {	
 Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
 if(Buffer_Rta_Lintech[0]==STX_LINTECH)
 {						
 g_cEstadoComSoft=2;
 }
 else
 {
 g_cEstadoComSoft=0;
 }
 }
 break;


 
 case 2:
 
 if (g_cContByteRx==3)
 {
 
 num_datos=cDatoRx+2;
 Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
 g_cEstadoComSoft=3;			 
 }	
 else
 {			
 Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
 g_cEstadoComSoft=2;
 }
 
 break;


 
 case 3:
 
 Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;	
 num_datos--;
 if (num_datos==0)
 {
 if(Buffer_Rta_Lintech[g_cContByteRx-2]==ETX)
 {
 bcc=0;
 for (k=0; k<g_cContByteRx-1; k++)
 {
 bcc=Buffer_Rta_Lintech[k]^bcc;
 }
 if (bcc==Buffer_Rta_Lintech[g_cContByteRx-1])	
 {
 buffer_ready=1;
 g_cEstadoComSoft=0;											 
 
 }
 else
 {
 g_cEstadoComSoft=0;											 
 }
 }	
 else 
 {
 g_cEstadoComSoft=0;													 
 }	
 
 }
 else 
 {
 g_cEstadoComSoft=3;													 
 }
 
 break;		
 


 
 default:
 g_cEstadoComSoft=0;
 break;
 }
 }
 


 
 if (TI != 0)
 {
 TI = 0;
 
 if (t_in != t_out)
 {
 SBUF = tbuf [t_out & (2-1)];
 t_out++;
 ti_restart = 0;
 }
 else
 {
 ti_restart = 1;
 }
 }
 
 }
 

 
 #pragma disable
 
 void com_initialize (void)
 {


 
 com_baudrate ();
 


 
 t_in = 0;
 t_out = 0;
 
 r_in = 0;
 r_out = 0;
 


 
 SM0 = 0; SM1 = 1;    
 SM2 = 0;
 REN = 1;             
 
 RI = 0;              
 TI = 0;              
 ti_restart = 1;
 
 ES = 1;              
 PS = 1;              
 }
 

 
 #pragma disable
 
 void com_baudrate ()
 
 {


 
 TI = 0;              
 t_in = 0;            
 t_out = 0;
 


 
 TR1 = 0;             
 ET1 = 0;             
 
 PCON |= 0x80;        
 
 TMOD &= ~0xF0;       
 TMOD |= 0x20;        
 
 TH1 =0xf4; 
 TL1=0xf4;
 TR1 = 1;             
 }
 

 
 #pragma disable
 
 char com_putchar (
 unsigned char c)
 {


 
 if (com_tbuflen () >= 2)
 return (-1);
 



 
 tbuf [t_in & (2 - 1)] = c;
 t_in++;
 
 if (ti_restart)
 {
 ti_restart = 0;
 TI = 1;                
 }
 
 return (0);
 }
 

 
 #pragma disable
 
 int com_getchar (void)
 {
 if (com_rbuflen () == 0)
 return (-1);
 
 return (rbuf [(r_out++) & (8 - 1)]);
 }
 

 
 #pragma disable
 
 unsigned char com_rbuflen (void)
 {
 return (r_in - r_out);
 }
 

 
 #pragma disable
 
 unsigned char com_tbuflen (void)
 {
 return (t_in - t_out);
 }
 

 



























 
