#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader

#define servo PIN_B0 
#define RMIN (int16)550
#define RMAX (int16)2450 
 
int count=0, i;
int16 retardo=1500, grados=0;
 
#int_TIMER2
void  TIMER2_isr(void) 
{
   count++;
   retardo=500 + ((2000/180)*grados); //
   if (retardo>RMAX)  retardo=RMAX;
   if (retardo<RMIN)  retardo=RMIN;
   if (count==2) {
      output_high(servo);
      delay_us(retardo);
      output_low(servo);
      count=0;
   } 
}
 
void main()
{
   SET_TRIS_A(0b00010000);    //Ponemos PIN_A4 como entrada
   setup_timer_2(T2_DIV_BY_16,196,16);      //630 us overflow, 10.0 ms interrupt
   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);
   while(TRUE)
   {
      if (!input(PIN_A4)){  //Si PIN A4 esta en bajo
         while (!input(PIN_A4));
         grados=grados + 5;
      }
  
   }
      
}
