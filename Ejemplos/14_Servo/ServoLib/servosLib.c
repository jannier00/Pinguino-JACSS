#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#define servo PIN_B0    //define pin de saida para servo
#include <servoSof.c>  //Libreria para usar servo

int8 grados=0;

void main()
{
   SET_TRIS_A(0b00010000);    //Ponemos PIN_A4 como entrada
   initServo(); //Funcion para iniciar servo
   while(TRUE)
   {
         setServo(grados); //Funcion para mover servo en grados
      }
  
   }
      
}
