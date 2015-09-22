#include <18F4550.h>
#device *=16 //Use 16 bit pointers (for 14 bit parts)
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS_CDC.c>
#include <1wire.c> 
#include <ds1820.c> 

/*-------1-wire definitions-------*/ 
//#define ONE_WIRE_PIN PIN_A0 

void main() 
{ 
float temperature; 

setup_adc_ports(NO_ANALOGS); 
setup_adc(ADC_OFF); 
setup_psp(PSP_DISABLED); 
setup_spi(FALSE); 
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1); 
setup_timer_1(T1_DISABLED); 
setup_timer_2(T2_DISABLED,0,1); 
setup_comparator(NC_NC_NC_NC); 
setup_vref(VREF_LOW|-2); 

usbInit();
delay_ms(300); 

while (1) 
{ 
  temperature = ds1820_read(); 
  printf(usbWrite,"TEMP: %f \r\n", temperature); 
  delay_ms(200);
 } 

} 
