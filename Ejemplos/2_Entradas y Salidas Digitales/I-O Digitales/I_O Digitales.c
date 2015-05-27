#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

void main()
{
   SET_TRIS_B (0b00000011);
   SETUP_ADC_PORTS (NO_ANALOGS); //Apagamo los analogicos para que sean digitales
   
   usbInit (); //Inicializa la comunicacion USB
   
   while (true)
   {
      if (!input (PIN_B0))
      {
        printf(usbWrite,"PIN B0 esta en bajo\n\r");  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_ms(2000);    //Pausa de 1 s.

      }
      if (!input (PIN_B1))
      {
        printf(usbWrite,"PIN B1 esta en bajo\n\r");  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_ms(2000);    //Pausa de 1 s.

      }
      
   }
}

