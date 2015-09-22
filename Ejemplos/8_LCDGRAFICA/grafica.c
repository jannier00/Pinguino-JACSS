#include <18F4550.h> 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
//#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#include <nokia3310Driver.c> 

/*#define SCK          PIN_D7               // Connections between PIC and LCD. 
#define SDI          PIN_D6 
#define D_C          PIN_D5 
#define _SCE         PIN_D4 
#define _RES         PIN_D3 */

#use delay(clock=8000000) 

void main() 
{ 
//Setup_Oscillator parameter not selected from Intr Oscillotar Config tab 

   // TODO: USER CODE!! 
   int i=0; 
    
//REmember, you must define and initialise all variables here!!!                                    
  // TODO: USER CODE!! 
char fName[10]="Ade", lName[10]="Olusegun";  
//you must call initlcd before doing anything else with lcd 
initlcd(); 

   while(true) 
      { 
         
         clearram();
         //delay_ms(500); 
         delay_ms(50); 
         //cursorxy(1,0); 
         cursorxy(i,0); 
         //putstr(fName);        
         putstr(fName);        
         // TODO: USER CODE!! 
         cursorxy(30,0); 
         putstr(lName);                                                          
         cursorxy(1,1); 
         putstr(fName);        
         // TODO: USER CODE!! 
         cursorxy(30,1); 
         putstr(lName);  
         cursorxy(1,5); 
         putstr(fName);        
         // TODO: USER CODE!! 
         cursorxy(30,5); 
         putstr(lName); 
         delay_ms(400);          
         i++; 
       }                                      
   //now you can call any function from the driver to modify brightness, determine where to write next, write a character, or a string. 
} 
