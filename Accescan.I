
#line 1 "Accescan.c" /0
  
#line 1 "Accescan.h" /0


 



 
 
 
 
 
 
 void Valida_Trama_Pto(unsigned char *buffer, unsigned char length_trama);
 unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd);
 void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama);
 void Cmd_Monitor();
 void Monitor_chr (unsigned char *str,unsigned char num_char);
 void Cmd_LPR_Salida_wiegand(unsigned char *buffer);
 void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2);
 
 
 
 
 
#line 1 "Accescan.c" /0
 
  
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
 
 
#line 2 "Accescan.c" /0
 
 
 extern void Block_write_clock_ascii(unsigned char *datos_clock);
 extern void PantallaLCD(unsigned char cod_msg);
 extern void Reloj_Pantalla_Lcd();
 extern unsigned char rx_Data(void);
 extern void Debug_txt_Tibbo(unsigned char * str);
 extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
 extern void Debug_Dividir_texto();
 extern void tx_aux(unsigned char caracter);
 extern unsigned char Dir_board();
 extern void Block_read_clock_ascii(unsigned char *datos_clock);
 extern void Two_ByteHex_Decimal(unsigned char *buffer,unsigned char id_h,unsigned char id_l);
 extern void ByteHex_Decimal(unsigned char *buffer,unsigned char valorhex);
 extern void PantallaLCD(unsigned char cod_msg);
 extern void LCD_txt (unsigned char * msg,char enable_char_add );
 extern unsigned char  ValidaSensoresPaso(void);
 extern void tx_aux(unsigned char caracter);
 extern void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char *buffer);
 extern unsigned char hex_bcd (unsigned char byte);
 extern void Trama_pto_Paralelo_P(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 extern void  send_port(unsigned char *buffer_port, unsigned char length_char);
 
 
 
 extern unsigned char Timer_wait;
 extern unsigned char Tipo_Vehiculo;
 extern unsigned char USE_LPR;
 extern unsigned char  Debug_Tibbo;
 sbit rx_ip = P0^0;				
 sbit lock = P1^7;						 
 sbit Atascado = P0^3;				 
 sbit led_err_imp = P0^2;			 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 void Valida_Trama_Pto(unsigned char *buffer, unsigned char length_trama)
 {
 unsigned char buff[11];
 
 if((length_trama==25)&&(*buffer==02)&&(*(buffer+2)=='H')&&*(buffer+(length_trama-1))==03)													 
 { 
 
 Block_write_clock_ascii(buffer+3);																																								 
 
 Reloj_Pantalla_Lcd();																																															 
 
 }
 
 else if(*buffer==0xb6)																																										 
 {
 if (Timer_wait>=20)																																																 
 {																																																									 
 PantallaLCD(0xb6);
 led_err_imp=0;																																																	 
 Timer_wait=0;
 lock=0;																																																					 
 Atascado=0;	
 }
 }
 
 else if (*buffer==0xAA)																																													 
 {
 
 if (Timer_wait>=20)																																																 
 {	
 Timer_wait=0;																																																		 
 lock=0;
 led_err_imp=1;																																																	 
 Atascado=0;	
 if ((Debug_Tibbo==0)&&(USE_LPR==1))
 {
 Debug_Tibbo=1;
 Debug_txt_Tibbo((unsigned char *) "LIVE");
 Debug_Tibbo=0;
 }
 }
 
 }	
 else if ((length_trama==19)&&(*buffer==02)&&(*(buffer+1)=='O')&&*(buffer+(length_trama-1))==03)										 
 {
 PantallaLCD(0XFE);
 }
 else if ((length_trama==1)&&(*buffer==0xA1))																																				 
 {
 PantallaLCD(0XE7);
 }
 
 else if ((length_trama==6)&&(*buffer==02)&&(*(buffer+1)=='W')&&*(buffer+(length_trama-1))==03)										 
 {
 if (USE_LPR==1)
 {
 
 ByteHex_Decimal(buff,*(buffer+2));																																				 
 buff[3]=' ';
 Two_ByteHex_Decimal(buff+4,*(buffer+3),*(buffer+4))	;																											 
 
 PantallaLCD_LINEA_2(0xB0,buff);
 
 
 
 while(!ValidaSensoresPaso());
 
 Cmd_LPR_Salida_wiegand(buff);
 }																																							
 
 else
 {
 
 ByteHex_Decimal(buff,*(buffer+2));																																				 
 buff[3]=' ';
 Two_ByteHex_Decimal(buff+4,*(buffer+3),*(buffer+4))	;																											 
 PantallaLCD_LINEA_2(0xB0,buff);																														 
 
 
 
 }
 }
 
 }




 
 unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd)
 {
 unsigned char j, NumDatos,time_out,MaxChrRx;
 unsigned int contador;
 
 NumDatos=0;
 MaxChrRx=11;
 
 if (USE_LPR==1)
 {
 for (j=0; j<MaxChrRx; j++)
 {
 contador=0;
 time_out=0;
 while ((rx_ip==1)&&(time_out==0))
 {
 contador++;
 if (contador>20000)
 {
 time_out=1;
 j=MaxChrRx;
 }				
 }
 if(time_out==1)break;
 NumDatos++;
 *buffer_cmd=rx_Data();
 buffer_cmd++;
 }
 
 *buffer_cmd=0;
 
 
 }
 return	NumDatos;
 }	


 




 

 
 
 
 
 
 
 


 






 


 
 void Monitor_chr (unsigned char *str,unsigned char num_char)
 {
 unsigned char j;
 for (j=0; j<num_char; j++)
 {
 tx_aux(*str);
 str++;
 }
 }
 


 
 void Cmd_LPR_Salida_wiegand(unsigned char *buffer)
 {
 unsigned char Buffer_Lpr[30];
 unsigned char j=3;
 Buffer_Lpr[0]=02;																			 
 Buffer_Lpr[1]=Dir_board();															 
 Buffer_Lpr[2]='S';																			 
 if(Tipo_Vehiculo!=0)																		 
 {
 Buffer_Lpr[j++]='M';																 
 }
 else
 {
 Buffer_Lpr[j++]='C';																 
 }
 
 
 
 for (j=4; *buffer != '\0'; j++)												 
 {
 Buffer_Lpr[j]=*buffer;
 buffer++;
 
 }
 Buffer_Lpr[j++]=':';																 
 
 Block_read_clock_ascii(Buffer_Lpr+j);								 
 Buffer_Lpr[j+10]=':';																 
 Buffer_Lpr[j+11]=03;																 
 
 
 
 Monitor_chr(Buffer_Lpr,j+12);												 
 }


 
 void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2)
 {
 
 
 unsigned char Buffer_Lpr[30];
 unsigned temp;
 unsigned char j=3;
 Buffer_Lpr[0]=02;																 
 Buffer_Lpr[1]=Dir_board();												 
 Buffer_Lpr[2]='S';																 
 
 if(*(buffer_S1_B2+8)!=0)												 
 {
 Buffer_Lpr[j++]='M';													 
 }
 else
 {
 Buffer_Lpr[j++]='C';													 
 }
 
 
 do
 {
 Buffer_Lpr[j++]=*buffer_S1_B0;									 
 buffer_S1_B0++;
 }while (*buffer_S1_B0!=0);
 
 
 
 
 
 Buffer_Lpr[j++]=':';														 
 
 temp=hex_bcd(*(buffer_S1_B2+0));								 
 Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
 Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
 
 temp=hex_bcd(*(buffer_S1_B2+1));								 
 Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
 Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
 
 temp=hex_bcd(*(buffer_S1_B2+2));								 
 Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
 Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
 
 temp=hex_bcd(*(buffer_S1_B2+3));								 
 Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
 Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
 
 temp=hex_bcd(*(buffer_S1_B2+4));								 
 Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
 Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
 
 
 
 Buffer_Lpr[j++]=':';														 
 
 
 Buffer_Lpr[j++]=03;	
 
 Monitor_chr(Buffer_Lpr,j);												 
 }
 
 
 
