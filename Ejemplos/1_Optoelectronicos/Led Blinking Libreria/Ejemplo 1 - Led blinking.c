/*
   Led blinking
   Enciende un led por 1 segundo, luego lo apaga por 1 segundo, de forma
   repetitiva
*/
#include <Cabecera_JACSS.c>             //Incluye las librerias necesarias

#define ledPin Pin_A4               //Etiqueta al Pin_A4 con el nombre de ledPin

void main()
{
   while(true)
   {
      digitalWrite(ledPin, 0);    //Enciende el led
      delay_ms(500);               //Espera 1 segundo
      digitalWrite(ledPin, 1);   //Apaga el led
      delay_ms(500);               //Espera 1 segundo
   }
}
