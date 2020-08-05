/*********************************************
jp
*********************************************/
/*
@file		eeprom.h
@brief	header file for eeprom.c
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_

/*funciones prototipo*/





void ackd();
void ack_lect(void);
void p_me (void);
void e_d_m (unsigned char a_serial_eeprom);
void start (void);
void stop (void);	
unsigned char l_d_m (void);
void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom);
void wrpage_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l,unsigned char data_eeprom); 
unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
void variable_inicio();

#endif	/* _EEPROM_H_ */