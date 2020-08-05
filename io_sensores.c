
#include <io_sensores.h>
#include <reg51.h>

/*funciones externas*/
extern void Debug_txt_Tibbo(unsigned char * str);
extern void load_and_send_info_reloj();
extern void load_and_send_id_cod();
extern void send_portERR(unsigned char cod_err);
extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 

sbit DataIn = P1^1;					//			
sbit sel_A = P3^5;					//Pulsador												*
sbit sel_B = P3^6;					//Entrada Sensor 2										*
sbit sel_C = P3^7;					//Entrada Sensor 1										*
sbit busy = P3^3;  					/*bussy de Entrada Interrupcion del Procesador principal			*/

#define NO_TIFIQUE_EVP			'N'
#define SI_NOTIFIQUE_EVP		'S'

#define AUTOMOVIL						0X00
#define MOTO								0X01

#define EE_CPRCN_ACTIVA				0x000C
extern bit COMPARACION_ACTIVA;
extern  unsigned char Tipo_Vehiculo;

//***************************************************************************************************
void sel_Funcion(void)
{
	sel_A=0;
	sel_B=0;
	sel_C=0;
}
//***************************************************************************************************
void sel_Auto(void)
{
	sel_A=0;
	sel_B=1;
	sel_C=0;
}
//***************************************************************************************************
/*
void sel_Pulsa(void)
{
	sel_A=1;
	sel_B=1;
	sel_C=0;
}
*/
//***************************************************************************************************
void sel_Sensor2(void)
{
	sel_A=0;
	sel_B=0;
	sel_C=1;
}
//***************************************************************************************************
void sel_Sensor1(void)
{
	sel_A=1;
	sel_B=0;
	sel_C=1;
}
//***************************************************************************************************
void sel_Dir1(void)
{
	sel_A=0;
	sel_B=1;
	sel_C=1;
}
//***************************************************************************************************
void sel_Dir2(void)
{
	sel_A=1;
	sel_B=1;
	sel_C=1;
}

char ValidaSensor(void)
{
	
char	Valido=0;
int	Tmin=500;
	while ((DataIn==1)&&(Valido==0))
	{
		Tmin--;
		if (Tmin==0)
		{
			 Valido=1;
		}
	}
	return Valido;
}
/*------------------------------------------------------------------------------
Funcion q debuelve la direccion de la tarjeta
------------------------------------------------------------------------------*/
unsigned char Dir_board()
{
	char Board=0x31;
	
	sel_Dir1();
	if (DataIn==1)
	{
	 	Board++;
	}
	sel_Dir2();
	if (DataIn==1)
	{
		Board=Board+2;
	}
	return (Board);
}
/*------------------------------------------------------------------------------
Detecto la activacion de los sensores de salida
si retorna 0xff el sensor activo
si retorno 0x00 loop no activo
01 saliendo
------------------------------------------------------------------------------*/

unsigned char  ValidaSensoresPaso(void)
{
 
		sel_Sensor2();																//garantiso q la barrera se encuentre en posicion baja	
   		if ((DataIn==1))				
		{  
			if (ValidaSensor()==1)
			{
			sel_Sensor1();	
				if (DataIn==0)														// sensor1  se encuentra activo puede ser carro, si esta inhabilitado debe ser moto					
				{  sel_Auto();
					if (DataIn==0)
					{					  															// sensor Auto activo es un carro 
					   Debug_txt_Tibbo((unsigned char *) "Sensores Auto y sensor1 activos.");
					   Tipo_Vehiculo=AUTOMOVIL;
							return 0xff;
						
					}
					else
						{
							Debug_txt_Tibbo((unsigned char *) "Sensor activo sensor1 .");
							Tipo_Vehiculo=MOTO;
							return 0xff ;
						}
						
				   	
				}
				else
				{ 	sel_Auto();
					if (DataIn==0)
					{
					
					Debug_txt_Tibbo((unsigned char *) "Detectado. Sensor Auto.");
						Tipo_Vehiculo=MOTO;
						return 0xff;	
					}
					else
					{
						Debug_txt_Tibbo((unsigned char *) "Sensores no detectados");
						return 0x00;	
					}
						

				}
			}
			else 
			{
				Debug_txt_Tibbo((unsigned char *) "Vehiculo Esperando Salir.");
				return 0x00;
			}
		}else 
		{
		Debug_txt_Tibbo((unsigned char *) "Vehiculo Saliendo. Un momento");
		return 0x01;
		}

}

unsigned char  ValidaSensores_Out(void)
{
char	valido=0;
	
		sel_Sensor2();																//garantiso q la barrera se encuentre en posicion baja	
   		if ((DataIn==1))				
			{  
				if (ValidaSensor()==1)
				{
					valido=0;
					return valido;
				}
				else
				{
					valido=1;
					return valido;
				}
			}
			else
			{
				valido=1;
				return valido;
			}
}
/*------------------------------------------------------------------------------
funcion de msj en lcd de informacion
toggle=0		envia por el pto paralelo STX, cmd (i), año, mes, dia, hora, minutos, seg, dia de la semana, ETX
toggle=1		
toggle=2		envia ID y COD_PARK   por el pto paralelo STX, cmd (D), ID_CLIENTE, COD_CLIENTE, ETX  
------------------------------------------------------------------------------*/
 void msj_lcd_informativo()
 {
 unsigned char info=0;
 static unsigned char toggle=0;
   		sel_Funcion();																	/*el pulsador funcion es el cmd q visualiza la informacion tres */
		if (DataIn==0)
		{
 		   if ((toggle==0)&&(info==0))										/*primer pulso de funcion*/
			{
			 if (busy==1)																		/*habilito transmicion pto paralelo*/
				{
					info=1;																			/*incremento info para ser limpiado cuando se suelte el pulsador*/
					load_and_send_info_reloj();									/*se envia el reloj al lcd */
					toggle=1;																		/*incremento a toggle para el proximo pulso sea otra opcion*/
				}
			 }		
				else if((toggle==1)&&(info==0))
				{
					if (busy==1)
					{
					info=1;
					load_and_send_id_cod();										/*muestro el codigo e id del cliente configurado*/
					toggle=2;
					}
				}
				else if ((toggle==2)&&(info==0))
				{
					if (busy==1)
					{
					info=1;
						if(rd_eeprom(0xa8,EE_CPRCN_ACTIVA)==1)		/*se muestra si hay comparacion activa si o no*/
						{
							send_portERR(SI_NOTIFIQUE_EVP);
						}else {send_portERR(NO_TIFIQUE_EVP);}
						
					toggle=0;
					}
				
				}
			
		  }
		
	  sel_Funcion();   								// el antirebote
		if (DataIn==1)
		{
			info=0;
		}

 }