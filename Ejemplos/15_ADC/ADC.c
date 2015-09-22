#include <18F4550.h>
#device ADC = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta l�nea si no usas Bootloader
#include <JACSS_CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

int16 Vout=0;
float V=0.0;

void main() 
{ 
 setup_adc_ports(AN0); 
 setup_adc(ADC_CLOCK_INTERNAL); 
 usbInit();
 delay_ms(2000);
 printf(usbWrite,"Lector analogico\r\n"); 
 delay_ms(200);
 
  
while (TRUE)
  {
   set_adc_channel(0); 
   delay_ms(5); 
   Vout = Read_ADC(); 
   V = (Vout/1023.00)*5; 
   printf(usbWrite,"ADC:%Lu  V:%f\n\r",Vout,V); 
   delay_ms(200);
  }
} 

