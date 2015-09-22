#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS_CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#define tiempo 800

void main()
{
   int16 i;
   usbInit();     //Inicializa la comunicacion
   
    while(true)
   {       
   
      for (i=0; i<1023; i++){
      //Imprime en la PC el valor leido 
      printf(usbWrite," %Lu \n\r",i);  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_us(tiempo);    //Pausa de 1 s.
      }
      
      for (i=1023; i>0; i--){
      //Imprime en la PC el valor leido 
      printf(usbWrite," %Lu \n\r",i);  //Salto de linea " \n "  Retorno de cursor " \r "
      delay_us(tiempo);    //Pausa de 1 s.
      }
   }
}

