#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta l�nea si no usas Bootloader
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#define ledPin Pin_A4               //Etiqueta al Pin_A4 con el nombre de ledPin

int16 contador=0;
int16 frec=0;                          // Variable que indica la frecuencia del PWM
int16 anchura=0;                  // Anchura del PWM


// 1Hz = 1 segundo = 1000mS
// 100 Hz = 0.01 segundos  = 10mS
// La interrupci�n se genera cada 1mS
#int_TIMER2
TIMER2_isr() 
{
contador++;

if (contador<=anchura)
       output_bit(ledPin,0);    
else
      output_bit(ledPin,1);
 
if (contador==frec)   contador=0;      // Reseteamos el contador

}




void main()
{

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(FALSE);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_16,19,16);    // Interrupcion cada 1 mS
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER2);         // Habilitamos la Interrupcion
   enable_interrupts(GLOBAL);

   
   while(1)
   {
   
      //Generamos un PWM de 10mS (100Hz) con una anchura del 50%
      frec=10;
      anchura=5;
   
   
      //Generamos un PWM de 1 segundo (1 Hz) con una anchura del 30%
      frec=1000;
      anchura=300;
     
      
   }  // Fin del While(1)

}