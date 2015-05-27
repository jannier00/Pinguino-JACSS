/*
   lightsOn
   Muestra como encender una lampara incandesdence al presionas un boton. Este
   ejemplo utiliza un integrado ULN2803 y un relevador para conectar y 
   desconectar la lampara de la toma de corriente alterna.
   Cuando el boton es presionado, enciende la lámpara por 3 segundos y luego la
   apaga.
   
   Hardware
   Pinguino JACSS    ULN2803
   Pin_B0            Pin 1
   GND               Pin 9
                     Pin 10 - Conectado a fuente de voltaje externa @ 5V
                     Pin_18 - Conectado a una terminal de la bobina de relevador
   Pin_A4            Push button a Vcc con resistencia de pull-down                     
                     
   -  Conectar la segunda terminal de la bobina del relevador a la fuente
      de voltaje externa @ 5V
   -  Conectar la terminal comun del relevador a una terminal de la lampara
   -  Conectar la otra terminal de la lampara al neutro de la toma de CA
   -  Conectar la terminal NO (Normally Open) a la fase de la toma de CA 
*/
#include <Cabecera JACSS.c>

#define buttonPin Pin_A4
#define lampPin   Pin_B0

void main()
{
   while(true)
   {
      //Si se presiona el boton del Pin_A4, enciende la lampara por 3 segundos
      //y luego la apaga
      if(digitalRead(buttonPin) == 0)   
      {
         digitalWrite(lampPin, 1);
         delay_ms(3000);
         digitalWrite(lampPin, 0);
      }
   }
}
