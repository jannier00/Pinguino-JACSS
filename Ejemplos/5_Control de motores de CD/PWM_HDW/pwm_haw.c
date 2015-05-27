#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=20000000)  //#use delay(clock=48000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.


void main(void){
   set_tris_a(0b00010000);
   SETUP_ADC_PORTS(NO_ANALOGS); //Apagamo los analogicos para que sean digitales
   long duty_PWM1, duty_PWM2;
   usbInit();

   // Para cristal de 20 mhz, frecuencia de 20 khz ,
   // T2DIV set to 1
   // Para calcular la frecuencia:
   // (4/frecuencia cristal)*T2_DIV*(periodo+1)

   // (4/20000000)*1*(249+1)=0.00005seg
   

   setup_timer_2(T2_DIV_BY_1, 249, 1);

   // seleccionamos PWM para los dos módulos CCP
   setup_ccp1 (CCP_PWM);
   setup_ccp2 (CCP_PWM);

   // cargamos el duty cicle para cada PWM. La frecuencia es igual para ambas, 20KHz.
   // Para calcular el tiempo en nivel alto, dado que usamos un LONG, sería:
   // Valor/T2_DIV*(1/frecuencia del cristal). Si se usa un valor tipo INT en lugar
   // de LONG se calcularía multiplicando la fórmula por 4: Valor/T2_DIV*(1/frecuencia del cristal)*4
   duty_PWM1 = 0; // 0.000025seg/(1*(1/20000000))=500, que es el valor a cargar. Duty 50%
   duty_PWM2 = 0; // 0.000050seg/(1*(1/20000000))=1000. Si te fijas, con 20KHZ el T=50us,

                    // que es lo que se ha puesto para el duty, osea, todo.
                    // Por tanto la señal será continua a 5v. Duty cicle 100%

   set_pwm1_duty(duty_PWM1);
   set_pwm2_duty(duty_PWM2);

   while(1){   //bucle infinito.
   if (!input(PIN_A4)){   //Esperamos presionar boton A4 manda a tierra
      delay_ms(800);       //Esperamos a dejar de presionar
      duty_PWM1=duty_PWM1+50;  //Incrementamos Duty en 50
      duty_PWM2=duty_PWM2+20; //Incrementamos Duty2 en 20
      set_pwm1_duty(duty_PWM1);   //Mandamos valor a CPP1
      set_pwm2_duty(duty_PWM2);   //Mandamos Valor a CPP2
      printf(usbWrite,"\rCCP1=%Ld     CCP2=%Ld   \r\n",duty_PWM1,duty_PWM2);  //Imprime en consola
   }
   if (duty_PWM1==1000) duty_PWM1=0;
   if (duty_PWM2==1000) duty_PWM2=0;
   
   }

}

