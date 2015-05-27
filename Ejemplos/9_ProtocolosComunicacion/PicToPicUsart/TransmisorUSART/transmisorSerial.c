/*--------------------------------------------------------*\
|  Comunicación entre dos PIC's por USART. Transmisor     |                
\*--------------------------------------------------------*/
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#include <kbd2.c>  //Libreria para Teclado Matricial 4x4
#use rs232(uart1, baud=9600)//usart1 -->ajuste de XMIT y RCV para la USART 1

void main() {
      char c;
      kbd_init(); //inizializo drivers teclado
      PORT_B_PULLUPS(TRUE); //Activan resistencias de pull-up
      
      while(true)
         {
         c = kbd_getc();
         if(c != 0 )//si se ha pulsado una tecla   
         { 
            putc(c);//Envía el caracter vía serie por la USART del PIC
            delay_us(100);    //Pausa de 1 s.
         }
      }
}
