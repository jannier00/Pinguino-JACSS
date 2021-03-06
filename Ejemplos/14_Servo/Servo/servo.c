#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta l�nea si no usas Bootloader
#define servo PIN_B0

int8 i=0;

void main(){

   while (true) {
      // Mover a 0�
      for (i=0; i<100; i++){
      output_high(servo);
      delay_us(550);
      output_low(servo);
      delay_ms(20);
      }
      
      // Mover a 90�
      for (i=0; i<100; i++){
      output_high(servo);
      delay_us(1500);
      output_low(servo);
      delay_ms(20);
      }
      
      // Mover a 180�
      for (i=0; i<100; i++){
      output_high(servo);
      delay_us(2450);
      output_low(servo);
      delay_ms(20);
      }
   }
}
