
#include "prog.h" 
#include <string.h>
extern int printf   (const char *, ...);
extern char putchar (char);
extern char getchar (void);
extern char  *strtok  (char *str, const char *set);
extern char   strcmp  (const char *s1, const char *s2);
extern char  *strcpy  (char *s1, const char *s2);
extern char _getkey (void);
extern char  *strcat  (char *s1, const char *s2);
extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
extern void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom);
extern int sprintf  (char *, const char *, ...);
extern int    atoi (const char *s1);
extern void LeerMemoria(unsigned int addres, unsigned char *res);
extern void EscribirMemoria(unsigned int addres,unsigned char *res);
extern char check_fechaOut(char *buffer);
extern void Block_read_clock_ascii(unsigned char *datos_clock);
extern void hex_ascii(unsigned char * datos,unsigned char * fecha_asii);
extern char check_fechaOut_2(char *buffer);

unsigned char cursor[20] = "/"; //current working directory
unsigned char validacion [9]={"admin"};	//usuario

#define True										0x01
#define False										0x00


/*define posiciones de memoria*/
#define EE_ID_CLIENTE					0x0000
#define EE_ID_PARK		  			0x0002
#define EE_TIEMPO_GRACIA		  0x0004
#define EE_SIN_COBRO					0x0006
#define EE_DEBUG							0x0008
#define EE_USE_LPR						0x000A
#define EE_CPRCN_ACTIVA				0x000C
#define EE_TIPO_PANTALLA			0X000E
#define	EE_DCTO_PASA_CAJA			0X000F
#define EE_JUST_ONE_TIME_CLAVE	0X0013
#define EE_HORARIO_1						0X0019
#define EE_HORARIO_2						0x0032
#define EE_HORARIO_3						0x004b
#define EE_HORARIO_4						0x0064
#define EE_HORARIO_5						0x007d
#define EE_HORARIO_6						0x0096
#define EE_HORARIO_7						0x00af
#define EE_HORARIO_8						0x00c8
#define EE_HORARIO_9						0x00e1
#define EE_HORARIO_10						0x00fa

#define EE_ID_REGISTER					0x0300
#define EE_FECHA_VENCIMIENTO		0X0350
/* Definicion del tamaño de comando y longitud de cmd*/

#define 	NUMCOMMAND 14
#define 	LONGSIZE 3



enum ee_horario_addr{
	ee_horario1_addr, ee_horario2_addr, ee_horario3_addr, ee_horario4_addr, ee_horario5_addr,
	ee_horario6_addr, ee_horario7_addr, ee_horario8_addr, ee_horario9_addr, ee_horario10_addr,
	ee_horarioNULL_addr
};


enum Dia_Semana_addr{
	Lunes_addr = 1, Martes_addr = 2, Miercoles_addr = 3, Jueves_addr =4, Viernes_addr = 5, 
	Sabado_addr = 6, Domingo_addr =0 
};
enum Dia_Semana_data{
	Domingo_dato=0x31, Lunes_dato=0x32, Martes_dato=0x33, Miercoles_dato=0x34, Jueves_dato=0x35, Viernes_dato=0x36, Sabado_dato=0x37  
};
enum Hora_Minutos_addr{
	Hora_High_addr_Desde = 7, Hora_Low_addr_Desde = 8, Minutos_High_addr_Desde = 9, Minutos_Low_addr_Desde = 10,
	Hora_High_addr_Hasta = 11, Hora_Low_addr_Hasta = 12, Minutos_High_addr_Hasta = 13, Minutos_Low_addr_Hasta = 14,
	Habilita_addr =15, Segundo_Tiempo=16
};
enum Hora_Minutos_data{
		Hora_High_data = 0, Hora_Low_data = 1, Minutos_High_data = 3, Minutos_Low_data =4
};

char comandos[NUMCOMMAND][LONGSIZE]=
{
  "0",       		//ID_CLIENTEValor del punto cero.
	"1", 	     //COD_PARKTiempo de recolecion entre muestra
	"2",		// T_GRACIAInformacion recolectada enviada al rs232
	"3",		//SIN_COBROAjuste del cero del sensor
	"4",	//DEBUG Calibracion por la curva de la pendiente
	"5",		//USE_LPR
	"6",//COMPARACION_ACTIVA
	"7",			//TIPO_PANTALLA
	"8",			//DCTO por caja
	"9",			// prog horario
	"10",			// ver cmd programados
	"11",     //AYUDA Ayuda!muestra todos los comandos
	"12",		//SALIRSalir de programacion
	"13"		////cmd escondido fecha de vencimiento password
};

/*------------------------------------------------------------------------------
Rutina que encripta la clave q se le envie
len_clave= longitud de la clave a encriptar
aleatorio=numero aleatoria para escoger el dato a sumar y para la or

------------------------------------------------------------------------------*/
unsigned char *hash_id(unsigned char *clave)
{
	unsigned char len_clave,i;
	static unsigned int temp, temp1;
	unsigned char aleatorio []={":[<%?XT]=)" };
	unsigned char suma []={"#*YJAIMEcamiltK"};/*caracteres q se le suman a la trama */
	unsigned char xxor []={"wrz25q68-91fS.@" };
	unsigned char usuario1[11];
	unsigned char vencimiento_password [4];
	
	LeerMemoria(EE_FECHA_VENCIMIENTO,vencimiento_password);	
	if (check_fechaOut_2(vencimiento_password) != True)
		
	{
		strcpy(aleatorio,"@#!$`Fb^&*");
	}
	else
	{
		strcpy(aleatorio, ":[<%?XT]=)");
	}
	
	len_clave=strlen(clave);											/*longitud de la clave a encriptar*/
	for (i=0; i<10;i++)
	{
		temp=*(clave+i)+aleatorio[i];								/*el primer caracter de la clave se le suma con el primero de los aleatorios*/
		temp1=temp & 15;														/*al  resultado se le hace una and con (0x0f)*/
		if (temp1==0)temp1++;	
		temp=*(clave+i) + suma[temp1-1];						/*se suma la clave  con el arreglo suma y el valor del arreglo biene de  aleatorio el resultado q da en temp */
		temp1=temp & 15;
		if (temp1==0)temp1++;
		temp=temp ^ xxor[temp1-1];									/*temp se le hace uan xor aleatoria*/
 		if (temp > 255)
		{
			temp=temp & 255;
		}			
		if ((temp >=122)&&(temp<=255))
		{
		temp=temp & 122;
		
		}
		if (temp < 48)
		{
			temp=temp+64;
		}
		if ((temp>=91)&&(temp<=96))
		{
			temp=temp +7 ;
		}
		if ((temp>=58)&&(temp<=64))
		{
			temp=temp - 7 ;
		}
		usuario1[i]=temp;
		
	}
	usuario1[10]=0;
	return usuario1;
}


/*------------------------------------------------------------------------------
Rutina de lista de comandos 
------------------------------------------------------------------------------*/

unsigned char ListaComandos(unsigned char *cmd)
{
   unsigned char buf[LONGSIZE];
   unsigned char i;
   
   for (i=0; i<NUMCOMMAND; i++)
   {
      strcpy(buf, &comandos[i][0]);
      if (strcmp(buf, cmd)==0)
         return(i);
   }
   
   return(0xFF);
}


/*------------------------------------------------------------------------------
Rutina que valida la clave
------------------------------------------------------------------------------*/
unsigned char  ValidarClave(unsigned char *buffer)
{
unsigned char buf [7];
  
      strcpy(buf, &validacion[0]);
      if (strcmp(buf, buffer)==0)
         return(0);
 
   
   return(0xFF);
}



/*------------------------------------------------------------------------------
Rutina que busca el \
------------------------------------------------------------------------------*/
unsigned char * GetCMD(unsigned char *in)
{
   char tokens[]=" \r\n";
   return(strtok(in,tokens));
}

/*------------------------------------------------------------------------------
Rutina de 
------------------------------------------------------------------------------*/
unsigned char * GetOption(unsigned char *in)
{
   unsigned char tokensSpace[]=" \r\n";
   unsigned char tokensQuote[]="\"\r\n";
   
   //trim leading spaces
   while (*in==' ')
      in++;
   
   //if first char is a quote, then end token on a quote.  ELSE end token on a space
   if (*in == '\"')
      return(strtok(in,tokensQuote));
   else
      return(strtok(in,tokensSpace));
} 
/*------------------------------------------------------------------------------
Rutina de muestra el cursor
------------------------------------------------------------------------------*/

void DisplayCursor(void)
{
   printf("\r\n\n%s> ", cursor);
}

/*------------------------------------------------------------------------------
Rutina que captura los datos pto serie y los muestra en el pto
------------------------------------------------------------------------------*/

void IngresaDato(unsigned char  *buffer, unsigned char modo)
{

 int i;
 
	do
	{ 
		*buffer = _getkey();
	if (modo == 0) 
		{
  		 printf("%c", *buffer);
		}
		else 
		{
			putchar('*');
		}
	   i=*buffer; 
	   buffer++;
	 	*buffer='\0';	
		
	} while(i != '\r');
    
      
         
} 
/*
void LeerMemoria(unsigned int  addres,unsigned char  *res)
{
unsigned char i;
	do {
		*res=rd_eeprom(0xa8,addres);
		i=*res;
		addres++;
		res++;
		}while(i !='\0');
		*res='\0';
}
*/
/*------------------------------------------------------------------------------
Rutina que programa el id_cliente en eeprom
------------------------------------------------------------------------------*/
void prog_id_cliente()
{
	unsigned char buffer[10];
	unsigned int dataee;
	
	dataee=rd_eeprom(0xa8,EE_ID_CLIENTE);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	
	printf("\r\n\n ACTUAL ID_CLIENTE=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	
	printf("\r\n\n DIGITE SU NUEVO ID_CLIENTE=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_ID_CLIENTE,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_ID_CLIENTE);																					/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);																									/*lo convierto a un string*/
	printf("\r\n\n DATO MODIFICADO ID_CLIENTE=%s\r\n\n",buffer);									/*lo muetro por la pantalla el dato grabado*/
}
/*------------------------------------------------------------------------------
Rutina que programa el cod_park
------------------------------------------------------------------------------*/
void prog_cod_park()
{
	unsigned char buffer[10];
	unsigned int dataee;
	
	dataee=rd_eeprom(0xa8,EE_ID_PARK);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	
	printf("\r\n\n ACTUAL COD_PARK=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	
	printf("\r\n\n DIGITE SU NUEVO COD_PARK=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_ID_PARK,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_ID_PARK);																					/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);																									/*lo convierto a un string*/
	printf("\r\n\n DATO MODIFICADO COD_PARK=%s\r\n\n",buffer);									/*lo muetro por la pantalla el dato grabado*/
}
/*------------------------------------------------------------------------------
Rutina que programa el tiempo de gracia de cobro (00) no tiene tiempo de gracia
------------------------------------------------------------------------------*/
void Prog_tiempo_gracia()
{
	unsigned char buffer[10];
	unsigned int dataee;
	
	dataee=rd_eeprom(0xa8,EE_TIEMPO_GRACIA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	
	printf("\r\n\n ACTUAL TIEMPO_GRACIA=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	
	printf("\r\n\n DIGITE SU NUEVO TIEMPO_GRACIA=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_TIEMPO_GRACIA,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_TIEMPO_GRACIA);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);																									/*lo convierto a un string*/
	printf("\r\n\n DATO MODIFICADO TIEMPO_GRACIA=%s\r\n\n",buffer);								/*lo muetro por la pantalla el dato grabado*/
}
/*------------------------------------------------------------------------------
Rutina que programa SIN COBRO
SIN_COBRO=0 inhabilitado, SIN_COBRO=1 sin cobro, SIN_COBRO=2 sin cobro y salida el mismo dia");
------------------------------------------------------------------------------*/
void Prog_Sin_Cobro()
{
	

	unsigned char buffer[10];
	unsigned int dataee;

	dataee=rd_eeprom(0xa8,EE_SIN_COBRO);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL SIN_COBRO INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else if (dataee==1)
	{
		printf("\r\n\n ACTUAL SIN_COBRO GRATIS=%s\r\n\n",buffer);		
	}
	else 
	{
			printf("\r\n\n ACTUAL SIN_COBRO GRATIS Y SALIDA EL MISMO DIA=%s\r\n\n",buffer);		
	}
	
	printf("\r\n\n DIGITE SU NUEVO SIN_COBRO=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_SIN_COBRO,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_SIN_COBRO);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);																									/*lo convierto a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL SIN_COBRO INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else if (dataee==1)
	{
		printf("\r\n\n ACTUAL SIN_COBRO GRATIS=%s\r\n\n",buffer);		
	}
	else 
	{
			printf("\r\n\n ACTUAL SIN_COBRO GRATIS Y SALIDA EL MISMO DIA=%s\r\n\n",buffer);		
	}
	
}
/*------------------------------------------------------------------------------------
Rutina que habilita el debuger
 Habilita = 1 ; 
inahabilita = 0;
--------------------------------------------------------------------------------------*/
void Prog_debug()
{
	unsigned char buffer[10];
	unsigned int dataee;

	
	dataee=rd_eeprom(0xa8,EE_DEBUG);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL DEBUG INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL DEBUG HABILITADO=%s\r\n\n",buffer);			
	}
	
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE DEBUG=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_DEBUG,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_DEBUG);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL DEBUG INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL DEBUG HABILITADO=%s\r\n\n",buffer);			
	}
}
/*------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------*/

void Prog_Use_Lpr()
{
		unsigned char buffer[10];
	unsigned int dataee;

	
	dataee=rd_eeprom(0xa8,EE_USE_LPR);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL USE_LPR INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL USE_LPR HABILITADO=%s\r\n\n",buffer);			
	}
	
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE USE_LPR=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_USE_LPR,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_USE_LPR);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL USE_LPR INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL USE_LPR HABILITADO=%s\r\n\n",buffer);			
	}
}
/*------------------------------------------------------------------------------------
Compara la placa de la tarjeta y la del software
--------------------------------------------------------------------------------------*/
void Prog_Comparacion_Activa()
{
	unsigned char buffer[10];
	unsigned int dataee;

	
	dataee=rd_eeprom(0xa8,EE_CPRCN_ACTIVA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL COMPARACION ACTIVA INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL COMPARACION ACTIVA  HABILITADO=%s\r\n\n",buffer);			
	}
	
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE COMPARACION ACTIVA =");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_CPRCN_ACTIVA,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_CPRCN_ACTIVA);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL COMPARACION ACTIVA  INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL COMPARACION ACTIVA  HABILITADO=%s\r\n\n",buffer);			
	}
}
/*------------------------------------------------------------------------------------
cmd del tipo de pantalla a usar
Rasberry = 1
lcd = 0
--------------------------------------------------------------------------------------*/
void tipo_pantalla()
{
	unsigned char buffer[10];
	unsigned int dataee;

	
	dataee=rd_eeprom(0xa8,EE_TIPO_PANTALLA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n PANTALLA LCD HABILITADA=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n PANTALLA RASPBERRY  HABILITADA=%s\r\n\n",buffer);			
	}
	
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE LA PANTALLA =");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_TIPO_PANTALLA,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_TIPO_PANTALLA);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		printf("\r\n\n PANTALLA LCD HABILITADA=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n PANTALLA RASPBERRY  HABILITADA=%s\r\n\n",buffer);			
	}
	
}
/*------------------------------------------------------------------------------------
cmd que habilita si pasa por caja si no
(0) pasa por caja
(1) aplica el descuento
------------------------------------------------------------------------------------*/
void dcto_caja()
{
	unsigned char buffer[10];
	unsigned int dataee;

	
	dataee=rd_eeprom(0xa8,EE_DCTO_PASA_CAJA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n PASA POR CAJA PARA VALIDAR DCTO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n APLICA DESCUENTO=%s\r\n\n",buffer);			
	}
	
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE DCTO =");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,EE_DCTO_PASA_CAJA,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,EE_DCTO_PASA_CAJA);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		printf("\r\n\n PASA POR CAJA PARA VALIDAR DCTO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n APLICA DESCUENTO=%s\r\n\n",buffer);			
	}
}
unsigned char *Addr_Horarios()
{
	unsigned char ee_addr_horario[11];
		/*direcciones de memoria de almacenamiento*/
	ee_addr_horario[ee_horario1_addr]= EE_HORARIO_1;
	ee_addr_horario[ee_horario2_addr]= EE_HORARIO_2;
	ee_addr_horario[ee_horario3_addr]= EE_HORARIO_3;
	ee_addr_horario[ee_horario4_addr]= EE_HORARIO_4;
	ee_addr_horario[ee_horario5_addr]= EE_HORARIO_5;
	ee_addr_horario[ee_horario6_addr]= EE_HORARIO_6;
	ee_addr_horario[ee_horario7_addr]= EE_HORARIO_7;
	ee_addr_horario[ee_horario8_addr]= EE_HORARIO_8;
	ee_addr_horario[ee_horario9_addr]= EE_HORARIO_9;
	ee_addr_horario[ee_horario10_addr]= EE_HORARIO_10;
	ee_addr_horario[ee_horarioNULL_addr] = False;
	return ee_addr_horario;
}
unsigned char Prog_Horarios_on_off(unsigned int addr)
{
	unsigned char buffer[10];
	unsigned int dataee,habilitado;
	dataee = rd_eeprom(0xa8,addr);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n ACTUAL HORARIO INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n ACTUAL HORARIO HABILITADO=%s\r\n\n",buffer);			
	}
	printf("\r\n\n DIGITE EL NUEVO ESTADO DE HORARIO=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);																												/*trae el dato digitado*/
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,addr,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,addr);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
	if(dataee==0)
	{
		habilitado= False;
		printf("\r\n\n ACTUAL HORARIO INHABILITADO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		habilitado=True;
		printf("\r\n\n ACTUAL HORARIO HABILITADO=%s\r\n\n",buffer);			
	}
	return habilitado;
}
void Prog_Horario_Desde_Hasta(unsigned int addr)
{
		unsigned char buffer[10];
		unsigned char dataee;
	/*la hora de inicio del horario*/
	
	printf("\r\n\n Digite Hora 08:00 Minutos Desde: ");
	IngresaDato(buffer,0);
	printf("\r\n\n Hora Programada Desde: ");
	wr_eeprom(0xa8, addr + Hora_High_addr_Desde,buffer[Hora_High_data]);
	dataee = rd_eeprom(0xa8,addr + Hora_High_addr_Desde);
	printf("%c",dataee);
	wr_eeprom(0xa8,addr + Hora_Low_addr_Desde ,buffer[Hora_Low_data]);
	dataee = rd_eeprom(0xa8,addr + Hora_Low_addr_Desde); //
	printf("%c:",dataee);
	wr_eeprom(0xa8,addr + Minutos_High_addr_Desde,buffer[Minutos_High_data]);
	dataee = rd_eeprom(0xa8,addr + Minutos_High_addr_Desde);
	printf("%c",dataee);
	wr_eeprom(0xa8,addr + Minutos_Low_addr_Desde,buffer[Minutos_Low_data]);
	dataee = rd_eeprom(0xa8,addr +  Minutos_Low_addr_Desde);
	printf("%c",dataee);

/*la hora de final del horario*/

	printf("\r\n\n Digite Hora:Minutos Hasta: ");
	IngresaDato(buffer,0);
	
	printf("\r\n\n Hora Programada Hasta: ");

	wr_eeprom(0xa8,addr + Hora_High_addr_Hasta,buffer[Hora_High_data]);
	dataee = rd_eeprom(0xa8,addr + Hora_High_addr_Hasta);
	printf("%c",dataee);
	wr_eeprom(0xa8,addr + Hora_Low_addr_Hasta ,buffer[Hora_Low_data]);
	dataee = rd_eeprom(0xa8,addr + Hora_Low_addr_Hasta);
	printf("%c:",dataee);
	wr_eeprom(0xa8,addr + Minutos_High_addr_Hasta,buffer[Minutos_High_data]);
	dataee = rd_eeprom(0xa8,addr + Minutos_High_addr_Hasta);
	printf("%c",dataee);
	wr_eeprom(0xa8,addr + Minutos_Low_addr_Hasta ,buffer[Minutos_Low_data]);
	dataee = rd_eeprom(0xa8,addr + Minutos_Low_addr_Hasta);
	printf("%c",dataee);
}
void Ver_Horario_Desde_Hasta(unsigned int addr)
{
	unsigned char dataee;
	printf("\r\n Desde  Hasta\r\n");
	dataee = rd_eeprom(0xa8,addr + Hora_High_addr_Desde);
	printf(" %c",dataee);
	dataee = rd_eeprom(0xa8,addr + Hora_Low_addr_Desde); //
	printf("%c:",dataee);
	dataee = rd_eeprom(0xa8,addr + Minutos_High_addr_Desde);
	printf("%c",dataee);
	dataee = rd_eeprom(0xa8,addr +  Minutos_Low_addr_Desde);
	printf("%c   ",dataee);	
		/*hasta*/
	dataee = rd_eeprom(0xa8,addr + Hora_High_addr_Hasta);
	printf("%c",dataee);
	
	dataee = rd_eeprom(0xa8,addr + Hora_Low_addr_Hasta);
	printf("%c:",dataee);
	
	dataee = rd_eeprom(0xa8,addr + Minutos_High_addr_Hasta);
	printf("%c",dataee);
	
	dataee = rd_eeprom(0xa8,addr + Minutos_Low_addr_Hasta);
	printf("%c",dataee);
}
void Prog_Horario_dias(unsigned int addr)
{
	unsigned char buffer[10];
	unsigned char dataee, j, temp;
	/*los dias programados*/
	printf("\r\n\n Digite los dias a programar Lunes = 2, ");
	printf("Martes = 3, Miercoles = 4, Jueves = 5, ");
	printf("Viernes = 6, Sabado = 7, Domingo = 1 : ");
	IngresaDato(buffer,0);
	
	
	printf("\r\n\n Dias Programados : ");
	
	for(j=0; j < 8 ; j++)
	{
		
		if ((temp=buffer[j]) == Lunes_dato) 
		{
			printf(" Lunes");
		
			wr_eeprom(0xa8,addr+Lunes_addr,Lunes_dato);	
			dataee=rd_eeprom(0xa8,addr+Lunes_addr);	
			printf("= %c,",dataee);
		}
		
		if ((temp=buffer[j]) == Martes_dato)
		{
			printf(" Martes");
			wr_eeprom(0xa8,addr+Martes_addr,Martes_dato);
			dataee=rd_eeprom(0xa8,addr+Martes_addr);	
			printf("= %c,",dataee);
		}
	
		if ((temp=buffer[j])== Miercoles_dato)
		{
			printf(" Miercoles");
			wr_eeprom(0xa8,addr+Miercoles_addr,Miercoles_dato);
			dataee=rd_eeprom(0xa8,addr+Miercoles_addr);	
			printf("= %c,",dataee);
		}
		
		if ((temp=buffer[j]) == Jueves_dato)
		{
			printf(" Jueves");
			wr_eeprom(0xa8,addr+Jueves_addr,Jueves_dato);
			dataee=rd_eeprom(0xa8,addr+Jueves_addr);	
			printf("= %c,",dataee);
		}
		
		if ((temp=buffer[j]) == Viernes_dato)
		{	
			printf(" Viernes");
			wr_eeprom(0xa8,addr+Viernes_addr,Viernes_dato);
			dataee=rd_eeprom(0xa8,addr+Viernes_addr);	
			printf("= %c,",dataee);
		}
		
		if ((temp=buffer[j]) == Sabado_dato) 
		{
			printf(" Sabado");
			wr_eeprom(0xa8,addr+Sabado_addr,Sabado_dato);
			dataee=rd_eeprom(0xa8,addr+Sabado_addr);	
			printf("= %c,",dataee);
		}
		
		if ((temp=buffer[j]) == Domingo_dato)
		{
			printf(" Domingo");
			wr_eeprom(0xa8,addr+Domingo_addr,Domingo_dato);
			dataee = rd_eeprom(0xa8,addr+Domingo_addr);	
			printf("= %c",dataee);
		}
	}
	
}
void Ver_Horario_dias(unsigned int addr)
{
	unsigned char dataee;
	/*Lunes*/
	dataee=rd_eeprom(0xa8,addr+Lunes_addr);	
		if(dataee == Lunes_dato)
		{
		printf(" Lunes = Programado\r\n");
		}
		else
		{
		printf(" Lunes = No programado\r\n");
		}
		/*Martes*/
		
		dataee=rd_eeprom(0xa8,addr+Martes_addr);	
		if(dataee == Martes_dato)
		{
		printf(" Martes = Programado\r\n");
		}
		else
		{
		printf(" Martes = No programado\r\n");
		}
		
		/*Miercoles*/
		dataee=rd_eeprom(0xa8,addr+Miercoles_addr);	
		if(dataee == Miercoles_dato)
		{
		printf(" Miercoles = Programado\r\n");
		}
		else
		{
		printf(" Miercoles = No programado\r\n");
		}
		/*Jueves*/
			dataee=rd_eeprom(0xa8,addr+Jueves_addr);	
		if(dataee == Jueves_dato)
		{
		printf(" Jueves = Programado\r\n");
		}
		else
		{
		printf(" Jueves = No programado\r\n");
		}
		/*Viernes*/
			dataee=rd_eeprom(0xa8,addr+Viernes_addr);	
		if(dataee == Viernes_dato)
		{
		printf(" Viernes = Programado\r\n");
		}
		else
		{
		printf(" Viernes = No programado\r\n");
		}
		/*Sabado*/
		
			dataee=rd_eeprom(0xa8,addr+Sabado_addr);	
		if(dataee == Sabado_dato)
		{
		printf(" Sabado = Programado\r\n");
		}
		else
		{
		printf(" Sabado = No programado\r\n");
		}
		/*Domingo*/
		dataee=rd_eeprom(0xa8,addr+Domingo_addr);	
		if(dataee == Domingo_dato)
		{
		printf(" Domingo = Programado\r\n");
		}
		else
		{
		printf(" Domingo = No programado\r\n");
		}
}
void Prog_Horarios()
{
	unsigned char buffer[10];
	unsigned char ee_addr_horario[11];
	unsigned int addr,temp;
	unsigned char dataee;
	unsigned char j;
	
	/*direcciones de memoria de almacenamiento*/
	
		strcpy (ee_addr_horario,(Addr_Horarios()));
		
	/*se programa el banco de horarios del 1 al 10*/
	
	printf("\r\n\n Digite el numero del Horario a programar = ");		
	IngresaDato(buffer,0);	
	
	j=(atoi(buffer)) - 1;
	
	/*mi direccion eeprom*/
	
	addr= ee_addr_horario[j];
	temp= addr;
	
	/*habilita o desabilita el uso del horario*/
	
	addr =addr + Habilita_addr ;
	if (Prog_Horarios_on_off(addr) == True);
	{
		/*limpiar la memoria*/
	addr= temp;
		for(j=Lunes_addr; j < 8 ; j++)
		{
			wr_eeprom(0xa8,addr+j,0xff);
		}
	
	addr= temp;
	/*los dias programados*/
	
	Prog_Horario_dias(addr);
		
	/*la hora de inicio del horario*/
	
	Prog_Horario_Desde_Hasta(addr);
	
	/*programo si hay segundo horario*/
	printf("\r\n\n Programar segundo Horario si=(1) no=(0)= ");		
	IngresaDato(buffer,0);
	dataee=atoi(buffer);																													/*lo convierto a un dato hex*/
	wr_eeprom(0xa8,addr+Segundo_Tiempo,dataee);																					/*grabo el dato en la eeprom*/
	
	dataee=rd_eeprom(0xa8,addr+Segundo_Tiempo);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
		if(dataee==True)
		{
			/*la hora de inicio del segundo horario*/
			addr =addr + Minutos_Low_addr_Desde ;
			Prog_Horario_Desde_Hasta(addr);
		}
	}
	
}
void Prog_fecha_vencimiento()
{
	unsigned char buffer[11];
	unsigned char *cmd;
	
	unsigned char fecha[7];
	unsigned int dataee;

	LeerMemoria(EE_FECHA_VENCIMIENTO,buffer);		
		 hex_ascii(buffer,fecha);
															
	
	printf("\r\n\n ACTUAL FECHA VENCIMIENTO PASSWORD:%s\r\n\n",fecha);														/*se muestra el id_cliente actual en pantalla*/
		
	 do{
	printf("\r\n\n/>Id Registro:");
	LeerMemoria(EE_ID_REGISTER,buffer);
	printf("%s", buffer);
	strcpy (validacion,hash_id(buffer));
	printf("\r\n\n/>Password:");
	
		/*para pruebas*/
	//printf("\r\n\%s ", validacion);
		
	IngresaDato(buffer,1);					//ingreso el password por teclado 
	cmd = GetCMD(buffer);					//quita el carri return	
	EscribirMemoria(EE_ID_REGISTER,validacion);

	}while(ValidarClave(cmd)!=0);
	
	
	printf("\r\n\n DIGITE LA NUEVA FECHA DE VENCIMIENTO=");																	/*digite el nuevo id_cliente*/
	IngresaDato(buffer,0);	
	printf("\r\n\n %s\r\n\n",buffer);	
	
		fecha[0]=buffer[0];	
		fecha[1]=buffer[1];	
		fecha[2]=0;	
		
		dataee=atoi(fecha);
		printf("\r\n\n %X",dataee);	
	/*lo convierto a un dato hex*/
	 wr_eeprom(0xa8,EE_FECHA_VENCIMIENTO,dataee);																					/*grabo el dato en la eeprom*/
		fecha[0]=buffer[2];	
		fecha[1]=buffer[3];	
		fecha[2]=0;	
		dataee=atoi(fecha);
		printf("\r\n\n %X",dataee);	
		wr_eeprom(0xa8,EE_FECHA_VENCIMIENTO+1,dataee);
		fecha[0]=buffer[4];	
		fecha[1]=buffer[5];	
		fecha[2]=0;	
		dataee=atoi(fecha);	
		printf("\r\n\n %X",dataee);			
		wr_eeprom(0xa8,EE_FECHA_VENCIMIENTO+2,dataee);
		wr_eeprom(0xa8,EE_FECHA_VENCIMIENTO+3,0);
		
		LeerMemoria(EE_FECHA_VENCIMIENTO,buffer);
		
		hex_ascii(buffer,fecha);	
		printf("\r\n\n ACTUAL FECHA DE VENCIMIENTO PROGRAMADA =%s\r\n\n",fecha);	
	
}
void Ver_Horario()
{
	unsigned char buffer[10];
	unsigned char ee_addr_horario[11];
	unsigned char conta;
	
	unsigned int addr,temp;
	unsigned char dataee;
	/*direcciones de memoria de almacenamiento*/
	
	strcpy (ee_addr_horario,(Addr_Horarios()));
	for (conta=0;conta < 10 ; conta++)
	{
	
	printf("\r\n numero del Horario  programado = %c\r\n",conta+49);
		
	/*HORARIO HABILITADO O INHABILITADO*/
	addr= ee_addr_horario[conta];
	temp= addr;
		/*habilita o desabilita el uso del horario*/
	
	addr =addr + Habilita_addr ;
	dataee = rd_eeprom(0xa8,addr);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
		if(dataee == 1)
		{
		printf("\r\n  HORARIO HABILITADO  = ON\r\n");														/*se muestra el id_cliente actual en pantalla*/
		
		printf("\r\n Dias Programados\r\n");
	addr=temp;
	Ver_Horario_dias(addr);
	
	Ver_Horario_Desde_Hasta(addr);
	dataee=rd_eeprom(0xa8,addr+Segundo_Tiempo);																				/*leo el dato grabado*/
	sprintf(buffer,"%d",dataee);	
			if(dataee==True)
			{
			printf("\r\n  SEGUNDO HORARIO HABILITADO  = ON\r\n");
			/*la hora de inicio del segundo horario*/
			addr =addr + Minutos_Low_addr_Desde ;
			Ver_Horario_Desde_Hasta(addr);
			}
			else
			{
			printf("\r\n SEGUNDO HORARIO HABILITADO  = OFF\r\n");
			}
		}
		else
		{
		printf("\r\n  HORARIO HABILITADO  = OFF\r\n");			
		}
		
	}
	
		
}
void Ver_Prog()
{
	unsigned char buffer[10];
	unsigned int dataee;
	unsigned char fecha[7];
	
	
	dataee=rd_eeprom(0xa8,EE_ID_CLIENTE);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	/*ID programado*/
	printf("\r\n ID_CLIENTE_PROGRAMADO = %s\r\n",buffer);		
	/*codigo de parkeadero*/
	dataee=rd_eeprom(0xa8,EE_ID_PARK);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	
	printf("\r\n COD_PARK_PROGRAMADO = %s\r\n",buffer);		
	/*tiempo de gracia*/
	dataee=rd_eeprom(0xa8,EE_TIEMPO_GRACIA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	
	printf("\r\n TIEMPO_GRACIA PROGRAMADO = %s\r\n",buffer);		
	/*SIN COBRO*/
	dataee=rd_eeprom(0xa8,EE_SIN_COBRO);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\ SIN_COBRO HABILITADO = OFF \r\n");														/*se muestra el id_cliente actual en pantalla*/
	}
	else if (dataee==1)
	{
		printf("\r\n SIN_COBRO = ON\r\n");		
	}
	
	/*estado de debug*/
	dataee=rd_eeprom(0xa8,EE_DEBUG);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==1)
	{
		printf("\r\n PROGRAMADO DEBUG = ON\r\n");	
	}	
	else
	{
		printf("\r\n PROGRAMADO DEBUG = OFF\r\n");	
	}	
	/*Estado del lpr*/
	dataee=rd_eeprom(0xa8,EE_USE_LPR);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n USE_LPR = OFF\r\n");														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n USE_LPR = ON\r\n");			
	}
	/*comparacion activa*/
	dataee=rd_eeprom(0xa8,EE_CPRCN_ACTIVA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n  COMPARACION ACTIVA = OFF\r\n");														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n COMPARACION ACTIVA  = ON\r\n");			
	}
	
	/*Tipo de pantalla*/
	dataee=rd_eeprom(0xa8,EE_TIPO_PANTALLA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n PANTALLA LCD PROGRAMADA\r\n");														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n PANTALLA RASPBERRY  PROGRAMADA\r\n");			
	}
	/*DESCUENTOS*/
	dataee=rd_eeprom(0xa8,EE_DCTO_PASA_CAJA);																					/*se lee el id_cliente actual */
	sprintf(buffer,"%d",dataee);																									/*se convierte  un entero a un string*/
	if(dataee==0)
	{
		printf("\r\n\n PASA POR CAJA PARA VALIDAR DCTO=%s\r\n\n",buffer);														/*se muestra el id_cliente actual en pantalla*/
	}
	else
	{
		printf("\r\n\n APLICA DESCUENTO=%s\r\n\n",buffer);			
	}
	
	/*horarios Programados*/
	 Ver_Horario();
	
	/*Fecha de vencimiento clave*/
	LeerMemoria(EE_FECHA_VENCIMIENTO,buffer);
		
		hex_ascii(buffer,fecha);	
		printf("\r\n ACTUAL FECHA DE VENCIMIENTO PROGRAMADA =%s\r\n",fecha);	
}
/*------------------------------------------------------------------------------
Rutina que muestra la lista de comandos
------------------------------------------------------------------------------*/
void Show()
{
   printf("\r\n ID_CLIENTE    --- CMD 0 Identificador del cliente maximo 255");
   printf("\r\n COD_PARK      --- CMD 1 El numero del parqueadero maximo 255");
   printf("\r\n T_GRACIA      --- CMD 2 Tiempo sin cobro 00,  maximo 255");
   printf("\r\n SIN_COBRO     --- CMD 3 Inhabilitado= 0, Gratis= 1 , Gratis y salida el mismo dia= 2");
   printf("\r\n DEBUG         --- CMD 4 Habilitar = 1, Inhabilitar = 0");
	 printf("\r\n USE_LPR       --- CMD 5 Habilitar = 1, Inhabilitar = 0");
   printf("\r\n COMPARACION_ACTIVA --- CMD 6 Habilitar = 1, Inhabilitar = 0");
	 printf("\r\n TIPO_PANTALLA     --- CMD 7 (0) pantalla lcd  serie (1) raspberry o bluetooth");
	 printf("\r\n DCTO_PASA_CAJA     --- CMD 8 (0) pasa por caja (1) aplica el descuento");
	 printf("\r\n HORARIO            --- CMD 9 Progama 10 horarios del 1 al 10");
	 printf("\r\n VER_PROGRAMACION   --- CMD 10 Muestra la programacion");
	 printf("\r\n AYUDA         --- CMD 11 Muestra los comandos");
   printf("\r\n SALIR         --- CMD 12 Salir de programacion");

}
/*------------------------------------------------------------------------------
Rutina de principal de programacion
------------------------------------------------------------------------------*/
void  First_Clave()
{
	unsigned char clave[11];
	
	
		Block_read_clock_ascii(clave);
		clave[10] = 0;
		strcpy (validacion,hash_id(clave));
		EscribirMemoria(EE_ID_REGISTER,validacion);
		validacion[0]=0x14;
		validacion[0]=0x0B;
		validacion[0]=0x14;
		validacion[0]=0;
		
		EscribirMemoria(EE_FECHA_VENCIMIENTO,validacion);
	
	
}
/*------------------------------------------------------------------------------
Rutina de principal de programacion
------------------------------------------------------------------------------*/

void menu(void)
{

unsigned char *cmd,*option1,*option2;
unsigned char opt_buffer[40];
unsigned char buffer[40];



  
  printf("\r\n\nSistema de Programacion verificador lintech \r\n\r\n");
if(rd_eeprom(0xa8,EE_JUST_ONE_TIME_CLAVE) != False)	
	{
		First_Clave();	
		wr_eeprom(0xa8,EE_JUST_ONE_TIME_CLAVE,0x0);
		//printf("%s", buffer);
	
	}
 
	
	
	do{
	printf("\r\n\n/>Id Registro:");
	LeerMemoria(EE_ID_REGISTER,buffer);
	buffer[10]=0;
	printf("%s", buffer);
	strcpy (validacion,hash_id(buffer));
	printf("\r\n\n/>Password:");
	
		/*para pruebas*/
//	printf("\r\n\%s ", validacion);
		
	IngresaDato(buffer,1);					//ingreso el password por teclado 
	cmd = GetCMD(buffer);					//quita el carri return	
	EscribirMemoria(EE_ID_REGISTER,validacion);
}while(ValidarClave(cmd)!=0);				//validamos el usuario

/*
  do{
printf("\r\n\n/>Password:");

	IngresaDato(buffer,1);					//ingreso el password por teclado 
	cmd = GetCMD(buffer);					//quita el carri return	
	strcpy (validacion,"123456");
	//strcpy (validacion,usuario);		/*valida el valor encriptado*/
/*
	}while(ValidarClave(cmd)!=0);				//
*/
	
	Show();
	while(1)
	{
	DisplayCursor();


		IngresaDato(buffer,0);	
		cmd = GetCMD(buffer);	
		option1 = GetOption(cmd + strlen(cmd) + 1);
    option2 = GetOption(option1 + strlen(option1) + 1);	 

 	 //si empieza con un '/', eso significa que incluye el caminoa
      //al archivo.  si el archivo no empieza con un '/', debe agregarce 
      //la ruta del archivo.
      if (option1 && (option1[0]=='/'))
      {
         //option1 is a full path
         strcpy(opt_buffer, option1);
      }
      else if (option1)
      {
         // tack on the current working directory to option1
         strcpy(opt_buffer, cursor);
         strcat(opt_buffer, option1);
      }         





    if (cmd)
      {
         switch(ListaComandos(cmd))
         {
            case 0:  										/*Se programa la identificacion del cliente*/
							prog_id_cliente();
						
				     break;
            
            case 1:  //Tiempo de recolecion entre muestra
						prog_cod_park();
               break;
            
            case 2:  //Informacion recolectada enviada al rs232
		
						Prog_tiempo_gracia();
					
               break;
            
            case 3:  //Ajuste del cero del sensor
						Prog_Sin_Cobro();

               break;
            
						case 4:  //Ajuste de calibracion por medio de la pendiente
							Prog_debug();
               break;
						case 5:  //help me
							Prog_Use_Lpr();
               break;
						case 6:  //help me
           Prog_Comparacion_Activa();

               break;
						case 7:  		//tipo de pantalla
						tipo_pantalla();
						  break;
						
						case 8:  		//tipo de pantalla
						dcto_caja();
						  break;
						case 9:		//cmd configuracion los horarios
							Prog_Horarios();
            break;
						case 10:  //help me
           
							Ver_Prog();
               break;
						case 11:  //help me
           
							Show();
               break;
						case 12:  //salir
						return;

               break;
           	case 13:
							Prog_fecha_vencimiento();
							break;
		
					
            default:
               printf("\r\nComando no existe '%s'", cmd);
               break;
        }
      }
	}
}

	

