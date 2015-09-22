#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <Arduino_Functions.c>

/*
   convert255to100(_value);
   
   Recibe una variable con valores entre 0-255 y convierte los valores en un
   rango entre 0-100.
   Esta función es particularmente útil para asignar valores de porcentaje PWM
*/
int8 convert255to100(int8 _value)
{
   return((_value*100.0)/255.0);
}
/******************************************************************************/

/*
   convertDECtoVolt(_value)
   Recibe una variable con valores entre 0-255 y lo convierte a su equivalente
   en voltaje entre 0V y 5V. Util para variables que contengan valores analógicos
*/
float convertDECtoVolt(float _value)
{
   _value = (_value*5)/255;
   return(_value);
}
/******************************************************************************/

/*
   convertDECtoDegree(_value)
   Convierte el valor analogico leido de un LM35 y lo convierte en su equivalente
   en grados.
*/
float convertDECtoDegree(float _value)
{
   _value = 100*(_value*5)/255;
   return(_value);
}
/******************************************************************************/
