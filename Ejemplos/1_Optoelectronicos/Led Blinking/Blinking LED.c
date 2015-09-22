#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader

#define ledPin Pin_A4               //Etiqueta al Pin_A4 con el nombre de ledPin

void main()
{
   while(true)
   {
      output_bit (ledPin, 0);    //Enciende el led
      delay_ms(200);               //Espera 1 segundo
      output_bit (ledPin, 1);   //Apaga el led
      delay_ms(200);               //Espera 1 segundo
   }
}
