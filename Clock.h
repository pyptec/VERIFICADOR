
/*********************************************
Diseñado Ing. Jaime Pedraza
Marzo 07 de 2019
*********************************************/
/*
@file		#endif	 _Clock_H_ 
@brief	header file for Clock.c
*/

#ifndef _CLOCK_H_
#define _CLOCK_H_

//*********************************************************************************
/*funcion prototipo*/
void p_ck (void);
void eb_clk(char dir_clk);
char lee_clk (unsigned char dir_clk);
void wr_clk (unsigned char dir_clk, unsigned char byte);
unsigned char two_one (unsigned char byte_h,unsigned char byte_l);
void Block_write_clock_ascii(unsigned char *datos_clock);
void Block_read_clock_ascii(unsigned char *datos_clock);
void cond_ini_clock();
char check_fechaOut(char *buffer);
void Block_read_Clock(unsigned char *datos_clock);
void analiza_tiempo(char *buffer,unsigned int Val_DctoMinutos);
void Block_read_Clock_Hex(unsigned char *datos_clock);
void hex_ascii(unsigned char * datos,unsigned char * fecha_asii);
unsigned char check_fechaOut_2(unsigned char *buffer);
void Block_read_Clock_Rasberry(unsigned char *datos_clock);
/*rutinas de conversion*/
void Two_ByteHex_Decimal(unsigned char *buffer,unsigned char id_h,unsigned char id_l);
void ByteHex_Decimal(unsigned char *buffer,unsigned char valorhex);
unsigned char bcd_hex (unsigned char l_data);
unsigned char hex_bcd (unsigned char byte);
unsigned char validar_clk(unsigned char *datos_clock);
void Block_read_clock_ascii_rasberry(unsigned char *datos_clock);

#endif	/* _CLOCK_H_*/