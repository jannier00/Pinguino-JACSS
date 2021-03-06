/*                                     PWM.c

Esta libreria contiene funciones dise�adas para la facil y rapida programacion
de salidas PWM por software a traves del uso del Timer3, al igual que las dos
salidas de PWM por hardware en Pin_C1 y Pin_C2.

La salida de PWM por software tiene una frecuencia aproximada de 250Hz y puede
configurar el ciclo de trabajo entre 0% y 100%, en intervalos de 10%.

La salida de PWM por hardware tiene una frecuencia aproximada de 30kHz y puede
configurar un ciclo de trabajo entre 0% y 100%, en intervalos de 1%.

Los Pins de salida del PWM se seleccionan definiendo PWMx_PIN donde x puede ser
desde 0 hasta 11. Los pines PWM por default son los siguientes:

         PWM0_PIN    PIN_B0   
         PWM1_PIN    PIN_B1
         PWM2_PIN    PIN_B2
         PWM3_PIN    PIN_B3
         PWM4_PIN    PIN_B4
         PWM5_PIN    PIN_B5
         PWM6_PIN    PIN_B6
         PWM7_PIN    PIN_B7
         PWM8_PIN    PIN_D0
         PWM9_PIN    PIN_D1
         PWM10_PIN   PIN_D2
         PWM11_PIN   PIN_A4
         
         PIN_C1
         PIN_C2
            
   En caso de seleccionar el Pin_C1 o el Pin_C2 se activar� el PWM por hardware
   del pin seleccionado con un ciclo configurable entre 0% y 100% en intervalos
   de 1%
*/
#ifndef __Software_PWM__
#define __Software_PWM__

int1 _PWM0_Flag,_PWM1_Flag,_PWM2_Flag,_PWM3_Flag,_PWM4_Flag,_PWM5_Flag,
     _PWM6_Flag,_PWM7_Flag,_PWM8_Flag,_PWM9_Flag,_PWM10_Flag,_PWM11_Flag;

int8 _PWM0_DutyCycle,_PWM1_DutyCycle,_PWM2_DutyCycle,_PWM3_DutyCycle,
     _PWM4_DutyCycle,_PWM5_DutyCycle,_PWM6_DutyCycle,_PWM7_DutyCycle,
     _PWM8_DutyCycle,_PWM9_DutyCycle,_PWM10_DutyCycle,_PWM11_DutyCycle;

#ifndef PWM0_PIN
   #define PWM0_PIN  PIN_B0       //Pin para PWM0
#endif
#ifndef PWM1_PIN
   #define PWM1_PIN  PIN_B1       //Pin para PWM1
#endif
#ifndef PWM2_PIN
   #define PWM2_PIN  PIN_B2       //Pin para PWM2
#endif
#ifndef PWM3_PIN
   #define PWM3_PIN  PIN_B3       //Pin para PWM3
#endif
#ifndef PWM4_PIN
   #define PWM4_PIN  PIN_B4       //Pin para PWM4
#endif
#ifndef PWM5_PIN
   #define PWM5_PIN  PIN_B5       //Pin para PWM5
#endif
#ifndef PWM6_PIN
   #define PWM6_PIN  PIN_B6       //Pin para PWM6
#endif
#ifndef PWM7_PIN
   #define PWM7_PIN  PIN_B7       //Pin para PWM7
#endif
#ifndef PWM8_PIN
   #define PWM8_PIN  PIN_D0       //Pin para PWM8
#endif
#ifndef PWM9_PIN
   #define PWM9_PIN  PIN_D1       //Pin para PWM9
#endif
#ifndef PWM10_PIN
   #define PWM10_PIN PIN_D2       //Pin para PWM10
#endif
#ifndef PWM11_PIN             
   #define PWM11_PIN PIN_A4       //Pin para PWM11
#endif

#define _Set_PWM_Timer3 63135
#define _totalResolution 10


//****************************************************************************/
//                         analogWrite(PWMx,DutyCycle)
//
//Funcion que permite la configuracion de PWM por software en cualquier PIN
//digital disponible. En caso de necesitar mas pines PWM es posible agregarlos 
//a�adiendo mas "case" siguiendo la secuencia en las variables.
//
// PWMx => Especifica el numero de PWM, cada salida debe tener uno diferente
// DutyCycle => Porcentaje de ciclo de trabajo del PWM. Es un valor de 0 a 100
//
// Ejemplo:
//          PWM_Output(0,50);   //Salida PWM0 de 50%
//          PWM_Output(1,0);    //Salida PWM1 de 0%
//                              //y apaga el PWM para uso como I/O
//****************************************************************************//
void analogWrite(int8 _PWMx, int8 _DutyCycle)
{
   static int1 _T3_Config_Flag = False, _T2_Config_Flag = False;
   //Pregunta si el Timer3 ya ha sido configurado
   //Si el Pin corresponde a un pin de PWM por hardware, no configura el timer3
   if(!_T3_Config_Flag && _PWMx != Pin_C1 && _PWMx != Pin_C2) 
   {
      setup_timer_3(T3_INTERNAL | T3_DIV_BY_2);
      set_timer3(_Set_PWM_Timer3);
      
      enable_interrupts(GLOBAL);
      enable_interrupts(INT_TIMER3);
      _T3_Config_Flag = True;    //Indica que el Timer3 ya ha sido configurado
   }   
   
   //Si no es un pin de PWM por hardware, divide entre 5 el ciclo de trabajo 
   //para convertirlo en un valor entre 0 y 20
   if(_PWMx != Pin_C1 && _PWMx != Pin_C2)    _DutyCycle /= 10;
   else
   {
      if(!_T2_Config_Flag)          //Configura por �nica vez el Timer2
      {  
         setup_timer_2(T2_DIV_BY_4, 100, 1);
         _T2_Config_Flag = True;
      }
   }
          
   
   if(_DutyCycle == 0)
   {
      switch(_PWMx)                  //Desactiva la salida PWM en el PIN deseado
      {                 
         case 0:
            _PWM0_Flag = False;
            output_low(PWM0_PIN);
            break;
         case 1:
            _PWM1_Flag = False;
            output_low(PWM1_PIN);
            break;
         case 2:
            _PWM2_Flag = False;
            output_low(PWM2_PIN);
            break;   
         case 3:
            _PWM3_Flag = False;
            output_low(PWM3_PIN);
            break;
         case 4:
            _PWM4_Flag = False;
            output_low(PWM4_PIN);
            break;
         case 5:
            _PWM5_Flag = False;
            output_low(PWM5_PIN);
            break;   
         case 6:
            _PWM6_Flag = False;
            output_low(PWM6_PIN);
            break;   
         case 7:
            _PWM7_Flag = False;
            output_low(PWM7_PIN);
            break;
         case 8:
            _PWM8_Flag = False;
            output_low(PWM8_PIN);
            break;
         case 9:
            _PWM9_Flag = False;
            output_low(PWM9_PIN);
            break;
         case 10:
            _PWM10_Flag = False;
            output_low(PWM10_PIN);
            break;
         case 11:
            _PWM11_Flag = False;
            output_low(PWM11_PIN);
            break; 
         case Pin_C1:
            setup_ccp2(CCP_OFF);
            output_low(Pin_C1);
            break;
         case Pin_C2:
            setup_ccp1(CCP_OFF);
            output_low(Pin_C2);
            break;            
         default:
         break;
      }
   }
   else
   {
      switch(_PWMx)                    //Asigna una salida PWM en el PIN deseado
      {                 
         case 0:
            _PWM0_Flag = True;
            _PWM0_DutyCycle = _DutyCycle;
            break;
         case 1:
            _PWM1_Flag = True;
            _PWM1_DutyCycle = _DutyCycle;
            break;
         case 2:
            _PWM2_Flag = True;
            _PWM2_DutyCycle = _DutyCycle;
            break;   
         case 3:
            _PWM3_Flag = True;
            _PWM3_DutyCycle = _DutyCycle;
            break;
         case 4:
            _PWM4_Flag = True;
            _PWM4_DutyCycle = _DutyCycle;
            break;
         case 5:
            _PWM5_Flag = True;
            _PWM5_DutyCycle = _DutyCycle;
            break;   
         case 6:
            _PWM6_Flag = True;
            _PWM6_DutyCycle = _DutyCycle;
            break;   
         case 7:
            _PWM7_Flag = True;
            _PWM7_DutyCycle = _DutyCycle;
            break;
         case 8:
            _PWM8_Flag = True;
            _PWM8_DutyCycle = _DutyCycle;
            break;
         case 9:
            _PWM9_Flag = True;
            _PWM9_DutyCycle = _DutyCycle;
            break;
         case 10:
            _PWM10_Flag = True;
            _PWM10_DutyCycle = _DutyCycle;
            break;
         case 11:
            _PWM11_Flag = True;
            _PWM11_DutyCycle = _DutyCycle;
            break;
         case Pin_C1:                        //PWM por hardware
            setup_ccp2(CCP_PWM);
            set_pwm2_duty(_DutyCycle);
            break;
         case Pin_C2:
            setup_ccp1(CCP_PWM);             //PWM por hardware
            set_pwm1_duty(_DutyCycle);
            break;
         default:
         break;
      }
   }
}

//****************************************************************************//
//                               Timer3_Interrupt()
//****************************************************************************//
#INT_TIMER3
void Timer3_Interrupt()
{   
   set_timer3(_Set_PWM_Timer3);
   static int8 T3_Counter = 1;
   
   if(_PWM0_Flag)
   {
      if (T3_Counter <= _PWM0_DutyCycle) output_high(PWM0_PIN); 
      else output_low(PWM0_PIN);
   }
   
   if(_PWM1_Flag)
   {
      if (T3_Counter <= _PWM1_DutyCycle) output_high(PWM1_PIN); 
      else output_low(PWM1_PIN);
   }
   
   if(_PWM2_Flag)
   {
      if (T3_Counter <= _PWM2_DutyCycle) output_high(PWM2_PIN); 
      else output_low(PWM2_PIN);
   }
   
   if(_PWM3_Flag)
   {
      if (T3_Counter <= _PWM3_DutyCycle) output_high(PWM3_PIN); 
      else output_low(PWM3_PIN);
   }
   
   if(_PWM4_Flag)
   {
      if (T3_Counter <= _PWM4_DutyCycle) output_high(PWM4_PIN); 
      else output_low(PWM4_PIN);
   }
   
   if(_PWM5_Flag)
   {
      if (T3_Counter <= _PWM5_DutyCycle) output_high(PWM5_PIN); 
      else output_low(PWM5_PIN);
   }
   
   if(_PWM6_Flag)
   {
      if (T3_Counter <= _PWM6_DutyCycle) output_high(PWM6_PIN); 
      else output_low(PWM6_PIN);
   }
   
   if(_PWM7_Flag)
   {
      if (T3_Counter <= _PWM7_DutyCycle) output_high(PWM7_PIN); 
      else output_low(PWM7_PIN);
   }
   
   if(_PWM8_Flag)
   {
      if (T3_Counter <= _PWM8_DutyCycle) output_high(PWM8_PIN); 
      else output_low(PWM8_PIN);
   }
   
   if(_PWM9_Flag)
   {
      if (T3_Counter <= _PWM9_DutyCycle) output_high(PWM9_PIN); 
      else output_low(PWM9_PIN);
   }
   
   if(_PWM10_Flag)
   {
      if (T3_Counter <= _PWM10_DutyCycle) output_high(PWM10_PIN); 
      else output_low(PWM10_PIN);
   }
   
   if(_PWM11_Flag)
   {
      if (T3_Counter <= _PWM11_DutyCycle) output_high(PWM11_PIN); 
      else output_low(PWM11_PIN);
   }
   
   T3_Counter ++;
   if(T3_Counter > _totalResolution) T3_Counter = 1;
}

#endif
