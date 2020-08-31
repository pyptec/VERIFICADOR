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


#endif	/* _VERIFICADOR_H_ */

