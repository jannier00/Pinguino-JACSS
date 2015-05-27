////////////////////////////////////////////////////////////////////////////////
//                    COMUNICACI�N I2C EN TRE DOS PIC                         //
//                                                                            //
//                      Aplicaci�n para PIC MAESTRO                           //
//                                                                            //
//       El PIC maestro lee teclado y env�a el dato al PIC esclavo via I2C    //
//                      que lo mostrar� en un lcd.                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <18F4550.h>
//#device adc = 10 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb_bootloader.h> //Comenta esta l�nea si no usas Bootloader
//#include <JACSS CDC.c>  //Libreria que genera el puerto COM virtual para comunicacion.
#include <kbd2_1.c>  //Libreria para Teclado Matricial 4x4
#use i2c(MASTER, SDA=PIN_C6, SLOW, SCL=PIN_C7, NOFORCE_SW)

//#define use_portb_kbd TRUE       //Configuraci�n puerto b para lectura teclado

#use standard_io(b)
#use standard_io(c)


int tecla;                       //Dato a transmitir
   
/******************************************************************************/
/***************************** Env�o I2C **************************************/
    
void envio_I2C (){
   
       i2c_start();         //Comienzo de la comunicaci�n I2C ...
      i2c_write(0xA0);     //...con la direcci�n del PIC esclavo...
      i2c_write(tecla);    // Envia dato
      i2c_stop();          //Finalizaci�n de la transmisi�n
      i2c_start();         //Comienzo de la comunicaci�n I2C ...
      i2c_write(0xA2);     //...con la direcci�n del PIC esclavo...
      i2c_write(tecla);    // Envia dato
      i2c_stop();          //Finalizaci�n de la transmisi�n
  }

/******************************************************************************/
/*************************** FUNCI�N PRINCIPAL ********************************/

void main(){

   kbd_init();                   //Inicializa funci�n de teclado
   port_b_pullups(TRUE);         
    
   while (true){
      
// Lectura del teclado     
      tecla=kbd_getc();       //En "tecla" valor de la tecla pulsada
      if(tecla!=0){           //Si se pulsa teclado...
            envio_I2C();      //...se env�a el dato
            delay_ms(100);
      }
   }
}
