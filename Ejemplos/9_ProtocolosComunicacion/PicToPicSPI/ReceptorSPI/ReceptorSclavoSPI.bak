// COMUNICACION ISP ENTRE DOS PICS RECEPTOR ESCLAVO//
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.

#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6      //CONFIGURAMOS LOS PINES PARA LA LCD
#define LCD_DB7   PIN_D7
#define LCD_RS    PIN_C0
#define LCD_RW    PIN_C1
#define LCD_E     PIN_C2

#include <flex_lcd.c> 



void main()
{
   int16 dato = 0;
   lcd_init();
   setup_spi(spi_slave | spi_l_to_h | spi_clk_div_16);     //configurar spi como esclavo
   for(;;)
   {
      if(spi_data_is_in())      //si hay un dato en el spi
      {        
         dato=(spi_read()); 
         lcd_gotoxy(1,1);
         printf(lcd_putc," %Ld   ",dato);
         //delay_ms(200);
      }
   }
}
