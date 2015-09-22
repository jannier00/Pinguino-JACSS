////////////////////////////////////////////////////////////////////////////////
/// PROGRAMA PARA VISUALIZAR DATOS EN LA LCD USANDO LA LIBREIRA  flex_lcd.h  ///
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
#include <usb/usb_bootloader.h>//Comenta esta l�nea si no usas Bootloader

#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6      //CONFIGURAMOS LOS PINES PARA LA LCD
#define LCD_DB7   PIN_D7
#define LCD_RS    PIN_C0
#define LCD_RW    PIN_C1
#define LCD_E     PIN_C2

#include <flex_lcd.c> 

void main() {

   lcd_init();
   printf(lcd_putc,"\f");  //BORRAMOS PANTALLA
   delay_ms(200);

   while (1) {
      lcd_gotoxy(1,1);
      printf(lcd_putc,"HOLA");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"MUNDO");
      delay_ms(200);
    }
   }

