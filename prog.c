
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

unsigned char cursor[20] = "/"; //current working directory
unsigned char validacion [9]={"admin"};	//usuario




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
#define EE_CAMBIO_ID					0XF000

/* Definicion del tamaño de comando y longitud de cmd*/

#define 	NUMCOMMAND 11
#define 	LONGSIZE 3



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
	"9",     //AYUDA Ayuda!muestra todos los comandos
	"10"		//SALIRSalir de programacion
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
	unsigned char usuario1[6];
	
	len_clave=strlen(clave);											/*longitud de la clave a encriptar*/
	for (i=0; i<len_clave;i++)
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
	 printf("\r\n AYUDA         --- CMD 9 Muestra los comandos");
   printf("\r\n SALIR         --- CMD 10 Salir de programacion");

}

/*------------------------------------------------------------------------------
Rutina de principal de programacion
------------------------------------------------------------------------------*/

void menu(void)
{

unsigned char *cmd,*option1,*option2,*usuario;
unsigned char opt_buffer[40];
unsigned char buffer[40];



  
  printf("\r\n\nSistema de Programacion verificador lintech \r\n\r\n");

 
	
	
	do{
	printf("\r\n\n/>Usuario:");
	
  // main loop
	IngresaDato(buffer,0);					//ingreso el usuario por teclado 
	cmd = GetCMD(buffer);					//quita el carri return	
	usuario=hash_id(buffer);				//el usuario es encriptado
}while(ValidarClave(cmd)!=0);				//validamos el usuario


  do{
printf("\r\n\n/>Password:");

	IngresaDato(buffer,1);					//ingreso el password por teclado 
	cmd = GetCMD(buffer);					//quita el carri return	
	strcpy (validacion,"123456");
	//strcpy (validacion,usuario);		/*valida el valor encriptado*/

	}while(ValidarClave(cmd)!=0);				//

	
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
						case 9:  //help me
           
							Show();
               break;
						case 10:  //salir
						return;

               break;
           
		
					
            default:
               printf("\r\nComando no existe '%s'", cmd);
               break;
        }
      }
	}
}

	

