/*--------------------------------------------------------*\
|  Comunicación entre dos PIC's por USART. Receptor        |                
\*--------------------------------------------------------*/
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
//#include <kbd2.c>  //Libreria para Teclado Matricial 4x4
#use rs232(uart1, baud=9600)//usart1 -->ajuste de XMIT y RCV para la USART 1
char dato;

#int_rda
void rd_isr(void)//función de interrupción por recepción de datos USART              
   {
   dato= getc();
   if (dato!=0){   //si la variable es diferente de 0
      printf(usbWrite,"%c",dato);  //Salto de linea " \n "  Retorno de cursor " \r "
      }
   }


void main()
   {
   
   enable_interrupts(global);//Habilito interrupción USART
   enable_interrupts(int_rda);
   usbInit();     //Inicializa la comunicacion


   while(true){     
      //Código principal   
      } 
}
