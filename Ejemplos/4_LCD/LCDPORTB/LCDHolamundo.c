////////////////////////////////////////////////////////////////////////////////
/// PROGRAMA PARA VISUALIZAR DATOS EN LA LCD USANDO LA LIBREIRA   lcd.h      ///
///     LCD_ENABLE_PIN   PIN_B0                                              ///
///     LCD_RS_PIN       PIN_B1                                              ///
///     LCD_RW_PIN       PIN_B2     //CONEXION PANTALLA PUERTO B             ///
///     LCD_DATA4        PIN_B4                                              ///
///     LCD_DATA5        PIN_B5                                              ///
///     LCD_DATA6        PIN_B6                                              ///
///     LCD_DATA7        PIN_B7                                              ///
////////////////////////////////////////////////////////////////////////////////
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <lcd.c> 

void main() {

   lcd_init();
   printf(lcd_putc,"\f");  //BORRAMOS PANTALLA

   while (1) {
      lcd_gotoxy(1,1);
      printf(lcd_putc,"HOLA MUNDO");
      delay_ms(200);
    }
   }

