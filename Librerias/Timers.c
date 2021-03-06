//                  ** Timers.c **                    //
// Esta libreria permite una configuracion sencilla   //
// de los Timer 0, 1 con la funcion:             //
//                                                    //
// setupTimer(_timer, milliseconds, us mode);         //

/*
   _timer         Timer a utilizar. Puede ser 0 o 1
   _milliseconds  Tiempo de interrupci�n en ms
   us mode        Indica que el tiempo de interrupcion
                  ser� en us.

   Esta funcion no configura los timers 2 y 3 debido 
   a que estos son empleados para el PWM.
*/

// Retorna 1 se se configuro correctamente el timer   //
// Retorna 0 si hubo error.                           //
//                                                    //
// Para hacer funcionar las interrupciones se         //
// requieren los siguientes #DEFINE:                  //
//                                                    //
// #DEFINE FOSC <Valor Reloj Interno en Hertz/4>      //
//    Ejemplo: Para un reloj de 48MHz de pone:        //
//             #DEFINE FOSC 12000000                  //
//                                                    //
// #DEFINE TIMER# <timer a usar>                      //
//       Ejemplo: #DEFINE TIMER0                      //
//                #DEFINE TIMER3                      //
//                                                    //
// Adicionalmente el codigo a ejecutar al momento     //
// de generarse la interrupcion debera estar          //
// declarado dentro de una funcion correspondiente    //
// al timer:                                          //
//                                                    //
// void interruptTimer#(){                            //
//    ...;                                            //
//    ...;                                            //
//    ...;                                            //
// }                                                  //
////////////////////////////////////////////////////////



#ifndef _Configure_Timers_h
#define _Configure_Timers_h

#include <stdlib.h>

#ifndef FOSC
#define FOSC getenv("CLOCK")/4
#endif

unsigned int16 _timer0Set, _timer1Set;

#ifdef TIMER0
void interruptTimer0();
#INT_TIMER0
void _timer0_int()
{
   set_timer0(_timer0Set);
   interruptTimer0();
}
#endif

#ifdef TIMER1
void interruptTimer1();
#INT_TIMER1
void _timer1_int(){
   set_timer1(_timer1Set);
   interruptTimer1();
}
#endif

int1 setupTimer (unsigned int8 _timer, float _miliseconds, int1 _micro = 0)
{
   unsigned int i;
   if(_miliseconds == 0){
      if (_timer == 0){
         setup_timer_0(RTCC_OFF);
         return 1;
      }
      else if (_timer == 1){
         setup_timer_1(T1_DISABLED);
         return 1;
      }
      else if (_timer == 3){
         setup_timer_3(T3_DISABLED);
         return 1;
      }
      else return 0;
   }
   if(_micro) _miliseconds = 1000000.0/_miliseconds;
   else _miliseconds = 1000.0/_miliseconds;
   if(_miliseconds > FOSC) return 0;
   _miliseconds = (FOSC * 1.0)/(_miliseconds);
   enable_interrupts(GLOBAL);
   switch(_timer)
   {
      case 0:
         enable_interrupts(INT_TIMER0);
         for(i=1; _miliseconds > 65535; i++) _miliseconds/=2;
         if (i == 1) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
         else if (i == 2) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
         else if (i == 3) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);
         else if (i == 4) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_8);
         else if (i == 5) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_16);
         else if (i == 6) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);
         else if (i == 7) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64);
         else if (i == 8) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_128);
         else if (i == 9) setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
         else return 0;
         _timer0Set = 65535 - _miliseconds;
         set_timer0(_timer0Set);
         break;
      case 1:
         enable_interrupts(INT_TIMER1);
         for(i=1; _miliseconds > 65535; i++) _miliseconds/=2;
         if (i == 1) setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
         else if (i == 2) setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
         else if (i == 3) setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);
         else if (i == 4) setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
         else return 0;
         _timer1Set = 65535 - _miliseconds;
         set_timer1(_timer1Set);
         break;
      default:
         return 0;
         break;
   }
   return 1;
}

#endif
