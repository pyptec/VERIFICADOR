/*********************************************
Diseñado Ing. Jaime Pedraza
Marzo 05 de 2019
*********************************************/
/*
@file		prog.h
@brief	header file for delay.c
*/

#ifndef _PROG_H_
#define _PROG_H_


/*funcion prototipo*/
void menu(void);
void Show();
void DisplayCursor(void);
void IngresaDato(unsigned char  *buffer, unsigned char modo);
unsigned char  ValidarClave(unsigned char *buffer);
unsigned char ListaComandos(unsigned char *cmd);
void prog_id_cliente();
void prog_cod_park();
void Prog_tiempo_gracia();
void Prog_Sin_Cobro();
void Prog_debug();
void Prog_Use_Lpr();
void Prog_Comparacion_Activa();
void tipo_pantalla();
unsigned char *hash_id(unsigned char *clave);
void dcto_caja();

#endif	/* _PROG_H_ */