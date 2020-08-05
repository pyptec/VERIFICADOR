
#line 1 "io_sensores.c" /0
 
  
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
 
 
#line 2 "io_sensores.c" /0
 
  
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
 
 
#line 3 "io_sensores.c" /0
 
 
 
 extern void Debug_txt_Tibbo(unsigned char * str);
 extern void load_and_send_info_reloj();
 extern void load_and_send_id_cod();
 extern void send_portERR(unsigned char cod_err);
 extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
 
 sbit DataIn = P1^1;					 
 sbit sel_A = P3^5;					 
 sbit sel_B = P3^6;					 
 sbit sel_C = P3^7;					 
 sbit busy = P3^3;  					 
 
 
 
 
 
 
 extern bit COMPARACION_ACTIVA;
 extern  unsigned char Tipo_Vehiculo;
 
 void sel_Funcion(void)
 {
 sel_A=0;
 sel_B=0;
 sel_C=0;
 }
 
 void sel_Auto(void)
 {
 sel_A=0;
 sel_B=1;
 sel_C=0;
 }
 







 
 
 void sel_Sensor2(void)
 {
 sel_A=0;
 sel_B=0;
 sel_C=1;
 }
 
 void sel_Sensor1(void)
 {
 sel_A=1;
 sel_B=0;
 sel_C=1;
 }
 
 void sel_Dir1(void)
 {
 sel_A=0;
 sel_B=1;
 sel_C=1;
 }
 
 void sel_Dir2(void)
 {
 sel_A=1;
 sel_B=1;
 sel_C=1;
 }
 
 char ValidaSensor(void)
 {
 
 char	Valido=0;
 int	Tmin=500;
 while ((DataIn==1)&&(Valido==0))
 {
 Tmin--;
 if (Tmin==0)
 {
 Valido=1;
 }
 }
 return Valido;
 }


 
 unsigned char Dir_board()
 {
 char Board=0x31;
 
 sel_Dir1();
 if (DataIn==1)
 {
 Board++;
 }
 sel_Dir2();
 if (DataIn==1)
 {
 Board=Board+2;
 }
 return (Board);
 }


 
 
 unsigned char  ValidaSensoresPaso(void)
 {
 
 sel_Sensor2();																 
 if ((DataIn==1))				
 {  
 if (ValidaSensor()==1)
 {
 sel_Sensor1();	
 if (DataIn==0)														 
 {  sel_Auto();
 if (DataIn==0)
 {					  															 
 Debug_txt_Tibbo((unsigned char *) "Sensores Auto y sensor1 activos.");
 Tipo_Vehiculo=0X00;
 return 0xff;
 
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Sensor activo sensor1 .");
 Tipo_Vehiculo=0X01;
 return 0xff ;
 }
 
 
 }
 else
 { 	sel_Auto();
 if (DataIn==0)
 {
 
 Debug_txt_Tibbo((unsigned char *) "Detectado. Sensor Auto.");
 Tipo_Vehiculo=0X01;
 return 0xff;	
 }
 else
 {
 Debug_txt_Tibbo((unsigned char *) "Sensores no detectados");
 return 0x00;	
 }
 
 
 }
 }
 else 
 {
 Debug_txt_Tibbo((unsigned char *) "Vehiculo Esperando Salir.");
 return 0x00;
 }
 }else 
 {
 Debug_txt_Tibbo((unsigned char *) "Vehiculo Saliendo. Un momento");
 return 0x00;
 }
 
 }
 
 





 
 void msj_lcd_informativo()
 {
 unsigned char toggle, info;
 
 
 sel_Funcion();																	 
 if (DataIn==0)
 {
 if ((toggle==0)&&(info==0))
 {
 if (busy==1)
 {
 info=1;
 load_and_send_info_reloj();								 
 toggle=1;
 }
 }		
 else if((toggle==1)&&(info==0))
 {
 if (busy==1)
 {
 info=1;
 load_and_send_id_cod();
 toggle=2;
 }
 }
 else if ((toggle==2)&&(info==0))
 {
 if (busy==1)
 {
 info=1;
 if(rd_eeprom(0xa8,0x0066)=='S')
 {
 send_portERR('S');
 }else {send_portERR('N');}
 
 toggle=0;
 }
 
 }
 
 }
 
 sel_Funcion();   								 
 if (DataIn==1)
 {
 info=0;
 }
 
 }
