
#line 1 "prog.c" /0
 
  
#line 1 "prog.h" /0



 



 
 
 
 
 
 
 
 void menu(void);
 void Show();
 void DisplayCursor(void);
 void IngresaDato(unsigned char  *buffer, unsigned char modo);
 unsigned char  ValidarClave(unsigned char *buffer);
 unsigned char ListaComandos(unsigned char *cmd);
 void prog_id_cliente();
 void prog_cod_park();
 void Prog_tiempo_gracia();
 void Prog_Sin_Cobro();
 void Prog_debug();
 void Prog_Use_Lpr();
 void Prog_Comparacion_Activa();
 
 
 
 
#line 2 "prog.c" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\string.h" /0







 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 
 
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char  *strcat  (char *s1, const char *s2);
 extern char  *strncat (char *s1, const char *s2, size_t n);
 
 extern char   strcmp  (const char *s1, const char *s2);
 extern char   strncmp (const char *s1, const char *s2, size_t n);
 
 extern char  *strcpy  (char *s1, const char *s2);
 extern char  *strncpy (char *s1, const char *s2, size_t n);
 
 extern size_t strlen  (const char *);
 
 extern char  *strchr  (const char *s, char c);
 extern int    strpos  (const char *s, char c);
 extern char  *strrchr (const char *s, char c);
 extern int    strrpos (const char *s, char c);
 
 extern size_t strspn  (const char *s, const char *set);
 extern size_t strcspn (const char *s, const char *set);
 extern char  *strpbrk (const char *s, const char *set);
 extern char  *strrpbrk(const char *s, const char *set);
 extern char  *strstr  (const char *s, const char *sub);
 extern char  *strtok  (char *str, const char *set);
 
 extern char   memcmp  (const void *s1, const void *s2, size_t n);
 extern void  *memcpy  (void *s1, const void *s2, size_t n);
 extern void  *memchr  (const void *s, char val, size_t n);
 extern void  *memccpy (void *s1, const void *s2, char val, size_t n);
 extern void  *memmove (void *s1, const void *s2, size_t n);
 extern void  *memset  (void *s, char val, size_t n);
 #pragma RESTORE
 
 
#line 3 "prog.c" /0
 
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
 
 unsigned char cursor[20] = "/";  
 unsigned char validacion [9]={"admin"};	 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 char comandos[9][19]=
 {
 "ID_CLIENTE",       		 
 "COD_PARK", 	      
 "T_GRACIA",		 
 "SIN_COBRO",		 
 "DEBUG",	 
 "USE_LPR",
 "COMPARACION_ACTIVA",
 "AYUDA",      
 "SALIR"		 
 };
 


 
 
 unsigned char ListaComandos(unsigned char *cmd)
 {
 unsigned char buf[19];
 unsigned char i;
 
 for (i=0; i<9; i++)
 {
 strcpy(buf, &comandos[i][0]);
 if (strcmp(buf, cmd)==0)
 return(i);
 }
 
 return(0xFF);
 }
 
 


 
 unsigned char  ValidarClave(unsigned char *buffer)
 {
 unsigned char buf [7];
 
 strcpy(buf, &validacion[0]);
 if (strcmp(buf, buffer)==0)
 return(0);
 
 
 return(0xFF);
 }
 
 
 

 

 unsigned char * GetCMD(unsigned char *in)
 {
 char tokens[]=" \r\n";
 return(strtok(in,tokens));
 }
 


 
 unsigned char * GetOption(unsigned char *in)
 {
 unsigned char tokensSpace[]=" \r\n";
 unsigned char tokensQuote[]="\"\r\n";
 
 
 while (*in==' ')
 in++;
 
 
 if (*in == '\"')
 return(strtok(in,tokensQuote));
 else
 return(strtok(in,tokensSpace));
 } 


 
 
 void DisplayCursor(void)
 {
 printf("\r\n\n%s> ", cursor);
 }
 


 
 
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












 


 
 void prog_id_cliente()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 dataee=rd_eeprom(0xa8,0x0000);																					 
 sprintf(buffer,"%d",dataee);																									 
 
 printf("\r\n\n ACTUAL ID_CLIENTE=%s\r\n\n",buffer);														 
 
 printf("\r\n\n DIGITE SU NUEVO ID_CLIENTE=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x0000,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x0000);																					 
 sprintf(buffer,"%d",dataee);																									 
 printf("\r\n\n DATO MODIFICADO ID_CLIENTE=%s\r\n\n",buffer);									 
 }


 
 void prog_cod_park()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 dataee=rd_eeprom(0xa8,0x0002);																					 
 sprintf(buffer,"%d",dataee);																									 
 
 printf("\r\n\n ACTUAL COD_PARK=%s\r\n\n",buffer);														 
 
 printf("\r\n\n DIGITE SU NUEVO COD_PARK=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x0002,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x0002);																					 
 sprintf(buffer,"%d",dataee);																									 
 printf("\r\n\n DATO MODIFICADO COD_PARK=%s\r\n\n",buffer);									 
 }


 
 void Prog_tiempo_gracia()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 dataee=rd_eeprom(0xa8,0x0004);																					 
 sprintf(buffer,"%d",dataee);																									 
 
 printf("\r\n\n ACTUAL TIEMPO_GRACIA=%s\r\n\n",buffer);														 
 
 printf("\r\n\n DIGITE SU NUEVO TIEMPO_GRACIA=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x0004,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x0004);																				 
 sprintf(buffer,"%d",dataee);																									 
 printf("\r\n\n DATO MODIFICADO TIEMPO_GRACIA=%s\r\n\n",buffer);								 
 }



 
 void Prog_Sin_Cobro()
 {
 
 
 unsigned char buffer[10];
 unsigned int dataee;
 
 dataee=rd_eeprom(0xa8,0x0006);																					 
 sprintf(buffer,"%d",dataee);																									 
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL SIN_COBRO INHABILITADO=%s\r\n\n",buffer);														 
 }
 else if (dataee==1)
 {
 printf("\r\n\n ACTUAL SIN_COBRO GRATIS=%s\r\n\n",buffer);		
 }
 else 
 {
 printf("\r\n\n ACTUAL SIN_COBRO GRATIS Y SALIDA EL MISMO DIA=%s\r\n\n",buffer);		
 }
 
 printf("\r\n\n DIGITE SU NUEVO SIN_COBRO=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x0006,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x0006);																				 
 sprintf(buffer,"%d",dataee);																									 
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL SIN_COBRO INHABILITADO=%s\r\n\n",buffer);														 
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
 
 void Prog_debug()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 
 dataee=rd_eeprom(0xa8,0x0008);																					 
 sprintf(buffer,"%d",dataee);																									 
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL DEBUG INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL DEBUG HABILITADO=%s\r\n\n",buffer);			
 }
 
 printf("\r\n\n DIGITE EL NUEVO ESTADO DE DEBUG=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x0008,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x0008);																				 
 sprintf(buffer,"%d",dataee);	
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL DEBUG INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL DEBUG HABILITADO=%s\r\n\n",buffer);			
 }
 }
 
 void Prog_Use_Lpr()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 
 dataee=rd_eeprom(0xa8,0x000A);																					 
 sprintf(buffer,"%d",dataee);																									 
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL USE_LPR INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL USE_LPR HABILITADO=%s\r\n\n",buffer);			
 }
 
 printf("\r\n\n DIGITE EL NUEVO ESTADO DE USE_LPR=");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x000A,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x000A);																				 
 sprintf(buffer,"%d",dataee);	
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL USE_LPR INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL USE_LPR HABILITADO=%s\r\n\n",buffer);			
 }
 }
 
 void Prog_Comparacion_Activa()
 {
 unsigned char buffer[10];
 unsigned int dataee;
 
 
 dataee=rd_eeprom(0xa8,0x000C);																					 
 sprintf(buffer,"%d",dataee);																									 
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL COMPARACION ACTIVA INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL COMPARACION ACTIVA  HABILITADO=%s\r\n\n",buffer);			
 }
 
 printf("\r\n\n DIGITE EL NUEVO ESTADO DE COMPARACION ACTIVA =");																	 
 IngresaDato(buffer,0);																												 
 dataee=atoi(buffer);																													 
 wr_eeprom(0xa8,0x000C,dataee);																					 
 
 dataee=rd_eeprom(0xa8,0x000C);																				 
 sprintf(buffer,"%d",dataee);	
 if(dataee==0)
 {
 printf("\r\n\n ACTUAL COMPARACION ACTIVA  INHABILITADO=%s\r\n\n",buffer);														 
 }
 else
 {
 printf("\r\n\n ACTUAL COMPARACION ACTIVA  HABILITADO=%s\r\n\n",buffer);			
 }
 }
 
 


 
 void Show()
 {
 printf("\r\n ID_CLIENTE    --- Identificador del cliente maximo 255");
 printf("\r\n COD_PARK      --- El numero del parqueadero maximo 255");
 printf("\r\n T_GRACIA      --- Tiempo sin cobro 00,  maximo 255");
 printf("\r\n SIN_COBRO     --- Inhabilitado= 0, Gratis= 1 , Gratis y salida el mismo dia= 2");
 printf("\r\n DEBUG         --- Habilitar = 1, Inhabilitar = 0");
 printf("\r\n USE_LPR       --- Habilitar = 1, Inhabilitar = 0");
 printf("\r\n COMPARACION_ACTIVA --- Habilitar = 1, Inhabilitar = 0");
 printf("\r\n AYUDA         --- Muestra los comandos");
 printf("\r\n SALIR         --- Salir de programacion");
 
 }
 


 
 
 void menu(void)
 {
 
 unsigned char *cmd,*option1,*option2;
 unsigned char opt_buffer[40];
 unsigned char buffer[40];
 
 
 
 
 printf("\r\n\nSistema de Programacion Verificador\r\n\r\n");
 
 
 
 
 do{
 printf("\r\n\n/>Usuario:");
 
 
 IngresaDato(buffer,0);					 
 cmd = GetCMD(buffer);					 
 
 }while(ValidarClave(cmd)!=0);				 
 
 
 do{
 printf("\r\n\n/>Password:");
 
 IngresaDato(buffer,1);					 
 cmd = GetCMD(buffer);					 
 strcpy (validacion,"123456");
 
 
 }while(ValidarClave(cmd)!=0);				 
 
 
 Show();
 while(1)
 {
 DisplayCursor();
 
 
 IngresaDato(buffer,0);	
 cmd = GetCMD(buffer);	
 option1 = GetOption(cmd + strlen(cmd) + 1);
 option2 = GetOption(option1 + strlen(option1) + 1);	 
 
 
 
 
 if (option1 && (option1[0]=='/'))
 {
 
 strcpy(opt_buffer, option1);
 }
 else if (option1)
 {
 
 strcpy(opt_buffer, cursor);
 strcat(opt_buffer, option1);
 }         
 
 
 
 
 
 if (cmd)
 {
 switch(ListaComandos(cmd))
 {
 case 0:  										 
 prog_id_cliente();
 
 break;
 
 case 1:   
 prog_cod_park();
 break;
 
 case 2:   
 
 Prog_tiempo_gracia();
 
 break;
 
 case 3:   
 Prog_Sin_Cobro();
 
 break;
 
 case 4:   
 Prog_debug();
 break;
 case 5:   
 Prog_Use_Lpr();
 break;
 case 6:   
 Prog_Comparacion_Activa();
 
 break;
 case 7:   
 
 Show();
 break;
 case 8:   
 return;
 
 break;
 
 
 
 default:
 printf("\r\nComando no existe '%s'", cmd);
 break;
 }
 }
 }
 }
 
 
 
