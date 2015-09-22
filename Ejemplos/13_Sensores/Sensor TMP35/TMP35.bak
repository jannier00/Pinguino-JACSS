#include <18F4550.h>
#device ADC = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

int16 Vout=0; 
float  Temp=0.0;
void temp_sensor(int8 j) 
{ 
 set_adc_channel(j); 
 delay_ms(5); 
 Vout = Read_ADC();  
 Temp = (((float)Vout*5)/1023) * 100.0; // div by 1024/5 (=204.8) for 10 bit 
 //Temp = (Temp - 32) * (0.55555); 
 printf(usbWrite,"V:%Lu T:%.2f     ",Vout, Temp);
 return;
}


void main() 
{ 
 setup_adc_ports(AN0_TO_AN1); 
 setup_adc(ADC_CLOCK_INTERNAL); 
 usbInit();
 delay_ms(2000);
 printf(usbWrite,"Starting TMP35\r\n"); 
 delay_ms(200);
 
  
while (TRUE)
  {
   temp_sensor(0);
   delay_ms(1000);
   temp_sensor(1);
   delay_ms(1000);
   printf(usbWrite,"\r\n\n");
   delay_ms(200);
  }
} 

