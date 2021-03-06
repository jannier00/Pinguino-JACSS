/* 
This library includes the definitions to translate the most common Arduino
functions to the CCS built-in functions. To see the reference of each
instruction please refer to the CCS Help File for each CCS functions.

Esta librer�a incluye las definiciones para traducir las funciones de Arduino
m�s comunes a las funciones incluidas en CCS. Para obtener la referencia de cada 
funci�n de CCS favor de ver el archivo Help del CCS.
*/

/* Additional libraries */
#include <analogWrite.c>

/* Reserved words and names definitions */
#define digitalToggle   output_toggle
#define digitalWrite    output_bit
#define digitalRead     input

//****************************************************************************//
//                               analogRead(_Channel)
//Lee cualquiera de las entradas anal�gicas de la G-PiC Lite!. No es necesario
//poner un delay entre cada lectura, pues este tiempo est� incluido dentro de la
//funci�n.
//
//NOTA: Se emplea para lecturas de 8 bits de resolucion
//****************************************************************************//
#define AN0  0
#define A1  1
#define A2  2
#define A3  3
#define A4  4
#define A5  5
#define A6  6
#define A7  7
#define A8  8
#define A9  9
#define A10 10
#define A11 11
#define A12 12

int8 analogRead(int8 _Channel)
{
   int8 _adcReading;
   
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(ALL_ANALOG);
   set_adc_channel(_Channel);
   delay_us(10);
   
   _adcReading = read_adc();
     
   setup_adc_ports(NO_ANALOGS);
   return(_adcReading);
}



