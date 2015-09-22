//***********************************************************************************************************
//////////// CONFIGURACION DE LAS INTERRUPCIONES EXT, EXT1 Y EXT2       //////////////////
//***********************************************************************************************************
#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#device HIGH_INTS=TRUE           //ACTIVACION DE LAS PRIORIDADES EN LAS INTERRUPCIONES
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb_bootloader.h> //Comenta esta línea si no usas Bootloader

//******************************************************************************************
///////////////////////ASIGNACION DE NOMBRES A LOS RESPECTIVOS PUERTOS//////////////////////

#BYTE PORTB=0xf81
#BYTE PORTC=0xf82
#BYTE PORTD=0xf83

//*****************************************************************************************
////LAS FUNC. FAST_IO EVITAN RECONFIGURAR LOS PUERTOS CON FUNCIONES OUTPUT E INPUT/////

#USE FAST_IO(A)                  
#USE FAST_IO(B)                  
#USE FAST_IO(C) 
#USE FAST_IO(D)
#USE FAST_IO(E)

//****************************************************************************************
////////////////////////////////DEFINICION DE VARIABLES///////////////////////////////////

INT8 CONT=0;

//****************************************************************************************
////////////////////DEFINICION DE LAS FUNCIONES DE PROGRAMA///////////////////////////////

VOID CONTROL_EXT();
VOID CONTROL_EXT1();
VOID CONTROL_EXT2();

//****************************************************************************************
///////////////  FUNCIONES DE LAS INTERRUPCIONES EXT, EXT1 Y EXT2    ////////////////////

#INT_EXT                            //FUNCION DE LA INTERRUPCION EXT0, PRIORIDAD ALTA POR DEFECTO
VOID CONTROL_EXT()
{
      PORTD=0B10000001;     //SACA DETERMINADO VALOR POR EL PUERTO D
      DELAY_MS(500);        
      PORTD=0B01000010;
      DELAY_MS(500);           //RETARDO DE MEDIO SEGUNDO
      PORTD=0B00100100;
      DELAY_MS(500);
      PORTD=0B00011000;
      DELAY_MS(500);
}


#INT_EXT1 HIGH                    //FUNCION DE LA INTERRUPCION EXT1, PRIORIDAD ALTA 
VOID CONTROL_EXT1()
{
      PORTD=0B11000000;
      DELAY_MS(500);
      PORTD=0B01100000;
      DELAY_MS(500);
      PORTD=0B00110000;
      DELAY_MS(500);
      PORTD=0B00011000;
      DELAY_MS(500); 
      PORTD=0B00001100;
      DELAY_MS(500);
      PORTD=0B00000110;
      DELAY_MS(500);
      PORTD=0B00000011;
      DELAY_MS(500);
}


#INT_EXT2                  //FUNCION DE LA INTERRUPCION EXT2, PRIORIDAD BAJA
VOID CONTROL_EXT2()
{
      PORTD=0B11000011;
      DELAY_MS(500);
      PORTD=0B01100110;
      DELAY_MS(500);
      PORTD=0B00111100;
      DELAY_MS(500);
      PORTD=0B00011000;
      DELAY_MS(500); 
      PORTD=0B00111100;
      DELAY_MS(500);
      PORTD=0B00110110;
      DELAY_MS(500);
      PORTD=0B11000011;
      DELAY_MS(500);
}

//***************************************************************************************
////////////////////////FUNCION PRINCIPAL (MAIN)/////////////////////////////////////////

VOID MAIN (VOID)
{
//////////////////////  CONFIGURACION DE PUERTOS  ///////////////////////////////////////

SET_TRIS_B(0B00000111);                     //SE CONFIGURA RB0 RB1 RB2 COMO ENTRADA
SET_TRIS_D(0);                                     //SE CONFIGURA EL PUERTO D COMO SALIDA PARA LOS LEDS
PORTB=0;
PORTD=0;
SETUP_ADC_PORTS(NO_ANALOGS);     //NO CANALES ANALOGOS TODOS DIGITALES
SETUP_ADC(ADC_OFF);                         //NO SE UTILIZA CONVERSOR A/D
ENABLE_INTERRUPTS(INT_EXT);            //INT_EXT HABILITADA
ENABLE_INTERRUPTS(INT_EXT1);          //INT_EXT1 HABILITADA
ENABLE_INTERRUPTS(INT_EXT2);          //INT_EXT2 HABILITADA
ENABLE_INTERRUPTS(GLOBAL);           //INT GLOBALES HABILITADAS
PORT_B_PULLUPS(TRUE);                     //RESISTENCIAS DE PULL-UP HABILITADAS
EXT_INT_EDGE(H_TO_L);                        //FLANCO DE BAJADA PARA INT EXT
EXT_INT_EDGE(1,L_TO_H);                     //FLANCO DE SUBIDA PARA INT EXT1
EXT_INT_EDGE(2,H_TO_L);                     //FLANCO DE BAJADA PARA INT EXT2
WHILE (1)
{                 
SLEEP();                                                //CONDICION DE REPOSO
}

}  
