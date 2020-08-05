
#line 1 "eeprom.c" /0
 
  
#line 1 "eeprom.h" /0


 



 
 
 
 
 
 
 
 
 
 
 
 void ackd();
 void ack_lect(void);
 void p_me (void);
 void e_d_m (unsigned char a_serial_eeprom);
 void start (void);
 void stop (void);	
 unsigned char l_d_m (void);
 void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom);
 void wrpage_eeprom (unsigned char control,unsigned char dir_h,unsigned char dir_l,unsigned char data_eeprom); 
 unsigned char rd_eeprom (unsigned char control,unsigned int Dir); 
 
 
 
#line 2 "eeprom.c" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\reg51.h" /0






 
 
 
 
 
 
 sfr P0   = 0x80;
 sfr P1   = 0x90;
 sfr P2   = 0xA0;
 sfr P3   = 0xB0;
 sfr PSW  = 0xD0;
 sfr ACC  = 0xE0;
 sfr B    = 0xF0;
 sfr SP   = 0x81;
 sfr DPL  = 0x82;
 sfr DPH  = 0x83;
 sfr PCON = 0x87;
 sfr TCON = 0x88;
 sfr TMOD = 0x89;
 sfr TL0  = 0x8A;
 sfr TL1  = 0x8B;
 sfr TH0  = 0x8C;
 sfr TH1  = 0x8D;
 sfr IE   = 0xA8;
 sfr IP   = 0xB8;
 sfr SCON = 0x98;
 sfr SBUF = 0x99;
 
 
 
 
 sbit CY   = 0xD7;
 sbit AC   = 0xD6;
 sbit F0   = 0xD5;
 sbit RS1  = 0xD4;
 sbit RS0  = 0xD3;
 sbit OV   = 0xD2;
 sbit P    = 0xD0;
 
 
 sbit TF1  = 0x8F;
 sbit TR1  = 0x8E;
 sbit TF0  = 0x8D;
 sbit TR0  = 0x8C;
 sbit IE1  = 0x8B;
 sbit IT1  = 0x8A;
 sbit IE0  = 0x89;
 sbit IT0  = 0x88;
 
 
 sbit EA   = 0xAF;
 sbit ES   = 0xAC;
 sbit ET1  = 0xAB;
 sbit EX1  = 0xAA;
 sbit ET0  = 0xA9;
 sbit EX0  = 0xA8;
 
 
 sbit PS   = 0xBC;
 sbit PT1  = 0xBB;
 sbit PX1  = 0xBA;
 sbit PT0  = 0xB9;
 sbit PX0  = 0xB8;
 
 
 sbit RD   = 0xB7;
 sbit WR   = 0xB6;
 sbit T1   = 0xB5;
 sbit T0   = 0xB4;
 sbit INT1 = 0xB3;
 sbit INT0 = 0xB2;
 sbit TXD  = 0xB1;
 sbit RXD  = 0xB0;
 
 
 sbit SM0  = 0x9F;
 sbit SM1  = 0x9E;
 sbit SM2  = 0x9D;
 sbit REN  = 0x9C;
 sbit TB8  = 0x9B;
 sbit RB8  = 0x9A;
 sbit TI   = 0x99;
 sbit RI   = 0x98;
 
 
#line 3 "eeprom.c" /0
 
 
 
 extern void          _nop_     (void);
 extern void Delay (void);
 extern void Delay_20us(unsigned int cnt);
 extern void Delay_1ms(unsigned int cnt);
 
 
 
 sbit sda = P1^2;
 sbit scl = P1^3;
 
 
 bit memory_flag;
 unsigned char byte_rd;
 unsigned char l_data_eeprom;
 unsigned char l_chr;
 
 
 
 
 
 
 void ackd(void)
 {
 unsigned int i;
 memory_flag=1;
 scl=1;
 sda=1;
 for (i=0; i<1100; i++)					    
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
 
 void p_me (void)
 {
 scl=1;
 _nop_();
 scl=0;
 }
 
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
 
 void start (void)
 {
 sda=1;
 scl=1;
 _nop_();
 sda=0;
 scl=0;
 return;
 }
 
 void stop (void)
 {
 scl=0;
 sda=0;
 scl=1;
 sda=1;
 return;
 }
 
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
 
 
 
 
 
 void wr_eeprom (unsigned char control,unsigned int Dir, unsigned char data_eeprom)
 {
 unsigned char dir_h, dir_l;
 dir_l=Dir;
 Dir>>=8;
 dir_h=Dir;
 
 
 scl=0;
 sda=0;
 
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
 Delay_20us(98);										 
 Delay_20us(98);
 }
 
 
 



























 
 
 
 
 
 
 unsigned char rd_eeprom (unsigned char control,unsigned int Dir) 
 {
 
 unsigned char dir_h, dir_l;
 dir_l=Dir;
 Dir>>=8;
 dir_h=Dir;
 
 scl=0;
 sda=0;
 
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
