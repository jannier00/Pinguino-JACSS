#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#include <kbd2_1.c>  //Libreria para Teclado Matricial 4x4

void main()
{
   char c;    //variable para almacenar letras
   usbInit();     //Inicializa la comunicacion
   kbd_init();
   PORT_B_PULLUPS(TRUE); //Activan resistencias de pull-up
   
    while(true)
   { 
      c = kbd_getc(); //leer variable del teclado
      if (c!=0){   //si la variable es diferente de 0
      printf(usbWrite,"%c",c);  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_us(1000);    //Pausa de 1 s.
      }
   }
}

