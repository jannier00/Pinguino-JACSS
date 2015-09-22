#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS_CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

void main()
{
   SET_TRIS_A(0b00010000);    //Ponemos PIN_A4 como entrada
   SET_TRIS_B(0b00010000);
   OUTPUT_A(0x00); //Limpimos puerto A poniendolo a 0
   OUTPUT_B(0x00); //Limpimos puerto B poniendolo a 0
   SETUP_ADC_PORTS(NO_ANALOGS); //Apagamo los analogicos para que sean digitales
     
   usbInit();     //Inicializa la comunicacion USB
   
    while(true)
   { 
      output_b(0b00000011); //Ponemos en alto B0 y B1
      output_high(PIN_B2);  //Ponemos en alto B2
      output_bit(PIN_B3, 1); //Ponemos en alto B3
      if (!input(PIN_A4)){  //Si PIN A4 esta en bajo
         printf(usbWrite,"\rPIN A4 esta en bajo \r\n");  //Imprime en consola
         printf(usbWrite,"PUERTO B ESTATUS\r\n 0x%X \r\n", input_b()); //Leemos puerto B y lo imprimimos en cosola        
         delay_ms(1000);  // Retardo de 1s.
      }
   }
}

