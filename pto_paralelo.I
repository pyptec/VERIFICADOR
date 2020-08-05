
#line 1 "pto_paralelo.c" /0
  
#line 1 "pto_paralelo.h" /0


 



 
 
 
 
 
 
 
 
 unsigned char recibe_port(unsigned char *buffer_port);
 void  send_port(unsigned char *buffer_port, unsigned char length_char);
 void Pulso_Bus(void);
 void send_portERR(unsigned char cod_err);
 void cond_ini_pto();
 void Trama_pto_Paralelo(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 void load_and_send_info_reloj();
 void load_and_send_id_cod();
 void Trama_pto_Paralelo_C_s(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2);
 void Trama_pto_Paralelo_P(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 void Trama_pto_Paralelo_new(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd);
 
 
#line 1 "pto_paralelo.c" /0
 
  
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
 
 
#line 2 "pto_paralelo.c" /0
 
 
 extern void Delay (void);
 extern void Delay_20us(unsigned int cnt);
 extern void off_Timer0_Interrup(void);
 extern void on_Timer0_Interrup(void);
 extern void          _nop_     (void);
 extern void Block_read_Clock(unsigned char *datos_clock);
 extern char lee_clk (unsigned char dir_clk);
 extern int sprintf  (char *, const char *, ...);
 extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
 extern void Debug_Dividir_texto();
 extern void Block_read_Clock_Hex(unsigned char *datos_clock);
 
 
 sbit port_clk = P3^4;				 
 sbit busy = P3^3;  					 
 sbit ready = P3^2;					 
 sbit rx_in_data = P0^6;				 
 sbit led_err_imp = P0^2;			 
 
 
 
 
 
 
 
 extern int ID_CLIENTE;						
 extern int COD_PARK;
 












 
 unsigned char recibe_port(unsigned char *buffer_port)
 {
 
 unsigned int count;
 unsigned char time_out,num_data;
 unsigned int T_ANCHO=40;
 
 
 
 if (busy==0)
 {
 
 off_Timer0_Interrup();
 num_data=0;
 rx_in_data=0;
 time_out=0;
 ready=0;
 count=T_ANCHO;
 
 
 count=T_ANCHO;
 while ((busy==0)&&(time_out==0))
 {
 if(port_clk==1)
 {
 
 count--;
 if (count==0)
 {
 time_out=1;
 break;
 }
 }
 else 
 {
 if (num_data<40)
 {
 P2 = 0x00; 	
 P2 = 0xff; 	
 while(port_clk==0)
 {
 *buffer_port=P2 & 0xff;
 }
 buffer_port++;
 num_data++; 
 count=T_ANCHO;
 }
 else{break;}
 }
 }
 
 
 ready=1;
 rx_in_data=1;
 }
 on_Timer0_Interrup();
 
 return num_data;
 }
 




 
 void  send_port(unsigned char *buffer_port, unsigned char length_char)
 {
 
 long int cont;
 unsigned char timeOut,length;
 
 length=length_char;
 
 ready=1;																				 
 port_clk=1;	 																		 
 rx_in_data=0;	 																	 
 timeOut=0;																			 
 ready=0;																				 
 cont=20000;
 
 while ((busy==1)&&(timeOut==0)) 								 
 {
 ready=0;
 port_clk=1;
 cont--;
 if (cont==0)
 {
 timeOut=1;
 led_err_imp=0;															 
 }
 }
 if ((timeOut==0)&&(busy==0))
 {
 
 
 do 
 {
 
 P2=*buffer_port;
 Pulso_Bus();
 buffer_port++;
 
 length_char--;
 }while (length_char);
 
 }
 P2=0XFF;
 ready=1;
 port_clk=1;
 rx_in_data=1;																		 
 Delay_20us(98);
 
 
 }


 





 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 void Trama_pto_Paralelo_P(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd)
 {
 
 static unsigned char Buffer_port[30];
 unsigned char j=3;
 Buffer_port[0]=02;																 
 Buffer_port[1]=cmd;																 
 Buffer_port[2]=21;																 
 do
 {
 Buffer_port[j++]=*buffer_S1_B0;									 
 buffer_S1_B0++;
 }while (*buffer_S1_B0!=0);
 Buffer_port[2]=j-3;
 if(*(buffer_S1_B2+5)!=0)													 

 {
 Buffer_port[j++]='-';														 
 Buffer_port[j++]=*(buffer_S1_B2+5)+0x30;				 
 }
 

 
 
 
 
 
 
 
 
 
 
 Buffer_port[j++]='.';														 
 
 
 if(*(buffer_S1_B2+8)!=0)
 {
 Buffer_port[j++]='M';													 
 }
 else
 {
 Buffer_port[j++]='C';													 
 }
 
 
 ready=0;
 
 send_port(Buffer_port,j);													 
 Debug_Dividir_texto();																							 
 
 DebugBufferMF(Buffer_port,j,1);		
 Debug_Dividir_texto();	
 
 
 }
 




 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 void Trama_pto_Paralelo_new(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2,unsigned char cmd)
 {
 unsigned char Buffer_port[30];
 unsigned char j=2;
 
 Buffer_port[0]=02;																 
 Buffer_port[1]=cmd;																 
 
 do
 {
 Buffer_port[j++]=*buffer_S1_B0;									 
 buffer_S1_B0++;
 }while (*buffer_S1_B0!=0);
 
 if(*(buffer_S1_B2+5)!=0)													 

 {
 Buffer_port[j++]='-';														 
 Buffer_port[j++]=*(buffer_S1_B2+5)+0x30;				 
 }
 else
 {
 Buffer_port[j++]='-';														 
 Buffer_port[j++]='0';
 }
 
 if((*(buffer_S1_B2+9)&0x0f)==0x0f)									 
 {
 Buffer_port[1]='C';
 }
 else
 {
 Buffer_port[1]='s';
 }
 
 Buffer_port[j++]='-';														 
 Buffer_port[j++]=*(buffer_S1_B2+0)+0x030;				 
 Buffer_port[j++]=*(buffer_S1_B2+1)+0x030;				 
 Buffer_port[j++]=*(buffer_S1_B2+2)+0x030;				 
 Buffer_port[j++]=*(buffer_S1_B2+3)+0x030;				 
 Buffer_port[j++]=*(buffer_S1_B2+4)+0x030;				 
 Buffer_port[j++]='-';	
 
 Block_read_Clock_Hex(Buffer_port+j);
 
 Buffer_port[j++]=Buffer_port[j]+ 0x30;
 Buffer_port[j++]=Buffer_port[j]+ 0x30;
 Buffer_port[j++]=Buffer_port[j]+ 0x30;
 Buffer_port[j++]=Buffer_port[j]+ 0x30;
 Buffer_port[j++]=Buffer_port[j]+ 0x30;
 
 
 
 Buffer_port[j++]='.';														 
 
 
 if(*(buffer_S1_B2+8)!=0)
 {
 Buffer_port[j++]='M';													 
 }
 else
 {
 Buffer_port[j++]='C';													 
 }
 Buffer_port[j++]=03;	
 
 
 ready=0;
 while(busy==0);
 send_port(Buffer_port,j);													 
 
 Debug_Dividir_texto();																							 
 
 DebugBufferMF(Buffer_port,j,1);		
 Debug_Dividir_texto();	
 }
 
 
 


 
 void Pulso_Bus(void)
 {
 port_clk=0;
 Delay_20us(33);
 port_clk=1;
 Delay_20us(33);									 
 }


 
 void send_portERR(unsigned char cod_err)
 {
 unsigned char timeOut;
 long int cont;
 
 port_clk=1;	 																	 
 rx_in_data=0;	 																 
 timeOut=0;
 cont=15000;
 
 P2=cod_err;
 ready=0;																			 
 while ((busy==1)&&(timeOut==0)) 							 
 {
 cont--;
 if (cont==0)
 {
 timeOut=1;
 }
 }
 if ((timeOut==0)&&(busy==0))
 {
 P2=cod_err;
 Pulso_Bus();
 }
 ready=1;
 port_clk=1;
 rx_in_data=1;																		 
 
 P2=0XFF;
 
 Delay_20us(98);	
 Delay_20us(98);	
 
 }


 
 void cond_ini_pto()
 {	
 
 port_clk=1;
 ready=1;
 rx_in_data=1;
 P2=0xff;
 }



 
 void load_and_send_info_reloj()
 {
 unsigned char buffer_info[11];
 
 buffer_info[0]=02; 									 
 
 buffer_info[1]='i';										 
 
 Block_read_Clock(buffer_info+2);			 
 buffer_info[8]=lee_clk(0x8b); 				 
 buffer_info[9]=03; 									 
 
 
 
 send_port(buffer_info,10);
 }
 



 
 void load_and_send_id_cod()
 {
 unsigned char buffer_info[11];
 unsigned char buf[4];
 unsigned char i,k;
 k=0;
 
 buffer_info[0]=02; 											 
 buffer_info[1]='D';												 
 sprintf(buf,"%d",ID_CLIENTE);							 
 for(i=2;;i++)
 {
 if(buf[k] !=0)													 
 {
 buffer_info[i]=buf[k] ;									 
 k++;
 }
 else {buffer_info[i]=';'; i++; break;}	 
 
 }
 
 
 sprintf(buf,"%d",COD_PARK);								 
 
 for(k=0;;i++)
 {
 if(buf[k] !=0)
 {
 buffer_info[i]=buf[k] ;										 
 k++;
 }else {buffer_info[i]=';'; i++; break;}
 }
 
 
 buffer_info[i]=03; 										 
 send_port(buffer_info,i);									 
 }
