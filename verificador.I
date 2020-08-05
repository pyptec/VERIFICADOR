
#line 1 "verificador.c" /0


 
  
#line 1 "verificador.h" /0
 



 



 
 
 
 
 
 
 
 void Inicializa(unsigned char TipoMovimiento);
 void Check_Status(unsigned char Detalle);
 void Aut_Card_check_Status(void);
 void Mov_Card(unsigned char Posicion);
 void SecuenciaExpedidor(void);
 void Card_Insercion(char Tipo);
 void Dwload_EEprom (void);
 unsigned char  Secuencia_inicio_expedidor(void);
 char Trama_Validacion_P_N();
 void LoadVerify_EEprom(void);
 void RD_MF(unsigned char Sector, unsigned char Bloque);
 void WR_MF(unsigned char Sector, unsigned char Bloque,unsigned char *buffer);
 
 
 
 
 
#line 4 "verificador.c" /0
 
  
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
 
 
#line 5 "verificador.c" /0
 
 
 
 
 extern void EscribirCadenaSoft_buffer(unsigned char *buffer,unsigned char tamano_cadena);
 extern void EscribirCadenaSoft(unsigned char tamano_cadena,unsigned char tipo);
 extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
 extern void Debug_txt_Tibbo(unsigned char * str);
 extern unsigned char  ValidaSensoresPaso(void);
 extern void send_portERR(unsigned char cod_err);
 extern void Debug_chr_Tibbo(unsigned char Dat);
 extern void Debug_HexDec(unsigned char xfc);
 extern char check_fechaOut(char *buffer);
 extern unsigned char Dir_board();
 extern void PantallaLCD(unsigned char cod_msg);
 extern void Trama_pto_Paralelo_C_s(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2);
 extern void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2);
 extern void Trama_pto_Paralelo(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 extern void Trama_pto_Paralelo_P(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 extern void analiza_tiempo(char *buffer,unsigned int Val_DctoMinutos);
 extern void Trama_pto_Paralelo_new(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 extern void Block_read_Clock_Hex(unsigned char *datos_clock);
 
 sbit lock = P1^7;						 
 sbit Atascado = P0^3;				 
 sbit led_err_imp = P0^2;			 
 
 
 extern unsigned char g_cEstadoComSoft;
 extern unsigned char ValTimeOutCom;
 extern unsigned char g_cEstadoComSeqMF;
 extern unsigned char g_cContByteRx;
 extern unsigned char xdata Buffer_Rta_Lintech[];
 extern int ID_CLIENTE;
 extern int COD_PARK;
 extern unsigned int T_GRACIA;																				 
 extern unsigned char Timer_wait;
 extern unsigned int  SIN_COBRO;
 
 
 extern bit buffer_ready;
 extern unsigned char USE_LPR;
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 


 
 
 
 
 


 
 
 
 
 


 
 
 
 
 
 


 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 


 
 
 
 


 
 
 
 
 


 
 
 
 
 


 
 
 
 
 
 
 
 
 
 


 
 
 
 
 





 
 
 
 
 





 
 
 
 
 




 
 
 
 
 
 

 
 
 
 
 
 
 
 
 


 
 
 static unsigned char Estado=0x30;
 
 



 
 
 void Inicializa(unsigned char TipoMovimiento)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[10];
 bcc=0;
 if ((TipoMovimiento=='3')||(TipoMovimiento=='0')||(TipoMovimiento=='1'))
 {
 
 Debug_txt_Tibbo((unsigned char *) "Incializa Dispensador\r\n\r\n");
 
 g_scArrTxComSoft[0]=0xf2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X03;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]='0';
 g_scArrTxComSoft[6]=TipoMovimiento;
 g_scArrTxComSoft[7]=03;
 for (j=0; j<8; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[8]=bcc;
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,9,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,9);		 
 ValTimeOutCom=100;
 }
 }
 





 
 
 void Check_Status(unsigned char Detalle)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[10];
 Debug_txt_Tibbo((unsigned char *) "Check_Status\r\n\r\n");
 
 bcc=0;
 
 g_scArrTxComSoft[0]=0xf2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X03;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]='1';
 g_scArrTxComSoft[6]=Detalle;
 g_scArrTxComSoft[7]=03;
 for (j=0; j<8; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[8]=bcc;
 buffer_ready=0;
 g_cEstadoComSoft=0;
 DebugBufferMF(g_scArrTxComSoft,9,0);
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,9);
 ValTimeOutCom=100;
 }
 




 
 
 void Card_Insercion(char Tipo)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[10];
 if (Tipo==0x30)
 {
 Debug_txt_Tibbo((unsigned char *) "Habilita Insersion\r\n\r\n");
 g_scArrTxComSoft[6]=0x30;
 }
 else
 {
 
 Debug_txt_Tibbo((unsigned char *) "Inhabilita Insersion\r\n\r\n");
 g_scArrTxComSoft[6]=0x31;
 
 }
 
 bcc=0;
 
 g_scArrTxComSoft[0]=0xF2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X03;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]='3';
 
 g_scArrTxComSoft[7]=03;
 for (j=0; j<8; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 
 g_scArrTxComSoft[8]=bcc;
 
 buffer_ready=0;
 g_cEstadoComSoft=0;
 DebugBufferMF(g_scArrTxComSoft,9,0);
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,9);
 ValTimeOutCom=100;
 }
 


 
 
 void Mov_Card(unsigned char Posicion)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[10];
 bcc=0;
 
 if ((Posicion=='2')||(Posicion=='1')||(Posicion=='0')||(Posicion=='9')||(Posicion=='3'))
 {
 if (Posicion=='2')
 {
 Debug_txt_Tibbo((unsigned char *) "Moviendo Tarjeta a RF\r\n\r\n");
 }
 else if (Posicion=='1')
 {
 Debug_txt_Tibbo((unsigned char *) "Moviendo Tarjeta a IC\r\n\r\n");
 }
 else if (Posicion=='0')
 {
 Debug_txt_Tibbo((unsigned char *) "Moviendo Tarjeta a Bezel\r\n\r\n");
 }
 else if (Posicion=='9')
 {
 Debug_txt_Tibbo((unsigned char *) "Expulsando Tarjeta\r\n\r\n");
 }
 else if (Posicion=='3')
 {
 Debug_txt_Tibbo((unsigned char *) "Capturando Tarjeta\r\n\r\n");
 }
 
 g_scArrTxComSoft[0]=0xf2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X03;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]='2';
 g_scArrTxComSoft[6]=Posicion;
 g_scArrTxComSoft[7]=03;
 for (j=0; j<8; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[8]=bcc;
 buffer_ready=0;
 g_cEstadoComSoft=0;
 DebugBufferMF(g_scArrTxComSoft,9,0);
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,9);
 ValTimeOutCom=100;
 }
 
 }
 
 
 
 
 
 void Aut_Card_check_Status(void)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[10];
 bcc=0;
 
 Debug_txt_Tibbo((unsigned char *) "Aut_Card_check_Status\r\n\r\n");
 
 g_scArrTxComSoft[0]=0xf2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X03;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]=0x50;
 g_scArrTxComSoft[6]=0x31;
 g_scArrTxComSoft[7]=03;
 for (j=0; j<8; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[8]=bcc;
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,9,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,9);		 
 ValTimeOutCom=100;
 }	
 


 
 
 void Dwload_EEprom (void)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[21];
 bcc=0;
 Debug_txt_Tibbo((unsigned char *) "Download MF EEprom\r\n\r\n");
 
 g_scArrTxComSoft[0]=0xF2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X0E;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]=0x60;
 g_scArrTxComSoft[6]='3';
 g_scArrTxComSoft[7]=0x00;
 g_scArrTxComSoft[8]=0Xd0;
 g_scArrTxComSoft[9]=0X00;
 g_scArrTxComSoft[10]=0X01;
 g_scArrTxComSoft[11]=0x06;
 g_scArrTxComSoft[12]='3';
 g_scArrTxComSoft[13]='V';
 g_scArrTxComSoft[14]='0';
 g_scArrTxComSoft[15]='p';
 g_scArrTxComSoft[16]='4';
 g_scArrTxComSoft[17]='r';
 g_scArrTxComSoft[18]=03;
 
 for (j=0; j<19; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[19]=bcc;
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,20,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,20);		 
 ValTimeOutCom=100;
 }
 


 
 
 void LoadVerify_EEprom(void)
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[15];
 bcc=0;
 Debug_txt_Tibbo((unsigned char *) "Carga y Verifica de EEprom\r\n\r\n");
 
 
 g_scArrTxComSoft[0]=0xF2;
 g_scArrTxComSoft[1]=0X00;
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X07;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]=0x60;
 g_scArrTxComSoft[6]='3';
 g_scArrTxComSoft[7]=0x00;
 g_scArrTxComSoft[8]=0x21;
 g_scArrTxComSoft[9]=0x00;
 g_scArrTxComSoft[10]=0x01;
 g_scArrTxComSoft[11]=03;
 g_scArrTxComSoft[12]=0xc6;
 
 
 
 for (j=0; j<13; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[13]=bcc;
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,14,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,14);		 
 ValTimeOutCom=100;
 
 }
 


 
 
 void RD_MF(unsigned char Sector, unsigned char Bloque)
 {
 
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[15];
 bcc=0;
 
 Debug_txt_Tibbo((unsigned char *) "Leyendo MF > Sector: ");
 Debug_chr_Tibbo(Sector);
 
 Debug_txt_Tibbo((unsigned char *) " Bloque: ");
 Debug_chr_Tibbo(Bloque);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 
 g_scArrTxComSoft[0]=0xF2;
 g_scArrTxComSoft[1]=0X00;									  
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=0X08;
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]=0X60;
 g_scArrTxComSoft[6]='3';
 g_scArrTxComSoft[7]=0x00;
 g_scArrTxComSoft[8]=0xb0;
 g_scArrTxComSoft[9]=Sector;
 g_scArrTxComSoft[10]=Bloque;
 g_scArrTxComSoft[11]=0x01;
 g_scArrTxComSoft[12]=03;
 
 for (j=0; j<13; j++)
 {
 bcc=g_scArrTxComSoft[j]^bcc;
 }
 g_scArrTxComSoft[13]=bcc;
 
 
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,14,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,14);		 
 ValTimeOutCom=100;
 
 }
 


 
 
 
 void WR_MF(unsigned char Sector, unsigned char Bloque,unsigned char *buffer)	   
 
 {
 unsigned char j, bcc;
 unsigned char	g_scArrTxComSoft[31];
 bcc=0;
 
 Debug_txt_Tibbo((unsigned char *) "Escribe MF > Sector: ");
 Debug_chr_Tibbo(Sector);
 
 Debug_txt_Tibbo((unsigned char *) " Bloque: ");
 Debug_chr_Tibbo(Bloque);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 g_scArrTxComSoft[0]=0xF2;
 g_scArrTxComSoft[1]=0X00;									  
 g_scArrTxComSoft[2]=0X00;
 g_scArrTxComSoft[3]=24;																	 
 
 g_scArrTxComSoft[4]='C';
 g_scArrTxComSoft[5]=0X60;
 g_scArrTxComSoft[6]='3';
 g_scArrTxComSoft[7]=0x00;
 g_scArrTxComSoft[8]=0xd1;
 g_scArrTxComSoft[9]=Sector;															 
 g_scArrTxComSoft[10]=Bloque;														 
 g_scArrTxComSoft[11]=0x01;
 
 if(Sector!=2)
 {	


 
 
 g_scArrTxComSoft[12]=0x00;															 
 g_scArrTxComSoft[13]=0x00;
 g_scArrTxComSoft[14]=0x00;
 g_scArrTxComSoft[15]=0x00;
 g_scArrTxComSoft[16]=0x00;
 


 
 g_scArrTxComSoft[17]=0x00;
 g_scArrTxComSoft[18]=0x00;
 g_scArrTxComSoft[19]=0x00;
 


 
 
 g_scArrTxComSoft[20]=*buffer;
 


 
 g_scArrTxComSoft[21]=0x0f&Dir_board();
 


 
 g_scArrTxComSoft[22]=02;
 


 
 
 g_scArrTxComSoft[23]=0x00;															 
 g_scArrTxComSoft[24]=0x00;
 g_scArrTxComSoft[25]=0x00;
 g_scArrTxComSoft[26]=0x00;
 g_scArrTxComSoft[27]=0x00;
 
 }
 else 
 {
 for (j=12; j<=28; j++)
 {
 g_scArrTxComSoft[j]=0x00;	
 }
 }
 
 
 
 
 g_scArrTxComSoft[28]=03;
 
 for (j=0; j<=28; j++)
 {
 bcc=bcc^g_scArrTxComSoft[j];
 }
 g_scArrTxComSoft[29]=bcc;
 
 
 buffer_ready=0;																		 
 g_cEstadoComSoft=0;												 
 DebugBufferMF(g_scArrTxComSoft,30,0);								 
 EscribirCadenaSoft_buffer(g_scArrTxComSoft,30);		 
 ValTimeOutCom=100;
 
 }
 






 
 
 char Trama_Validacion_P_N()
 {
 char Trama_Validacion_P_N=2;
 
 if ((ValTimeOutCom==1)||(buffer_ready==1))
 {
 if (buffer_ready==1)
 {
 buffer_ready=0;
 
 
 if (Buffer_Rta_Lintech[4]=='P')
 {
 
 Trama_Validacion_P_N=0;
 
 }
 else if (Buffer_Rta_Lintech[4]=='N')
 {
 
 Trama_Validacion_P_N=3;
 
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Respuesta  DESCONOCIDA \r\n");				 
 Trama_Validacion_P_N=3;		
 
 }
 }
 else
 {
 
 Trama_Validacion_P_N=1;																									 
 
 }
 
 }
 
 return Trama_Validacion_P_N;
 }
 




 
 
 unsigned char  Secuencia_inicio_expedidor(void)
 {
 char temp;
 switch (Estado)
 {
 case 0x30:
 
 
 Inicializa('3');	   																		 
 Estado=0x31;																			 
 
 
 return (Estado);
 break;
 
 






 
 
 case 0x31:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 Estado=0x31;																													 
 }	
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "INICIALIZA SIN MOVIMIENTO ERROR\r\n\r\n");					 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 Estado=0x30;																																 
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
 Estado=0x30;																															 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "INICIALIZA SIN MOVIMIENTO OK\r\n\r\n");						 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 Estado=0x32;																																	 
 }			
 return (Estado);
 break;
 


 
 
 case 0x32:
 
 Dwload_EEprom();	   																																 
 Estado=0X33;																												 
 
 
 
 return (Estado);
 break;
 






 
 
 case 0X33:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 Estado=0X33;																													 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "DWLOAP EEPROM ERROR\r\n\r\n");											 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 Estado=0x30;																																 
 }			
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
 Estado=0x30;																															 
 }				
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "DWLOAP EEPROM OK\r\n\r\n");												 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 Estado=0x00;																																				 
 }			
 
 return (Estado);
 break;
 


 
 
 case 0x00:
 
 return (Estado);
 break;
 
 
 default:
 return Estado=0x30;	
 break;
 }
 }
 


 
 
 void SecuenciaExpedidor(void)
 {
 unsigned char temp;
 static unsigned char buffer_S1_B0[17];
 static unsigned char buffer_S1_B1[17];
 static unsigned char buffer_S1_B2[17];
 static unsigned char error_rx=0; 
 unsigned char clock_temp[6];
 switch (g_cEstadoComSeqMF)
 {
 
 
 case 0X00:
 
 if (ValTimeOutCom==1)
 {
 
 Card_Insercion(0x30);																													 
 g_cEstadoComSeqMF=0X01;																							 
 
 }
 
 break;
 






 
 
 case 0X01:	
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0X01;																									 
 }	
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "CARD_INSERCION ERROR\r\n\r\n");										 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 g_cEstadoComSeqMF=0X00;																													 
 }			
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
 if (error_rx>= 3)
 {
 error_rx=0;
 Atascado=1;																																						 
 led_err_imp=0;																																				 
 Timer_wait=0;
 ValTimeOutCom=3;				
 g_cEstadoComSeqMF=0X00;			 
 break;																																							 
 }			
 
 error_rx++;
 g_cEstadoComSeqMF=0X00;																												 
 }				
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "CARD_INSERCION OK\r\n\r\n");												 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 g_cEstadoComSeqMF=0X02;																										 
 }			
 
 break;
 


 
 
 case 0X02:	
 
 Check_Status(0x30);																																 
 g_cEstadoComSeqMF=0x03;																										 
 
 break;			
 






 
 
 case 0x03:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x03;																												 
 }	
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_CARD_INSERCION  FALLO\r\n\r\n");											 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																					 
 g_cEstadoComSeqMF=0X02;																													 
 }			
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
 if (error_rx>= 3)
 {
 error_rx=0;
 Atascado=1;																																						 
 led_err_imp=0;																																				 
 Timer_wait=0;
 ValTimeOutCom=3;				
 g_cEstadoComSeqMF=0X00;			 
 break;																																							 
 }	
 error_rx++;
 g_cEstadoComSeqMF=0X02;
 }			
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "CHECK SATUS OK\r\n\r\n");																	 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																		 
 if (Buffer_Rta_Lintech[7]=='0')																												 
 {
 
 Debug_txt_Tibbo((unsigned char *) "No tiene tarjetas en el mecanismo\r\n\r\n");				 
 g_cEstadoComSeqMF=0X02;																										 
 
 
 }
 else if	(Buffer_Rta_Lintech[7]=='1')	 																						 
 {
 
 Debug_txt_Tibbo((unsigned char *) "Tarjeta en la boca\r\n\r\n");											 
 g_cEstadoComSeqMF=0X02;																										 
 
 }
 else if (Buffer_Rta_Lintech[7]=='2') 																						 
 {
 Debug_txt_Tibbo((unsigned char *) "Tarjeta lista para leer en RF\r\n\r\n");							 
 
 error_rx=0;
 if (ValidaSensoresPaso()!=0)																												 
 {
 Debug_txt_Tibbo((unsigned char *) "Vehiculo en el loop\r\n\r\n");										 
 Aut_Card_check_Status();																														 
 g_cEstadoComSeqMF=0x05;																											 
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Vehiculo no en el loop\r\n\r\n");							 
 send_portERR(0XE0);																													 
 PantallaLCD(0XE0);																													 
 g_cEstadoComSeqMF=0x20;																								 
 
 }
 
 }
 
 }			
 
 break;
 






 
 
 case 0X04:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 
 g_cEstadoComSeqMF=0X04;																										 
 
 }	
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "NO EJECT TARJET ERROR\r\n\r\n");										 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 g_cEstadoComSeqMF=0X00;																													 
 }	
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");	 
 g_cEstadoComSeqMF=0X00;	
 
 }				
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "EJECT TARJET OK\r\n\r\n");													 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);														 
 g_cEstadoComSeqMF=0X02;																										 
 }			
 
 
 break;	








 
 
 case 0x05:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x05;																														 
 }	
 else if (temp==3)
 {
 
 Debug_txt_Tibbo((unsigned char *) "SEQ_REQUEST FALLO\r\n\r\n");												 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																		 
 g_cEstadoComSeqMF=0X02;																										 
 
 }	
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde SEQ REQUEST...\r\n\r\n");			 
 send_portERR(0XE1);	   																												 
 PantallaLCD(0XE1);																														 
 g_cEstadoComSeqMF=0x20;																											 
 
 }				
 }
 else
 {
 if (Buffer_Rta_Lintech[0x0a]=='1')																									 
 {
 if (Buffer_Rta_Lintech[0x0b]=='0')	
 {
 Debug_txt_Tibbo((unsigned char *) "Tarjeta valida MF50\r\n\r\n");							 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);														 
 LoadVerify_EEprom();  																												 
 g_cEstadoComSeqMF=0x06;																							 
 
 }else
 {
 Debug_txt_Tibbo((unsigned char *) "Tarjeta invalida no es MF50\r\n\r\n");					 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);		
 send_portERR(0XE1);
 PantallaLCD(0XE1);																									 
 g_cEstadoComSeqMF=0x20;																						 
 
 }
 }
 else 
 {
 Debug_txt_Tibbo((unsigned char *) "Tarjeta invalida no es MF50\r\n\r\n");						 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);			
 send_portERR(0XE1);	   																									 
 PantallaLCD(0XE1);																											 
 g_cEstadoComSeqMF=0x20;																								 
 
 }
 
 }			
 
 break;






 
 case 0x06:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x06;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "LOAD VERIFY EEPROM ERROR\r\n\r\n");										 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																													 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "LOAD VERIFY EEPROM OK\r\n\r\n");												 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 RD_MF(1,1);																																								 
 g_cEstadoComSeqMF=0x07;																														 
 }			
 
 
 break;



 
 
 case 0x07:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x07;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B1 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B1 OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 
 if (Buffer_Rta_Lintech[3] >=0x18)
 {
 
 for (temp=0; temp<16; temp++)
 {
 buffer_S1_B1[temp]=Buffer_Rta_Lintech[0x0a+temp];														 
 }
 if (((buffer_S1_B1[0x01]==ID_CLIENTE)&&(buffer_S1_B1[0x03]==COD_PARK))||((ID_CLIENTE==0)&&(COD_PARK==0)))		
 {
 
 Debug_txt_Tibbo((unsigned char *) "ID_CLIENTE: ");
 Debug_HexDec(buffer_S1_B1[0x01]);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 Debug_txt_Tibbo((unsigned char *) "COD_PARK:");
 Debug_HexDec(buffer_S1_B1[0x03]);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 
 
 if 	((buffer_S1_B1[0X00]==0x01))					
 {
 Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA 1 ROTACION: ");
 Debug_chr_Tibbo(buffer_S1_B1[0X00]);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 RD_MF(1, 2);																																				 
 g_cEstadoComSeqMF=0x08;
 
 }
 else 
 {
 send_portERR(0XE1);
 PantallaLCD(0XE1);																											 
 g_cEstadoComSeqMF=0x20;																								 
 }
 }
 else
 {
 send_portERR(0XE5);
 PantallaLCD(0XE5);																												 
 g_cEstadoComSeqMF=0x20;																								 
 }
 }
 else
 {
 send_portERR(0xDF);
 PantallaLCD(0xDF);																												 
 g_cEstadoComSeqMF=0x20;
 }
 
 }			
 break;



 
 case 0x08:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x08;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 for (temp=0; temp<16; temp++)
 {
 buffer_S1_B2[temp]=Buffer_Rta_Lintech[0x0a+temp];															 
 }
 
 if((buffer_S1_B2[0x0A]==02)||(buffer_S1_B2[0x0A]==0) )															 
 {																																																							 
 Debug_txt_Tibbo((unsigned char *) "ERROR: SIN INGRESO\r\n\r\n");
 send_portERR(0XE6);
 PantallaLCD(0XE6);
 g_cEstadoComSeqMF=0x20;
 break;
 }
 
 if(SIN_COBRO!=0)																																																 
 {
 if (SIN_COBRO==1)
 {
 Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
 RD_MF(1, 0);																																																 
 g_cEstadoComSeqMF=0x09;																																							 
 break;
 }
 else if( SIN_COBRO ==2)
 {
 Block_read_Clock_Hex(clock_temp);																																						 
 
 
 if ((buffer_S1_B2[0]==clock_temp[0])&&(buffer_S1_B2[1]==clock_temp[1])&&(buffer_S1_B2[2]==clock_temp[2]))		 
 {
 Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
 RD_MF(1, 0);																																															 
 g_cEstadoComSeqMF=0x09;																																						 
 break;
 }																							                                            
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n\r\n");																							 
 send_portERR(0XE8);
 PantallaLCD(0XE8);																																								 
 g_cEstadoComSeqMF=0x20;
 break;							
 }
 
 
 }
 }
 
 
 else if(T_GRACIA!=0)																																		 
 {
 Debug_txt_Tibbo((unsigned char *) "Tiempo Gracia: ");
 Debug_chr_Tibbo(T_GRACIA);																														 
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");		
 analiza_tiempo(buffer_S1_B2,T_GRACIA); 																								 
 
 }
 
 temp=check_fechaOut(buffer_S1_B2+0X0B);																					 
 if(temp==1)
 {
 Debug_txt_Tibbo((unsigned char *) "TARJETA SIN PAGO\r\n\r\n");
 send_portERR(0XE7);
 PantallaLCD(0XE7);																														 
 RD_MF(1, 0);																																			 
 g_cEstadoComSeqMF=0x0c;					
 
 }
 else if (temp==2)
 {
 Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n\r\n");
 send_portERR(0XE8);
 PantallaLCD(0XE8);																												 
 g_cEstadoComSeqMF=0x20;	
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
 RD_MF(1, 0);																																			 
 g_cEstadoComSeqMF=0x09;	
 }
 
 }
 
 
 
 
 break;
 



 
 
 case 0x09:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x09;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 
 for (temp=0; temp<16; temp++)
 {
 
 buffer_S1_B0[temp]=Buffer_Rta_Lintech[0x0a+temp];																	 
 if(Buffer_Rta_Lintech[0x0a+temp]==0)
 {
 break;
 }
 }
 Debug_txt_Tibbo((unsigned char *) "Numero Ticket: ");
 Debug_txt_Tibbo(buffer_S1_B0);
 Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
 
 lock=1;		
 Timer_wait=0;																																								 
 Trama_pto_Paralelo_new(buffer_S1_B0,buffer_S1_B2,'s');																			 
 if(USE_LPR)
 {
 Cmd_LPR_Salida(buffer_S1_B0,buffer_S1_B2);																								 
 }
 
 WR_MF(1, 2,buffer_S1_B2+0x08);
 g_cEstadoComSeqMF=0x0a;
 }
 break;
 
 case 0x0a:
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x0a;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_WR_S1B2 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_WR_S1B2 OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 WR_MF(2, 0,buffer_S1_B2+0x08);
 g_cEstadoComSeqMF=0x0b;	
 }
 break;
 
 case 0x0b:
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x0b;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 
 
 
 Debug_txt_Tibbo((unsigned char *) "ok tarjeta ok...\r\n\r\n");															 
 Mov_Card('3');
 g_cEstadoComSeqMF=0X04;
 }
 break;




 
 
 
 case 0x0c:	
 
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x0c;																														 
 }
 else if (temp==3)
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n\r\n");														 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 g_cEstadoComSeqMF=0X00;																															 
 }				
 else
 {	
 Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
 g_cEstadoComSeqMF=0X00;																														 
 }
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0_EJECT OK\r\n\r\n");															 
 DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,0X01);																 
 
 for (temp=0; temp<16; temp++)
 {
 
 buffer_S1_B0[temp]=Buffer_Rta_Lintech[0x0a+temp];																	 
 if(Buffer_Rta_Lintech[0x0a+temp]==0)
 {
 break;
 }
 }
 Trama_pto_Paralelo_P(buffer_S1_B0,buffer_S1_B2,'P');		
 g_cEstadoComSeqMF=0x20;		
 }	
 break;
 


 
 
 case 0x20:
 Card_Insercion(0x31);	
 g_cEstadoComSeqMF=0x21;
 break;
 
 
 case 0x21:
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x21;																												 
 }	
 else	g_cEstadoComSeqMF=0X00;																													 
 }
 else
 {
 g_cEstadoComSeqMF=0x22;																															 
 }		
 
 break;
 
 case 0x22:
 Mov_Card('9');
 g_cEstadoComSeqMF=0x23;
 
 case 0x23:                       
 if((temp=Trama_Validacion_P_N())!=0)
 {
 if(temp==2)
 {
 g_cEstadoComSeqMF=0x23;																												 
 }	
 else	g_cEstadoComSeqMF=0X00;																													 
 }
 else
 {
 g_cEstadoComSeqMF=0X00;																																 
 }		
 
 break;
 


 
 
 default:
 g_cEstadoComSeqMF=0X00;	
 break;	
 
 }	
 }
 
 
 
