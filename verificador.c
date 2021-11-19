/*
		FUNCIONES PARA DISPENSADOR																*
*/
#include "verificador.h"
#include <reg51.h>
#include <string.h>

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
extern void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B);


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
extern char  *strcpy  (char *s1, const char *s2);
extern char lee_clk (unsigned char dir_clk);
extern char check_fechaOut_2(unsigned char *buffer); 
extern void Unique_Identifier_UID(void);
extern void Power_off(void);
extern void  send_port(unsigned char *buffer_port, unsigned char length_char);
extern unsigned char bcd_hex (unsigned char l_data);

/*funcion prototipo programacion*/
extern unsigned char *Addr_Horarios();

sbit lock = P1^7;						//Relevo 	
sbit Atascado = P0^3;				//Rele de on/off del verificador o transporte
sbit led_err_imp = P0^2;			//Error 
sbit busy = P3^3;  					/*bussy de Entrada Interrupcion del Procesador principal			*/
sbit ready = P3^2;					//Salida. solicitud envio Datos		
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
unsigned char buffer_S_B[17];
extern idata unsigned char placa[];

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
//#define SEQ_TIPO_TARJETAS	0X14
#define SEQ_ROTACION			0X15
#define SEQ_MENSUAL				0X16
#define SEQ_LOAD_PASSWORD	0X17
#define SEQ_UID						0X18
#define SEQ_UID_RPTA				0x19
#define SEQ_POWER_OFF			0X1a
#define SEQ_POWER_OFF_RPTA			0X1B
#define SEQ_POWER_ON			0X1C
#define SEQ_POWER_ON_RPTA		0X1D
#define SEQ_TIPO_CARD			0X1E

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

#define True										0x01
#define False										0x00

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
#define Rtype				        0x0A
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
#define PRMR_TARJETA_VENCIDA					0XEC
#define PRMR_MENSUAL_FUERA_HORARIO		0Xb5
#define PRMR_NO_ROTACION							0XFD

/*MENSAJES INFORMATIVOS*/

#define ERROR_COD_PARK					170			//0XE5
#define ERROR_LOOP							171					//0XE0
#define TARJETA_INVALIDA				172			//0XE1
#define TARJETA_SIN_FORMATO	    173			//0xDF
#define TARJETA_VENCIDA					180
#define MENSUAL_FUERA_HORARIO		181
#define HORARIO_NO_PROG					182
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
definiciones para, funcion horario. 0 esta inhabilitado 1 habilitado
------------------------------------------------------------------------------*/

#define 	HABILITA_ADDR 		15
#define 	Segundo_Tiempo		16
/*
definicion  de daos del reloj
					*/

#define RDIA						0x87
#define RMES						0x89
#define RANO						0x8D
#define RDIA_SEMANA			0x8B
#define RHORA						0x85
#define RMIN						0x83
#define Sabado					7
#define Domingo					1

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

#define		MF_MENSUAL_ANO			0X05
#define		MF_MENSUAL_MES			0X06
#define		MF_MENSUAL_DIA			0X07
#define		MF_UID_0			0X04
#define		MF_UID_1			0X05
#define		MF_UID_2			0X06
#define		MF_UID_3			0X07

#define		MF_EXPIRA_ANO			0X08
#define		MF_EXPIRA_MES			0X09
#define		MF_EXPIRA_DIA			0X0A


#define OPERATE_FAIL					0X6F
/*----------------------------------------------------------------------------
define posiciones de memoria EEPROM
------------------------------------------------------------------------------*/
#define	EE_DCTO_PASA_CAJA			0X000F
#define EE_CPRCN_ACTIVA				0x000C
#define	EE_HABILITA_APB_MENSUAL 0X0015

enum Hora_Minutos_addr{
	Hora_High_addr_Desde = 7, Hora_Low_addr_Desde = 8, Minutos_High_addr_Desde = 9, Minutos_Low_addr_Desde = 10,
	Hora_High_addr_Hasta = 11, Hora_Low_addr_Hasta = 12, Minutos_High_addr_Hasta = 13, Minutos_Low_addr_Hasta = 14
};
/*----------------------------------------------------------------------------
definiciones de la tarjeta MF tipo de cliente esto esta en la posicion (0) de la memoria MF
(0) si el dato es cero esta inactiva
(1) activa o ROTACION
(2) mensualidad
------------------------------------------------------------------------------*/
enum Tipos_MF_TIPO_TARJETA{
	INACTIVA,					
	ROTACION, 					
	MENSUALIDAD,
	PREPAGO,
	CORTESIA,
	LOCATARIO,
	TARJETA_PERDIDA = 0X10,
	INHABILITADA = 0X11
};

enum expedidor {
 fecha_Int_Ano,
 fecha_Int_Mes,	
 fecha_Int_Dia,	
 fecha_Int_Hora,
 fecha_Int_Min,		
 Tipo_Tarjeta,
 Apb,
 Horario,
 Pico_Placa,
 Type_Vehiculo,
 Uid_0,
 Uid_1,
 Uid_2,
 Uid_3,
 Expira_ano,
 Expira_mes,
 Expira_dia
 
};
/*tipos de APB antipassback*/
enum Tipos_MF_APB{
	
	APB_INICIADO,				
	APB_INT,							
	APB_OUT,							
	APB_NO							
};
enum EE_AntiPassBack{
	APB_INHABILITADO_SOFT,
	APB_HABILITADO_SOFT
};
/*comandos pto paralelo*/
enum CMD_Trama_Pto_Paralelo{
	STX=02,
	CMD_PTO_PARALELO_EXPEDIDOR='a',
//	ETX= 03,
	//NULL=0,
	CMD_MONITOR_EXPEDIDOR='E',
	CMD_PTO_PARALELO_EXPEDIDOR_MENSUAL='M'
};
enum Tipos_Vehiculos{
		AUTOMOVIL,					
		MOTO,
		BICICLETA
};
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
unsigned char Horarios(unsigned char Horario)
{
	unsigned char Estado_Horario;
	
	unsigned char Addr_horarios [11];
	unsigned char dia_semana,EE_dia_semana;
	unsigned int addr;
	if (Horario != False )
	{
		/*se Lee la direccion del horario*/
		strcpy (Addr_horarios,(Addr_Horarios()));
		
		addr= Addr_horarios[Horario -1] ;
		
		 /*leemos si esta habilitado*/
		
		if ((rd_eeprom(0xa8,addr + HABILITA_ADDR)) == True)
		{
			/*miramos si el dia de la semana esta habilitado*/
			dia_semana = lee_clk(RDIA_SEMANA);
			Debug_txt_Tibbo((unsigned char *) "DIA DE LA SEMANA: ");
			Debug_chr_Tibbo(dia_semana);
			Debug_txt_Tibbo((unsigned char *) "\r\n");
			
		
			EE_dia_semana = rd_eeprom(0xa8,addr + dia_semana - 1 ) -0x30;
			Debug_txt_Tibbo((unsigned char *) "DIA PROGRAMADO: ");
			Debug_chr_Tibbo(EE_dia_semana);
			Debug_txt_Tibbo((unsigned char *) "\r\n");
		
			if ( EE_dia_semana == dia_semana)
			{
				/*miramos si esta en el rango del horario*/
				Estado_Horario = Bloque_Horario(addr);
			}
			else
			{

				Debug_txt_Tibbo((unsigned char *) "HORARIO DEL DIA NO PROGRAMADO\r\n");
				Estado_Horario= False;
			}
		}
		else 
		{
			
			Estado_Horario= False;
			PantallaLCD(HORARIO_NO_PROG);
			Debug_txt_Tibbo((unsigned char *) "INHABILITADO HORARIO \r\n");
			
		}
		
	}
	else
	{
		
		/*es mensual sin horarios*/
		Estado_Horario = False;				//True;
	
		Debug_txt_Tibbo((unsigned char *) "NO TIENE HORARIO PROGRAMADO\r\n");
	}
	return Estado_Horario;
}
unsigned int Hora_Maxima(unsigned int addr)
{
	unsigned char Hora_High,  Minuto_High;
	unsigned char HoraIni , MinutoIni; 
	unsigned int  Hora_Prog;
	
	 Hora_High 		= (rd_eeprom(0xa8, (addr + Hora_High_addr_Desde )) - 0x30)  << 4;
	 HoraIni 			= Hora_High | ((rd_eeprom(0xa8, (addr + Hora_Low_addr_Desde ))) - 0x30);
	 Minuto_High 	= ((rd_eeprom(0xa8, (addr + Minutos_High_addr_Desde ))) - 0x30)  << 4;
	 MinutoIni	 	=  Minuto_High | ((rd_eeprom(0xa8, (addr +  Minutos_Low_addr_Desde ))) - 0x30);
	 Debug_chr_Tibbo(HoraIni);
	 Debug_chr_Tibbo(MinutoIni);
	 Debug_txt_Tibbo((unsigned char *) "\r\n");
	
	 return Hora_Prog = (HoraIni *60) + (MinutoIni ) ;
}
unsigned char En_Horario(unsigned int HoraNow, unsigned int Hora_Prog,unsigned int addr)
{
	unsigned char Estado_Horario;	
	
	if( Hora_Prog  <=  HoraNow )				//HoraNow >=  Hora_Prog 
	{
		
		/*hasta la hora que puede ingresar el vehiculo */
		Debug_txt_Tibbo((unsigned char *) "HORA PROGRAMADA HASTA: ");
		Hora_Prog = Hora_Maxima(addr+4);

	
		if( HoraNow <= Hora_Prog)
		{
			send_portERR(PRMR_MENSUAL_FUERA_HORARIO);
			Debug_txt_Tibbo((unsigned char *) "EN HORARIO PROGRAMADO\r\n");
			Estado_Horario = True;
		}
		else
		{
			send_portERR(PRMR_MENSUAL_FUERA_HORARIO);
			PantallaLCD(MENSUAL_FUERA_HORARIO);
			Debug_txt_Tibbo((unsigned char *) "DESPUES DEL HORARIO PROGRAMADO\r\n");
			Estado_Horario = False;
		}
		
	}
	else
	{
				send_portERR(PRMR_MENSUAL_FUERA_HORARIO);
				PantallaLCD(MENSUAL_FUERA_HORARIO);
				Debug_txt_Tibbo((unsigned char *) "ANTES DEL HORARIO PROGRAMADO\r\n");
				Estado_Horario = False;
			
	}
	return Estado_Horario;
}

unsigned Bloque_Horario(unsigned int addr)
{
	unsigned char Estado_Horario;	
	unsigned int HoraNow, Hora_Prog;
	
	/*la hora del momento de entrada del vehiculo*/
	
	Debug_txt_Tibbo((unsigned char *) "HORA AHORA: ");
	Debug_chr_Tibbo(lee_clk(RHORA));
	Debug_chr_Tibbo(lee_clk(RMIN));
	Debug_txt_Tibbo((unsigned char *) "\r\n");
	HoraNow = (lee_clk(RHORA) * 60) + (lee_clk(RMIN) );
	
	/* desde la hora en que puede ingresar vehiculo */
	
	
	Debug_txt_Tibbo((unsigned char *) "HORA PROGRAMADA DESDE: ");
	Hora_Prog = Hora_Maxima(addr);
	Estado_Horario=En_Horario(HoraNow,Hora_Prog,addr);
	if(Estado_Horario == False )
	{
		
			
			if(rd_eeprom(0xa8,addr + Segundo_Tiempo ) == True)
			{
				Debug_txt_Tibbo((unsigned char *) "HORA PROGRAMADA SEGUNDA DESDE: ");
				Hora_Prog = Hora_Maxima(addr+10);
				Estado_Horario=En_Horario(HoraNow,Hora_Prog,addr+10);
			}
			else
			{
				send_portERR(PRMR_MENSUAL_FUERA_HORARIO);
				PantallaLCD(MENSUAL_FUERA_HORARIO);
				Estado_Horario = False;
			}
	}

return Estado_Horario;
}
unsigned char  Responde_Lectura_Tarjeta_Sector1_Bloque1 (unsigned char *Atributos_Expedidor)
{
	unsigned char temp;
	unsigned char Estado_expedidor;
	static unsigned char falla=0;
	
	Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B1 OK\r\n");															/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
																
			
			if (Buffer_Rta_Lintech[Pos_Length] >=0x18)
			{
				
					for (temp=0; temp<16; temp++)
					{
						buffer_S_B[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];														/*almaceno la informacion de MF en un arreglo*/
					}
					DebugBufferMF(buffer_S_B,16,RESPUESTA);																/*imprimo la trama recibida*/	
					if (((buffer_S_B[MF_ID_CLIENTE]==ID_CLIENTE)&&(buffer_S_B[MF_COD_PARK]==COD_PARK))||((ID_CLIENTE==0)&&(COD_PARK==0)))		
					{
						Debug_txt_Tibbo((unsigned char *) "ID_CLIENTE: ");
						Debug_HexDec(buffer_S_B[MF_ID_CLIENTE]);
						Debug_txt_Tibbo((unsigned char *) "\r\n");
						
						Debug_txt_Tibbo((unsigned char *) "COD_PARK:");
						Debug_HexDec(buffer_S_B[MF_COD_PARK]);
						Debug_txt_Tibbo((unsigned char *) "\r\n");
						
						Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA: ");
						Debug_chr_Tibbo(buffer_S_B [MF_TIPO_TARJETA]);
						Debug_txt_Tibbo((unsigned char *) "\r\n");
						
						
						*(Atributos_Expedidor + Tipo_Tarjeta) = buffer_S_B [MF_TIPO_TARJETA];
					
						/*fecha de vencimiento de mensual o prepago*/
						*(Atributos_Expedidor + Expira_ano) = buffer_S_B [MF_EXPIRA_ANO];
						*(Atributos_Expedidor + Expira_mes) = buffer_S_B [MF_EXPIRA_MES];
						*(Atributos_Expedidor + Expira_dia) = buffer_S_B [MF_EXPIRA_DIA];
						falla=0;																																	
							Estado_expedidor=SEQ_RD_S1B2;
					
					}
					else
					{
						falla=0;
						send_portERR(0XE5);
						PantallaLCD(ERROR_COD_PARK);																												/*envio el msj por la pantalla lcd o la raspberry*/
						Estado_expedidor=SEQ_EXPULSAR_TARJ;																								/* codigo de parqueo erro expulso la tarjeta */		
					}
			}
			else
			{
				if ((Buffer_Rta_Lintech[Rtype]) == OPERATE_FAIL)
				{
					falla++;
				}
				 if (falla <= 2)
					
				{
					Debug_txt_Tibbo((unsigned char *) "ERROR OPERATE FAIL\r\n");
					Estado_expedidor = SEQ_POWER_ON;
															
				}
				else
				{
					falla=0;
					send_portERR(0XDF);
					PantallaLCD(TARJETA_SIN_FORMATO);																												/*envio el msj por la pantalla lcd o la raspberry*/
					Estado_expedidor=SEQ_EXPULSAR_TARJ;
				}
			}
			return Estado_expedidor;
}			

unsigned char Responde_Lectura_Tarjeta_Sector1_Bloque2 (unsigned char *Atributos_Expedidor)
{
	unsigned char temp;
	unsigned char Estado_expedidor;
	if (Buffer_Rta_Lintech[Pos_Length] >=0x18)
		 {
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 OK\r\n");															/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
											
			for (temp=0; temp<16; temp++)
			{
				buffer_S_B[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];															/*almaceno la informacion de MF en un arreglo*/
			}
			DebugBufferMF(buffer_S_B,16,RESPUESTA);	
			/*fecha entrada*/

			*(Atributos_Expedidor + fecha_Int_Ano) =		buffer_S_B [MF_FECHA_INT] ;
			*(Atributos_Expedidor + fecha_Int_Mes) =		buffer_S_B [MF_FECHA_INT+1] ;
			*(Atributos_Expedidor + fecha_Int_Dia) =		buffer_S_B [MF_FECHA_INT+2] ;
			*(Atributos_Expedidor + fecha_Int_Hora)=		buffer_S_B [MF_FECHA_INT+3] ;
			*(Atributos_Expedidor + fecha_Int_Min) =		buffer_S_B [MF_FECHA_INT+4] ;
			/*horario*/
			Debug_txt_Tibbo((unsigned char *) "HORARIO:");
			Debug_chr_Tibbo((buffer_S_B [MF_TIPO_VEHICULO] & 0XF0) >> 4);
			*(Atributos_Expedidor + Horario)= ((buffer_S_B [MF_TIPO_VEHICULO] & 0XF0) >> 4);
				Debug_txt_Tibbo((unsigned char *) "\r\n");
			/*pico y placa*/
			*(Atributos_Expedidor + Pico_Placa)= ((buffer_S_B [MF_IN_PAGO] & 0XF0) >> 4);			
			/*APB*/
			*(Atributos_Expedidor + Apb)=		buffer_S_B [MF_APB] ;
			/*tipo de tarjeta*/
			*(Atributos_Expedidor + Type_Vehiculo	) = buffer_S_B [MF_TIPO_VEHICULO]& 0x0f;
						
			 /*------------------------------------------------------------------------------	
				tipo de tarjeta
				------------------------------------------------------------------------------*/

				if 	(*(Atributos_Expedidor + Tipo_Tarjeta) ==ROTACION)					
				{
						if((*(Atributos_Expedidor + Apb) == 02)||(*(Atributos_Expedidor + Apb) == 0) )															/*Pregunto por el antipasban si es 02 ya salio y no posee entrada*/
						{
							Debug_txt_Tibbo((unsigned char *) "ERROR: SIN INGRESO\r\n");
							send_portERR(0xA2);																															/*error audio*/	
							send_portERR(0XE6);
							PantallaLCD(SIN_INGRESO);
							Estado_expedidor=SEQ_EXPULSAR_TARJ;
						}
						else
						{
							Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA ROTACION\r\n");
							Estado_expedidor=SEQ_ROTACION;
						
						}
	
				}
				else if (*(Atributos_Expedidor + Tipo_Tarjeta)== MENSUALIDAD)	
				{
					if(	(rd_eeprom(0xa8,EE_HABILITA_APB_MENSUAL) == APB_HABILITADO_SOFT) )
					{
						Debug_txt_Tibbo((unsigned char *) "ANTIPASSBACK SOFTWARE HABILITADO MENSUAL \r\n");
						if((*(Atributos_Expedidor + Apb) == APB_OUT)||(*(Atributos_Expedidor + Apb) == APB_INICIADO) ) 
						{
							Debug_txt_Tibbo((unsigned char *) "ERROR: SIN INGRESO MENSUAL\r\n");
							//send_portERR(0xA2);																															/*error audio*/	
							send_portERR(0XE6);
							PantallaLCD(SIN_INGRESO);
							Estado_expedidor=SEQ_EXPULSAR_TARJ;
						
						}
						else
						{
							Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA MENSUALIDAD\r\n ");
							Estado_expedidor=SEQ_MENSUAL;
						}
					}
					else
					{
						Debug_txt_Tibbo((unsigned char *) "ANTIPASSBACK SOFTWARE INHABILITADO MENSUAL \r\n");
						Debug_txt_Tibbo((unsigned char *) "TIPO DE TARJETA MENSUALIDAD\r\n ");
						Estado_expedidor = SEQ_MENSUAL;
					}
				}
				else if(*(Atributos_Expedidor + Tipo_Tarjeta)== TARJETA_PERDIDA)	
				{
					Debug_txt_Tibbo((unsigned char *) "TIPO NUEVO DE TARJETA SIN DEFINIR\r\n");
				  send_portERR(PRMR_NO_ROTACION);
					Estado_expedidor=SEQ_ROTACION;
					//Estado_expedidor=SEQ_EXPULSAR_TARJ;
				  
				}
			 
		}
	else
		 {
			 g_cEstadoComSeqMF=SEQ_RD_S1B2;
		 }
		 
		 return Estado_expedidor;
	
}
unsigned char Responde_Lectura_Tarjeta_Sector1_Bloque0 (unsigned char *Atributos_Expedidor)
{
	unsigned char temp;
	unsigned char Estado_expedidor;
	static unsigned char buffer_S1_B0[17];
	if (Buffer_Rta_Lintech[Pos_Length] >=0x18)
		 {
	Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 OK\r\n");															/* trama OK CARGA numero de ticket*/
																		
			
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
			Debug_txt_Tibbo((unsigned char *) "\r\n");
			
		
			/*transmito pto paralelo*/	
			if (*(Atributos_Expedidor + Tipo_Tarjeta)!= MENSUALIDAD)	
			{
			Timer_wait=0;																																								/*activo la barrera*/
			Trama_pto_Paralelo_new(buffer_S1_B0,buffer_S_B,'s');
			}
			else
			{
			
			Armar_Trama_Pto_Paralelo_Expedidor_Mensual(Atributos_Expedidor);
			}
			if(USE_LPR)
			{
				
				/*envio datos a Monitor*/
				if (*(Atributos_Expedidor + Tipo_Tarjeta)!= MENSUALIDAD)	
				{
				
				Cmd_LPR_Salida(Atributos_Expedidor,buffer_S1_B0);	
				}
				else
				{
				Cmd_LPR_Salida(Atributos_Expedidor,buffer_S1_B0);	
				}
			}
			
			
			
			Estado_expedidor=SEQ_WR_S1B2;
			
		}else
		 {
			Estado_expedidor= SEQ_RD_S1B0;
		 }
		 return Estado_expedidor;
}

void Armar_Trama_Pto_Paralelo_Expedidor_Mensual(unsigned char *Atributos_Expedidor)
{
	static unsigned char buffer[28];
	//unsigned char j;
	/*la trama esta compuesta de
	STX,CMD,-,NoTICKET,-,FECHAINT,-,placa,ETX*/

	buffer[0]=STX;
	buffer[1]=CMD_PTO_PARALELO_EXPEDIDOR_MENSUAL;
	buffer[2]=*(Atributos_Expedidor + Uid_0);
	buffer[3]=*(Atributos_Expedidor + Uid_1);
	buffer[4]=*(Atributos_Expedidor + Uid_2);
	buffer[5]=*(Atributos_Expedidor + Uid_3);
	
	//leo la fecha de entrada
	//Block_read_Clock_Hex(buffer+6);					
	
	buffer[6]=*(Atributos_Expedidor + fecha_Int_Ano)+0x030;							/*año de entrada*/
	buffer[7]=*(Atributos_Expedidor + fecha_Int_Mes)+0x030;					/*mes de entrada*/
	buffer[8]=*(Atributos_Expedidor + fecha_Int_Dia)+0x030;					/*dia de entrada*/
	buffer[9]=*(Atributos_Expedidor + fecha_Int_Hora)+0x030;					/*hora de entrada*/
	buffer[10]=*(Atributos_Expedidor + fecha_Int_Min)+0x030;					/*minutos de entrada*/

	/*placa*/
	if (rd_eeprom(0xa8,EE_CPRCN_ACTIVA)!=0)
	{		
		
			buffer[11]= placa[0];
			buffer[12]= placa[1];
			buffer[13]= placa[2];
			buffer[14]= placa[3];
			buffer[15]= placa[4];
			buffer[16]= placa[5];

	}

	if(*(Atributos_Expedidor + Type_Vehiculo) == AUTOMOVIL )
	{
		buffer[17]= 'C';
	}
	else
	{
		buffer[17]= 'M';
	}
	
	buffer[18]= ETX;
	buffer[19]= 0;
	ready=0;
			while(busy==0);
		send_port(buffer,19);													/*trama transmitida pto paralelo*/
		
			Debug_Dividir_texto();																							/*division del texto */
			Debug_txt_Tibbo((unsigned char *) "Datos enviados pto paral=");				
			DebugBufferMF(buffer,19,0);		
			Debug_Dividir_texto();
}
unsigned char Analiza_Uid_Card(unsigned char *Atributos_Expedidor )
{
	unsigned char Estado_expedidor;
	unsigned char temp;
			if (Buffer_Rta_Lintech[Pos_Length] >= 0x0f)
			{
				
					for (temp=0; temp<16; ++temp)
					{
						buffer_S_B[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];														/*almaceno la informacion de MF en un arreglo*/
					 
					}
					
					Debug_txt_Tibbo((unsigned char *) "buffer_UID\r\n");
					DebugBufferMF(buffer_S_B,16,RESPUESTA);
					
	
					Debug_txt_Tibbo((unsigned char *) "UID_CARD :");	
					Debug_chr_Tibbo((buffer_S_B [ MF_UID_0]));	
					Debug_chr_Tibbo((buffer_S_B [ MF_UID_1]));	
					Debug_chr_Tibbo((buffer_S_B [ MF_UID_2]));	
					Debug_chr_Tibbo((buffer_S_B [ MF_UID_3]));
					Debug_txt_Tibbo((unsigned char *) "\r\n");
					
				*(Atributos_Expedidor + Uid_0) = buffer_S_B [MF_UID_0];		
				*(Atributos_Expedidor + Uid_1) = buffer_S_B [MF_UID_1];			
				*(Atributos_Expedidor + Uid_2) = buffer_S_B [MF_UID_2];			
				*(Atributos_Expedidor + Uid_3) = buffer_S_B [MF_UID_3];	
				Estado_expedidor=SEQ_LOAD_PASSWORD;																												//SEQ_TIPO_CARD;
			}		
			else
			{
				Estado_expedidor=SEQ_POWER_OFF;
			}	
			return Estado_expedidor;
}

unsigned char Validar_Pago()
{
	unsigned char Estado_expedidor;
	unsigned char temp;
		temp=check_fechaOut(buffer_S_B+MF_FECHA_OUT);																					/*se analiza la fecha de salida*/
				if(temp==1)
				{
						Debug_txt_Tibbo((unsigned char *) "TARJETA SIN PAGO\r\n");
						send_portERR(0xA2);																															/*error audio*/	
						send_portERR(0XE7);
						PantallaLCD(SIN_PAGO);																														/*envio el msj por la pantalla lcd o la raspberry*/
																																							/**leo el ticket*/	
						Estado_expedidor=SEQ_RD_S1B0_EJECT;					
												
				}
				else if (temp==2)
				{
					Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n\r\n");
					send_portERR(0xA2);																																/*error audio*/					
					send_portERR(0XE8);
					PantallaLCD(EXCEDE_GRACIA);																												/*envio el msj por la pantalla lcd o la raspberry*/
					Estado_expedidor=SEQ_EXPULSAR_TARJ;	
				}
				else
				{
					Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n");
																																														/* Leo el ticket */
					Estado_expedidor=SEQ_RD_S1B0;	
				}
		return 	Estado_expedidor;
}	
unsigned char Valida_Mismo_Dia(unsigned char *Atributos_Expedidor)
{
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned char Valida_Mismo_Dia;
	/*miramos si sale el mismo dia*/
	Valida_Mismo_Dia = False;
	year	= bcd_hex(lee_clk(RANO));
	month = bcd_hex( lee_clk(RMES));
	day = bcd_hex(lee_clk(RDIA));
	
		if (((year == *(Atributos_Expedidor + fecha_Int_Ano)) && (month == *(Atributos_Expedidor + fecha_Int_Mes)) && (day == *(Atributos_Expedidor + fecha_Int_Dia))))
		{
			Valida_Mismo_Dia = True;
		}
	
			return Valida_Mismo_Dia ;
}
void SecuenciaExpedidor(void)
{
	unsigned char temp;
	unsigned int tiempo;
	unsigned char Estado_Tiempo[4];
	static unsigned char Atributos_Expedidor[18];
	static unsigned char buffer_S1_B0[17];
	unsigned char clock_temp[6];
	static unsigned char contador=0;
	unsigned char *atributos;
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_CARD_INSERCION  FALLO\r\n");											/* trama no valida*/
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
			Debug_txt_Tibbo((unsigned char *) "CHECK STATUS OK\r\n");																	/* trama valida Habilitado */
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																		/*imprimo la trama recibida*/
				if (Buffer_Rta_Lintech[Pos_St0]=='0')																												/* CANAL LIBRE	  no tiene tarjetas en el mecanismo*/
						{
							
							Debug_txt_Tibbo((unsigned char *) "No tiene tarjetas en el mecanismo\r\n");				/* trama valida Habilitado insercion por boca*/
							ValTimeOutCom=TIME_CARD;
							g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/*volvemos a preguntar	*/					
							if (ValidaSensoresPaso()!=0)																												/*pregunto q alla presencia vehicular*/
							{
								if(contador==0)																																		/*contador si es cero envia el msj a la pantala*/
								{	
									contador++;
									Debug_txt_Tibbo((unsigned char *) "Vehiculo en el loop\r\n");										/* se encuentra un sensor activo*/
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
							
							Debug_txt_Tibbo((unsigned char *) "Tarjeta en la boca\r\n");											/* trama valida Habilitado insercion por boca*/
							g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
							//g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/*volvemos a preguntar	*/			
																	   
						}
						else if (Buffer_Rta_Lintech[Pos_St0]=='2') 																						// TARJETA EN RF
						{
							Debug_txt_Tibbo((unsigned char *) "Tarjeta lista para leer en RF\r\n");							/* trama valida Habilitado insercion por boca*/
													
							
							if (ValidaSensoresPaso()!=0)																												/*pregunto q alla presencia vehicular*/
							{
							Debug_txt_Tibbo((unsigned char *) "Vehiculo en el loop\r\n");										/* se encuentra un sensor activo*/
							//Aut_Card_check_Status();																														/* se pregunta el tipo de tarjeta si es MF50*/
																																																	/*tarjeta presente prioridad  */
							g_cEstadoComSeqMF=SEQ_TIPO_CARD;//SEQ_REQUEST;																											/*vamos al siguiente paso a con validar el cmd*/		
							}
							else
							{
								Debug_txt_Tibbo((unsigned char *) "Vehiculo no en el loop\r\n");							/* no hay vehiculo en el loop*/
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
				
				Debug_txt_Tibbo((unsigned char *) "SEQ_REQUEST FALLO\r\n");												/* trama no valida*/
				DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																		/*imprimo la trama recibida*/
				g_cEstadoComSeqMF=SEQ_CHECK_STATUS;																										/* error de la trama */	
																																															/* (3) Trama invalida cmd (N)*/	
			}	
			else
			{
				Debug_txt_Tibbo((unsigned char *) "Dispensador No Responde SEQ REQUEST...\r\n");			/* pto serie no responde*/
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
																																																	// verifico q la clave este cargada en el transporte
										g_cEstadoComSeqMF=SEQ_UID;	//SEQ_MF_LINTECH;																							/* la tarjeta es MF50*/
									}
									else if (Buffer_Rta_Lintech[Card_type_L]=='1')	
									{
										/*tarjeta MF70*/
										Debug_txt_Tibbo((unsigned char *) "Tarjeta valida MF70\n");		
										DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);														/* se muestra la trama*/
										g_cEstadoComSeqMF=SEQ_UID;	//SEQ_MF_LINTECH;											
									}
									else
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
			Debug_txt_Tibbo((unsigned char *) "LOAD VERIFY EEPROM OK\r\n");												/* trama OK CARGA LA EEPROM Y LA VERIFICA*/
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
			g_cEstadoComSeqMF = Responde_Lectura_Tarjeta_Sector1_Bloque1(Atributos_Expedidor);
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B2 ERROR\r\n");														/* trama no valida respuesta incorrecta falla en la escritura de la clave*/
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
		 g_cEstadoComSeqMF = Responde_Lectura_Tarjeta_Sector1_Bloque2 (Atributos_Expedidor);
	
		}
		break;
	case SEQ_ROTACION:
		
			
			/*------------------------------------------------------------------------------	
			sinc0bro  por  programacion
		------------------------------------------------------------------------------*/	
			if(SIN_COBRO!=0)																																																/*SINCOBRO =0 deshabilitado  =(1) sin cobro = (2) sin cobro salida el mismo dia*/
			{
				if (SIN_COBRO==1)
				{
					Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n");
																																															
					g_cEstadoComSeqMF=SEQ_RD_S1B0;																																							/*sin cobro*/
					break;
				}
				else if( SIN_COBRO ==2)
				{
					Block_read_Clock_Hex(clock_temp);																																						/*leo el reloj año,mes,dia*/
									
		
					if ((buffer_S_B[0]==clock_temp[0])&&(buffer_S_B[1]==clock_temp[1])&&(buffer_S_B[2]==clock_temp[2]))		/*se analiza la fecha de entrada de la tarjeta MF y la fecha del dia */
					{
						Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n");
																																																											/* Leo el ticket */
						g_cEstadoComSeqMF=SEQ_RD_S1B0;																																						/*sin cobro el mismo dia*/
						break;
					}																							                                            
						else
						{
						Debug_txt_Tibbo((unsigned char *) "Excede T.GRACIA\r\n");																							/*excede el timepo de gracia del dia */
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
			else if(buffer_S_B[MF_LSB]!=0)
			{
				
			 if((rd_eeprom(0xa8,EE_DCTO_PASA_CAJA))!= 0)	
			{
				Debug_Dividir_texto();
				Debug_txt_Tibbo((unsigned char *) "CODIGO DE DCTO: ");																											/*codigo de descuento es elnumero de descuento del cliente*/
				Debug_chr_Tibbo(buffer_S_B[MF_LSB]);
				Debug_txt_Tibbo((unsigned char *) "\r\n");	
				/*parte alta del tiempo libre*/
				temp=(buffer_S_B[MF_IN_PAGO]&0x70)>>4	;																																		/*se lee la parte alta del tiempo libre y se rotsa 4 bits a la derecha*/
				tiempo=temp;																																																/*se carga en un entero*/
				tiempo=(tiempo <<8) | buffer_S_B[MF_DCTO]	;																																/*MF_DCTO= tiene la parte baja del tiempo libre, se une la parte alta con la baja */
																																																										/*creando un int del tiemo de dcto*/
				Debug_txt_Tibbo((unsigned char *) "TIEMPO DE DCTO: ");
				sprintf( Estado_Tiempo,"%u",tiempo);																																				/*se muestra el tiempo de descuento*/
				Debug_txt_Tibbo((unsigned char *)Estado_Tiempo);
				Debug_txt_Tibbo((unsigned char *) "\r\n");
				Debug_Dividir_texto();	
				
			/*tipo de descuento es el bit 8 de MF_IN_PAGO*/	
			/*por fecha que es fijar fecha de salida*/
					if ((buffer_S_B[MF_IN_PAGO]&0x80)==0) 																																	/*tipo de descuento por fecha de salida*/
					{
					Debug_txt_Tibbo((unsigned char *) "DCTO FIJA FECHA DE OUT: ");																						/**/
					analiza_tiempo(buffer_S_B,tiempo); 
				/*fijar fecha de salida*/
					}
					else
					{
						if (tiempo== 0x64)																																									/*tipo de dcto por porcentaje al 100 %*/
						{
							Debug_txt_Tibbo((unsigned char *) "DCTO PORCENTAJE 100% ");
							Debug_txt_Tibbo((unsigned char *) "Salida Autorizada\r\n");
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
				Debug_txt_Tibbo((unsigned char *) "\r\n");		
				analiza_tiempo(buffer_S_B,T_GRACIA); 																								// Simula Fecha Max de Salida.
			
			}
		/*------------------------------------------------------------------------------	
		comun para todos
		------------------------------------------------------------------------------*/		
			g_cEstadoComSeqMF=Validar_Pago();
			
			
	break;
	case SEQ_MENSUAL:
		
	/*valida si esta programdo el horario y si esta en el rango*/
	if (Horarios(Atributos_Expedidor [Horario]) == True)
	{
		/*cheque la fecha de expiracion del mensual*/
		atributos = &Atributos_Expedidor [Expira_ano];//;buffer_S1_B1[MF_EXPIRA_ANO];
		
		if ( check_fechaOut_2(atributos) == True )
			{
					/*valida que la salida sea el mismo dia*/
				if(	Valida_Mismo_Dia(Atributos_Expedidor) == True)
				{
					Debug_txt_Tibbo((unsigned char *) "MENSUAL AL DIA\r\n");	
					g_cEstadoComSeqMF=SEQ_RD_S1B0;
				}
				else
				{
				Debug_txt_Tibbo((unsigned char *) "SALIDA NO ES EL MISMO DIA \r\n");	
				g_cEstadoComSeqMF = Validar_Pago();
				}	
				
			}
				else 
				{
				send_portERR(PRMR_TARJETA_VENCIDA);	
						
				PantallaLCD(TARJETA_VENCIDA);
				Debug_txt_Tibbo((unsigned char *) "MENSUAL EXPIRA\r\n");
				g_cEstadoComSeqMF = SEQ_EXPULSAR_TARJ;
				
				}
	}
	else
		{
			if(Horarios(Atributos_Expedidor [Horario])==0)
			{
				/*cheque la fecha de expiracion del mensual*/
				atributos = &Atributos_Expedidor [Expira_ano];//;buffer_S1_B1[MF_EXPIRA_ANO];
				if ( check_fechaOut_2(atributos) == True )
				{
				Debug_txt_Tibbo((unsigned char *) "MENSUAL AL DIA HORARIO 0\r\n");	
				g_cEstadoComSeqMF=SEQ_RD_S1B0;
				}
				else
				{
				send_portERR(PRMR_TARJETA_VENCIDA);	
				PantallaLCD(TARJETA_VENCIDA);
				Debug_txt_Tibbo((unsigned char *) "MENSUAL EXPIRA\r\n");
				g_cEstadoComSeqMF = SEQ_EXPULSAR_TARJ;
				}
		}
		else
			{
			g_cEstadoComSeqMF = SEQ_EXPULSAR_TARJ;
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
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
			g_cEstadoComSeqMF=Responde_Lectura_Tarjeta_Sector1_Bloque0 (Atributos_Expedidor);
	
		}
	break;

			
/*------------------------------------------------------------------------------
envio a escribir en  la Mf en el  sector 1 bloque 2
		
------------------------------------------------------------------------------*/		
	case SEQ_WR_S1B2:
	
				
				WR_MF(1, 2,Atributos_Expedidor);//buffer_S_B+MF_TIPO_VEHICULO
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
			//g_cEstadoComSeqMF=SEQ_WR_S2B0;	
				/*capturo la tarjeta*/
			lock=1;		
			Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 OK\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			send_portERR(0xA1);																																					/*audio gracias*/	
			send_portERR(0XFF);
			PantallaLCD(GRACIAS); 
	
			Debug_txt_Tibbo((unsigned char *) "ok tarjeta ok...\r\n");															/* pto serie no responde*/
			
		if (Atributos_Expedidor [ Tipo_Tarjeta]!= MENSUALIDAD)	
			{
						
			g_cEstadoComSeqMF=SEQ_RTA_CAPTURE;
			}
			else
			{
				g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
			}
		}
		break;
	case SEQ_WR_S2B0:
		WR_MF(2, 0,buffer_S_B+MF_TIPO_VEHICULO);//s1b2
	
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 ERROR\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_RTA_S2B0 OK\r\n");															/* trama OK CARGA numero de ticket*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			send_portERR(0xA1);																																					/*audio gracias*/	
			send_portERR(0XFF);
			PantallaLCD(GRACIAS); 
	
			Debug_txt_Tibbo((unsigned char *) "ok tarjeta ok...\r\n");															/* pto serie no responde*/
			
		if (Atributos_Expedidor [ Tipo_Tarjeta]!= MENSUALIDAD)	
			{
						
			g_cEstadoComSeqMF=SEQ_RTA_CAPTURE;
			}
			else
			{
				g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
			}
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
					Debug_txt_Tibbo((unsigned char *) "rele off\r\n");													/* trama ok la tarjeta fue ejectada*/
				
				}
			
					g_cEstadoComSeqMF=SEQ_RTA_CAPTURE_RPTA;			
				
				
			}	
				else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "NO EJECT TARJET ERROR\r\n");										/* Tama incorrecta no ejecta la tarjeta*/
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
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0 ERROR\r\n");														/* trama no valida respuesta incorrecta el leer el bloque*/
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
			
			Debug_txt_Tibbo((unsigned char *) "SEQ_RD_S1B0_EJECT OK\r\n");															/* trama OK CARGA numero de ticket*/
			//DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,RESPUESTA);																/*imprimo la trama recibida*/		
			
	for (temp=0; temp<16; temp++)
			{
				
				buffer_S1_B0[temp]=Buffer_Rta_Lintech[Pos_IniDatMF+temp];																	/*almaceno la informacion de MF el ticket*/
				if(Buffer_Rta_Lintech[Pos_IniDatMF+temp]==0)
				{
					break;
				}
			}
			DebugBufferMF(buffer_S1_B0,16,RESPUESTA);	
			Trama_pto_Paralelo_P(buffer_S1_B0,buffer_S_B,'P');		
			g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;		
		}	
			break;
	case SEQ_UID:
		Unique_Identifier_UID();
	g_cEstadoComSeqMF = SEQ_UID_RPTA;
	break;
	case SEQ_UID_RPTA:
			if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_UID_RPTA;																												/*no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_UID_RPTA FALLO\r\n");											/* trama no valida*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																					/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_UID;																													/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_UID_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_UID;
				}
			
			}
		}
		else
		{
			g_cEstadoComSeqMF = Analiza_Uid_Card(Atributos_Expedidor );
		}
		break;
	case SEQ_LOAD_PASSWORD:
		LoadVerify_EEprom();
		g_cEstadoComSeqMF=SEQ_MF_LINTECH;		
		break;
	case SEQ_POWER_OFF:
		Power_off();
		g_cEstadoComSeqMF=SEQ_POWER_OFF_RPTA;
		break;
	case SEQ_POWER_OFF_RPTA:
		if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_POWER_OFF_RPTA;																												/*no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_POWER_OFF_RPTA  FALLO\r\n");											/* trama no valida*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																					/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_POWER_OFF;																													/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_POWER_OFF_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_POWER_OFF;
				}
			
			}
		}
		else
		{
			g_cEstadoComSeqMF=SEQ_UID;
		}
		break;
	case SEQ_POWER_ON:
		Power_off();
	g_cEstadoComSeqMF = SEQ_POWER_ON_RPTA;
		break;
	case SEQ_POWER_ON_RPTA:
			if((temp=Trama_Validacion_P_N())!=0)
		{
			if(temp==2)
			{
			g_cEstadoComSeqMF=SEQ_POWER_ON_RPTA;																												/*no ha respondido*/
			}	
			else if (temp==3)
			{
			Debug_txt_Tibbo((unsigned char *) "SEQ_POWER_ON_RPTA  FALLO\r\n");											/* trama no valida*/
			DebugBufferMF(Buffer_Rta_Lintech,g_cContByteRx,1);																					/*imprimo la trama recibida*/
			g_cEstadoComSeqMF=SEQ_POWER_ON;																													/* (3) Trama invalida cmd (N)*/	
			}			
			else
			{
				
				/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_POWER_ON_RPTA;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_POWER_ON;
				}
			
			}
		}
		else
		{
			g_cEstadoComSeqMF=SEQ_LOAD_PASSWORD;
		}
		break;
	case SEQ_TIPO_CARD:
		Aut_Card_check_Status();
		g_cEstadoComSeqMF=SEQ_REQUEST;						
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
			else if (temp==3)
			{
				
					g_cEstadoComSeqMF=SEQ_INICIO;																															/* (3) Trama invalida cmd (N)*/	
			}				
			else
			{	
						/*Dispensador No Responde PTO SERIE ...*/
		
				if(temp=error_rx_pto()==0)
				{
					g_cEstadoComSeqMF=SEQ_EXPULSAR_CHECK;
				}
				else if(temp=error_rx_pto()==1)
				{
					g_cEstadoComSeqMF=SEQ_EXPULSAR_TARJ;
				}					
																					
			}
																																					
		}
		else
		{
			//lock=0;
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
			lock=0;
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


	
