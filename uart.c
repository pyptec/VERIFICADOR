/*------------------------------------------------------------------------------
SIO.C:  Serial Communication Routines.

Copyright 1995-2002 KEIL Software, Inc.
------------------------------------------------------------------------------*/

#include <reg51.h>
#include "uart.h"


/*variables globales */
extern idata unsigned char tbuf [];
extern idata unsigned char rbuf [];
extern unsigned char g_cEstadoComSoft;
extern unsigned char xdata Buffer_Rta_Lintech[];
extern	unsigned char g_cContByteRx;
extern unsigned char ValTimeOutCom;
extern unsigned char cont_trama;
/*constantes globales*/
extern const unsigned  char ACK;
extern const unsigned  char ETX;
extern const unsigned  char STX_LINTECH;
unsigned char error_rx=0;

/*externos bits*/
extern bit buffer_ready;
/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a next_in and a next_out index.

If next_in = next_out, the buffer is empty.

(next_in - next_out) % buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/
#define TBUF_SIZE   2           /*** Must be one of these powers of 2 (2,4,8,16,32,64,128) ***/
#define RBUF_SIZE   8          /*** Must be one of these powers of 2 (2,4,8,16,32,64,128) ***/
/*definiciones de los estados de recepcion*/
#define  ESPERA_RX 					0  					//espera el primer cmd de recepcion del verificado 
#define  ESPERA_INICIO_RTA  1		// se almacena el stx
#define  LEN_DATA						2
#define  STORE_DATA					3



/*tiempo de delay entre funciones*/
#define TIME_CARD				5			//50
#define TIME_EJECT				5			//60
#define RET_MINIMO				3

#define TBUF_SIZE_LINTECH   50  
//#define XTAL   22118400
//#define TBUF_SPACE  idata       /*** Memory space where the transmit buffer resides ***/
//#define RBUF_SPACE  idata       /*** Memory space where the receive buffer resides ***/

#define CTRL_SPACE  data        /*** Memory space for the buffer indexes ***/

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif TBUF_SIZE > 128
#error TBUF_SIZE is too large.  It must be smaller than 129.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif RBUF_SIZE > 128
#error RBUF_SIZE is too large.  It must be smaller than 129.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif
*/
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
//static idata unsigned char tbuf [TBUF_SIZE];
//static idata unsigned char rbuf [RBUF_SIZE];

static CTRL_SPACE unsigned char t_in = 0;
static CTRL_SPACE unsigned char t_out = 0;

static CTRL_SPACE unsigned char r_in = 0;
static CTRL_SPACE unsigned char r_out = 0;

static bit ti_restart = 0;  /* NZ if TI=1 is required */
bit aSk=0; 									/*indica que llego el 06 = ask de que recivio el msj*/

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
static void com_isr (void) interrupt 4 
{
static xdata unsigned char cDatoRx;
xdata unsigned char k,bcc;
static xdata unsigned char num_datos;
/*------------------------------------------------
Received data interrupt.
	cada trama enviada al transporte tiene una aceptacion de un ask que si fue recivida.
	tengo dos buffer
	rbuf= guarda toda la informacion como llega
	Buffer_Rta_Lintech= que guarda la respuesta del transporte
	g_cContByteRx= contador de datos recibidos
------------------------------------------------*/
if (RI != 0)
  {
		
  RI = 0;
	cDatoRx=SBUF;
  if (((r_in - r_out) & ~(RBUF_SIZE-1)) == 0)
    {
    rbuf [r_in & (RBUF_SIZE-1)] = cDatoRx;		//cDatoRx;
    r_in++;
			
    }
		if(g_cContByteRx>TBUF_SIZE_LINTECH)
			{
						g_cEstadoComSoft=ESPERA_RX;
			}
		switch (g_cEstadoComSoft)
		{
/*------------------------------------------------------------------------------------------------
			espera el ASK de respuesta
-------------------------------------------------------------------------------------------------*/			
			case ESPERA_RX:
			
			
			if(cDatoRx==ACK)  							// espera el ask
			{
				aSk=1;
			g_cContByteRx=0;r_in=0;r_out=0;	
			g_cEstadoComSoft=ESPERA_INICIO_RTA;
			
			}
		break;
/*------------------------------------------------------------------------------------------------
			se almacena la trama 
-------------------------------------------------------------------------------------------------*/
			case ESPERA_INICIO_RTA:
			{	
				Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
				if(Buffer_Rta_Lintech[0]==STX_LINTECH)
				{						
					g_cEstadoComSoft=LEN_DATA;
				}
				else
				{
					g_cEstadoComSoft=ESPERA_RX;
				}
			}
			break;
/*------------------------------------------------------------------------------------------------
			se toma la longitud de la trama a recibir y se le suman 2 caracteres ETX y BCC
-------------------------------------------------------------------------------------------------*/
		case LEN_DATA:
			
		if (g_cContByteRx==3)
		{
			
			num_datos=cDatoRx+2;
			Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
			g_cEstadoComSoft=STORE_DATA;			//numero de datos a recibir
		}	
		else
		{			
			Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;
			g_cEstadoComSoft=LEN_DATA;
		}

		break;
/*------------------------------------------------------------------------------------------------
		se almacena los datos 
/*-------------------------------------------------------------------------------------------------*/
		case STORE_DATA:
			
				Buffer_Rta_Lintech[g_cContByteRx++]=cDatoRx;	
				num_datos--;
 				if (num_datos==0)
				{
					
					if(Buffer_Rta_Lintech[g_cContByteRx-2]==ETX)
					{
						bcc=0;
						for (k=0; k<g_cContByteRx-1; k++)
						{
							bcc=Buffer_Rta_Lintech[k]^bcc;
						}
							if (bcc==Buffer_Rta_Lintech[g_cContByteRx-1])	
							{
								aSk=0;
								error_rx=0;
								buffer_ready=1;
								g_cEstadoComSoft=ESPERA_RX;											/* bcc ok trama valida*/
													
							}
							else
							{
								g_cEstadoComSoft=ESPERA_RX;											/* bcc no concuerda  trama no valida*/
							}
					}	
					else 
					{
						g_cEstadoComSoft=ESPERA_RX;													/*  no concuerda  ETX en la trama no valida*/
					}	
								
				}
				else 
				{
					g_cEstadoComSoft=STORE_DATA;													/* espera datos*/
				}
 			 
		break;		

/*------------------------------------------------------------------------------------------------
		
/*-------------------------------------------------------------------------------------------------*/				
		default:
			g_cEstadoComSoft=ESPERA_RX;
		break;
		}
  }

/*------------------------------------------------
Transmitted data interrupt.
------------------------------------------------*/
if (TI != 0)
  {
  TI = 0;

  if (t_in != t_out)
    {
    SBUF = tbuf [t_out & (TBUF_SIZE-1)];
    t_out++;
    ti_restart = 0;
    }
  else
    {
    ti_restart = 1;
    }
  }

}
/*
void tx_chr (unsigned char data_com)
 {

  SBUF=data_com;
	sendactive=1;
	while (sendactive==1) 
	{
	}
 }
*/
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

void com_initialize (void)
{
/*------------------------------------------------
Setup TIMER1 to generate the proper baud rate.
------------------------------------------------*/
com_baudrate ();

/*------------------------------------------------
Clear com buffer indexes.
------------------------------------------------*/
t_in = 0;
t_out = 0;

r_in = 0;
r_out = 0;

/*------------------------------------------------
Setup serial port registers.
------------------------------------------------*/
SM0 = 0; SM1 = 1;   /* serial port MODE 1 */
SM2 = 0;
REN = 1;            /* enable serial receiver */

RI = 0;             /* clear receiver interrupt */
TI = 0;             /* clear transmit interrupt */
ti_restart = 1;

ES = 1;             /* enable serial interrupts */
PS = 1;             /* set serial interrupts to low priority */
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

void com_baudrate ()
  
{
/*------------------------------------------------
Clear transmit interrupt and buffer.
------------------------------------------------*/
TI = 0;             /* clear transmit interrupt */
t_in = 0;           /* empty transmit buffer */
t_out = 0;

/*------------------------------------------------
Set timer 1 up as a baud rate generator.
------------------------------------------------*/
TR1 = 0;            /* stop timer 1 */
ET1 = 0;            /* disable timer 1 interrupt */

PCON |= 0x80;       /* 0x80=SMOD: set serial baudrate doubler */

TMOD &= ~0xF0;      /* clear timer 1 mode bits */
TMOD |= 0x20;       /* put timer 1 into MODE 2 */

TH1 =0xf4;// (unsigned char) (256 - (XTAL / (16L * 12L * baudrate)));
TL1=0xf4;
TR1 = 1;            /* start timer 1 */
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

char com_putchar (
  unsigned char c)
{
/*------------------------------------------------
If the buffer is full, return an error value.
------------------------------------------------*/
if (com_tbuflen () >= TBUF_SIZE)
  return (-1);

/*------------------------------------------------
Add the data to the transmit buffer.  If the
transmit interrupt is disabled, then enable it.
------------------------------------------------*/
tbuf [t_in & (TBUF_SIZE - 1)] = c;
t_in++;

if (ti_restart)
  {
  ti_restart = 0;
  TI = 1;               /* generate transmit interrupt */
  }

return (0);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

int com_getchar (void)
{
if (com_rbuflen () == 0)
  return (-1);

return (rbuf [(r_out++) & (RBUF_SIZE - 1)]);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

unsigned char com_rbuflen (void)
{
return (r_in - r_out);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable

unsigned char com_tbuflen (void)
{
return (t_in - t_out);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*
void clean_tx()
{
	t_in = 0;
	t_out = 0;
}
*/
/*
void tx_chr (unsigned char data_com)
 {

	 
	 tbuf [t_in & (TBUF_SIZE - 1)]=data_com
	 t_in++;
	  if (t_in != t_out)
    {
    SBUF = tbuf [t_out & (TBUF_SIZE-1)];
    t_out++;
    ti_restart = 0;
    }
	 
	 
	 
 	SBUF=data_com;
	ti_restart=1;
	while (ti_restart==1) 
	{
	}
 }}*/
