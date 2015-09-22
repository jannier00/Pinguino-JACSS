#include <18F4550.h>
#device *= 16 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <JACSS_CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#define DHT11_pin PIN_B0 //Defines pin a utilizar del sensor
#include <dht11.c>  //esta es la libreria creada

void main() 
{ 
   usbInit(); //iniciamos comunicacion USB
   DHT11_init();  //Iniciamos Sensor 
  
   unsigned char state = 0; 
    
   while(TRUE) 
   { 
      state = get_data(); 
      
      switch(state) 
      { 
            case 1: 
            { 
            } 
            case 2: 
            { 
               printf(usbWrite,"\fNo Sensor Found!"); 
               break; 
            } 
            case 3: 
            { 
               printf(usbWrite,"\fChecksum Error!"); 
               break; 
            } 
            default: 
            { 
               printf(usbWrite,"R.H:%u.%02u\n\r ", values[0], values[1] );
               printf(usbWrite,"Tmp:%u.%02u C\n\r ",values[2], values[3]); 
               delay_ms(200);
               break; 
            } 
      } 
      delay_ms(1000); 
   }; 
} 

