#ifndef _VERIFICADOR_H_
#define _VERIFICADOR_H_


/*********************************************
Diseñada por Ing. Jaime Pedraza 
Marzo 01/2019
*********************************************/
/*
@file		verificador.h
@brief	header file for verificador.c
*/


unsigned char  Secuencia_inicio_expedidor(void);
char Trama_Validacion_P_N();
unsigned char error_rx_pto();
unsigned Bloque_Horario(unsigned int addr);
void Armar_Trama_Pto_Paralelo_Expedidor_Mensual(unsigned char *Atributos_Expedidor);
unsigned char Responde_Lectura_Tarjeta_Sector1_Bloque0 (unsigned char *Atributos_Expedidor);
unsigned char Validar_Pago();
unsigned char Valida_Mismo_Dia(unsigned char *Atributos_Expedidor);

#endif	/* _VERIFICADOR_H_ */

