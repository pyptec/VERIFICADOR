#include <reg51.h>
#include <stdio.h>
#include "uart.h"
#include <INTRINS.H>
#include <math.h> 
#include <string.h>
#include <stdlib.h>

//*******************************************************************************************
//	DEFINICION DE IO DEL MICROCONTROLADOR													*
//*******************************************************************************************
											
sbit audio_s2 = P0^1;				//


sbit Atascado = P0^3;				//Rele de on/off del verificador o transporte

sbit led_err_imp = P0^2;			//Error 												*

sbit audio_s0 = P0^4;				//
sbit audio_s1 = P0^5;				//
sbit rx_in_data = P0^6;				//Indicador de Rx Transporte o Lectura Wiegand			*

sbit lock = P1^7;						//Relevo 												*
sbit sel_com = P0^7;				//Micro switch											*

/*pines de ip tibbo*/

sbit rx_ip = P0^0;					//		
sbit txd2 = P1^0;					//Transmision Aux Datos	IP														


/*io sensores */

sbit DataIn = P1^1;					//	dato de las entradas		
sbit sel_A = P3^5;					//Pulsador												*
sbit sel_B = P3^6;					//Entrada Sensor 2										*
sbit sel_C = P3^7;					//Entrada Sensor 1										*

/*pines del pto paralelo*/

sbit port_clk = P3^4;				//Recepcion AUX											*
sbit busy = P3^3;  					//Entrada Interrupcion del Procesador principal			*
sbit ready = P3^2;					//Salida. solicitud envio Datos							*

/*definiciones tamaño del buffer 
------------------------------------------------------------------------------*/
#define 	TBUF_SIZE   				50           
#define 	RBUF_SIZE   				50
#define 	TBUF_SIZE_LINTECH   50 
/*------------------------------------------------------------------------------*
definiciones de los estados de recepcion de datos pto serie
------------------------------------------------------------------------------*/
#define  	ESPERA_RX 				0  		//espera el primer cmd de recepcion del verificado 
#define  	ESPERA_INICIO_RTA 1			// se almacena el stx
#define 	LEN_DATA					2
#define 	STORE_DATA				3

/*tiempo de delay entre funciones
------------------------------------------------------------------------------*/
#define 	TIME_CARD					5			//50
#define 	TIME_EJECT				5			//60
#define 	RET_MINIMO				3

/*------------------------------------------------------------------------------*
definiciones de los estados del verificador o expedidor
------------------------------------------------------------------------------*/
/*


*/
#define SEQ_INICIO				0X00	
#define INICIA_LINTECH		0x30

/* definicion de variables globales*/
 
 idata unsigned char tbuf [TBUF_SIZE];														/* Buffer de transmicion de datos serie*/								
 idata unsigned char rbuf [RBUF_SIZE];
 unsigned char xdata Buffer_Rta_Lintech[TBUF_SIZE_LINTECH];

 unsigned char ValTimeOutCom=10;																	// define el tiempo de espera del timer 
 unsigned char Timer_wait=0;
 unsigned char g_cEstadoComSoft=ESPERA_RX;												// estado del buffer de recepcion de datos
 unsigned char g_cContByteRx=0;																		// contador de datos de recepcion serial
 unsigned char g_cEstadoComSeqMF=SEQ_INICIO;
 //unsigned char Back_g_cEstadoComSeqMF=0;
 unsigned char Tipo_Vehiculo=0;																		/*tipo de vehiculo*/
 unsigned int  T_GRACIA;																				/*tiempo de gracia del parqueo*/
 unsigned int  SIN_COBRO;																				/*SIN_COBRO=0 inhabilitado =(1) sin cobro = (2) sin cobro y salida el mismo dia*/
 unsigned char  Debug_Tibbo;

 unsigned char  COMPARACION_ACTIVA;

 unsigned char 	Raspberry;																		/*variable que define los msj por lcd (0) o rasberry (1)*/
unsigned int Timer_tivo=0;
/* definicion de variables globales de configuracion del parqueadero*/

int ID_CLIENTE;						
int COD_PARK;

/*constantes globales*/
const unsigned  char ACK= 06	;
const unsigned  char ETX= 03	;
const unsigned  char STX_LINTECH= 0xf2	;
bit buffer_ready=0;
bit prto_ready=0;
/*definicion de bit*/



/*librerias*/
#include "delay.h"
#include "lintech.h"
#include "verificador.h"
#include "tibbo.h"
#include "clock.h"
#include <eeprom.h>
#include <io_sensores.h>
#include <pto_paralelo.h>

#include <Accescan.h>
#include <pantallas.h>
#include <prog.h>

/*Definicion de funciones prototipo*/
void off_Timer0_Interrup(void);

extern size_t strlen  (const char *);
