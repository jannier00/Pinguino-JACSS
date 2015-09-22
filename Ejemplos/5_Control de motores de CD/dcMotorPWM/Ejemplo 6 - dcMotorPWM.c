/*
   dcMotorPWM
   Muestra una forma b�sica de utilizar el Puente H para controlar la 
   velocidad de giro de un motor de CD, acelerando y desacelerando de forma
   continua, siempre en el mismo sentido.
   Despues de que el motor desacelera, se apaga por 1 segundo y luego vuelve a 
   acelerar.
   
   Hardware
   G-PIC Lite!       SN754410 o L293D
   Pin_C1            Pin 1
   Pin_B0            Pin 2
   Pin_B1            Pin 7
                     Pin 8 - Conectado a 5V de la G-PiC Lite!
                     Pin 16 - Conectado a fuente de voltaje externa @ 5V
   
   Importante: Verificar la hoja de datos del puente H para saber c�mo 
   conectarlo correctamente
*/
#include <Cabecera_JACSS.c>

#define Enable12  Pin_C1         //PWM por hardware en el Pin_C1
#define motor1A   Pin_B0         //Pin de control del puente H
#define motor2A   Pin_B1         //Pin de control del puente H

int8 speed = 0;                  //Variable para guardar la velocidad

void main()
{
   digitalWrite(motor1A, 1);        //Define el sentido de giro inicial
   digitalWrite(motor2A, 0);

   while(true)
   {
      while(speed <= 100)              //Monitorea el valor de speed
      {
         analogWrite(Enable12, speed); //Actualiza el valor de PWM
         speed ++;                     //Incrementa la velocidad
         delay_ms(50);
      }
      while(speed >= 1)                //Monitorea el valor de speed
      {
         analogWrite(Enable12, speed); //Actualiza el valor de PWM
         speed --;                     //Decrementa la velocidad
         delay_ms(50);
      }
      
      delay_ms(1000);
   }
}
