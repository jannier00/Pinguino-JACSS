/*
   Led fading
   Controla la intensidad de brillo de un led mediante una salida PWM. El led
   de la placa enciende al brillo m�ximo y se atenua hasta apagarse, una 
   y otra vez.
   
   Los pines de salida PWM pueden ser configurados en la libreria 
   <analogWrite.c>.
   Este ejempo puede ser usado y modificado para cualquier aplicaci�n que 
   requiera PWM.
*/

#include <Cabecera_JACSS.c>

#define ledPin Pin_A4            //Define una etiqueta para el pin del led
int8 duty;                   //Declara una variable para el ciclo de trabajo

void main()                      //Inicia el programa principal
{
   //digitalWrite(ledPin, 0);      //Enciende el led
   //delay_ms(2000);
   
   while(true)
   {
      
      for (duty=0; duty < 100; duty++){ 
      analogWrite(11,duty);      //Habilita la salida PWM con un valor de 0 a 100
      delay_ms(100);             //Espera 100ms}  
      }
      for (duty=100; duty >0; duty--){ 
      analogWrite(11,duty);      //Habilita la salida PWM con un valor de 0 a 100
      delay_ms(100);             //Espera 100ms}  
   }

}
}

