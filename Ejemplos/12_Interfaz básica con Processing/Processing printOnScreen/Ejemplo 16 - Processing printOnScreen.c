/*
   Processing printOnScreen
   Lee el voltaje de salida de un sensor de temperatura LM35 e imprime el valor
   leido en la computadora en tres diferentes formatos:
   
   1) Valor decimal en un rango de 0 - 255 que entrega la funcion analogRead()
   2) Valor equivalente en voltaje en un rango de 0V - 5V
   3) Valor equivalente en grados en un rango de 0° a 100°, asumiendo que el
      sensor tiene una relacion de 10mV/°C. Es decir:
      
      Si el sensor entrega 0.25V, quiere decir que esta detectando 25°C
      
   Estas conversiones pueden ser realizadas mediante las funciones
   -  convertDECtoVolt(_value)
   -  convertDECtoDegree(_value)
   
   Para facilitar la recepcion de los datos en Processing, se envia un byte
   adicional que sirve para sincronizar el envio y la recepcion.
   
   Hardware
   Pin_A0   Terminal Vout del LM35
   
*/
#include <Cabecera_JACSS.c>
#include <JACSS_CDC.c>

#define lm35Pin         A1             //Entrada analogica para el LM35

int8 sensor;
float voltValue, degreeValue;
void main()
{
   usbInit();                          //Inicializa la comunicacion USB
        
   while(true)
   {
      sensor = analogRead(lm35Pin);    //Lee el voltaje del LM35
      
      //Convierte el valor analogico decimal a su equivalente en volts
      voltValue = convertDECtoVolt(sensor);
      
      //Convierte el valor analogico decimal a su equivalente en °C
      degreeValue = convertDECtoDegree(sensor);
      /*
         La función printf se usa para imprimir numeros y texto con caracteres
         ASCII.
         Parametros opcionales:
            \r       Regresa el cursor al extremo izquierdo del renglon
            \n       Indica un salto de línea
         
         Parametros obligatorios:
            usbWrite Indica que se imprimirá texto en el puerto USB
            %u       Indica que es una variable int8
            %g       Indica que es una variable float
            sensor   Nombre de la variable que se desea imprimir
      */
      
      //Imprime en la PC el valor leido
      printf(usbWrite,"Decimal Value: %u \r\n", sensor);  
      printf(usbWrite,"Volt Value: %g \r\n", voltValue);  
      printf(usbWrite,"Degree Value: %g \r\n\n", degreeValue); 
      
      usbWrite(1);               //Byte de sincronizacion

      delay_ms(1000);
   }
}
