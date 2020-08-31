
#include <eeprom.h>
#include <reg51.h>

/*funciones externas*/
extern void          _nop_     (void);
extern void Delay (void);
extern void Delay_20us(unsigned int cnt);
extern void Delay_1ms(unsigned int cnt);

//******************************************************************************************
// 		RUTINAS DE EEPROM 24FC1025
//******************************************************************************************
sbit sda = P1^2;
sbit scl = P1^3;
//******************************************************************************************

bit memory_flag;
unsigned char byte_rd;
unsigned char l_data_eeprom;
unsigned char l_chr;
/*define posiciones de memoria*/
#define EE_ID_CLIENTE					0x0000
#define EE_ID_PARK		  			0x0002
#define EE_TIEMPO_GRACIA		  0x0004
#define EE_SIN_COBRO					0x0006
#define EE_DEBUG							0x0008
#define EE_USE_LPR						0x000A
#define EE_CPRCN_ACTIVA				0x000C
#define EE_TIPO_PANTALLA			0X000E


extern int ID_CLIENTE;						
extern int COD_PARK;
extern unsigned char Tipo_Vehiculo;																		/*tipo de vehiculo*/
extern unsigned int  T_GRACIA;																				/*tiempo de gracia del parqueo*/
extern unsigned int  SIN_COBRO;																				/*SIN_COBRO=0 inhabilitado =(1) sin cobro = (2) sin cobro y salida el mismo dia*/
extern unsigned char  Debug_Tibbo;
 //unsigned char  USE_LPR;
extern unsigned char  COMPARACION_ACTIVA;
extern unsigned	char 	Tarjeta_on;																		/*detecto vehiculo en loop y tiene tarjeta en boca*/
extern unsigned char 	Raspberry;						
//*******************************************************************************************
void ackd(void)
{
	unsigned int i;
	memory_flag=1;
	scl=1;
	sda=1;
	for (i=0; i<1100; i++)					   //500
	{
	  if (sda==0)
	  {
	  	memory_flag=0;
		scl=0;

		break;
	  }
	}
	
	if (memory_flag==1)
	{ 

	}  
}	
//*******************************************************************************************
//*******************************************************************************************
void ack_lect(void)
{
	int i;
	scl=1;
	sda=1;
	memory_flag=1;
	for (i=0; i<650; i++)
	{
	  if (sda==0)
	  {
	  	memory_flag=0;
		scl=0;
		break;
	  }
	}
}	
//*******************************************************************************************
void p_me (void)
{
	  scl=1;
	  _nop_();
	  scl=0;
}
//*******************************************************************************************
void e_d_m (unsigned char a_serial_eeprom)
{
	unsigned char nbits;
  	for (nbits=0; nbits<8; nbits++)
	{
	  a_serial_eeprom <<=1;
	  if (CY==1)
	  { 
	    sda=1;
	  }
	  else
	  {
	    sda=0;
	  }
	  scl=1;
	  p_me();  				
	}
	return;
}
//********************************************************************************************
void start (void)
{
 	sda=1;
 	scl=1;
 	_nop_();
 	sda=0;
 	scl=0;
 	return;
}
//*******************************************************************************************
void stop (void)
{
 	scl=0;
 	sda=0;
 	scl=1;
 	sda=1;
 	return;
}
//*******************************************************************************************
unsigned char l_d_m (void)
{
	unsigned char nbits;
   	for (nbits=0; nbits<8; nbits++)
	{
		scl=0;
   		if (sda==1) 
	  	{
			l_data_eeprom = l_data_eeprom| 0x01;
			if (nbits<=6) 
			{
				l_data_eeprom<<=1;
				scl=1;
			}
	  	}
	   	if (sda==0)
	  	{
	  		l_data_eeprom = l_data_eeprom & 0xFE;
			if (nbits<=6) 
		 	{
		  		l_data_eeprom <<=1;
				scl=1;
		 	}
 	  	}
    }
	scl=0;
 	return l_data_eeprom;
}
//*******************************************************************************************
//*******************************************************************************************
//	ESCRIBE EN EEPROM																		*
//*******************************************************************************************
//void wr_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l,unsigned char data_eeprom)
void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom)
{
	unsigned char dir_h, dir_l;
	dir_l=Dir;
	Dir>>=8;
	dir_h=Dir;


 	scl=0;
	sda=0;
// 	wait();
    start();
	e_d_m(control);
	ackd();
	e_d_m(dir_h);
	ackd();
	e_d_m(dir_l);
	ackd();
	e_d_m(data_eeprom);
	ackd();
	stop();
	Delay_1ms(13);
	
	scl=1;
	sda=1;
	Delay_20us(98);										/*wait long*/
	Delay_20us(98);
}
//*******************************************************************************************
//	ESCRIBE EN EEPROM																		*
//*******************************************************************************************
/*
void wrpage_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l,unsigned char data_eeprom)  
{
 	unsigned int j;
	scl=0;
	sda=0;
 	wait();

	wait();

    start();
	e_d_m(control);
	ack();
	e_d_m(dir_h);
	ack();
	e_d_m(dir_l);
	ack();
 	for (j=0; j<=127; j++)
	{
		e_d_m(data_eeprom);
		ack();
	}
	stop();
	Delay_1ms(13);
	scl=1;
	sda=1;
 return;
}																						   */
//*******************************************************************************************	
//*******************************************************************************************	
//	LEE EN EEPROM																			*
//*******************************************************************************************
//unsigned char rd_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l) 
unsigned char rd_eeprom (unsigned char control,unsigned int Dir) 
{

	unsigned char dir_h, dir_l;
	dir_l=Dir;
	Dir>>=8;
	dir_h=Dir;

 	scl=0;
	sda=0;
 //	wait();
 	start();
	e_d_m(control);
	ack_lect();
	e_d_m(dir_h);
  	ack_lect();
	e_d_m(dir_l);
 	ack_lect();
	start();
	e_d_m(control+0x01);
 	ackd();
	scl=0;
 	l_d_m();
	stop();
  	scl=1;
	sda=1;
	return l_data_eeprom;
}
/*------------------------------------------------------------------------------
Rutina que lee la eeprom, los bit de configuracion 
------------------------------------------------------------------------------*/
void variable_inicio()
{
	
	ID_CLIENTE=rd_eeprom(0xa8,EE_ID_CLIENTE);	
	COD_PARK=rd_eeprom(0xa8,EE_ID_PARK);
	T_GRACIA=rd_eeprom(0xa8,EE_TIEMPO_GRACIA);
	SIN_COBRO=rd_eeprom(0xa8,EE_SIN_COBRO);
	Debug_Tibbo=rd_eeprom(0xa8,EE_DEBUG);
//	USE_LPR=rd_eeprom(0xa8,EE_USE_LPR);
	COMPARACION_ACTIVA=rd_eeprom(0xa8,EE_CPRCN_ACTIVA);
	Raspberry = rd_eeprom(0xa8,EE_TIPO_PANTALLA);
}
//***********************************************************************************************
//Rutina que lee la eeprom 
//***********************************************************************************************
void LeerMemoria(unsigned int addres, unsigned char *res)
	{
unsigned char i;
do {
 	*res=rd_eeprom(0xa8,addres);;
	i=*res;
	addres++;
	res++;
}while(i !='\0');
 	*res='\0';
	}
void EscribirMemoria(unsigned int addres,unsigned char *res)
	{


while(*res !='\0'){
	if(*res =='\r'){*res='\0';}  
	wr_eeprom(0xa8,addres,*res);
 
    addres++;
		res++;
	}
 wr_eeprom(0xa8,addres,*res);
	 addres++;
		res++;
  wr_eeprom(0xa8,addres,0);

}