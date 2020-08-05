
#line 1 "pantallas.c" /0
  
#line 1 "pantallas.h" /0


 



 
 
 
 
 
 
 void tx_chrlcd (unsigned char data_com, char enable_char_add);
 void LCD_txt (unsigned char * msg,char enable_char_add );
 void PantallaLCD(unsigned char cod_msg);
 void LCD_txt_num_char(unsigned char * msg,unsigned char length_char, unsigned char enable_char_add);
 void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char *buffer);
 
 
#line 1 "pantallas.c" /0
 
  
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
 
 
#line 2 "pantallas.c" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\string.h" /0







 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 
 
 
 
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
 
 
#line 3 "pantallas.c" /0
 
 
 sbit sel_com = P0^7;				 
 
 
 
 extern char putchar (char c);
 extern void Block_read_Clock(unsigned char *datos_clock);
 extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
 extern void Debug_Dividir_texto();
 extern void clean_tx();
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 




 
 void tx_chrlcd (unsigned char data_com, unsigned char enable_char_add)
 {
 unsigned char d;
 d=putchar(data_com);
 if (enable_char_add != 0)	d=putchar(0x00);
 
 }




 
 void LCD_txt (unsigned char * msg,unsigned char enable_char_add )
 {
 unsigned char i;
 
 for (i=0; msg[i] != '\0'; i++)
 {
 tx_chrlcd(msg[i],enable_char_add);
 }
 }





 
 void LCD_txt_num_char(unsigned char * msg,unsigned char length_char, unsigned char enable_char_add)
 {
 unsigned char i;
 
 for (i=0; i<length_char; i++)
 {
 tx_chrlcd(msg[i],enable_char_add);
 }
 }





 
 void Reloj_Pantalla_Lcd()
 {
 
 unsigned char Ini_Clock_LCD   []={0xaa,0x80,0x28,0x07,0x20,0x00,0,0,0,0,20,0,0} ;
 sel_com=0;																																			 
 Block_read_Clock(Ini_Clock_LCD+5);																							 
 
 
 
 REN = 0;																																				 
 LCD_txt_num_char(Ini_Clock_LCD,13,0);																						 
 
 REN = 1;																																				 
 sel_com=1;																																			 
 }


 
 void PantallaLCD(unsigned char cod_msg)
 {
 
 unsigned char Ini_LCD_Line_one   []={0xaa,0x80,0x18,0x01,0x02,0x00} ;
 
 
 
 unsigned char num_chr;
 
 
 sel_com=0;
 
 
 
 LCD_txt (Ini_LCD_Line_one,0);
 
 switch (cod_msg)
 {
 
 case 'P':
 
 num_chr=strlen((unsigned char *) "ERROR: VALIDANDO PLACA... ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "ERROR: VALIDANDO PLACA... ",1);
 
 break;
 
 case 0XE0:
 
 num_chr=strlen((unsigned char *) "ERROR: LOOP1 SIN PRESENCIA VEHICULAR ");	
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "ERROR: LOOP1 SIN PRESENCIA VEHICULAR ",1);
 
 break;
 
 case 0XE1:
 
 num_chr=strlen((unsigned char *) "ERROR: TARJETA INVALIDA ");	
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "ERROR: TARJETA INVALIDA ",1);
 
 break;
 
 case 0XE5:
 
 num_chr=strlen((unsigned char *) "TARJETA NO ES DEL PARQ. ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "TARJETA NO ES DEL PARQ. ",1);
 
 break;
 
 case 0xDF:
 
 num_chr=strlen((unsigned char *) "TARJETA SIN FORMATO ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt((unsigned char *)        "TARJETA SIN FORMATO ",1);
 
 break;
 
 case 0XE7:
 
 num_chr=strlen((unsigned char *) "TARJETA NO REGISTRA PAGO ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "TARJETA NO REGISTRA PAGO ",1);
 
 break;
 
 case 0XE8:
 
 num_chr=strlen((unsigned char *) "EXCEDE TIEMPO DE GRACIA ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt ((unsigned char *)       "EXCEDE TIEMPO DE GRACIA ",1);
 
 break;
 
 case 	0XB6:
 
 num_chr=strlen((unsigned char *) "FUERA DE LINEA ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt((unsigned char *)        "FUERA DE LINEA ",1);
 
 break;
 
 case	0XFE:
 
 num_chr=strlen((unsigned char *) "BIENVENIDO ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt((unsigned char *)        "BIENVENIDO ",1);
 
 break;
 
 case	0XE6:
 
 num_chr=strlen((unsigned char *) "ERROR: SIN INGRESO ");
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt((unsigned char *)        "ERROR: SIN INGRESO ",1);
 
 break;
 
 }
 sel_com=1;	
 }
 void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char *buffer)
 {
 
 unsigned char Ini_LCD_Line_one   []={0xaa,0x80,0x18,0x01,0x02,0x00} ;
 
 
 unsigned char num_chr;
 
 sel_com=0;
 
 
 
 
 
 switch (cod_msg)
 {
 
 case	0xB0:
 LCD_txt (Ini_LCD_Line_one,0);
 num_chr=strlen((unsigned char *) "WIEGAND ");
 num_chr=num_chr+strlen(buffer)+1;
 tx_chrlcd(0x00,0);
 tx_chrlcd(num_chr*2,0);
 LCD_txt((unsigned char *)        "WIEGAND ",1);	 											 
 LCD_txt(buffer,1);	 																									 
 
 
 
 
 break;
 
 
 }
 sel_com=1;	
 }
