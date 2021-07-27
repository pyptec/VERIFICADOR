#include <Accescan.h>
#include <reg51.h>

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
extern unsigned char validar_clk(unsigned char *datos_clock);
extern void Debug_chr_Tibbo(unsigned char Dat);
extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 

extern unsigned int Timer_tivo;
extern unsigned char Timer_wait;
extern unsigned char Tipo_Vehiculo;

extern unsigned char  Debug_Tibbo;
extern void Formato_eeprom();
sbit rx_ip = P0^0;				
sbit lock = P1^7;						//Relevo 
sbit Atascado = P0^3;				//Rele de on/off del verificador o transporte
sbit led_err_imp = P0^2;			//Error 	


#define STX											02 
#define ETX											03 

/*MENSAJES PRIMARIO CON CMD DE MSJ*/
#define PRMR_ON_LINE								0xB7
#define PRMR_OFF_LINE								0xb6
#define PRMR_BIENVENIDO							0X02		//FE
#define PRMR_SIN_PAGO								0XE7
#define PRMR_NO_IN_PARK 						0XB2
#define	PRMR_GRACIAS								'V'
#define PRMR_ERROR_LOOP							06					//0XE0
#define PRMR_EXPIRO									0XB4
#define	PRMR_EXCEDE_HORARIO					0X07
#define PRMR_NO_MENSUAL 						0XB1
#define	PRMR_DIREJASE_A_CAJA				0XA1
#define	PRMR_MENSUAL_NO_PAGO				0X08
#define PRMR_UN_MOMENTO							0X09
#define PRMR_SOLICITA_EVN						0XAA
#define PRMR_MSJ_EXCLUSIVO					0X55
/*mensaje de mensual*/
#define GRACIAS									91						//0XFF,01
#define LECTURA_WIEGAND					92//0xB0
#define	NO_IN_PARK							93	
#define EXPIRO									94						//B4
#define	EXCEDE_HORARIO					95
#define NO_MENSUAL_NI_PREPAGO		96
#define	DIREJASE_A_CAJA					90
#define MENSUAL_NO_PAGO					97
/*mensaje informativo DE PANTALLAS*/
#define ERROR_LOOP							171					//0XE0
#define OFF_LINE								174
#define UN_MOMENTO							175
/*direcciones de eeprom*/
#define EE_USE_LPR						0x000A
#define EE_DEBUG							0x0008
/*define variables de esta funcion*/
 unsigned char USE_LPR;
 
//unsigned char S1_B2[]={0x13, 0x03, 0x1D, 0x0B, 0x0E, 00, 00, 00, 00, 00, 0x01, 0x13, 0x03, 0x1D, 0x0E, 0x1D};
//unsigned char S1_B0[]={0x32, 0x31, 0x30, 0x37, 0x31, 0x35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01};
//unsigned char S_B[]={0xE7, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01};
/*------------------------------------------------------------------------------
Rutina q valida los cmd del pto paralelo
temp= usado para contar 20 fuera de linea y mostralo en la pantalla
------------------------------------------------------------------------------*/
void Valida_Trama_Pto(unsigned char *buffer, unsigned char length_trama)
{
	
	 static unsigned char cont;
	 unsigned char bcc=0;
	 unsigned char j; 
	 unsigned char buff[11];
	USE_LPR=rd_eeprom(0xa8,EE_USE_LPR);
	/*-------------------------------CMD H reloj para el board y la pantalla lcd------------------------------------------*/
		if((length_trama==26)&&(*buffer==STX)&&(*(buffer+2)=='H')&&*(buffer+(length_trama-2))==ETX)													/*cmd de Accescan que me envia el reloj actualizado*/
		{ 
			Debug_txt_Tibbo((unsigned char *) "primario BCC= ");
			Debug_chr_Tibbo(*(buffer+25));
			Debug_txt_Tibbo((unsigned char *) "\r\n");
			for (j=0; j<length_trama-1; j++)
			{
				bcc=*(buffer+j) ^ bcc;
			}
			Debug_txt_Tibbo((unsigned char *) "calculo BCC= ");
			Debug_chr_Tibbo(bcc);
			Debug_txt_Tibbo((unsigned char *) "\r\n");
			if (bcc == *(buffer+25))
			{
				if(validar_clk(buffer+3)==0)
				{
				Block_write_clock_ascii(buffer+3);																																								/* se escribe el reloj de hardware*/
		
				Reloj_Pantalla_Lcd();																																															/* Escribo el reloj en la pantalla lcd*/
				}
			}	
			else
			{
				buff[0]=02;
				buff[1]=05;
				buff[2]=03;
				buff[3]=0;
				 send_port(buff,4);
				
				Debug_txt_Tibbo((unsigned char *) "REENVIAR trama Hora: ");
				Debug_chr_Tibbo(buff[0]);
				Debug_chr_Tibbo(buff[1]);
				Debug_chr_Tibbo(buff[2]);
				Debug_txt_Tibbo((unsigned char *) "\r\n");
			}
			
		}
		/*-------------------------------CMD B6 fuera de linea -------------------------------------------------------------*/
		else if(*buffer==PRMR_OFF_LINE)																																										/*cmd de Accescan que dice q esta fuera de linea*/
		{
				cont++;
				if (cont>4)			/* con un tiempo de retardo =((1/(22118400/12)*65535)*30)*/
				{
				PantallaLCD(OFF_LINE);
				led_err_imp=0;																																																	/*error led on*/
				Timer_wait=0;
				lock=0;																																																					/*relevo off despues de 1 minuto*/
				Atascado=0;	
				cont=0;
				}
		}
		/*-------------------------------CMD B7 en linea (AA) NO SE SABE  ------------------------------------------------------------------*/
		else if ((*buffer==PRMR_ON_LINE)	||(*buffer==PRMR_SOLICITA_EVN) )																																										/*en linea*/
		{		
			
			if (Timer_wait>=20)																																																/* se envia el msj fuera de linea*/
			{	
				Timer_wait=0;																																																		/*se inicia el timer*/
				lock=0;
				led_err_imp=1;																																																	/*relevo off despues de 1 minuto*/
				Atascado=0;	
			}	
			
			if ((Debug_Tibbo==0)&&(USE_LPR==1)&& (Timer_tivo>=600))
			{
				Timer_tivo=0;
				Debug_Tibbo=1;
				Debug_txt_Tibbo((unsigned char *) "LIVE");
				Debug_Tibbo=0;
			}
			
			
		}	
		
			/*-------------------------------CMD B1 PRMR_NO_MENSUAL_NI PREPAGO -----------------------------------------------------------------*/
		else if ((*buffer==PRMR_NO_MENSUAL)	)																																										/*en linea*/
		{
			PantallaLCD(NO_MENSUAL_NI_PREPAGO);																																														/*MSJ MENSUAL NO EN PARQUEADERO*/
		}		
			/*-------------------------------CMD B2 PRMR_NO_IN_PARK   -----------------------------------------------------------------*/
		else if ((*buffer==PRMR_NO_IN_PARK)	)																																										/*en linea*/
		{
			PantallaLCD(NO_IN_PARK);																																														/*MSJ MENSUAL NO EN PARQUEADERO*/
		}	
			/*-------------------------------	CMD 55 PRMR_MSJ_EXCLUSIVO  ------------------------------------------------------------------*/
		else if ((length_trama==3)&&(*(buffer+1)==PRMR_MSJ_EXCLUSIVO)&&*(buffer+(length_trama-1))==ETX)																																				/* */
		{
				 Formato_eeprom();																																														/*mesualidad vencida*/
		}			
			/*-------------------------------CMD A1    DIREJASE_A_CAJA	              ------------------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_DIREJASE_A_CAJA	))																																				/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(DIREJASE_A_CAJA);
		}
		
			/*-------------------------------CMD 06  error de loop    ------------------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_ERROR_LOOP))																																		/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(ERROR_LOOP);
		}
		
				/*-------------------------------CMD 07  EXCEDE_HORARIO   ------------------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_EXCEDE_HORARIO))																																		/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(EXCEDE_HORARIO);
		}
		

				/*-------------------------------CMD 08 MENSUAL_NO_PAGO   ------------------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_MENSUAL_NO_PAGO))																																		/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(MENSUAL_NO_PAGO);
		}	
	
		/*-------------------------------CMD 09 UN_MOMENTO	   ------------------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_UN_MOMENTO	))																																		/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(UN_MOMENTO	);
		}			
			/*-------------------------------CMD B4  EXPIRO mensualidad vencida ----------------------------------------------------------*/
		else if ((length_trama==1)&&(*buffer==PRMR_EXPIRO))																																		/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD(EXPIRO);																																															/*mesualidad vencida*/
		}
		/*-------------------------------CMD 'V'=PRMR_GRACIAS  msj Gracias y nombre del mensual-----------------------------------------*/
		else if ((length_trama==19)&& (*(buffer+1)==PRMR_GRACIAS)&&*(buffer+(length_trama-1))==ETX)													/*cmd 0xA1 audio caja que es igual a no registra pago */
		{
				 PantallaLCD_LINEA_2(GRACIAS,buffer+2);																																			/*SE ENVIA EL MSJ GRACIAS lo q envia el software*/
		}
		
		/*-------------------------------CMD de wiegand---------------------------------------------------*/
		else if ((length_trama==6)&&(*buffer==STX)&&(*(buffer+1)=='W')&&*(buffer+(length_trama-1))==ETX)										/* cmd q comunica con monitor por wiegand*/
		{
				/*-------------------------------se covierte el numero serie de la tarjeta------------------------------*/
							ByteHex_Decimal(buff,*(buffer+2));																																				/*convierto el primer byte_hex a decimal		*/
							buff[3]=' ';
							Two_ByteHex_Decimal(buff+4,*(buffer+3),*(buffer+4))	;																											/*convierto un byte de 16 bits a decimal*/		
				/*------------------------------------------------------------------------------------------------------*/
					
			if (USE_LPR==1)
				{
							/*-------------------------------mensaje en la pantalla --------------------------------------------*/
							/*ValidaSensoresPaso=0 no hay presencia  ValidaSensoresPaso=0xff  hay presencia*/
							if (ValidaSensoresPaso()!= 0xff)
							{								
								PantallaLCD(ERROR_LOOP);
								PantallaLCD_LINEA_2(LECTURA_WIEGAND,buff);																																/*msj rasberry*/
							}
							else
							{
								Cmd_LPR_Salida_wiegand(buff);
								PantallaLCD_LINEA_2(LECTURA_WIEGAND,buff);																																/*msj rasberry*/
							}
					
							
																																																												/*transmito el codigo de la tarjeta a la pantalla lcd*/
													
				}																																							
				
				else
				{
					 /*-------------------------------mensaje en la pantalla---------------------------------------------------*/
																															
							if (ValidaSensoresPaso()!= 0xff)
							{								
								PantallaLCD(ERROR_LOOP);
							}
							PantallaLCD_LINEA_2(LECTURA_WIEGAND,buff);																																/*transmito el codigo de la tarjeta a la pantalla lcd*/
																																																		
					/*--------------------------------------------------------------------------------------------------------*/	
				
				}
		}
		
}
/*------------------------------------------------------------------------------
Rutina q recibe  los cmd de Monitor por el tibbo
return el num de caracteres recibidos
y almacena la trama en un apuntador
------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------
Rutina q valida los cmd de Monitor
------------------------------------------------------------------------------*/
/*
void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama)
{		
	length_trama=1;
		if	((*(buffer+2)==ETX)&&(*(buffer+1)=='P')) 																																						/* APERTURA DE BARRETA*/ 
/*				{
	 				lock=1;																																																						/*habilita el relevo ON*/
//					Timer_wait=0;
//	 			}
//		else if (*buffer=='<')
//		{																																																												/*placa*/
//		}
//}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/*
void Cmd_Monitor()
{
		
	
}
*/
/*------------------------------------------------------------------------------
Transmito un caracter al software monitor 
------------------------------------------------------------------------------*/
void Monitor_chr (unsigned char *str,unsigned char num_char)
{
	unsigned char j;
	for (j=0; j<num_char; j++)
		{
		tx_aux(*str);
		str++;
		}
}
	
/*------------------------------------------------------------------------------
Transmito CMD de salida wiegand 
------------------------------------------------------------------------------*/
void Cmd_LPR_Salida_wiegand(unsigned char *buffer)
{
	unsigned char Buffer_Lpr[30];
	unsigned char j=3;
	Buffer_Lpr[0]=STX;																			/*inicio de cmd STx*/
	Buffer_Lpr[1]=Dir_board();															/*direccion de la tarjeta*/
	Buffer_Lpr[2]='S';																			/*cmd S que salida wiegand*/
	if(Tipo_Vehiculo!=0)																		/*Tipo de vehiculo*/
		{
			Buffer_Lpr[j++]='M';																/*moto*/
		}
		else
		{
			Buffer_Lpr[j++]='C';																/*carro*/
		}
	
		
		
		for (j=4; *buffer != '\0'; j++)												/*numero del tab o tarjeta Mf*/
			{
		   		Buffer_Lpr[j]=*buffer;
					buffer++;
				
			}
			Buffer_Lpr[j++]=':';																/*separador del tab  o tarjeta MF*/
									
			Block_read_clock_ascii(Buffer_Lpr+j);								/*año,mes,dia,hora,minutos,*/
			Buffer_Lpr[j+10]=':';																/*separador fecha*/
			Buffer_Lpr[j+11]=ETX;																/*fin de la trama*/
		
					
		
			Monitor_chr(Buffer_Lpr,j+12);												/*rutina de envio de la trama a monitor*/
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B2)
{
	
	
	unsigned char Buffer_Lpr[30];
	unsigned temp;
	unsigned char j=3;
	Buffer_Lpr[0]=STX;																/*inicio de cmd STx*/
	Buffer_Lpr[1]=Dir_board();												/*direccion de la tarjeta*/
	Buffer_Lpr[2]='S';																/*numero de digitos de transmicion NO IMPORTA NO ES VALIDADO EN PRINCIPAL*/
	
		if(*(buffer_S1_B2+8)!=0)												/*Tipo de vehiculo*/
		{
			Buffer_Lpr[j++]='M';													/*moto*/
		}
		else
		{
			Buffer_Lpr[j++]='C';													/*carro*/
		}
	
	
	do
	{
	 Buffer_Lpr[j++]=*buffer_S1_B0;									/*ticket o consecutivo*/
		buffer_S1_B0++;
	}while (*buffer_S1_B0!=0);
	
	
	
	
	
		Buffer_Lpr[j++]=':';														/*separador de la fecha de entrada*/

		temp=hex_bcd(*(buffer_S1_B2+0));								/*año a ascii*/
		Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
		Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
		
		temp=hex_bcd(*(buffer_S1_B2+1));								/*mes a ascii*/
		Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
		Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
	
		temp=hex_bcd(*(buffer_S1_B2+2));								/*Dia a ascii*/
		Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
		Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
	
		temp=hex_bcd(*(buffer_S1_B2+3));								/*Hora a ascii*/
		Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
		Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
	
		temp=hex_bcd(*(buffer_S1_B2+4));								/*Minutos a ascii*/
		Buffer_Lpr[j++]=((temp & 0xf0)>>4)| 0x30;
		Buffer_Lpr[j++]=((temp & 0x0f))| 0x30;
	
		
	
		Buffer_Lpr[j++]=':';														/*separador tipo fecha*/
																										/**/
				
		Buffer_Lpr[j++]=ETX;	
	
		Monitor_chr(Buffer_Lpr,j);												/*rutina de envio de la trama a monitor*/
}
/*	
void live()
{
																																											/*en linea*/
		
			
		
	//		if ((Debug_Tibbo==0)&&(USE_LPR==1))
//			{
	//			Debug_Tibbo=1;
		//		Debug_txt_Tibbo((unsigned char *) "LIVE");
		//		Debug_Tibbo=0;
		//	}
//			
			
			
//}
	