/*
   dcMotorControl
   Controla el sentido de giro de un motor de CD con un push buttons. El PWM
   se configura al inicio para la velocidad deseada.
   
   Hardware
   Pinguino JACSS      SN754410 o L293D
   Pin_C1            Pin 1
   Pin_B0            Pin 2
   Pin_B1            Pin 7
                     Pin 8 - Conectado a 5V de la G-PiC Lite!
                     Pin 16 - Conectado a fuente de voltaje externa @ 5V
   
   Pin_A4            Push button a Vcc con resistencia de pull-down
   
   Importante: Verificar la hoja de datos del puente H para saber cómo 
   conectarlo correctamente
*/
#include <Cabecera JACSS.c>

#define buttonPin    Pin_A4

#define Enable12  Pin_C1         //PWM por hardware en el Pin_C1
#define motor1A   Pin_B0         //Pin de control del puente H
#define motor2A   Pin_B1         //Pin de control del puente H

#define speed     50                //Constante para guardar la velocidad

void main()
{
   digitalWrite(motor1A, 1);        //Define el sentido de giro del motor
   digitalWrite(motor2A, 0);
   
   analogWrite(Enable12, speed);    //Enciende el motor
      
   while(true)
   {
      /*
         Si se presiona el boton, se invierte el sentido del motor
      */
      if(digitalRead(buttonPin) == 0)
      {
         digitalToggle(motor1A);       //Invierte el estado logico del pin
         digitalToggle(motor2A);       //Invierte el estado logico del pin
         delay_ms(250);                //Tiempo para evitar el rebote del boton
      }
   }
}
