//Programa que utiliza en timer0 para contar aproximadamente 1s
#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <stdlib.h>
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#define ledPin PIN_A4               //Etiqueta al Pin_A4 con el nombre de ledPin

int16 cont=0, segundos=0;

#int_RTCC //funcion de interrupcion por timer0
void  RTCC_isr(void)
{
 cont++; 
      if (cont >= 11) { // about 11 interrupts per second 
         segundos++; 
         cont=0; //Reinicio contador
      } 
}


void main()
{ 
   enable_interrupts(global);//Habilito interrupción USART
   enable_interrupts(INT_RTCC);
   SETUP_ADC_PORTS(NO_ANALOGS); //Apagamo los analogicos para que sean digitales
   setup_adc(ADC_OFF);
   setup_timer_0 (RTCC_DIV_16);
   delay_ms(100);

   
   while(1){
   if (segundos==180) output_bit(ledPin, 0); //si pasan 180s  3m enciendo el LED
   }  
}
