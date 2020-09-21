/*********************************************
jp
*********************************************/
/*
@file		Accescan.h
@brief	header file for Accescan.c
*/

#ifndef _ACCESCAN_H_
#define _ACCESCAN_H_

/*funciones prototipo*/
void Valida_Trama_Pto(unsigned char *buffer, unsigned char length_trama);
unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd);
void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama);
void Cmd_Monitor();
void Monitor_chr (unsigned char *str,unsigned char num_char);
void Cmd_LPR_Salida_wiegand(unsigned char *buffer);
void Cmd_LPR_Salida(unsigned char *buffer_S1_B0,unsigned char *buffer_S1_B);

void live();



#endif	/*_ACCESCAN_H_ */