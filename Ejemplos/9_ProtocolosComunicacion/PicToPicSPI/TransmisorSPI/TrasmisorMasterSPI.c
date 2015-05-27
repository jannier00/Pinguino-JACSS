// COMUNICACION ISP ENTRE DOS PICS TRANSMISOR MASTER//
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.


void main()
{

   char dato=0;
   int16 lectura=0;

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
//CONFIGURACIÓN BUS SPI-------------------------------------------------------------
 setup_spi(spi_master | spi_l_to_h | spi_clk_div_16);//Vamos a trabajar como maestro|que funciona en activo alto
 //lcd_init();
 set_adc_channel(0);
 
 while(true)
 {
  set_adc_channel(0);
  delay_ms(200);
  lectura=read_adc();
  spi_write(lectura);//Escribir lo que haya en el pin A0
  //lcd_gotoxy(1,1);
  //printf(lcd_putc," %Ld   ",lectura);
 // delay_ms(200);
 }
}


