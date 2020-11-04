#ifndef _LINTECH_H_
#define _LINTECH_H_

/*funciones prototipo  */

void Inicializa(unsigned char TipoMovimiento);
void Check_Status(unsigned char Detalle);
void Aut_Card_check_Status(void);
void Mov_Card(unsigned char Posicion);
void SecuenciaExpedidor(void);
void Card_Insercion(char Tipo);
void Dwload_EEprom (void);
void LoadVerify_EEprom(void);
void RD_MF(unsigned char Sector, unsigned char Bloque);
void WR_MF(unsigned char Sector, unsigned char Bloque,unsigned char *buffer);
void Unique_Identifier_UID(void);
void Power_off(void);
void Clave_Seguridad_S2(void);
#endif	/* _lintech_H_ */