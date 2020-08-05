/*********************************************
Diseñado Ing. Jaime Pedraza
Marzo 05 de 2019
*********************************************/
/*
@file		tibbo.h
@brief	header file for delay.c
*/

#ifndef _TIBBO_H_
#define _TIBBO_H_
/*pines del micro*/

/*funcion prototipo*/

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
void Debug_pto_paralelo(unsigned char *buffer, unsigned char Length_trama );
void Debug_monitor(unsigned char *buffer, unsigned char Length_trama );

#endif	/* _TIBBO_H_ */