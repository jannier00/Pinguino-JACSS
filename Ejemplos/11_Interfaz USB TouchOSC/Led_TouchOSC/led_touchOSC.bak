#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#define ledPin PIN_A4

char message;     //  This will hold one byte of the serial message

void main() {  
   SET_TRIS_A(0x00);    //Ponemos todo como salida
   OUTPUT_B(0b00010000); //Limpimos puerto B poniendolo a 0
   SETUP_ADC_PORTS(NO_ANALOGS); //Apagamo los analogicos para que sean digitales
   usbInit();     //Inicializa la comunicacion

   while(1){
      if (usbAvailable() > 0) { //  Check if there is a new message
         message = usbRead();    //  Put the serial input into the message

         if (message == 'r'){  //  If a capitol R is received...
         output_high (ledPin); //redLED = 255;       //  Set redLED to 255 (on)
         }
         if (message == 'R'){  //  If a lowercase r is received...
         output_low (ledPin);         //  Set redLED to 0 (off)
         }

      }   

   }
}
