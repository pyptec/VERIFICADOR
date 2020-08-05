/*
		FUNCIONES PARA DISPENSADOR																*
*/
#include "verificador.h"
#include <reg51.h>


/*funciones prototipo externas */

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

extern void Inicializa(unsigned char TipoMovimiento);
extern void Check_Status(unsigned char Detalle);
extern void Aut_Card_check_Status(void);
extern void Mov_Card(unsigned char Posicion);
extern void SecuenciaExpedidor(void);
extern void Card_Insercion(char Tipo);
extern void Dwload_EEprom (void);
extern void LoadVerify_EEprom(void);
extern void RD_MF(unsigned char Sector, unsigned char Bloque);
extern void WR_MF(unsigned char Sector, unsigned char Bloque,unsigned char *buffer);
extern int sprintf  (char *, const char *, ...);
extern void Debug_Dividir_texto();
extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
extern void Delay_1ms(unsigned int cnt);
extern void Delay_10ms(unsigned int cnt);
extern unsigned char  ValidaSensores_Out(void);

sbit lock = P1^7;						//Relevo 	
sbit Atascado = P0^3;				//Rele de on/off del verificador o transporte
sbit led_err_imp = P0^2;			//Error 	
/*variables externas*/
extern bit aSk;
extern unsigned char g_cEstadoComSoft;
extern unsigned char ValTimeOutCom;
extern unsigned char g_cEstadoComSeqMF;
extern unsigned char g_cContByteRx;
extern unsigned char xdata Buffer_Rta_Lintech[];
extern idata unsigned char rbuf [];
extern int ID_CLIENTE;
extern int COD_PARK;
extern unsigned int T_GRACIA;																				/*tiempo de gracia del parqueo*/
extern unsigned char Timer_wait;
extern unsigned int  SIN_COBRO;
extern unsigned char error_rx;


/*externo bit*/

extern bit buffer_ready;
extern bit prto_ready;
extern unsigned char USE_LPR;

/*----------------------------------------------------------------------------
Definiciones de sequencias de verificador y expedidor
------------------------------------------------------------------------------*/

#define SEQ_INICIO						0X00	
#define SEQ_CARD_INSERCION		0X01
#define SEQ_CHECK_STATUS			0X02
#define SEQ_RTA_CARD_POS			0x03
#define SEQ_RTA_CAPTURE				0X04
#define SEQ_RTA_CAPTURE_RPTA	0x05
#define SEQ_REQUEST						0x06
#define SEQ_MF_LINTECH				0x07
#define SEQ_RD_S1B1		 				0x08
#define SEQ_RD_S1B1_RPTA			0x09
#define SEQ_RD_S1B2 					0x0a
#define SEQ_RD_S1B2_RPTA			0x0b
#define SEQ_RD_S1B0 					0x0c
#define SEQ_RD_S1B0_RPTA			0x0d
#define SEQ_WR_S1B2						0x0e
#define SEQ_WR_S1B2_RPTA			0x0f

#define SEQ_WR_S2B0						0x10
#define SEQ_WR_S2B0_RPTA			0X11
#define SEQ_RD_S1B0_EJECT			0x12
#define SEQ_RD_S1B0_EJECT_RPTA			0x13

#define SEQ_EXPULSAR_TARJ			0x20
#define SEQ_EXPULSAR_CHECK 		0x21
#define SEQ_EXPULSAR 					0x22
#define SEQ_EXPULSAR_FROM			0x23


/*----------------------------------------------------------------------------
 definiciones de lintech en la inicializacion de expedidor o verificador
------------------------------------------------------------------------------*/

#define INICIA_LINTECH					0x30
#define	SEQ_CAPTURE_DATOS_INI		0x31
#define GRABA_EEPROM						0x32
#define SEQ_CAPTURA_OK_EEPROM		0X33
#define FIN_OK									0x00

/*----------------------------------------------------------------------------
 definiciones de lintech en el comando Check_Status
------------------------------------------------------------------------------*/

#define	S_DETAIL				0x31
#define S_NORMAL				0x30

/*------------------------------------------------------------------------------
 definiciones de lintech en el comando Card_Insercion
------------------------------------------------------------------------------*/

#define Habilita			 	0x30
#define Inhabilita			0x31

/*------------------------------------------------------------------------------
Definicion de Lintech en el comando Inicializa
------------------------------------------------------------------------------*/

#define	TO_FRONT				'0'
#define	CAPTURE_BOX			'1'
#define	SIN_MOVIMIENTO	'3'

/*------------------------------------------------------------------------------
Definicion de Lintech en el comando mover tarjeta (Mov_Card)
------------------------------------------------------------------------------*/

#define 	MovPos_Front				'0'		
#define 	MovPos_IC						'1'
#define  	MovPos_RF						'2'
#define  	MovPos_Capture			'3'
#define 	MovPos_EjectFront		'9'

/*------------------------------------------------------------------------------
Definicion de la trama Lintech de las respuestas de los cmd
------------------------------------------------------------------------------*/

#define Pos_TipoResp				4
#define Pos_Length					3
#define Pos_St0							7
#define Pos_St1							8
#define Pos_St2							9
#define Pos_IniDatMF				0x0a
#define	Card_type_H					0x0a
#define	Card_type_L					0x0b

/*----------------------------------------------------------------------------
definicion de recepcion serial 
------------------------------------------------------------------------------*/

#define  ESPERA_RX 					0  					//espera el primer cmd de recepcion del verificado 

/*----------------------------------------------------------------------------
tiempo de delay entre funciones
------------------------------------------------------------------------------*/

#define 	TIME_CARD					50		//50


/*----------------------------------------------------------------------------
definicion de datos de trama lintech
------------------------------------------------------------------------------*/

#define 	ETX								03
#define 	STX_LINTECH				0xf2

/*----------------------------------------------------------------------------
msj de lcd tarjeta y lcd serie SALIDA
------------------------------------------------------------------------------*/

#define INGRESE									85		//		0xDF
#define SIN_INGRESO							86		//			0XE6
#define SIN_PAGO								87		//0XE7
#define EXCEDE_GRACIA						88		//0XE8
#define MENSUAL_NO_PARK					89
#define DIRIJASE_CAJA						90
#define GRACIAS									91		//0XFF
#define ACERQUESE_CAJA					0XEB

/*MENSAJES INFORMATIVOS*/

#define ERROR_COD_PARK					170			//0XE5
#define ERROR_LOOP							171					//0XE0
#define TARJETA_INVALIDA				172			//0XE1
#define TARJETA_SIN_FORMATO	    173			//0xDF

/*MENSAJES PRINCIPAL ACTIVA RELES*/

#define AUDIO_ENTRADA			0XA0
#define AUDIO_CAJA				0XA1
#define AUDIO_GRACIAS			0XA2
/*----------------------------------------------------------------------------
definiciones para, el debuger. saber si la trama es enviada, o la trama es de respuesta
------------------------------------------------------------------------------*/

#define 	ENVIADOS					0X0
#define		RESPUESTA					0X01

/*----------------------------------------------------------------------------
definiciones de la tarjeta MF tipo de cliente esto esta en la posicion (0) de la memoria MF
(0) si el dato es cero esta inactiva
(1) activa o ROTACION

------------------------------------------------------------------------------*/

#define INACTIVA					0x00
#define ROTACION 					0x01

/*----------------------------------------------------------------------------
posicion de  MF  bloque 1 sector 1
(0) tipo de tarjeta 
(01) el id del cliente
(03)codigo del parqueadero	
------------------------------------------------------------------------------*/
#define 	MF_TIPO_TARJETA		0X00
#define 	MF_ID_CLIENTE			0x01
#define		MF_COD_PARK				0x03

/*----------------------------------------------------------------------------
posicion de  MF bloque 2 sector 1
(00) donde esta grabado la fecha de entrada (año,mes,dia,hora,minutos) estan en hex  
(0b) donde esta grabado la fecha de salida (año,mes,dia,hora,minutos) estan en hex 
MF_LSB_SDCT=0 SIN DESCUENTO
MF_LSB_FECHA_OUT=1 FIJA LA FECHA DE SALIDA EN LA TARJETA
MF_LSB_POR_CNTJ=2		DESCUENTO POR PORCENTAJE
MF_LSB_POR_DNR=3		DESCUENTO POR DINERO
------------------------------------------------------------------------------*/

#define 	MF_FECHA_INT			0X00				/*año,mes,dia,hora,minutos*/


#define		MF_DCTO						0x05				/*Tipo de descuento (00)sin descuento, (01xx xxxx) 0x40 fija fecha de salida,
																					10xx xxxx dcto por porcentaje xx xxxx= valor del porcentaje, 11xx xxxx dcto por dinero 
																																																											*/
#define		MF_LSB						0x06				/*codigo de descuento que programa el checpoint o el software */

#define   MF_TIPO_VEHICULO	0x08							/*tipo vehiculo 00 carro, 01 moto, 02 bicicleta, 04 tractomula*/

#define		MF_IN_PAGO				0x09						/*xx11xxxx los dos bits en (1) son la parte alta del tiempo de descuento*/
#define 	MF_APB						0x0A						/*antipassback 00 inicializado, 01 IN, 02 OUT, 03 NO USA*/

#define		MF_FECHA_OUT			0X0B				/*año,mes,dia,hora,minutos*/

/*----------------------------------------------------------------------------
define posiciones de memoria EEPROM
------------------------------------------------------------------------------*/
#define	EE_DCTO_PASA_CAJA			0X000F

/*----------------------------------------------------------------------------
Definicion de varaibles globales del objeto
------------------------------------------------------------------------------*/

static unsigned char Estado=INICIA_LINTECH;



/*------------------------------------------------------------------------------
funcion que valida la trama del verificador o transporte lintech
(0) significa que la trama es valida y sigue en el proceso
(1)	falla en la respuesta por pto serie o trama invalida
(2)	no ha recibido la trama del verificador o transporte
(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/

char Trama_Validacion_P_N()
{
	char Trama_Validacion_P_N=2;																										/*no ha llegado inf al pto serie*/
	
			if ((ValTimeOutCom==1)||(buffer_ready==1))
			{
				if (buffer_ready==1)
				{
					buffer_ready=0;
						
				
					if (Buffer_Rta_Lintech[Pos_TipoResp]=='P')
					{
										
						Trama_Validacion_P_N=0;																									/*trama ok*/
							
					}
					else if (Buffer_Rta_Lintech[Pos_TipoResp]=='N')
					{
 						
						Trama_Validacion_P_N=3;																									/*error de trama*/
						
					}
					else
					{
					 	//Debug_txt_Tibbo((unsigned char *) "Respuesta  DESCONOCIDA \r\n");				/*la respuesta es desconocida*/
						Trama_Validacion_P_N=3;		
						
					}
				}
				else
				{
			 							
					Trama_Validacion_P_N=1;																									/*pto serie no responde */
																						
 				}

			}
			
	return Trama_Validacion_P_N;
}

/*------------------------------------------------------------------------------
funcion de error de envios pto serie

variable en uart error_rx=cuenta los numeros de error pto serie y reset de transporte 
se limpia cuando llega la respuesta del transporte

cont_error_rx= cuenta el tiempo de ASK si completa 5 tiempos borra ek ask 

error_rx_pto=0 nos da mas tiempo para esperar la trama

error_rx_pto=1 reenvia la trama al transporte

aSk= significa que llego al pto serie el 06 y el transporte esta ejecutando y no ha contestado necesita tiempo

------------------------------------------------------------------------------*/
unsigned char error_rx_pto(void)
{
static unsigned cont_error_rx=0;
unsigned char error_rx_pto=0;

		
					
			Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
			Debug_chr_Tibbo	(error_rx);	
			Debug_chr_Tibbo	(cont_error_rx);
			Debug_chr_Tibbo	(error_rx_pto);
			Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
					if (aSk==0)
			
			{	
					error_rx++;
				if(error_rx>=10)
				{																																										/*no contesta debe reset el transporte*/
					Atascado=1;																																				/*off el rele de reset del verificador*/		
					Delay_10ms(110);
					error_rx=0;
					cont_error_rx=0;
					error_rx_pto=0;
					Atascado=0;	
					Delay_10ms(110);																																	/*On el rele de reset del verificador*/		
					ValTimeOutCom=TIME_CARD;
				}
				else
				{
					error_rx_pto=1;																																	/*reenvia el cmd y espera la respuesta*/
					ValTimeOutCom=TIME_CARD;
				}
			}
			else
			{
					cont_error_rx++;
				if(cont_error_rx>=3)
				{
					error_rx=0;																			/*paso tiempo de espera y no contesta limpio los reg y reenvio la trama*/
					cont_error_rx=0;
					error_rx_pto=1;
					aSk=0;
					ValTimeOutCom=TIME_CARD;
				}
				else
				{
					error_rx=0;																		/*damos tiempo de espera de la trama del transporte*/
					error_rx_pto=0;
					ValTimeOutCom=TIME_CARD;
				}
			}
				return (error_rx_pto);
}
/*------------------------------------------------------------------------------
Secuencia de los cmd de inicio (reset dispositivo y graba eeprom)

funcion  de los cmd de inicio (reset dispositivo y graba eeprom) retorna un (00) cuando a terminado exitoso
------------------------------------------------------------------------------*/

unsigned char  Secuencia_inicio_expedidor(void)
{
	char temp;
	
	
	switch (Estado)
	{
		case INICIA_LINTECH:

				Atascado=0;	
				Inicializa(SIN_MOVIMIENTO);	   																		//Inicio el transporte sin movimiento
				Estado=SEQ_CAPTURE_DATOS_INI;																			// entra a validar la respuesta del transporte
			  
 			
			return (Estado);
		break;


/*------------------------------------------------------------------------------
Envia la primera trama de inicializacion del transporte sin movimiento
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/
			
		case SEQ_CAPTURE_DATOS_INI:

			if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			Estado=SEQ_CAPTURE_DATOS_INI;																													/*(2)no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "INICIALIZA SIN MOVIMIENTO ERROR\r\n\r\n");					/* trama no valida*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/	
			Estado=INICIA_LINTECH;																																/* (3) Trama invalida cmd (N)*/	
			}
			else
			{
				
				Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
				Estado=INICIA_LINTECH;																															/*(1) no responde el pto*/	
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "INICIALIZA SIN MOVIMIENTO OK\r\n\r\n");						/* trama valida Habilitado */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/
			Estado=GRABA_EEPROM;																																	/*(0) respuesta ok*/
		}			
		return (Estado);
 		break;
			
/*------------------------------------------------------------------------------
Envia el cmd de grabar claves a la eeprom del transporte 
------------------------------------------------------------------------------*/
			
			case GRABA_EEPROM:
				
				Dwload_EEprom();	   																																//envio el cmd de grabar la eeprom
				Estado=SEQ_CAPTURA_OK_EEPROM;																												// entra a validar la respuesta del transporte
			
 			

			return (Estado);
				break;
	
/*------------------------------------------------------------------------------
se analiza la respuesta 
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/
			
		case SEQ_CAPTURA_OK_EEPROM:
		
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			Estado=SEQ_CAPTURA_OK_EEPROM;																													/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "DWLOAP EEPROM ERROR\r\n\r\n");											/* trama no valida respuesta incorrecta falla en la escritura de la clave*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/	
			Estado=INICIA_LINTECH;																																/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				
				
				Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");
				Estado=INICIA_LINTECH;																															/*NO RESPONDE PTO SERIE */
			}				
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "DWLOAP EEPROM OK\r\n\r\n");												/* trama valida Habilitado */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/
			Estado=FIN_OK;																																				/*respuesta ok clave grabada con exito*/
		}			

		return (Estado);
 		break;

/*------------------------------------------------------------------------------
Fin de la secuencia de comandos con exito 
------------------------------------------------------------------------------*/		
			
				case FIN_OK:

				return (Estado);
		break;
			
					
				default:
				return Estado=INICIA_LINTECH;	
				break;
	}
}

/*------------------------------------------------------------------------------
Prosedimiento  q hace paso a paso la secuencia de la MF
contador= numero de ciclos q pasa para mostrar el mensaje de loop solo se muestra el msj cuando contador=0
temp= atributo para uso general
Tiempo= trae el dato de mf tiempo de descuento maximo 999
Estado_Tiempo = arreglo del tiempo a ascii
error_rx= no llega la respuesta en el tiempo solicitado y espera otro tiempo maximo 3
cont_error_rx= envio la trama otra vez solo una vez
------------------------------------------------------------------------------*/

void SecuenciaExpedidor(void)
{
	unsigned char temp;
	unsigned int tiempo;
	unsigned char Estado_Tiempo[4];
	static unsigned char buffer_S1_B0[17];
	static unsigned char buffer_S1_B1[17];
	static unsigned char buffer_S1_B2[17];
	unsigned char clock_temp[6];
	static unsigned char contador=0;
	switch (g_cEstadoComSeqMF)
	{
//***********************************************************************************************
		
		case SEQ_INICIO:

			if (ValTimeOutCom==1)
			{
				
				Atascado=0;																																				/*activo el rele de reset del verificador*/		
				Card_Insercion(Habilita);																													/* se envia el cmd de inicio insercion por boca*/
				g_cEstadoComSeqMF=SEQ_CARD_INSERCION;																							// entra a validar la respuesta del transporte
			  
 			}
 
 		break;

/*------------------------------------------------------------------------------
Analizo la trama de insercion por boca
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/			
		
		case SEQ_CARD_INSERCION:	
			
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_CARD_INSERCION;																									/*no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "CARD_INSERCION ERROR\r\n\r\n");										/* trama no valida respuesta incorrecta falla en la escritura */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																													/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_CARD_INSERCION;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_INICIO;
				}
														
			}				
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "CARD_INSERCION OK\r\n\r\n");												/* trama valida Habilitado */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/*respuesta ok*/
		}			
			
		break;
			
/*------------------------------------------------------------------------------
chequeo el estado de los sensores 
------------------------------------------------------------------------------*/			
		
		case SEQ_CHECK_STATUS:	
					if (ValTimeOutCom==1)
			{				
				Check_Status(S_NORMAL);																																/* envio el cmd de pregunta en q estado estan los sensores*/
				g_cEstadoComSeqMF=SEQ_RTA_CARD_POS;																										// entra a validar la respuesta del transporte
			}
			break;			
				
/*------------------------------------------------------------------------------
Valido la informacion de los sensores
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/		

		case SEQ_RTA_CARD_POS:
			
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_RTA_CARD_POS;																												/*no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_CARD_INSERCION  FALLO\r\n\r\n");											/* trama no valida*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																					/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																													/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_RTA_CARD_POS;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_CHECK_STATUS;
				}
			
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "CHECK SATUS OK\r\n\r\n");																	/* trama valida Habilitado */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																		/*imprimo la trama recibida*/
				if (Buffer_Rta_Lintech[Pos_St0]=='0')																												/* CANAL LIBRE	  no tiene tarjetas en el mecanismo*/
						{
							
							Debug_txt_Tibbo((unsigned char *) "No tiene tarjetas en el mecanismo\r\n\r\n");				/* trama valida Habilitado insercion por boca*/
							ValTimeOutCom=TIME_CARD;
							g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/*volvemos a preguntar	*/					
							if (ValidaSensoresPaso()!=0)																												/*pregunto q alla presencia vehicular*/
							{
								if(contador==0)																																		/*contador si es cero envia el msj a la pantala*/
								{	
									contador++;
									Debug_txt_Tibbo((unsigned char *) "Vehiculo en el loop\r\n\r\n");										/* se encuentra un sensor activo*/
									send_portERR(0xA0);																																	/*audio ingrese tarjeta con rele*/
									PantallaLCD(INGRESE);																															/*msj por raspberry "ingrese su tarjeta"*/
									 									
								}	else
								{
									contador++;																																				/*espera 20 ciclos para poder enviar msj nueva mente*/
									if (contador>= 20)contador=0;
								}	
							}else contador=0;
							
							
						}
						else if	(Buffer_Rta_Lintech[Pos_St0]=='1')	 																						/*  se detecta la tarjeta en la boca TARJETA EN BEZZEL*/
						{
							
							Debug_txt_Tibbo((unsigned char *) "Tarjeta en la boca\r\n\r\n");											/* trama valida Habilitado insercion por boca*/
							g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
							//g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/*volvemos a preguntar	*/			
																	   
						}
						else if (Buffer_Rta_Lintech[Pos_St0]=='2') 																						// TARJETA EN RF
						{
							Debug_txt_Tibbo((unsigned char *) "Tarjeta lista para leer en RF\r\n\r\n");							/* trama valida Habilitado insercion por boca*/
													
							
							if (ValidaSensoresPaso()!=0)																												/*pregunto q alla presencia vehicular*/
							{
							Debug_txt_Tibbo((unsigned char *) "Vehiculo en el loop\r\n\r\n");										/* se encuentra un sensor activo*/
							Aut_Card_check_Status();																														/* se pregunta el tipo de tarjeta si es MF50*/
																																																	/*tarjeta presente prioridad  */
							g_cEstadoComSeqMF=SEQ_REQUEST;																											/*vamos al siguiente paso a con validar el cmd*/		
							}
							else
							{
								Debug_txt_Tibbo((unsigned char *) "Vehiculo no en el loop\r\n\r\n");							/* no hay vehiculo en el loop*/
								send_portERR(0XE0);																																/*envio msj al PRINCIPAL lcd tarjeta*/
								PantallaLCD(ERROR_LOOP);																													/*envio el msj por la pantalla lcd o la raspberry*/
								g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																								/*volvemos a preguntar	*/	
								
							}
																														
						}
																															
		}			
			
		break;
	

/*------------------------------------------------------------------------------
El vehiculo esta ubicado en el loop vengo de preguntar el tipo de tarjeta que esta en el lector
(1 0)	 es una tarjeta valida Mf50	
las demas combinaciones son tomadas como no validas
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N) 		
------------------------------------------------------------------------------*/	

	case SEQ_REQUEST:
		
	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_REQUEST;																														/*no ha respondido*/
			}	
				else if (temp==3)
			{
				
				Debug_txt_Tibbo((unsigned char *) "SEQ_REQUEST FALLO\r\n\r\n");												/* trama no valida*/
				DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																		/*imprimo la trama recibida*/
				g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/* error de la trama */	
																																															/* (3) Trama invalida cmd (N)*/	
			}	
			else
			{
				Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde SEQ REQUEST...\r\n\r\n");			/* pto serie no responde*/
				send_portERR(0XE1);	   																																		/*la tarjeta no es valida no la puede identificar*/
				PantallaLCD(TARJETA_INVALIDA);																														/*envio el msj por la pantalla lcd o la raspberry*/
				g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																											/* capturo la respuesta y regreso a chequear verificador*/	
			
			}				
		}
		else
		{
			 if (Buffer_Rta_Lintech[Card_type_H]=='1')																									/* pregunto si la tarjeta en el transporte es MF 50 */
						  {
								if (Buffer_Rta_Lintech[Card_type_L]=='0')	
							   	{
										Debug_txt_Tibbo((unsigned char *) "Tarjeta valida MF50\r\n\r\n");							/* trama valida son MF50*/
										DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);														/* se muestra la trama*/
										LoadVerify_EEprom();  																												// verifico q la clave este cargada en el transporte
										g_cEstadoComSeqMF=SEQ_MF_LINTECH;																							/* la tarjeta es MF50*/
										
								 }else
								 	{
									Debug_txt_Tibbo((unsigned char *) "Tarjeta invalida no es MF50\r\n\r\n");					/* trama no valida */
									DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);		
									send_portERR(0xA2);																															/*error audio*/	
									send_portERR(0XE1);																															/*envio msj principal tarjeta invalidad*/
									PantallaLCD(TARJETA_INVALIDA);																									/*envio el msj por la pantalla lcd o la raspberry*/
									g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																						/* capturo la respuesta y regreso a chequear verificador*/
							     
								 	}
						 }
							else 
						 	{
							Debug_txt_Tibbo((unsigned char *) "Tarjeta invalida no es MF50\r\n\r\n");						/* trama no valida */
							DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);			
							send_portERR(0xA2);																															/*error audio*/	
							send_portERR(0xe1);	   																									//la tarjeta no es valida 
							PantallaLCD(TARJETA_INVALIDA);																											/*envio el msj por la pantalla lcd o la raspberry*/
							g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																								/* capturo la respuesta y regreso a chequear verificador*/			 
						
						 	}
																																																			/*respuesta ok inicia revisando sensores*/
		}			
					
			break;
/*------------------------------------------------------------------------------
Verifica la eeprom 
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/	
	case SEQ_MF_LINTECH:
		
	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_MF_LINTECH;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "LOAD VERIFY EEPROM ERROR\r\n\r\n");										/* trama no valida respuesta incorrecta falla en la escritura de la clave*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
				Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde PTO SERIE ...\r\n\r\n");				
				g_cEstadoComSeqMF=SEQ_INICIO;																													/* respuesta incorrecta*/
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "LOAD VERIFY EEPROM OK\r\n\r\n");												/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
																																																/*leo MF */
			g_cEstadoComSeqMF=SEQ_RD_S1B1;																														/*respuesta ok inicia clave verificada*/
		}			

	
	break;
/*------------------------------------------------------------------------------
Leo la Mf en el sector 1  bloque 1
		se lee ID_CLIENTE, COD_PARK Y TIPO_TARJETA
------------------------------------------------------------------------------*/		

	case SEQ_RD_S1B1:
		
		RD_MF(1,1);																																									/*leo MF */
		g_cEstadoComSeqMF=SEQ_RD_S1B1_RPTA;	
	
	break;
/*------------------------------------------------------------------------------
Analiso lo leido en Mf en el sector 1  bloque 1
		se lee ID_CLIENTE, COD_PARK Y TIPO_TARJETA
------------------------------------------------------------------------------*/			
	
	case SEQ_RD_S1B1_RPTA:
		
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_RD_S1B1_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B1 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta falla en la escritura de la clave*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
				
					/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B1_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B1;;
				}
																												/* respuesta incorrecta*/
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B1 OK\r\n\r\n");															/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			
			if (Buffer_Rta_Lintech[Pos_Length] >=0x18)
			{
				
					for (temp=0; temp<16; temp++)
					{
						buffer_S1_B1[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];														/*almaceno la informacion de MF en un arreglo*/
					}
					if (((buffer_S1_B1[MF_ID_CLIENTE]==ID_CLIENTE)&&(buffer_S1_B1[MF_COD_PARK]==COD_PARK))||((ID_CLIENTE==0)&&(COD_PARK==0)))		
					{
							
						Debug_txt_Tibbo((unsigned char *) "ID_CLIENTE: ");
						Debug_HexDec(buffer_S1_B1[MF_ID_CLIENTE]);
						Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
						
						Debug_txt_Tibbo((unsigned char *) "COD_PARK:");
						Debug_HexDec(buffer_S1_B1[MF_COD_PARK]);
						Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");


						
						if 	((buffer_S1_B1[MF_TIPO_TARJETA]==ROTACION))					
						{
							Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA 1 ROTACION: ");
							Debug_chr_Tibbo(buffer_S1_B1[MF_TIPO_TARJETA]);
							Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
							
																																								
							g_cEstadoComSeqMF=SEQ_RD_S1B2;
							
						}
						else 
						{
							send_portERR(0xe1);
							PantallaLCD(TARJETA_INVALIDA);																											/*envio el msj por la pantalla lcd o la raspberry*/
							g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																								/* expulso la tarjeta */		
						}
					}
					else
					{
						send_portERR(0XE5);
						PantallaLCD(ERROR_COD_PARK);																												/*envio el msj por la pantalla lcd o la raspberry*/
						g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;																								/* codigo de parqueo erro expulso la tarjeta */		
					}
			}
			else
			{
				send_portERR(0XDF);
				PantallaLCD(TARJETA_SIN_FORMATO);																												/*envio el msj por la pantalla lcd o la raspberry*/
				g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;
			}
			
		}			
	break;
/*------------------------------------------------------------------------------
Leo la Mf en el  sector 1 bloque 2
	Se lee Fecha in, fecha out
------------------------------------------------------------------------------*/		
	case SEQ_RD_S1B2:	
	
	RD_MF(1, 2);																																				/* Leo fecha y hora de ingreso y de salida*/
	g_cEstadoComSeqMF=SEQ_RD_S1B2_RPTA;
	break;	
		
/*------------------------------------------------------------------------------
ANALIZO LO LEIDO  Mf en el  sector 1 bloque 2
	Se lee Fecha in, fecha out
------------------------------------------------------------------------------*/
	case SEQ_RD_S1B2_RPTA:
				
	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_RD_S1B2_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta falla en la escritura de la clave*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
					/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B2_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B2;
				}
																														
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 OK\r\n\r\n");															/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			for (temp=0; temp<16; temp++)
			{
				buffer_S1_B2[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];															/*almaceno la informacion de MF en un arreglo*/
			}
			
			if((buffer_S1_B2[MF_APB]==02)||(buffer_S1_B2[MF_APB]==0) )															/*Pregunto por el antipasban si es 02 ya salio y no posee entrada*/
			{																																																							/*00 inhabilitada la tarjeta*/
			Debug_txt_Tibbo((unsigned char *) "ERROR: SIN INGRESO\r\n\r\n");
			send_portERR(0xA2);																															/*error audio*/	
			send_portERR(0XE6);
			PantallaLCD(SIN_INGRESO);
			g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;
				break;
			}
		/*------------------------------------------------------------------------------	
			sinc0bro 
		------------------------------------------------------------------------------*/	
			if(SIN_COBRO!=0)																																																/*SINCOBRO =0 deshabilitado  =(1) sin cobro = (2) sin cobro salida el mismo dia*/
			{
				if (SIN_COBRO==1)
				{
					Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
																																															
					g_cEstadoComSeqMF=SEQ_RD_S1B0;																																							/*sin cobro*/
					break;
				}
				else if( SIN_COBRO ==2)
				{
					Block_read_Clock_Hex(clock_temp);																																						/*leo el reloj año,mes,dia*/
									
		
					if ((buffer_S1_B2[0]==clock_temp[0])&&(buffer_S1_B2[1]==clock_temp[1])&&(buffer_S1_B2[2]==clock_temp[2]))		/*se analiza la fecha de entrada de la tarjeta MF y la fecha del dia */
					{
						Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
																																																											/* Leo el ticket */
						g_cEstadoComSeqMF=SEQ_RD_S1B0;																																						/*sin cobro el mismo dia*/
						break;
					}																							                                            
						else
						{
						Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n\r\n");																							/*excede el timepo de gracia del dia */
						send_portERR(0XE8);
						PantallaLCD(EXCEDE_GRACIA);																																								/*envio el msj por la pantalla lcd o la raspberry*/
						g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;
						break;							
		 				}
					
					
				}
			}
/*------------------------------------------------------------------------------
		validacion descuentos  se maneja en 2 posiciones de memoria del MF
			MF_LSB= tipo de descuento 			
------------------------------------------------------------------------------*/			
			else if(buffer_S1_B2[MF_LSB]!=0)
			{
				
			 if((rd_eeprom(0xa8,EE_DCTO_PASA_CAJA))!= 0)	
			{
				Debug_Dividir_texto();
				Debug_txt_Tibbo((unsigned char *) "CODIGO DE DCTO: ");																											/*codigo de descuento es elnumero de descuento del cliente*/
				Debug_chr_Tibbo(buffer_S1_B2[MF_LSB]);
				Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");	
				/*parte alta del tiempo libre*/
				temp=(buffer_S1_B2[MF_IN_PAGO]&0x70)>>4	;																																		/*se lee la parte alta del tiempo libre y se rotsa 4 bits a la derecha*/
				tiempo=temp;																																																/*se carga en un entero*/
				tiempo=(tiempo <<8) | buffer_S1_B2[MF_DCTO]	;																																/*MF_DCTO= tiene la parte baja del tiempo libre, se une la parte alta con la baja */
																																																										/*creando un int del tiemo de dcto*/
				Debug_txt_Tibbo((unsigned char *) "TIEMPO DE DCTO: ");
				sprintf( Estado_Tiempo,"%u",tiempo);																																				/*se muestra el tiempo de descuento*/
				Debug_txt_Tibbo((unsigned char *)Estado_Tiempo);
				Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
				Debug_Dividir_texto();	
				
			/*tipo de descuento es el bit 8 de MF_IN_PAGO*/	
			/*por fecha que es fijar fecha de salida*/
					if ((buffer_S1_B2[MF_IN_PAGO]&0x80)==0) 																																	/*tipo de descuento por fecha de salida*/
					{
					Debug_txt_Tibbo((unsigned char *) "DCTO FIJA FECHA DE OUT: ");																						/**/
					analiza_tiempo(buffer_S1_B2,tiempo); 
				/*fijar fecha de salida*/
					}
					else
					{
						if (tiempo== 0x64)																																									/*tipo de dcto por porcentaje al 100 %*/
						{
							Debug_txt_Tibbo((unsigned char *) "DCTO PORCENTAJE 100% ");
							Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
																																																								/* Leo el ticket */
							g_cEstadoComSeqMF=SEQ_RD_S1B0;																																		/**/																							
							break;							
						}
						else
						{
							Debug_txt_Tibbo((unsigned char *) "DCTO NO EL 100%: ");
							send_portERR(ACERQUESE_CAJA);	
							PantallaLCD(DIRIJASE_CAJA);	
							g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;								
						}							
					}
			}
		}
	/*------------------------------------------------------------------------------	
			tiempo de gracia
		T_GRACIA=1 tiene tiempo de gracia 
		------------------------------------------------------------------------------*/	
			else if(T_GRACIA!=0)																																		/*si es diferente de cero tiene tiempo de gracia*/
			{
				Debug_txt_Tibbo((unsigned char *) "Tiempo Gracia: ");
				Debug_chr_Tibbo(T_GRACIA);																														/*imprimo la trama recibida*/	
				Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");		
				analiza_tiempo(buffer_S1_B2,T_GRACIA); 																								// Simula Fecha Max de Salida.
			
			}
		/*------------------------------------------------------------------------------	
		comun para todos
		------------------------------------------------------------------------------*/		
			temp=check_fechaOut(buffer_S1_B2+MF_FECHA_OUT);																					/*se analiza la fecha de salida*/
				if(temp==1)
				{
						Debug_txt_Tibbo((unsigned char *) "TARJETA SIN PAGO\r\n\r\n");
						send_portERR(0xA2);																															/*error audio*/	
						send_portERR(0XE7);
						PantallaLCD(SIN_PAGO);																														/*envio el msj por la pantalla lcd o la raspberry*/
						//RD_MF(1, 0);																																			/**leo el ticket*/	
						g_cEstadoComSeqMF=SEQ_RD_S1B0_EJECT;					
												
				}
				else if (temp==2)
				{
					Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n\r\n");
					send_portERR(0xA2);																																/*error audio*/					
					send_portERR(0XE8);
					PantallaLCD(EXCEDE_GRACIA);																												/*envio el msj por la pantalla lcd o la raspberry*/
					g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;	
				}
				else
				{
					Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n\r\n");
																																														/* Leo el ticket */
					g_cEstadoComSeqMF=SEQ_RD_S1B0;	
				}
			
		}
	
			
			
			
	break;
	case SEQ_RD_S1B0:
			
			RD_MF(1, 0);																																		/* Leo el ticket */
			g_cEstadoComSeqMF=SEQ_RD_S1B0_RPTA;
	break;
/*------------------------------------------------------------------------------
Leo la Mf en el  sector 1 bloque 0
		lee el ticket
------------------------------------------------------------------------------*/		
		
	case SEQ_RD_S1B0_RPTA:
	
	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_RD_S1B0_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B0_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B0;
				}																												/* respuesta incorrecta*/
			}
		}
		else
		{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 OK\r\n\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			
	for (temp=0; temp<16; temp++)
			{
				
				buffer_S1_B0[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];																	/*almaceno la informacion de MF el ticket*/
				if(Buffer_Rta_Lintech[Pos_IniDatMF+temp]==0)
				{
					break;
				}
			}
			Debug_txt_Tibbo((unsigned char *) "Numero Ticket: ");
			Debug_txt_Tibbo(buffer_S1_B0);
			Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
			
			//lock=1;		
			
			Timer_wait=0;																																								/*activo la barrera*/
			Trama_pto_Paralelo_new(buffer_S1_B0,buffer_S1_B2,'s');																			/*transmito pto paralelo*/
			if(USE_LPR)
			{
				Cmd_LPR_Salida(buffer_S1_B0,buffer_S1_B2);																								/*envio datos a Monitor*/
			}
			
			
			
			g_cEstadoComSeqMF=SEQ_WR_S1B2;
		}
	break;

			
/*------------------------------------------------------------------------------
envio a escribir en  la Mf en el  sector 1 bloque 2
		
------------------------------------------------------------------------------*/		
	case SEQ_WR_S1B2:
	
				
				WR_MF(1, 2,buffer_S1_B2+MF_TIPO_VEHICULO);
				g_cEstadoComSeqMF=SEQ_WR_S1B2_RPTA;
			
	break;
/*------------------------------------------------------------------------------
espero la respuesta de la escritura en  la Mf en el  sector 1 bloque 2
		
------------------------------------------------------------------------------*/	
	case SEQ_WR_S1B2_RPTA:
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF= SEQ_WR_S1B2_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_WR_S1B2 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta el escribir bloque*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
				
					/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_WR_S1B2_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_WR_S1B2;
				}					
				
				
			
			}
		}
		else
		{
		
			Debug_txt_Tibbo((unsigned char *) "SEQ_WR_S1B2 OK\r\n\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			g_cEstadoComSeqMF=SEQ_WR_S2B0;	
		}
		break;
	case SEQ_WR_S2B0:
		WR_MF(2, 0,buffer_S1_B2+MF_TIPO_VEHICULO);
	
		g_cEstadoComSeqMF=SEQ_WR_S2B0_RPTA;	
		break;
	
	
	case SEQ_WR_S2B0_RPTA:

	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_WR_S2B0_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
						/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_WR_S2B0_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_WR_S2B0;
				}					
		
						
			}
		}
		else
		{
		
			/*capturo la tarjeta*/
			lock=1;		
			Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 OK\r\n\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			send_portERR(0xA1);																																					/*audio gracias*/	
			send_portERR(0XFF);
			PantallaLCD(GRACIAS); 
	
			Debug_txt_Tibbo((unsigned char *) "ok tarjeta ok...\r\n\r\n");															/* pto serie no responde*/
			
		
						
			g_cEstadoComSeqMF=SEQ_RTA_CAPTURE;
		}
		break;
		
		case SEQ_RTA_CAPTURE:
			
			Mov_Card(MovPos_Capture);	
			g_cEstadoComSeqMF=SEQ_RTA_CAPTURE_RPTA;
			prto_ready=1;																												/*le da prioridad a la respuesta de la captura de tarjeta*/
			break;
/*------------------------------------------------------------------------------
El vehiculo no se encuentra en el loop  ejecto la tarjeta y espero la respuesta
			(0) LA RESPUESTA ES OK
			(1) NO RESPONDE EL PTO SERIE
			(2) NO RECIBIDO LA TRAMA 
			(3) ERROR DE TRAMA CMD (N)
------------------------------------------------------------------------------*/	

	case SEQ_RTA_CAPTURE_RPTA:
		
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
				
					if (ValidaSensores_Out()==1)
				{
					lock=0;
					Debug_txt_Tibbo((unsigned char *) "rele off\r\n\r\n");													/* trama ok la tarjeta fue ejectada*/
				
				}
			
					g_cEstadoComSeqMF=SEQ_RTA_CAPTURE_RPTA;			
				
				
			}	
				else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "NO EJECT TARJET ERROR\r\n\r\n");										/* Tama incorrecta no ejecta la tarjeta*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_INICIO;																													/* (3) Trama invalida cmd (N)*/	
			}	
			else
			{
							/*Dispensador No Responde PTO SERIE ...*/
			
			
		
				
					if(temp=error_rx_pto()==0)
					{
						g_cEstadoComSeqMF=SEQ_RTA_CAPTURE_RPTA;		
					}
					else if(temp=error_rx_pto()==1)
					{
						
					g_cEstadoComSeqMF=SEQ_RTA_CAPTURE;
					}					
		
				
			
			}				
		}
		else
		{
							
			prto_ready=0;
			Debug_txt_Tibbo((unsigned char *) "EJECT TARJET OK\r\n\r\n");													/* trama ok la tarjeta fue ejectada*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);														/*imprimo la trama recibida*/
			
			lock=0;
			ValTimeOutCom=3;		
			g_cEstadoComSeqMF=SEQ_CHECK_STATUS;										//SEQ_INICIO;																											/*respuesta ok inicia revisando sensores*/
																										
		}			
			

 		break;
		


case SEQ_RD_S1B0_EJECT:	
		RD_MF(1, 0);																																			/**leo el ticket*/	
		g_cEstadoComSeqMF=SEQ_RD_S1B0_EJECT_RPTA;	
	break;	
/*------------------------------------------------------------------------------
Leo la Mf en el  sector 1 bloque 0 leo el ticket
LA tarjeta no tiene pago		
	
------------------------------------------------------------------------------*/
	
	
	case SEQ_RD_S1B0_EJECT_RPTA:	
		
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_RD_S1B0_EJECT_RPTA;																														/*no ha respondido*/
			}
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/	
			g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
						/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B0_EJECT_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_RD_S1B0_EJECT;
				}					
																						/* respuesta incorrecta*/
			}
		}
		else
		{
			
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0_EJECT OK\r\n\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			
	for (temp=0; temp<16; temp++)
			{
				
				buffer_S1_B0[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];																	/*almaceno la informacion de MF el ticket*/
				if(Buffer_Rta_Lintech[Pos_IniDatMF+temp]==0)
				{
					break;
				}
			}
			Trama_pto_Paralelo_P(buffer_S1_B0,buffer_S1_B2,'P');		
			g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
		}	
			break;
	
/*------------------------------------------------------------------------------
expulsa la tarjeta por que no pertenece a MF50
------------------------------------------------------------------------------*/		
	
	case SEQ_EXPULSAR_TARJ:
			Card_Insercion(Inhabilita);	
			g_cEstadoComSeqMF=SEQ_EXPULSAR_CHECK;
	break;
	
	
	case SEQ_EXPULSAR_CHECK:
	if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_EXPULSAR_CHECK;																												/*no ha respondido*/
			}	
			else	g_cEstadoComSeqMF=SEQ_INICIO;																													/* respuesta incorrecta*/																									
		}
		else
		{
			g_cEstadoComSeqMF=SEQ_EXPULSAR;																															/*respuesta ok inicia clave verificada*/
		}		
		
		break;
		
	case SEQ_EXPULSAR:
		Mov_Card(MovPos_EjectFront);
	g_cEstadoComSeqMF=SEQ_EXPULSAR_FROM;
	
	case SEQ_EXPULSAR_FROM:                       
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_EXPULSAR_FROM;																												/*no ha respondido*/
			}	
			else	g_cEstadoComSeqMF=SEQ_INICIO;																													/* respuesta incorrecta*/																									
		}
		else
		{
			g_cEstadoComSeqMF=SEQ_INICIO;																																/*respuesta ok inicia clave verificada*/
		}		
		
	break;
	
/*------------------------------------------------------------------------------
expulsa la tarjeta por que no pertenece a MF50
------------------------------------------------------------------------------*/			
		
				default:
				g_cEstadoComSeqMF=SEQ_INICIO;	
				break;	
			
	}	
}


	
