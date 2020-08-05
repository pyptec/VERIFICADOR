#include <reg51.h>
#include "tibbo.h" 

/*funciones externas*/
extern void Delay_20us(unsigned int cnt);
extern void          _nop_     (void);
extern unsigned char hex_bcd (unsigned char byte);

/*variable externas*/
extern unsigned char Debug_Tibbo;

sbit rx_ip = P0^0;					//		
sbit txd2 = P1^0;					//Transmision Aux Datos	IP								*

#define EE_DEBUG							0x0008
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void time_bit()
{
	unsigned char j;

	for (j=0; j<=7; j++) 				//18 para 19200  ...   41 Para 9600	 //42 142us //7 a 9600 transmision
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	//	_nop_();
//		_nop_();
	//	_nop_();
	//	_nop_();
	}
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void time_mbit(void)
{
	unsigned char j;
	for (j=0; j<=4; j++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}

}

/*------------------------------------------------------------------------------
Transmite un caracter  por tibbo a 9600 bd
------------------------------------------------------------------------------*/
void tx_aux(unsigned char caracter)
{
	unsigned char j, temporal, bitTX;

	EA=0;
	txd2=0;
	time_bit();
		_nop_();
		_nop_();
  	temporal=caracter;
	bitTX=caracter&0x01;
	if (bitTX==0x00)
	{
	 	txd2=0;
	}
	else
	{
	   	txd2=1;
	}
	time_bit();
	for (j=1; j<=7; j++)
	{
		temporal>>=1;
		bitTX=temporal&(0x01);
		if (bitTX==0x00)
		{
	 		txd2=0;
		}
		else
		{
	   		txd2=1;
		}
		time_bit();
	}
	txd2=1;
	time_bit();
	time_bit(); 

	EA=1;
}


void Debug_HexDec(unsigned char xfc)
{
	unsigned int valor;
	unsigned char centena, decena, unidad;
	valor=0;

	centena=0;
	decena=0;
	unidad=0;
	// if((rd_eeprom(0xa8,EE_DEBUG))!= 0)	
	if (Debug_Tibbo==1)
	{	 
	 	while (xfc>=0x064) 				// resto 100
		{
			xfc=xfc-0x64;
			centena=centena+1;
		}
		while (xfc>=0x0a)				// resto 10
		{
			xfc=xfc-0x0a;
			decena=decena+1;
		}
		unidad=xfc;
	    tx_aux(centena|0x30);
		tx_aux(decena|0x30);
	   	tx_aux(unidad|0x30);
		
	}
}

/*------------------------------------------------------------------------------
Transmito un caracter pasandolo a ascii 
------------------------------------------------------------------------------*/
void Debug_chr_Tibbo(unsigned char Dat)
{
	unsigned char temp;
	// if((rd_eeprom(0xa8,EE_DEBUG))!= 0)	
	if (Debug_Tibbo==1)
	{
		temp=(Dat&0xf0)>>4;
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
			
		tx_aux(temp);
							 
		temp=(Dat&0x0f);
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
		tx_aux(temp);
		tx_aux(' ');
	
	}
}

/*------------------------------------------------------------------------------
Transmito un Buffer x y lo pasa a ascii 
io=0 datos enviados
io=1 datos recibidos
------------------------------------------------------------------------------*/
void DebugBufferMF(unsigned char *str,unsigned char num_char,char io)
{
  unsigned char j;
 
   //if((rd_eeprom(0xa8,EE_DEBUG))!= 0)	
  if (Debug_Tibbo==1)
  {
		Debug_Dividir_texto();
		if(io!=0)
		{
  	Debug_txt_Tibbo((unsigned char *) "Datos MF respuesta: ");
		}else Debug_txt_Tibbo((unsigned char *) "Datos MF Enviados: ");
		for (j=0; j<num_char; j++)
		{
		Debug_chr_Tibbo(*str);
		str++;
		}
		
		tx_aux('\n');
		tx_aux('\r');
		//Debug_Dividir_texto();
  }

}

/*------------------------------------------------------------------------------
imprime la trama hasta el caracter null
------------------------------------------------------------------------------*/
void Debug_txt_Tibbo(unsigned char * str)
{
	unsigned char i;
	i=0;
	// if((rd_eeprom(0xa8,EE_DEBUG))!= 0)						/*debug tibbo 1=on 0=off*/
			
	if (Debug_Tibbo==1)
	{
		for (i=0; str[i] != '\0'; i++)
		{
 	  		tx_aux(str[i]);
		}
		
	}
}

void Debug_Dividir_texto()
{
	Debug_txt_Tibbo((unsigned char *) "/*---------------------------------------*/\n\r");
}

/*------------------------------------------------------------------------------
Recibe la trama del tibbo a 9600bd
------------------------------------------------------------------------------*/
unsigned char rx_Data(void)
{
	unsigned char temporal;
	
		temporal=0xff;
		time_mbit();
//--------------------------------------------------------------------------------------------------
		time_bit();
//--------------------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
//------------------------------------------------------------------------------------
		time_bit();
	  	while (rx_ip==0)
		{
		}
//------------------------------------------------------------------------------------
 	return temporal; 

}	
void Debug_Fecha_actual(unsigned char *buffer)
{
	Debug_txt_Tibbo((unsigned char *) "Fecha Actual en Board: ");
			Debug_chr_Tibbo(hex_bcd(*buffer));														/*año*/
			tx_aux('/');
			Debug_chr_Tibbo(hex_bcd(*(buffer+1)));												/*mes*/
			tx_aux('/');
			Debug_chr_Tibbo(hex_bcd(*(buffer+2)));												/*dia*/
			tx_aux(' ');
			Debug_chr_Tibbo(hex_bcd(*(buffer+3)));												/*hora*/
			tx_aux(':');
			Debug_chr_Tibbo(hex_bcd(*(buffer+4)));												/*minutos*/
			Debug_txt_Tibbo((unsigned char *) "\n\r\n\r");
}
/*------------------------------------------------------------------------------
Condiciones iniciales de los pines
------------------------------------------------------------------------------*/
void cond_ini_tibbo(void)
{

	txd2=1;
	rx_ip=1;
}
void Debug_pto_paralelo(unsigned char *buffer, unsigned char Length_trama )
{
	
	Debug_txt_Tibbo((unsigned char *) "Recibe trama pto paral= ");					/*trama recibida pto paralelo */
	DebugBufferMF(buffer,Length_trama,1);																/*imprimo la trama recibida*/
	Debug_txt_Tibbo((unsigned char *) "\n\r");
	Debug_txt_Tibbo((unsigned char *) "longitud de la trama: ");		/*msj longitud de la trama */
	Debug_chr_Tibbo(Length_trama);																			/*numero de caracteres recibidos*/
	Debug_txt_Tibbo((unsigned char *) "\n\r");
	Debug_Dividir_texto();																							/*divido el texto*/
			
}	

//void Debug_monitor(unsigned char *buffer, unsigned char Length_trama )
//{
	//Debug_Dividir_texto();																							/*se divide el texto */			
//	Debug_txt_Tibbo((unsigned char *) "Recibe trama Monitor= ");				
//	Debug_txt_Tibbo(buffer);
//	Debug_txt_Tibbo((unsigned char *) "\n\r");
//	Debug_txt_Tibbo((unsigned char *) "longitud de la trama: ");		/*msj longitud de la trama */
//	Debug_chr_Tibbo(Length_trama);																			/*numero de caracteres recibidos*/
//	Debug_txt_Tibbo((unsigned char *) "\n\r");				
//	Debug_Dividir_texto();	
//}	
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*
void DebugRtaStatus(unsigned char Tipo)	 			// 1='N'  0='P'
{

	if ((Tipo=='N')&&(Debug_Tibbo==1)) 					
	{
		Debug_txt_Tibbo(0,(unsigned char *) "Respuesta Incorrecta: N ");

		if ((Buffer_Rta_Lintech[3]=='0')&&(Buffer_Rta_Lintech[4]=='0'))
		{
			Debug_txt_Tibbo(1,(unsigned char *) "Error No definido");
		}					
		else if ((Buffer_Rta_Lintech[3]=='0')&&(Buffer_Rta_Lintech[4]=='1'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "CMD con Error de Parametro");
		}
		else if ((Buffer_Rta_Lintech[3]=='0')&&(Buffer_Rta_Lintech[4]=='2'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "Error secuencia de ejecucion del CMD");
		}
		else if ((Buffer_Rta_Lintech[3]=='0')&&(Buffer_Rta_Lintech[4]=='3'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "CMD no soportado por Hardware");
		}
		else if ((Buffer_Rta_Lintech[3]=='1')&&(Buffer_Rta_Lintech[4]=='0'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "Tarjeta Atascada / Overtimme");
		}
 		else if ((Buffer_Rta_Lintech[3]=='A')&&(Buffer_Rta_Lintech[4]=='0'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "Dispensador Vacio");
		}
 		else if ((Buffer_Rta_Lintech[3]=='A')&&(Buffer_Rta_Lintech[4]=='1'))
		{
		 	Debug_txt_Tibbo(1,(unsigned char *) "Colector Lleno");
		}
	}
	else if ((Tipo=='P')&&(Debug_Tibbo==1))   						// valido st0	 0 canal libre, 1 tarjeta en la boca, 2 tarjeta en rf
	{
		
		if (Buffer_Rta_Lintech[3]=='0')
		{
			if (Buffer_Rta_Lintech[3]!=Notify)
			{
				Debug_txt_Tibbo(1,(unsigned char *) "Canal Libre");
				Notify=Buffer_Rta_Lintech[3];
			}
		}					
		else if (Buffer_Rta_Lintech[3]=='1')
		{
			if (Buffer_Rta_Lintech[3]!=Notify)
			{
				Debug_txt_Tibbo(1,(unsigned char *) "Tarjeta en Bezel");
				Notify=Buffer_Rta_Lintech[3];
			}		 	
		}
		else if (Buffer_Rta_Lintech[3]=='2')
		{
			if (Buffer_Rta_Lintech[3]!=Notify)
			{
		 		Debug_txt_Tibbo(1,(unsigned char *) "Tarjeta en RF");
				Notify=Buffer_Rta_Lintech[3];
			}
		}
		else
		{
			if (Buffer_Rta_Lintech[3]!=Notify)
			{
				Debug_txt_Tibbo(1,(unsigned char *) "P: Status Incorrecto...");
				Notify=Buffer_Rta_Lintech[3];
			}
 		}
  	}
		
}*/