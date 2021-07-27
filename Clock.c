
#include <Clock.h>
#include <reg51.h>

/*funcion prototipo*/
extern void Delay (void);
extern void Delay_20us(unsigned int cnt);
extern void Debug_Fecha_actual(unsigned char *buffer);
extern float pow   (float x, float y);
/*variables externas */
extern unsigned char  Debug_Tibbo;

sbit sck = P1^4 ;   							/* define I/O functions 		*/
sbit io  = P1^5 ;
sbit rst = P1^6 ;
/*CMD DE ESCRITURA RELOJ*/
#define WDIA						0x86
#define WMES						0x88
#define WANO						0x8C
#define WHORA						0x84
#define WMIN						0x82
#define WSEG						0x80
#define WDIA_SEMANA			0x8A
/*CMD DE LECTURA DEL RELOJ*/
#define RDIA						0x87
#define RMES						0x89
#define RANO						0x8D
#define RHORA						0x85
#define RMIN						0x83
#define RSEG						0x81
#define RDIA_SEMANA			0x8B

unsigned char data_clk;
//*********************************************************************************

//*********************************************************************************
void p_ck (void)
{
 	  sck=1;
	  Delay();
	  sck=0;
}
//*********************************************************************************
void eb_clk(char dir_clk) {
	int nbits;
	for (nbits=0; nbits<8; nbits++)
	{
	  dir_clk >>=1;
	  if (CY==1)
	  { 
	    io=1;
	  }
	  else
	  {
	    io=0;
	  }
	  p_ck();
	}
}
//*********************************************************************************
char lee_clk (unsigned char dir_clk) {            
int	nbits;
	rst=1;
	Delay_20us(3);
	eb_clk(dir_clk);
	for (nbits=0; nbits<8; nbits++)
	{
	  if (io==1) 
	  {
		data_clk = data_clk | 0x80;
		if (nbits<=6) {
		 data_clk >>=1;
		 }
	  }
	  if (io==0)
	  {
	  	data_clk = data_clk & 0x7F;
		if (nbits<=6) 
		 {
		  data_clk >>=1;
		 }
 	  }
	  p_ck();
   	} 
	rst=0;
 	return data_clk;
}
//*********************************************************************************
void wr_clk (unsigned char dir_clk, unsigned char byte) {            
	rst=1;
	Delay_20us(3);
	eb_clk(dir_clk);
	dir_clk=byte;
	eb_clk(dir_clk);
 	rst=0;
 	
 }
//****************************************************************************
 unsigned char two_one (unsigned char byte_h,unsigned char byte_l)
{
	unsigned char byte_out;
	byte_h=byte_h&0x0f;
	byte_h<<=4;
	byte_l=byte_l&0x0f;
	byte_out=byte_h|byte_l;
	return byte_out;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
unsigned char validar_clk(unsigned char *datos_clock)
{
	unsigned char validador=0;
	
	
		if ((*datos_clock >= '0') &&(*datos_clock <= '9'))																/*valida el dia parte alta */
		{
		
			 if ((*(datos_clock+1) >= '0') &&(*(datos_clock+1) <= '9'))											/*valida el dia parte baja*/
			{
				
				 if ((*(datos_clock+3) >= '0') &&(*(datos_clock+3) <= '9'))							/*valida el mes parte alta*/
				{
				  if ((*(datos_clock+4) >= '0') &&(*(datos_clock+4) <= '9'))						/*valida el mes parte baja */
					{
					 if ((*(datos_clock+8) >= '0') &&(*(datos_clock+8) <= '9'))					/*año baja*/			
						{
						 if ((*(datos_clock+0x0b) >= '0')&&(*(datos_clock+0x0b) <= '9'))					/*hora alta*/	
							{
						  if ((*(datos_clock+0x0c) >= '0') &&(*(datos_clock+0x0c) <= '9'))					/*hora baja*/	
								{
							 if ((*(datos_clock+0x0e) >= '0') &&(*(datos_clock+0x0e) <= '9'))				/*minutos alta*/
									{
							  if ((*(datos_clock+0x0f) >= '0') &&(*(datos_clock+0x0f) <= '9'))				/*minutos baja*/
									{
								 if ((*(datos_clock+0x11) >= '0') &&(*(datos_clock+0x11) <= '9'))				/*segundos alta*/
											{
									if ((*(datos_clock+0x12) >= '0') &&(*(datos_clock+0x12) <= '9'))				/*segundos baja*/
										{
										if ((*(datos_clock+0x14) >= '0') &&(*(datos_clock+0x14) <= '9'))				/*dia de la semana*/
										 {
														validador=0;
										 }
												}
												else
												{
													validador =1;
												}
											}
											else
											{
												validador =1;
										
											}
										}
										else
										{
											validador =1;
									
										}
								
									}
									else
									{
										validador =1;
									
									}
								}
								else
								{
									validador =1;
									//break;
								}
							}
							else
							{
								validador =1;
								
							}								
						}
						else
						{
							validador =1;
							
						}
					}
					
				}
				else
				{
					validador =1;
					
				}
			
			}
			
			else
			{
				validador =1;
			
			}
		
		}
		
		else 
		{
			validador =1;
			
		}
	
	return validador;
	
}
/*------------------------------------------------------------------------------
Escribo el reloj en ascii en bloque 

------------------------------------------------------------------------------*/

void Block_write_clock_ascii(unsigned char *datos_clock)	

{			
//dia,mes,año,hora,minutos,segundos,Dia de la semana 
		
			/*dia*/
			wr_clk(WDIA,two_one(*datos_clock,*(datos_clock+1)));			 							//dia
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*mes*/
			wr_clk(WMES,two_one(*datos_clock,*(datos_clock+1)));			 							//mes
			datos_clock++;
			datos_clock++;
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*año*/
			wr_clk(WANO,two_one(*datos_clock,*(datos_clock+1)));			 							//año
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*hora*/
			wr_clk(WHORA,two_one(*datos_clock,*(datos_clock+1)));			 							//hora
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*minutos*/
			wr_clk(WMIN,two_one(*datos_clock,*(datos_clock+1)));			 							//minutos
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*segundos*/
			wr_clk(WSEG,two_one(*datos_clock,*(datos_clock+1)));			 							//segundos
			datos_clock++;
			datos_clock++;
			datos_clock++;
			/*dia de la semana*/
			wr_clk(WDIA_SEMANA,(*datos_clock&0x0f));			 													//dia de la semana
	
}
/*------------------------------------------------------------------------------
rutinas de conversiones  de bcd a hex
------------------------------------------------------------------------------*/
unsigned char bcd_hex (unsigned char l_data)
 {
 	unsigned char temp,j;
	temp=l_data;
	temp>>=4;
	temp=temp & 0x0f;
	if (temp!=0x00)
	{
		l_data=l_data & 0x0f;
	 	for (j=0;j<temp;j++)
	 	{
	  	 	l_data=l_data+0x0a;
	 	}	
	}
	return l_data;
 }
 /*------------------------------------------------------------------------------
 Rutina que convierte de Hex a bcd 
------------------------------------------------------------------------------*/
 unsigned char hex_bcd (unsigned char byte)
{
  unsigned char nibble_h; 
	unsigned char nibble_l;
	unsigned char k,byte_out;

	nibble_h=0x00;
	nibble_l=0x00;

	for (k=0;k<byte;k++)
	{
		nibble_l=nibble_l+0x01;
		if (nibble_l==0x0a)
		{
			nibble_l=0x00;
			nibble_h=nibble_h+0x01;
		}
	 }
	 nibble_h<<=4;
	 nibble_h=nibble_h & 0xf0;
	 nibble_l=nibble_l & 0x0f;
	 byte_out=(nibble_h | nibble_l);
 	 return byte_out;
	
}
/*------------------------------------------------------------------------------
	CONVIERTE DE 1BYTE HEXADECIMAL A DECIMAL 											
valorhex= pasa el byte en Hex y lo convierte en decimal
buffer=   se debuelve la conversion en un arreglo en ascii
------------------------------------------------------------------------------*/
void ByteHex_Decimal(unsigned char *buffer,unsigned char valorhex)
{
	unsigned char numero, centena, decena;

	centena=0;
	decena=0;
	numero=valorhex;

	while (numero>=0x064) 													/* resto centena en ascii100*/
	{
		numero=numero-0x64;
		centena=centena+1;
	}
		*buffer=centena|0x30;
		buffer++;
	while (numero>=0x0a)														/* resto 10 decena en ascii*/
	{
		numero=numero-0x0a;
		decena=decena+1;
	}
	*buffer=decena|0x30;
	buffer++;
	*buffer=numero|0x30;														/*unidad en ascii */
	

}
/*------------------------------------------------------------------------------
	CONVIERTE DE 2BYTE HEXADECIMAL A DECIMAL 											

------------------------------------------------------------------------------*/
void Two_ByteHex_Decimal(unsigned char *buffer,unsigned char id_h,unsigned char id_l)
{
	unsigned int valor,numero;
	unsigned char temp,dmil, mil, centena, decena;

	valor=0;
	dmil=0;
	mil=0;
	centena=0;
	decena=0;
	
	 
	temp=id_h;
	temp=temp&0xf0;
	temp>>=4;

	valor=valor+(pow(16,3))*temp;
	temp=id_h;
	temp=temp&0x0f;
	valor=valor+(pow(16,2))*temp;

	temp=id_l;
	temp=temp&0xf0;
	temp>>=4;
	valor=valor+(pow(16,1))*temp;
	temp=id_l;
	temp=temp&0x0f;
	valor=valor+(pow(16,0))*temp;
	numero=valor;

	while (numero>=0x2710)				// resto 10.000 
	{
		numero=numero-0x2710;
		dmil=dmil+1;
	}
	*buffer=dmil|0x30;
		buffer++;
	while (numero>=0x03e8) 				// resto 1.000
	{
		numero=numero-0x03e8;
		mil=mil+1;
	}
	*buffer=mil|0x30;
		buffer++;
	while (numero>=0x064) 				// resto 100
	{
		numero=numero-0x64;
		centena=centena+1;
	}
	*buffer=centena|0x30;
		buffer++;
	while (numero>=0x0a)				// resto 10
	{
		numero=numero-0x0a;
		decena=decena+1;
	}
	*buffer=decena|0x30;
	buffer++;
	*buffer=numero|0x30;
	buffer++;
	*buffer=00;
	
}
/*------------------------------------------------------------------------------
 Lee el dato del reloj en bloque 
------------------------------------------------------------------------------*/
void Block_read_clock_ascii(unsigned char *datos_clock)
{
	unsigned char dato;

	//dia,mes,año,hora,minutos,segundos,Dia de la semana
		
		/*año*/
		dato=lee_clk(RANO);
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
	
	/*mes*/
		dato=lee_clk(RMES);
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;		

		/*dia*/
	  dato=lee_clk(RDIA);												//;bcd_hex(lee_clk(DIA));
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
		
		
		/*hora*/						
		dato=lee_clk(RHORA);
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;				
		
		/*minutos*/					
		dato=lee_clk(RMIN);
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;	
	
		/*segundos*/		
		dato=lee_clk(RSEG);
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
	
		/*dia de la semana*/
		dato=lee_clk(RDIA_SEMANA);
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
		*datos_clock=0;
								
}	
void Block_read_clock_ascii_rasberry(unsigned char *datos_clock)
{
	unsigned char dato;

	//dia,mes,año,hora,minutos,segundos,Dia de la semana
	/*mes*/
		dato=(lee_clk(RMES));
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;		

		/*dia*/
	  dato=(lee_clk(RDIA));												//;bcd_hex(lee_clk(DIA));
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
		
	
		/*año*/
		dato=(lee_clk(RANO));
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;
	
	
		
		/*hora*/						
		dato=(lee_clk(RHORA));
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;				
		
		/*minutos*/		
		dato=(lee_clk(RMIN));		
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		datos_clock++;	
		*datos_clock='.';
		datos_clock++;			
		
			/*segundos*/		
		dato=(lee_clk(RSEG));		
		*datos_clock=((dato&0xf0)>>4)| 0x30;			/*dato parte alta*/
	  datos_clock++;
		*datos_clock=(dato&0x0f)| 0x30;						/*dato parte bajo*/
		
		datos_clock++;	
		*datos_clock=0;
								
}	
void Block_read_Clock_Hex(unsigned char *datos_clock)
{
	
	/*año*/
	*datos_clock=bcd_hex(lee_clk(RANO));			
	 datos_clock++;
	/*mes*/
	*datos_clock=bcd_hex(lee_clk(RMES));			
	 datos_clock++;
	/*Dia*/
	*datos_clock=bcd_hex(lee_clk(RDIA));			
	 datos_clock++;
	/*hora*/	
	*datos_clock=bcd_hex(lee_clk(RHORA));			
	 datos_clock++;
	/*minutos*/	
	*datos_clock=bcd_hex(lee_clk(RMIN));			
	 datos_clock++;

}
/*
void Block_read_Clock_Hex_bcd(unsigned char *datos_clock)
{
		
	/*año*/
	/**datos_clock=hex_bcd(lee_clk(RANO));			
	 datos_clock++;
	/*mes*/
/*	*datos_clock=hex_bcd(lee_clk(RMES));			
	 datos_clock++;
	/*Dia*/
	/**datos_clock=hex_bcd(lee_clk(RDIA));			
	 datos_clock++;
	/*hora*/	
	/**datos_clock=hex_bcd(lee_clk(RHORA));			
	 datos_clock++;
	/*minutos*/	
/*	*datos_clock=hex_bcd(lee_clk(RMIN));			
	 datos_clock++;
}
*/
/*------------------------------------------------------------------------------
 Lee el dato del reloj en bloque 
------------------------------------------------------------------------------*/
void Block_read_Clock(unsigned char *datos_clock)
{
	/*año*/ 
	*datos_clock=lee_clk(RANO);			
	 datos_clock++;
	/*mes*/
	*datos_clock=lee_clk(RMES);			
	 datos_clock++;
	/*Dia*/
	*datos_clock=lee_clk(RDIA);			
	 datos_clock++;
	/*hora*/	
	*datos_clock=lee_clk(RHORA);			
	 datos_clock++;
	/*minutos*/	
	*datos_clock=lee_clk(RMIN);			
	 datos_clock++;
	/*segundos*/	
	*datos_clock=lee_clk(RSEG);
	datos_clock++;
	*datos_clock=00;
		
}
void cond_ini_clock()
{
	sck=0;					//Reloj
 	rst=0;
 	io=1;
}

/*------------------------------------------------------------------------------
funcion q verifica la fecha maxima de salida 
VERIFICA PAGO (sin_pago)....																		  
SI PAGO, VERIFICA QUE LA FECHA MAX DE SALIDA NO EXCEDE A LA ACTUAL (acceso_ok)							
(0) pago parqueadero y esta entre el tiempo permitido
(1) no hay pago
(2) excede tiempo de gracia
------------------------------------------------------------------------------*/
char check_fechaOut(char *buffer)
{
	char temp=1;
	unsigned char datos_clk[6];
	

	/*se valida la fecha de salida si es cero no ha pagado*/

	if ((*buffer==0x00)&&(*(buffer+1)==0x00)&&(*(buffer+2)==0x00)&&(*(buffer+3)==0x00)&&(*(buffer+4)==0x00))
		{
		
		temp=1;																													/*no hay pago*/
			
	 	}
	else
		{
			
			Block_read_Clock_Hex(datos_clk);															/*leo el clock actual*/
 	
		if (Debug_Tibbo==1)
		{
			 Debug_Fecha_actual(datos_clk);																/*muestro la fecha y hora actual*/
			
		}	
			
		
 		if (datos_clk[0]<*buffer)																				/*comparo el año leido de board_pcb con el dato de salida leido en  la MF */
		{
			temp=0;																												/*el tiempo esta entre el tiempo de gracia y activa la salida*/
 		}
		else if (datos_clk[0]==*buffer)																	/*el año es igual*/
		{
			if (datos_clk[1]<*(buffer+1))																	/*comparo el mes*/
			{
				temp=0;																											/*el tiempo esta entre el tiempo de gracia y activa la salida*/
			}
			else if (datos_clk[1]==*(buffer+1))														/*el mes es igual*/
 			{
				if (datos_clk[2]<*(buffer+2))																/*se compara el dia*/
				{
			  		temp=0;																									/*el tiempo esta entre el tiempo de gracia y activa la salida*/
				}
				else if(datos_clk[2]==*(buffer+2))													/*el dia es igual*/
				{
					if (datos_clk[3]<*(buffer+3))															/*comparo la hora del board_pcb con el dato de salida de la MF*/
					{
   			  			temp=0;																							/*el tiempo esta entre el tiempo de gracia y activa la salida*/
					}
					else if (datos_clk[3]==*(buffer+3))												/*es igual la hora*/
					{
						if (datos_clk[4]<*(buffer+4))   //(minut<=minutOut)
						{
					 		temp=0;																								/*el tiempo esta entre el tiempo de gracia y activa la salida*/
						}
						else 
						{
							temp=2;																								/*Excede T.GRACIA*/
						}

					}
					else
					{
						temp=2;																									/*Excede T.GRACIA*/
					}
				}
				else
				{
					temp=2;																										/*Excede T.GRACIA*/
				} 
			}
			else
			{
				temp=2;																											/*Excede T.GRACIA*/
			}
		}
		else
		{
	 		temp=2;																												/*Excede T.GRACIA*/
 
		}
		
	}
	return temp;
}

//*******************************************************************************************
void analiza_tiempo(char *buffer,unsigned int Val_DctoMinutos)
{


	int ano_inT;
	bit bisiesto=0;

	unsigned char ano_out, mes_out, dia_out, hora_out, min_out;
	unsigned char Horas_Dcto, Minutos_Dcto, Xtemp; 

	bit Tiempo_Dcto=0;

if ((*(buffer+0x0b)==0x00)&&(*(buffer+0x0c)==0x00)&&(*(buffer+0x0d)==0x00)&&(*(buffer+0x0e)==0x00)&&(*(buffer+0x0f)==0x00))
{

//--------------------------------------------------------------------------------------------------------------------------------------------
//	Descuento Maximo 999 minutos  = 16 Horas  39 Minutos
//--------------------------------------------------------------------------------------------------------------------------------------------
	if (Val_DctoMinutos >= 60 )
	{
		Horas_Dcto=Val_DctoMinutos/60;
		Minutos_Dcto=Val_DctoMinutos-(Horas_Dcto*60);

	}
	else
	{
	 	Minutos_Dcto=Val_DctoMinutos;
		Horas_Dcto=0;
	}
//*********************************************************************************************************************************************
//	Aumenta Primero Minutos
//---------------------------------------------------------------------------------------------------------------------------------------------
 	if (Minutos_Dcto!=0)
	{
		min_out=*(buffer+4)+Minutos_Dcto;					
		if (min_out>59)								   	// Si los minutos no sube de 60 no hay que aumentar nada
		{
//----------------------------------------------------
			Xtemp=(int)(min_out/60);					// Numero de Horas a aumentar
			min_out=min_out-(Xtemp*60);					// Total de minutos
//----------------------------------------------------
			hora_out=*(buffer+3)+Xtemp;					/*horas*/
			if (hora_out>23)
			{
				Xtemp= hora_out/24;					  	// Numero Dias a aumentar
				hora_out=hora_out-(Xtemp*24);		   	// Total de Horas
//----------------------------------------------------------------------------------------------------------------------------------------------
				dia_out=*(buffer+2)+Xtemp;
				if ((*(buffer+1)==1)||(*(buffer+1)==3)||(*(buffer+1)==5)||(*(buffer+1)==7)||(*(buffer+1)==8)||(*(buffer+1)==10)||(*(buffer+1)==12))  	// Meses de 31 dias
				{
					if (dia_out>31)
					{
					 	dia_out=dia_out-31;
						mes_out=*(buffer+1)+1;
						if (mes_out>12)
						{
						 	ano_out=*(buffer+0)+1;
							mes_out=1;
						}
						else
						{
						 	ano_out=*(buffer+0);
						}
					}
					else
					{
					 	mes_out=*(buffer+1);												/*mes*/
						ano_out=*(buffer+0);												/*año*/
					}	
 				}
				else if ((*(buffer+1)==4)||(*(buffer+1)==6)||(*(buffer+1)==9)||(*(buffer+1)==11))										// Meses de 30 dias
				{
					if (dia_out>30)
					{
					 	dia_out=dia_out-30;
						mes_out=*(buffer+1)+1;
						ano_out=*(buffer+0);					
					}
					else
					{
 						mes_out=*(buffer+1);
						ano_out=*(buffer+0);	
 					}

				}
				else if	(*(buffer+1)==2)																				// Febtrero Es biciesto?
				{
//----------------------------------------------------------------------------------------------------------------------------------------------
					ano_inT=*(buffer+0)+2000;
					bisiesto=0;

/*
					float flotante=12.345;
					int entero;

					entero=(int)flotante;
*/
 	
/*					division=(ano_inT/4);
					intpart=division;
					decpart= ano_inT - (intpart*4);
 					if (decpart==0)
					{
						division=(ano_inT/10);
				  		intpart = division;
				  		decpart = ano_inT - (intpart*10);
						if (decpart==0)
						{
							division=(ano_inT/40);
				  			intpart = division;
				  			decpart = ano_inT - (intpart*40);


							if (decpart==0)
							{
							 	bisiesto=1;
							}
							else
							{
							 	bisiesto=0;
							}
						}
						else
						{
							bisiesto=1;
						}
					}
*/

					if ((*(buffer+0)==16)||(*(buffer+0)==20)||(*(buffer+0)==24)||(*(buffer+0)==28)||(*(buffer+0)==32)||(*(buffer+0)==36))
					{
				 		bisiesto=1;
					}		
//----------------------------------------------------------------------------------------------------------------------------------------------					
					if (bisiesto==1)
					{
						if (dia_out>29)
						{
					 		ano_out=*(buffer+0);
							dia_out=dia_out-29;
							mes_out=*(buffer+1)+1;
					
						}
						else
						{
						 	ano_out=*(buffer+0);	
							dia_out=dia_out;
							mes_out=*(buffer+1);
						}
					}
					else
					{
						if (dia_out>28)
						{
							dia_out=dia_out-28;
							mes_out=*(buffer+1)+1;
					 		ano_out=*(buffer+0);
						}
						else
						{
 							dia_out=dia_out;
							mes_out=*(buffer+1);
							ano_out=*(buffer+0);	
 						}
					}
//---------------------------------------------------------------------------------------------------------------------------------------------
				}
				else																								// Error fecha
				{
 					dia_out=dia_out;
					mes_out=*(buffer+1);
					ano_out=*(buffer+0);	
				}
			}
			else
			{
				dia_out=*(buffer+2);
				mes_out=*(buffer+1);
				ano_out=*(buffer+0);
			}
		}
		else
		{
			hora_out=*(buffer+3);
			dia_out=*(buffer+2);
			mes_out=*(buffer+1);
			ano_out=*(buffer+0);
		}
	}
	else
	{
		min_out=*(buffer+4);
		hora_out=*(buffer+3);
		dia_out=*(buffer+2);
		mes_out=*(buffer+1);
		ano_out=*(buffer+0);
	}
//*********************************************************************************************************************************************
// 	AUMENTA HORAS
//---------------------------------------------------------------------------------------------------------------------------------------------
 		if (Horas_Dcto!=0)
		{
				hora_out=hora_out+Horas_Dcto;
				if (hora_out>23)
				{
					Xtemp= hora_out/24;					  	// Numero Dias a aumentar
					hora_out=hora_out-(Xtemp*24);		   	// Total de Horas
	//----------------------------------------------------------------------------------------------------------------------------------------------
					dia_out=dia_out+Xtemp;
					if ((mes_out==1)||(mes_out==3)||(mes_out==5)||(mes_out==7)||(mes_out==8)||(mes_out==10)||(mes_out==12))  	// Meses de 31 dias
					{
						if (dia_out>31)
						{
						 	dia_out=dia_out-31;
							mes_out=mes_out+1;
							if (mes_out>12)
							{
							 	ano_out=ano_out+1;
								mes_out=1;
							}
							else
							{
							 	ano_out=ano_out;
							}
						}
					}
					else if ((mes_out==4)||(mes_out==6)||(mes_out==9)||(mes_out==11))										// Meses de 30 dias
					{
						if (dia_out>30)
						{
						 	dia_out=dia_out-30;
							mes_out=mes_out+1;
							ano_out=ano_out;					
						}
						else
						{
	 						mes_out=mes_out;
							ano_out=ano_out;	
	 					}
					}
					else if	(mes_out==2)																				// Febtrero Es biciesto?
					{
//----------------------------------------------------------------------------------------------------------------------------------------------
						ano_inT=ano_out+2000;
						bisiesto=0;


/*						division=(ano_inT/4);
						intpart = division;
						decpart= ano_inT - (intpart*4);
	
	
						if (decpart==0)
						{
							division=(ano_inT/10);
					  		intpart = division;
					  		decpart = ano_inT - (intpart*10);
							if (decpart==0)
							{
								division=(ano_inT/40);
					  			intpart = division;
					  			decpart = ano_inT - (intpart*40);
								if (decpart==0)
								{
								 	bisiesto=1;
								}
								else
								{
								 	bisiesto=0;
								}
							}
							else
							{
								bisiesto=1;
							}
						}
*/
						if ((ano_out==16)||(ano_out==20)||(ano_out==24)||(ano_out==28)||(ano_out==32)||(ano_out==36))
						{
			 				bisiesto=1;
						}		

//----------------------------------------------------------------------------------------------------------------------------------------------					
						if (bisiesto==1)
						{
							if (dia_out>29)
							{
						 		dia_out=dia_out-29;
								mes_out=mes_out+1;
								ano_out=ano_out;						
							}
							else
							{
							 	dia_out=dia_out;
								mes_out=mes_out;
								ano_out=ano_out;
							}
						}
						else
						{
							if (dia_out>28)
							{
						 		dia_out=dia_out-28;
								mes_out=mes_out+1;
								ano_out=ano_out;													
							}
						}
	//----------------------------------------------------------------------------------------------------------------------------------------------
					}
	
				}
 		}

//----------------------------------------------------------------------------------------------------------------------------------------------
	*(buffer+0x0b)=ano_out;
	*(buffer+0x0c)=mes_out;
	*(buffer+0x0d)=dia_out;
	*(buffer+0x0e)=hora_out;
	*(buffer+0x0f)=min_out; 	
//----------------------------------------------------------------------------------------------------------------------------------------------
	}
	

}