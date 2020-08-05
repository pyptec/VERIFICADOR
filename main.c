/*------------------------------------------------------------------------------
MAIN.C:  sistema verificador

Diseñado Ing. Jaime Pedraza
------------------------------------------------------------------------------*/
#include <main.h>
extern bit aSk;
/*------------------------------------------------------------------------------
interrupcion por timer 
ValTimeOutCom se decrementa cada overflow de la interrupcion
Timer_wait		Incrementa cada overflow de la interrrupcion
------------------------------------------------------------------------------*/
 timer0_int() interrupt 1 using 2
    {
			if(ValTimeOutCom==1)
			{
			ValTimeOutCom=1;
			TF0=0;
			}
			else
			{
			ValTimeOutCom--;
			Timer_wait++;
			Timer_tivo++;
			TF0=0;
			}
		}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void ini_timer(void)
{

	TMOD=(TMOD & 0xF0) | 0x01;// configuro el reg TMOD sin precalador a 16 bits
		
 	TH0=0X00;								//registro de la parte alta del timer0 de 16 bits									*						
	TL0=0X00;								// registro de la parte baja del timer0
	TR0=1;									// habilita el timer0 reg TCOM bit 4 para interrup

}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void on_Timer0_Interrup(void)
{
	TF0=0;									//registro TCON bit 5 bandera de overflow 
	ET0=1;									// enable interrupcion bit 1 del registro IENO
	
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void off_Timer0_Interrup(void)
{
	ET0=0;									// enable interrupcion bit 1 del registro IENO
	
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
_getkey waits until a character is received from the serial port.  This may not
be the exact desired operation (for example if the buffer is empty, this
function hangs waiting for a character to be received).
------------------------------------------------------------------------------*/
char _getkey (void)
{
int k;

do
  {
  k = com_getchar ();
  }
while (k == -1);

return ((unsigned char) k);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
char putchar (char c)
{
volatile unsigned int i;

while (com_putchar (c) != 0)
  {
  for (i=0; i<1000; i++)
    {
    /*** DO NOTHING ***/
    }
  }

return (c);
}
/*------------------------------------------------------------------------------
escribir una cadena de datos por pto serie
------------------------------------------------------------------------------*/
void EscribirCadenaSoft_buffer(unsigned char *buffer,unsigned char tamano_cadena)
{
	unsigned char i,d;
		for(i=0;i<tamano_cadena;i++)
   	{
      d=putchar(*buffer);
			buffer++;
    }
}
/*
unsigned char recibe_cmd_LPR(unsigned char *buffer_cmd)
{
	 unsigned char j, MaxChrRx;
	 unsigned int contador;
	
	 bit time_out;
	 
	 MaxChrRx=11;

	
			for (j=0; j<MaxChrRx; j++)
			{
				contador=0;
				time_out=0;
				while ((rx_ip==1)&&(time_out==0))
				{
					contador++;
					if (contador>65000)
					{
						time_out=1;
						
					}				
				}
				if(time_out==1)break;
					
	 				*buffer_cmd=rx_Data();
						buffer_cmd++;
			}

			*buffer_cmd=0;
			return j;
}
/*------------------------------------------------------------------------------
Rutina de las condiciones iniciales del board
------------------------------------------------------------------------------*/
void inicia_board(void)
{
cond_ini_tibbo();							/* inicia tibbo*/
cond_ini_clock();							/* inicia clock*/
cond_ini_pto();								/*Inicializacion del pto*/
Atascado=0;										/*prendo el verificador*/
sel_com=1;										/*seleccionar el pto serie */
lock=0;												/*barrera off*/	
	
}


/*------------------------------------------------------------------------------
Note that the two function above, _getkey and putchar, replace the library
functions of the same name.  These functions use the interrupt-driven serial
I/O routines in SIO.C.
------------------------------------------------------------------------------*/


void main (void)
{
	
	static unsigned char buffer[40];
	//unsigned char buffer_clock[]={0x02,0x31,0x48,0x32,0x32,0x2f,0x30,0x33,0x2f,0X32,0X30,0X31,0X39,0x20,0x31,0x31,0x3a,0x33,0x39,0x3a,0x30,0x30,0x20,0x34,0x03,0}; //dia,mes,año,hora,minutos,SEGUNDOS,Dia de la semana
	//unsigned char S1_B2[]={0x13, 0x03, 0x1D, 0x0B, 0x0E, 00, 00, 00, 00, 00, 0x01, 0x13, 0x03, 0x1D, 0x0E, 0x1D};
	//unsigned char S1_B0[]={0x32, 0x31, 0x30, 0x37, 0x31, 0x35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01};

		char Length_trama;
ini_timer();																															/* initialize interrupt timer0 */
on_Timer0_Interrup();																											/* habilita el  timer0 */
com_initialize ();              																					/* initialize pto serie */
com_baudrate ();            																							/* setup for 9600 baud */
inicia_board();																														/*Rutina de las condiciones iniciales del board*/
EA = 1;                         																					/* Enable Interrupts global*/

time_bit();																																/*solo para pruebas*/
Delay_20us(33);
time_mbit();
tx_aux('a');	
	
sel_Funcion();																															/*el pulsador funcion es el cmd q da la entrada a programacion */
	
	if (DataIn==0)							
	{
	menu();																																		/*menu de configuracion*/
	}
	variable_inicio();																												/*leo y cargo  las variables de inicio de configuracion*/
while(Secuencia_inicio_expedidor());																				/* procedimiento de inicio de transporte (reset, y grabar eeprom)*/
	ValTimeOutCom=3;
																							
	while (1) 																																/* Loop Principal								*/	
	{   
	 
			if (prto_ready==0)																										/*testigo de prioridad cuando esta en (1) solo para capturar 
																																								tarjeta espera la respuesta*/
			{			
				if (rx_ip==0)																													/*pregunto si llega datos de monitor pto serie emulado*/
				{
				
				Length_trama=recibe_cmd_Monitor(buffer);														/*recibe la trama en buffer y saco la longitud de la trama*/
				//Debug_monitor(buffer,Length_trama);
																													
				}
				
						SecuenciaExpedidor();
			
				if (busy==0)																													/*comunicacion con el pto paralelo*/
				{
			
				
				Length_trama=recibe_port(buffer);																		/*recibe informacion del pto paralelo*/
																						/*divido el texto*/
		
				Valida_Trama_Pto(buffer,Length_trama);																/*valido la informacion recibida */
				 
						/*solo de prueba*/		
				Debug_pto_paralelo(buffer,Length_trama);
				
						
				}
					
			}
			else
			{
				SecuenciaExpedidor();
			}				
			msj_lcd_informativo();																									/*muestra la informacion de  ID cliente, cod parque, fecha,comparacion*/
	

		
	}
}
