/*********************************************
jp
*********************************************/
/*
@file		io_sensores.h
@brief	header file for io_sensores.c
*/

#ifndef _IO_SENSORES_H_
#define _IO_SENSORES_H_

/*funciones prototipo*/

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
unsigned char  ValidaSensores_Out(void);

#endif	/* _IO_SENSORES_H_ */