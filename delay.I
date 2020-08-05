
#line 1 "delay.c" /0


 





 
 
  
#line 1 "delay.h" /0


 



 
 
 
 
 
 void Delay_20us(unsigned int cnt);
 void Delay_1ms(unsigned int cnt);
 void Delay_10ms(unsigned int cnt);
 void Delay (void);
 
 
 
#line 11 "delay.c" /0
 
 






 
 void Delay_20us(unsigned int cnt)
 {
 unsigned int i;
 
 for(i = 0; i<cnt; i++) {
 
 }
 }
 
 







 
 void Delay_1ms(unsigned int cnt)
 {
 unsigned int i;
 for (i = 0; i < cnt; i++){ Delay_20us(150);}
 }
 








 






 
 void Delay (void)  {                   			 
 ;                                   			
 }
 
