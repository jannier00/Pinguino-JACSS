//***** Libreria para control de un ServoMotor por interrupcion por timer*****//
//    Funciones:                                                             //
//    initServo()     para incializar servo                                 //
//    setServo(int grados)   Recibe los grados para ubicar servo             //
//    Creada por: Ing. Jannier I. Abreu Bernat                            //
//***********************************************************************//
//#define servo PIN_B0    declara señal para servo 
#define RMIN (int16)550
#define RMAX (int16)2450 

int count_123=0;
int16 retardo_123=1500, grados_123=0;

#int_TIMER2
void  TIMER2_isr(void) 
{
   count_123++;
   retardo_123=500 + ((2000/180)*grados_123); //
   if (retardo_123>RMAX)  retardo_123=RMAX;
   if (retardo_123<RMIN)  retardo_123=RMIN;
   if (count_123==2) {
      output_high(servo);
      delay_us(retardo_123);
      output_low(servo);
      count_123=0;
   } 
}

void initServo(){
   setup_timer_2(T2_DIV_BY_16,196,16);      //630 us overflow, 10.0 ms interrupt
   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);
   return;
}

void setServo(int8 grad){
   grados_123=grad;
   return;
}
