/**********************************************
Diseñado por Ing. Jaime Pedraza  Marzo 4 de 2019
**********************************************/
/*
*
@file		delay.c
@brief	delay functions 
*
*/

#include "delay.h"

/*****************************************************************************************
	Function name: Delay_1us
	Input		:	cnt; Delay duration = cnt * 1u seconds
	Output	:	non
	Description
	: A delay function for waiting cnt*1u second.
*****************************************************************************************/
void Delay_20us(unsigned int cnt)
{
	unsigned int i;
	
	for(i = 0; i<cnt; i++) {

		}
}


/*****************************************************************************************
	Function name: Delay_1ms
	Input		:	cnt; Delay duration = cnt * 1m seconds
	Output	:	non
	Description
	: A delay function for waiting cnt*1m second. This function use wait_1us but the wait_1us
		has some error (not accurate). So if you want exact time delay, please use the Timer.
*****************************************************************************************/
void Delay_1ms(unsigned int cnt)
{
	unsigned int i;
	for (i = 0; i < cnt; i++){ Delay_20us(150);}
}

/*****************************************************************************************
	Function name: Delay_10ms
	Input		:	cnt; Delay duration = cnt * 10m seconds
	Output	:	non
	Description
	: A delay function for waiting cnt*10m second. This function use wait_1ms but the wait_1ms
		has some error (not accurate more than wait_1us). So if you want exact time delay, 
		please use the Timer.
*****************************************************************************************/

void Delay_10ms(unsigned int cnt)
{
	unsigned int i;
	for (i = 0; i <cnt; i++) Delay_1ms(10);
}

void Delay (void)  {                   			/* wait function 						*/
  ;                                   			
}

