#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

int8 i;

void main()
{

   usbInit();     //Inicializa la comunicacion
   
    while(true)
   {  
   
      for (i=0; i<10; i++) {
      //Imprime en la PC el valor leido 
      printf(usbWrite,"Numero: %d\n\r",i);  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_ms(2000);    //Pausa de 1 s.
      }
   }
}

