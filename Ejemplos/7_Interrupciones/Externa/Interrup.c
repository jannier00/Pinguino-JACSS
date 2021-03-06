//******************************************************************************************
/////////////       CONFIGURACION DE LA INTERRUPCION EXT POR RB0          //////////////////
//******************************************************************************************
#include <18F4550.h>
//#device ADC = 10 //Define la resolucion del ADC, comentar en caso de usar 8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
#use delay(clock=48000000)
#include <usb/usb_bootloader.h> //Comenta esta l�nea si no usas Bootloader

//******************************************************************************************
///////////////////////ASIGNACION DE NOMBRES A LOS RESPECTIVOS PUERTOS//////////////////////

#BYTE PORTB=0xf81
#BYTE PORTD=0xf83

//*****************************************************************************************
////LAS FUNC. FAST_IO EVITAN RECONFIGURAR LOS PUERTOS CON LAS FUNCIONES OUTPUT E INPUT/////

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
//****************************************************************************************
///////////////    FUNCION DE INTERRUPCION EXTERNA POR RB0            ////////////////////
//////    ESTA FUNCION SE EJECUTA CUANDO SE DETECTA UN FLANCO DE BAJADA EN RBO   /////////
/// POR CADA INTERRUPCION SE INCREMENTA EL VALOR DEL PUERTO D Y SE MUESTRA EN 8 LEDS  ////

#INT_EXT
VOID CONTROL_EXT()
{
PORTD++;                //INCREMENTA EL VALOR DEL PUERTO
}

//***************************************************************************************
////////////////////////FUNCION PRINCIPAL (MAIN)/////////////////////////////////////////

VOID MAIN (VOID)
{
//////////////////////  CONFIGURACION DE PUERTOS  ///////////////////////////////////////
SET_TRIS_B(0B00000001); //SE CONFIGURA RB0 COMO ENTRADA
SET_TRIS_D(0);          //SE CONFIGURA EL PUERTO D COMO SALIDA PARA LOS LEDS
PORTB=0;
PORTD=0;

/// SE HABILITA LA INTERRUPCION EXT Y SE HABILITAN INTERRUPCIONES GLOBALES ADEMAS SE ///
//  ACTIVAN LAS RESISTENCIAS DE PULL-UP DEL PUERTO B Y SE SELECCIONA FLANCO DE BAJA  ///

SETUP_ADC_PORTS(NO_ANALOGS);     //NO CANALES ANALOGOS TODOS DIGITALES
SETUP_ADC(ADC_OFF);              //NO SE UTILIZA CONVERSOR A/D
ENABLE_INTERRUPTS(INT_EXT);                             
ENABLE_INTERRUPTS(GLOBAL); 
PORT_B_PULLUPS(TRUE); 
EXT_INT_EDGE(H_TO_L);


WHILE(1)
{                 //CONDICION DE REPOSO
SLEEP();
}
}
