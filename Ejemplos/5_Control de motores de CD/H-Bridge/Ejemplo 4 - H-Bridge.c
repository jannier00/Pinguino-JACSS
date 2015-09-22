/*
   H-Bridge
   Muestra una forma básica de utilizar el Puente H para encender y apagar un 
   motor de CD. El motor enciende 3 segundos y se apaga 3 segundos
   
   Hardware
   Pinguino JACSS       L293D o SN754410 
   Pin_C1            Pin 1
   Pin_B0            Pin 2
   Pin_B1            Pin 7
                     Pin 8 - Conectado a 5V de la G-PiC Lite!
                     Pin 16 - Conectado a fuente de voltaje externa @ 5V
   
   Importante: Verificar la hoja de datos del puente H para saber cómo 
   conectarlo correctamente
*/
#include <Cabecera_JACSS.c>

#define Enable12  Pin_C1         //PWM por hardware en el Pin_C1
#define motor1A   Pin_B0         //Pin de control del puente H
#define motor2A   Pin_B1         //Pin de control del puente H

#define speed     50                //Constante para guardar la velocidad

void main()
{
   digitalWrite(motor1A, 1);        //Define el sentido de giro del motor
   digitalWrite(motor2A, 0);
   
   while(true)
   {
      analogWrite(Enable12, speed); //Enciende el motor
      delay_ms(3000);
      analogWrite(Enable12, 0);     //Apaga el motor
      delay_ms(3000);
   }
}
