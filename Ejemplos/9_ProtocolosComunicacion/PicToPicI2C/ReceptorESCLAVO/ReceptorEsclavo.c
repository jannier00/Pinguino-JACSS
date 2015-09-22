////////////////////////////////////////////////////////////////////////////////
//                    COMUNICACIÓN I2C EN TRE DOS PIC                         //
//                                                                            //
//                      Aplicación para PIC ESCLAVO                           //
//                                                                            //
//       El PIC maestro lee teclado y envía el dato al PIC esclavo via I2C    //
//                      que lo mostrará en un lcd.                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#use standard_io(c)
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
//#include <kbd2.c>  //Libreria para Teclado Matricial 4x4
#use I2C(SLAVE, SDA=PIN_B0 ,SLOW, SCL=PIN_B1, ADDRESS=0xA0, FORCE_HW)

#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6      //CONFIGURAMOS LOS PINES PARA LA LCD
#define LCD_DB7   PIN_D7
#define LCD_RS    PIN_C0
#define LCD_RW    PIN_C1
#define LCD_E     PIN_C2
#include <flex_lcd.c> 

void main() {

   int dato;
   lcd_init();
   printf(lcd_putc,  "INICIANDO...");
   lcd_gotoxy(2,2);
   delay_ms(200);

   while (1) {
   
         // Recepción por comunicación I2C 
         
         if ( i2c_poll( )) {
            dato=i2c_read();
            lcd_putc(dato);                  //Muestra dato recibido por pantalla
         if (dato=='*') lcd_putc("\f");   //Si es * borra la pantalla lcd
         }
   }
}

