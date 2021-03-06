/*
   processingGraph
   Lee el voltaje de salida de un sensor de temperatura LM35, convierte el valor
   decimal leido en su equivalente valor de temperatura e imprime el resultado
   por el puerto USB.
   
   Hardware
   Pin_A0   Terminal Vout del LM35
   
*/
#include <Cabecera_JACSS.c>
#include <JACSS_CDC.c>

#define lm35Pin         A1             //Entrada analogica para el LM35

int8 sensor;                           //Variable para guardar el valor decimal

float degreeValue;                     //Variable para guardar el valor de temp.

void main()
{
   usbInit();                          //Inicializa la comunicacion USB
        
   while(true)
   {
      sensor = analogRead(lm35Pin);    //Lee el voltaje del LM35
      
      //Convierte el valor analogico decimal a su equivalente en �C
      degreeValue = convertDECtoDegree(sensor);
      
      /*
         La funci�n printf se usa para imprimir numeros y texto con caracteres
         ASCII.
         Parametros opcionales:
            \r       Regresa el cursor al extremo izquierdo del renglon
                     
         Parametros obligatorios:
            usbWrite Indica que se imprimir� texto en el puerto USB
            %g       Indica que es una variable float
            sensor   Nombre de la variable que se desea imprimir
      */
      
      //Imprime en la PC el valor leido
      printf(usbWrite,"%g \r", degreeValue);  //Imprime unicamente el valor

      delay_ms(1000);
   }
}
