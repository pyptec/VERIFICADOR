
#line 1 "MAIN.C" /0




 
  
#line 1 "main.h" /0
  
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
 
 
#line 1 "main.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\stdio.h" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
 
 
 
#line 2 "main.h" /0
 
  
#line 1 "uart.h" /0



 



 
 
 
 
 
 void com_initialize (void);
 void com_baudrate ();
 char com_putchar (unsigned char c);
 int com_getchar (void);
 unsigned char com_rbuflen (void);
 unsigned char com_tbuflen (void);
 void clean_tx();
 
 
 
 
 
 
 
 
#line 3 "main.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\INTRINS.H" /0






 
 
 
 
 
 #pragma SAVE
 
 
#line 15 "C:\Keil_v5\C51\Inc\INTRINS.H" /1
 
 
 
#line 18 "C:\Keil_v5\C51\Inc\INTRINS.H" /0
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 
#line 29 "C:\Keil_v5\C51\Inc\INTRINS.H" /1
 
 
 
#line 32 "C:\Keil_v5\C51\Inc\INTRINS.H" /0
 
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 #pragma RESTORE
 
 
 
#line 4 "main.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\math.h" /0







 
 
 
 
 
 
#line 14 "C:\Keil_v5\C51\Inc\math.h" /1
 
  
 
 
 
  
 
 
 
 
 
 
 
 
#line 28 "C:\Keil_v5\C51\Inc\math.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 
 extern char  cabs  (char  val);
 extern int    abs  (int   val);
 extern long  labs  (long  val);
 
 
 extern float fabs  (float val);
 extern float sqrt  (float val);
 extern float exp   (float val);
 extern float log   (float val);
 extern float log10 (float val);
 extern float sin   (float val);
 extern float cos   (float val);
 extern float tan   (float val);
 extern float asin  (float val);
 extern float acos  (float val);
 extern float atan  (float val);
 extern float sinh  (float val);
 extern float cosh  (float val);
 extern float tanh  (float val);
 extern float atan2 (float y, float x);
 
 extern float ceil  (float val);
 extern float floor (float val);
 extern float modf  (float val, float *n);
 extern float fmod  (float x, float y);
 extern float pow   (float x, float y);
 
 
#line 60 "C:\Keil_v5\C51\Inc\math.h" /1
 
 
 
#line 63 "C:\Keil_v5\C51\Inc\math.h" /0
 
 #pragma RESTORE
 
 
#line 5 "main.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\string.h" /0







 
 
 
 
 
 
#line 14 "C:\Keil_v5\C51\Inc\string.h" /1
  
 
 
#line 17 "C:\Keil_v5\C51\Inc\string.h" /0
 
 
#line 19 "C:\Keil_v5\C51\Inc\string.h" /1
  
 
#line 21 "C:\Keil_v5\C51\Inc\string.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 extern char  *strcat  (char *s1, const char *s2);
 extern char  *strncat (char *s1, const char *s2, size_t n);
 
 extern char   strcmp  (const char *s1, const char *s2);
 extern char   strncmp (const char *s1, const char *s2, size_t n);
 
 extern char  *strcpy  (char *s1, const char *s2);
 extern char  *strncpy (char *s1, const char *s2, size_t n);
 
 extern size_t strlen  (const char *);
 
 extern char  *strchr  (const char *s, char c);
 extern int    strpos  (const char *s, char c);
 extern char  *strrchr (const char *s, char c);
 extern int    strrpos (const char *s, char c);
 
 extern size_t strspn  (const char *s, const char *set);
 extern size_t strcspn (const char *s, const char *set);
 extern char  *strpbrk (const char *s, const char *set);
 extern char  *strrpbrk(const char *s, const char *set);
 extern char  *strstr  (const char *s, const char *sub);
 extern char  *strtok  (char *str, const char *set);
 
 extern char   memcmp  (const void *s1, const void *s2, size_t n);
 extern void  *memcpy  (void *s1, const void *s2, size_t n);
 extern void  *memchr  (const void *s, char val, size_t n);
 extern void  *memccpy (void *s1, const void *s2, char val, size_t n);
 extern void  *memmove (void *s1, const void *s2, size_t n);
 extern void  *memset  (void *s, char val, size_t n);
 #pragma RESTORE
 
 
#line 6 "main.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\stdlib.h" /0







 
 
 
 
 
 
#line 14 "C:\Keil_v5\C51\Inc\stdlib.h" /1
  
 
 
#line 17 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 
#line 19 "C:\Keil_v5\C51\Inc\stdlib.h" /1
  
 
#line 21 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 
 
 typedef char wchar_t;
 
 
 
#line 28 "C:\Keil_v5\C51\Inc\stdlib.h" /1
 
 
 
 
 
 
 
 
 
  
 
 
#line 40 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 
 
 
 
 
#line 46 "C:\Keil_v5\C51\Inc\stdlib.h" /1
 
 
 
 
 
 
#line 52 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 
 extern char   cabs (char  val);
 
 extern int    abs  (int   val);
 
 extern long   labs (long  val);
 
 extern float  atof (const char *s1);
 extern long   atol (const char *s1);
 extern int    atoi (const char *s1);
 extern int    rand (void);
 extern void   srand (unsigned int seed);
 
 
#line 69 "C:\Keil_v5\C51\Inc\stdlib.h" /1
 
  
 
 
#line 73 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 extern float         strtod  (const char *nptr, char **endptr);
 extern long          strtol  (const char *nptr, char **endptr, unsigned char base);
 extern unsigned long strtoul (const char *nptr, char **endptr, unsigned char base);
 
 
 
 extern void init_mempool          (void xdata *p, size_t size);
 extern void xdata *malloc  (size_t size);
 extern void free                  (void xdata *p);
 extern void xdata *realloc (void xdata *p, size_t size);
 extern void xdata *calloc  (size_t nmemb, size_t size);
 
 
#line 87 "C:\Keil_v5\C51\Inc\stdlib.h" /1
 
 
#line 89 "C:\Keil_v5\C51\Inc\stdlib.h" /0
 
 #pragma RESTORE
 
 
#line 7 "main.h" /0
 
 
 
 
 
 
 sbit audio_s2 = P0^1;				 
 
 
 sbit Atascado = P0^3;				 
 
 sbit led_err_imp = P0^2;			 
 
 sbit audio_s0 = P0^4;				 
 sbit audio_s1 = P0^5;				 
 sbit rx_in_data = P0^6;				 
 
 sbit lock = P1^7;						 
 sbit sel_com = P0^7;				 
 
 
 
 sbit rx_ip = P0^0;					 
 sbit txd2 = P1^0;					 
 
 
 
 
 sbit DataIn = P1^1;					 
 sbit sel_A = P3^5;					 
 sbit sel_B = P3^6;					 
 sbit sel_C = P3^7;					 
 
 
 
 sbit port_clk = P3^4;				 
 sbit busy = P3^3;  					 
 sbit ready = P3^2;					 
 

 
 
 
 


 
 
 
 
 
 

 
 
 
 
 


 



 
 
 
 
 
 
 idata unsigned char tbuf [50];														 
 idata unsigned char rbuf [50];
 unsigned char xdata Buffer_Rta_Lintech[50];
 
 unsigned char ValTimeOutCom=10;																	 
 unsigned char Timer_wait=0;
 unsigned char g_cEstadoComSoft=0;												 
 unsigned char g_cContByteRx=0;																		 
 unsigned char g_cEstadoComSeqMF=0X00;
 unsigned char Tipo_Vehiculo=0;																		 
 unsigned int  T_GRACIA;																				 
 unsigned int  SIN_COBRO;																				 
 unsigned char  Debug_Tibbo;
 unsigned char  USE_LPR;
 unsigned char  COMPARACION_ACTIVA;
 
 
 
 int ID_CLIENTE;						
 int COD_PARK;
 
 
 const unsigned  char ACK= 06	;
 const unsigned  char ETX= 03	;
 const unsigned  char STX_LINTECH= 0xf2	;
 
 
 bit buffer_ready=0;
 
 
 
 
 
 
 
 
 
 
 
  
#line 1 "delay.h" /0


 



 
 
 
 
 
 void Delay_20us(unsigned int cnt);
 void Delay_1ms(unsigned int cnt);
 void Delay_10ms(unsigned int cnt);
 void Delay (void);
 
 
 
#line 116 "main.h" /0
 
  
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
 
 
 
 
 
#line 117 "main.h" /0
 
  
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
 
 
 
#line 118 "main.h" /0
 
  
#line 1 "clock.h" /0
 



 



 
 
 
 
 
 
 
 void p_ck (void);
 void eb_clk(char dir_clk);
 char lee_clk (unsigned char dir_clk);
 void wr_clk (unsigned char dir_clk, unsigned char byte);
 unsigned char two_one (unsigned char byte_h,unsigned char byte_l);
 void Block_write_clock_ascii(unsigned char *datos_clock);
 void Block_read_clock_ascii(unsigned char *datos_clock);
 void cond_ini_clock();
 char check_fechaOut(char *buffer);
 void Block_read_Clock(unsigned char *datos_clock);
 void analiza_tiempo(char *buffer,unsigned int Val_DctoMinutos);
 void Block_read_Clock_Hex(unsigned char *datos_clock);
 
 
 void Two_ByteHex_Decimal(unsigned char *buffer,unsigned char id_h,unsigned char id_l);
 void ByteHex_Decimal(unsigned char *buffer,unsigned char valorhex);
 unsigned char bcd_hex (unsigned char l_data);
 unsigned char hex_bcd (unsigned char byte);
 
 
 
#line 119 "main.h" /0
 
  
#line 1 "eeprom.h" /0


 



 
 
 
 
 
 
 
 
 
 
 
 void ackd();
 void ack_lect(void);
 void p_me (void);
 void e_d_m (unsigned char a_serial_eeprom);
 void start (void);
 void stop (void);	
 unsigned char l_d_m (void);
 void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom);
 void wrpage_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l,unsigned char data_eeprom); 
 unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
 
 
 
#line 120 "main.h" /0
 
  
#line 1 "io_sensores.h" /0


 



 
 
 
 
 
 
 
 void sel_Funcion(void);
 void sel_Auto(void);
 void sel_Pulsa(void);
 void sel_Sensor2(void);
 void sel_Sensor1(void);
 void sel_Dir1(void);
 void sel_Dir2(void);
 unsigned char Dir_board();
 unsigned char  ValidaSensoresPaso(void);
 char ValidaSensor(void);
 void msj_lcd_informativo();
 
 
#line 121 "main.h" /0
 
  
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
 
 
#line 122 "main.h" /0
 
 
  
#line 1 "Accescan.h" /0


 



 
 
 
 
 
 
 void Valida_Trama_Pto(unsigned char *buffer, unsigned char length_trama);
 unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd);
 void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama);
 void Cmd_Monitor();
 void Monitor_chr (unsigned char *str,unsigned char num_char);
 void Cmd_LPR_Salida_wiegand(unsigned char *buffer);
 void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2);
 
 
 
 
 
#line 124 "main.h" /0
 
  
#line 1 "pantallas.h" /0


 



 
 
 
 
 
 
 void tx_chrlcd (unsigned char data_com, char enable_char_add);
 void LCD_txt (unsigned char * msg,char enable_char_add );
 void PantallaLCD(unsigned char cod_msg);
 void LCD_txt_num_char(unsigned char * msg,unsigned char length_char, unsigned char enable_char_add);
 void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char *buffer);
 
 
#line 125 "main.h" /0
 
  
#line 1 "prog.h" /0



 



 
 
 
 
 
 
 
 void menu(void);
 void Show();
 void DisplayCursor(void);
 void IngresaDato(unsigned char  *buffer, unsigned char modo);
 unsigned char  ValidarClave(unsigned char *buffer);
 unsigned char ListaComandos(unsigned char *cmd);
 void prog_id_cliente();
 void prog_cod_park();
 void Prog_tiempo_gracia();
 void Prog_Sin_Cobro();
 void Prog_debug();
 void Prog_Use_Lpr();
 void Prog_Comparacion_Activa();
 
 
 
 
#line 126 "main.h" /0
 
 
 
 void off_Timer0_Interrup(void);
 
 extern size_t strlen  (const char *);
#line 6 "MAIN.C" /0
 




 
 timer0_int() interrupt 1 using 2
 {
 
 ValTimeOutCom--;
 Timer_wait++;
 TF0=0;
 
 }

 
 void ini_timer(void)
 {
 
 TMOD=(TMOD & 0xF0) | 0x01; 
 
 TH0=0X00;								 
 TL0=0X00;								 
 TR0=1;									 
 
 }

 
 void on_Timer0_Interrup(void)
 {
 TF0=0;									 
 ET0=1;									 
 
 }

 
 void off_Timer0_Interrup(void)
 {
 ET0=0;									 
 
 }

 




 
 char _getkey (void)
 {
 int k;
 
 do
 {
 k = com_getchar ();
 }
 while (k == -1);
 
 return ((unsigned char) k);
 }
 

 
 char putchar (char c)
 {
 volatile unsigned int i;
 
 while (com_putchar (c) != 0)
 {
 for (i=0; i<1000; i++)
 {
 
 }
 }
 
 return (c);
 }


 
 void EscribirCadenaSoft_buffer(unsigned char *buffer,unsigned char tamano_cadena)
 {
 unsigned char i,d;
 for(i=0;i<tamano_cadena;i++)
 {
 d=putchar(*buffer);
 buffer++;
 }
 }

































 
 void inicia_board(void)
 {
 cond_ini_tibbo();							 
 cond_ini_clock();							 
 cond_ini_pto();								 
 Atascado=0;										 
 sel_com=1;										 
 lock=0;												 
 
 }
 void variable_inicio()
 {
 
 ID_CLIENTE=rd_eeprom(0xa8,0x0000);	
 COD_PARK=rd_eeprom(0xa8,0x0002);
 T_GRACIA=rd_eeprom(0xa8,0x0004);
 SIN_COBRO=rd_eeprom(0xa8,0x0006);
 Debug_Tibbo=rd_eeprom(0xa8,0x0008);
 USE_LPR=rd_eeprom(0xa8,0x000A);
 COMPARACION_ACTIVA=rd_eeprom(0xa8,0x000C);
 }




 
 
 
 void main (void)
 {
 
 static unsigned char buffer[40];
 
 unsigned char S1_B2[]={0x13, 0x03, 0x1D, 0x0B, 0x0E, 00, 00, 00, 00, 00, 0x01, 0x13, 0x03, 0x1D, 0x0E, 0x1D};
 unsigned char S1_B0[]={0x32, 0x31, 0x30, 0x37, 0x31, 0x35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01};
 
 char Length_trama;
 ini_timer();
 on_Timer0_Interrup();	
 com_initialize ();              																					 
 com_baudrate ();            																							 
 inicia_board();
 EA = 1;                         																					 
 time_bit();
 Delay_20us(33);
 time_mbit();
 tx_aux('a');	
 sel_Funcion();																	 
 if (DataIn==0)
 {
 menu();
 }
 variable_inicio();
 while(Secuencia_inicio_expedidor());																			 
 ValTimeOutCom=1;
 while (1) 																															 
 {   
 
 
 if (rx_ip==0)
 {
 
 Length_trama=recibe_cmd_Monitor(buffer);
 Debug_Dividir_texto();				
 
 DebugBufferMF(buffer,Length_trama,1);																 
 Debug_Dividir_texto();				
 }
 SecuenciaExpedidor();		  																			 
 
 if (busy==0)
 {
 
 Length_trama=recibe_port(buffer);																		 
 
 
 
 
 
 
 
 
 Valida_Trama_Pto(buffer,Length_trama);															 
 
 }
 msj_lcd_informativo();
 
 
 }
 
 }






















































































































 

 
 
