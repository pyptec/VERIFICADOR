/*********************************************
jp
*********************************************/
/*
@file		pto_paralelo.h
@brief	header file for pantallas.c
*/

#ifndef _PANTALLAS_H_
#define _PANTALLAS_H_

/*funciones prototipo*/
void tx_chrlcd (unsigned char data_com, char enable_char_add);
void LCD_txt (unsigned char * msg,char enable_char_add );
void PantallaLCD(unsigned char cod_msg);
void LCD_txt_num_char(unsigned char * msg,unsigned char length_char, unsigned char enable_char_add);
void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char *buffer);
void Raspberry_data (unsigned char * msg);
void tx_chrlcd_2 (unsigned char data_com);
#endif	/*_PANTALLAS_H_ */