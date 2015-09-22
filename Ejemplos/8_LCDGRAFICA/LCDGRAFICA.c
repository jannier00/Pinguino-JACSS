#include <18F4550.h> 
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPBADEN
//#use delay(clock=48000000)  //#use delay(clock=20000000)
#include <usb/usb_bootloader.h> //Comenta esta línea si no usas Bootloader
#USE DELAY (Clock = 20000000)

#USE RS232 (BAUD = 57600, UART1)

#INCLUDE <math.h>
#INCLUDE "nokia_LCD.c"

char const logonokia [168]={ // 83x15
0xFF,0xFF,0xFF,0xFF,0x1F,0x1F,0x7F,0xFE,0xFC,0xF0,0xE0,0xC0,0x00,0x00,0x00,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0xFC,0xFE,0xFF,0xFF,0x1F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
0x0F,0x1F,0xFF,0xFF,0xFE,0xFC,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xE0,0xF0,
0xFC,0xFC,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x00,0xC0,0xF0,0xFC,0xFE,0x7F,0x1F,0x1F,0x7F,0xFF,0xFC,0xF0,0xC0,0x00,
0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x3F,0x7F,
0x7E,0x7C,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x1F,0x3F,0x7F,0x7F,0x7C,0x78,0x78,0x78,
0x78,0x78,0x78,0x78,0x7C,0x7F,0x7F,0x3F,0x1F,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,
0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7E,0x7C,0x78,0x70,0x60,0x40,0x00,0x7F,0x7F,0x7F,
0x7F,0x00,0x00,0x70,0x7C,0x7F,0x7F,0x1F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,
0x1F,0x7F,0x7F,0x7C,0x70,0x40};

char const logodspic [213]={ // 71x19
0x00,0x00,0xE0,0xF0,0xF0,0xF0,0xF0,0x70,0x70,0xF8,0xFF,0xFF,0xFF,0xFF,0x1F,0x00,
0x80,0xF0,0xF0,0xF0,0xF0,0xF0,0x30,0xF0,0xF0,0xF0,0x70,0x70,0x20,0x00,0x00,0xC0,
0xC6,0xE7,0xE7,0xF7,0xFF,0x3F,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x1E,0x00,0xF0,
0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x01,0xE0,0xFC,0xFE,0xFF,0xFF,0xFF,0x7F,0x0F,0x2F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x3E,0x08,0xE0,0xFE,0xFF,0xFF,0xFF,0xFF,0x03,0xF0,0xFF,
0xFF,0xFF,0xFF,0x3F,0x03,0x80,0xC0,0xE7,0xEF,0xEF,0xEF,0x1F,0x9F,0xFE,0xFC,0xFC,
0xF8,0xF8,0x00,0x00,0xE0,0xFE,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x1E,0x1F,0x1F,0x1F,
0x1F,0x0F,0x0F,0x03,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x03,0xC0,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0x0F,0xE0,0xF8,0xF8,0xF8,0xF8,0xF8,0x78,0x00,0x00,0x00,0x03,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00,0x01,0x03,0x07,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x00,0x00,0x06,0x07,0x07,0x07,0x07,0x07,
0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x07,0x07,0x07,0x07,0x07,
0x07,0x00,0x00,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
0x03,0x00,0x00,0x00,0x00};

char const nocturno[63]={ // 63 x 7
0x7F,0x7F,0x0F,0x1E,0x78,0x7F,0x7F,0x00,0x3C,0x7E,0x66,0x42,0x66,0x7E,0x3C,0x00,
0x1C,0x7E,0x7E,0x42,0x42,0x42,0x00,0x00,0x02,0x0E,0x7F,0x7F,0x42,0x42,0x00,0x00,
0x3E,0x7E,0x7C,0x40,0x40,0x7E,0x3E,0x00,0x7E,0x7E,0x06,0x02,0x02,0x02,0x00,0x00,
0x7E,0x7E,0x0E,0x06,0x02,0x7E,0x7E,0x00,0x3C,0x7E,0x66,0x42,0x66,0x7E,0x3C};

char const bola[8]={ //8x8
0x3C,0x7A,0xF1,0xFB,0xFF,0xFF,0x7E,0x3C};

// Funcion Principal
void main(void)
{
   int8 a;
   int16 j, k, offset;
   signed int16 l, i;
   
   char str1[] = "Con tres tipos";
   char str2[] = "de letra:";
   char str3[] = "NEGRITA";
   char str4[] = "COMPRIMIDA";
   char str5[] = "NORMAL";
   char str6[] = "Funciones para";
   char str7[] = "PUNTOS";
   char str8[] = "LINEAS";
   char str9[] = "CUADRADOS";
   char str10[] = "CIRCULOS";
   char str11[] = "BITMAPS";
   char str12[] = "Con movimiento";
   char str13[] = "queda mejor ;)";
   
   char str14[] = "Este ejemplo";
   char str15[] = "esta basado en";
   char str16[] = "las funciones";
   char str17[] = "de Nocturno";
   char str18[] = "en C30 y";
   char str19[] = "adapatadas al";
   char str20[] = "CCS";
   char str21[] = "por";
   char str22[] = "Pinguino JACSS";
   
   int8 Temp[504];     
   
   lcd_init();             // Inicialización LCD
   nokia_borra_pantalla();    // La limpiamos (por si acaso)
   
   delay_ms(300);     
      
   while(true) 
   {   
      
      nokia_borra_pantalla();

      nokia_texto(0, 0, NORMAL, str1);
      nokia_texto(0, 8, NORMAL, str2);

      nokia_texto(10, 18, NEGRITA, str3 );
      nokia_texto(20, 28, COMPRIMIDA, str4);
      nokia_texto(30, 40, NORMAL, str5);
      nokia_imprime_memoria();
  
      delay_ms(4000);
   
      nokia_borra_memoria();
      nokia_texto(0, 0, NORMAL, str6);
      nokia_texto(18, 8, NEGRITA, str7); //puntos
      nokia_imprime_memoria();
      
      delay_ms(2000);
   
      for (j = 0; j < 60; j++)
      {
         for (i = 0; i < 84; i++)
         {
            nokia_punto(i, (int8)(24 + sin((i + j) * PI / 20) * 22), NEGRO);
         }
         
         nokia_imprime_memoria();
         nokia_borra_memoria();
         delay_ms(20);
      }
    
      nokia_borra_memoria();
      
      nokia_texto(0,0,NORMAL,str6);
      nokia_texto(18,8,NEGRITA,str8); //lineas
      nokia_imprime_memoria();
   
      delay_ms(2000);
      
      nokia_borra_memoria();
      
      for (i = 0; i < 84; i += 4)   
      {
         nokia_linea(i, 0, 83 - i, 47, NEGRO);      
         nokia_imprime_memoria();
      }
      
      for (i = 0; i < 47; i += 4)
      {
         nokia_linea(83, i, 0, 47 - i, NEGRO);      
         nokia_imprime_memoria();
      }
      
      for (i = 0; i < 84; i += 4)   
      {
         nokia_linea(i, 0, 83 - i, 47, BLANCO);      
         nokia_imprime_memoria();
      }
      
      for (i = 0; i < 47; i += 4)
      {
         nokia_linea(83, i, 0, 47 - i, BLANCO);      
         nokia_imprime_memoria();
      }

      nokia_borra_memoria();
   
      nokia_texto(0, 0, NORMAL, str6);
      nokia_texto(8, 8, NEGRITA, str9);  //cuadrados
      nokia_imprime_memoria();
   
      delay_ms(2000);
   
      nokia_borra_memoria();
      
      nokia_rectangulo(0, 0, 25, 15, TRANSPARENTE, NEGRO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(10, 10, 38, 40, SOLIDO, NEGRO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(35, 0, 80, 15, SOLIDO, NEGRO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(83, 47, 42, 24, SOLIDO, NEGRO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(20, 10, 64, 37, TRANSPARENTE, BLANCO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(32, 14, 52, 34, SOLIDO, BLANCO);   
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_rectangulo(37, 19, 47, 29, TRANSPARENTE, NEGRO);   
      nokia_imprime_memoria();
      delay_ms(500);

      delay_ms(2000);

      nokia_borra_memoria();

      nokia_texto(0, 0, NORMAL, str6);
      nokia_texto(10, 8, NEGRITA, str10);   //circulos
      nokia_imprime_memoria();
      delay_ms(2000);
      nokia_borra_memoria();

      for (i = 4; i < 22; i += 3)
      {
         nokia_circulo(42, 24, i, TRANSPARENTE, NEGRO);
         nokia_imprime_memoria();
         delay_ms(500);
      }
      
      nokia_circulo(62, 30, 15, SOLIDO, NEGRO);
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_circulo(10, 10, 20, TRANSPARENTE, NEGRO);
      nokia_imprime_memoria();
      delay_ms(500);
      
      nokia_circulo(10, 10, 4, SOLIDO, NEGRO);
      nokia_imprime_memoria();


      delay_ms(2000); 
    
      nokia_borra_memoria();
      nokia_texto (0, 0, NORMAL, str6);
      nokia_texto (14, 8, NEGRITA, str11);   //bitmaps
      nokia_imprime_memoria();
      
      delay_ms(2000);
      memcpy(Temp, logodspic, sizeof(logodspic));
      nokia_bitmap(7, 20, Temp, 71, 19);
      nokia_imprime_memoria();
      
      delay_ms(2000);

      nokia_borra_memoria();
      nokia_texto(0, 0, NORMAL, str12);  //cn mvmnto
      nokia_texto(0, 8, NORMAL, str13);  //qda mjor
      nokia_imprime_memoria();
      
      delay_ms(2000);

      j = 0;
      offset = 1;
   
      for (i = 0; i < 29; i++)
      {
         memcpy(Temp, logodspic, sizeof(logodspic));
         nokia_bitmap(7, i, Temp, 71, 19);

         memcpy(Temp, bola, sizeof(bola));
         nokia_bitmap(j, (int8)(40 - abs(sin((k++) * PI / 20) * 36)), Temp, 8, 8);
         
         j = j + offset;
         
         if (j > 76)
         {     
            offset =- 1;
         }
         
         if (j == 0)
         {
            offset = 1;
         }

         nokia_imprime_memoria();
         nokia_borra_memoria();
         
         delay_ms(35);
      }
   
      for (i = 83; i > 0; i--)
      {
         memcpy(Temp, logodspic, sizeof(logodspic));
         nokia_bitmap(7, 29, Temp, 71, 19);
         
         memcpy(Temp, logonokia, sizeof(logonokia));
         nokia_bitmap(i, 0, Temp, 83, 15);

         memcpy(Temp, bola, sizeof(bola));
         nokia_bitmap(j, (int8)(40 - abs(sin((k++) * PI / 20) * 36)), Temp, 8, 8);
         
         j = j + offset;
         
         if (j > 76)
         {
            offset =- 1;
         }
         
         if (j == 0)
         {
            offset = 1;
         }

         nokia_imprime_memoria();
         nokia_borra_memoria();
         
         delay_ms(30);
      }
      
      for (i = -63; i < 20; i++)
      {
         memcpy(Temp, logodspic, sizeof(logodspic));
         nokia_bitmap(7, 29, Temp, 71, 19);
         
         memcpy(Temp, logonokia, sizeof(logonokia));
         nokia_bitmap(0, 0, Temp, 83, 15);
         
         memcpy(Temp, nocturno, sizeof(nocturno));
         nokia_bitmap(i, 18, Temp, 63, 7);

         memcpy(Temp, bola, sizeof(bola));
         nokia_bitmap(j, (int8)(40 - abs(sin((k++) * PI / 20) * 36)), Temp, 8, 8);
         
         j = j + offset;
         
         if (j > 76)
         {
            offset =- 1;
         }
         if (j == 0)
         {
            offset = 1;
         }

         nokia_imprime_memoria();
         nokia_borra_memoria();
         
         delay_ms(25);
      }


      for (i = 0; i < 50; i++)
      {
         memcpy(Temp, logodspic, sizeof(logodspic));
         nokia_bitmap(7, 29, Temp, 71, 19);
         
         memcpy(Temp, logonokia, sizeof(logonokia));
         nokia_bitmap(0, 0, Temp, 83, 15);
         
         memcpy(Temp, nocturno, sizeof(nocturno));
         nokia_bitmap(20, 18, Temp, 63, 7);

         memcpy(Temp, bola, sizeof(bola));
         nokia_bitmap(j, (int8)(40 - abs(sin((k++) * PI / 20) * 36)), Temp, 8, 8);
         
         j = j + offset;
         
         if (j > 76)
         {
            offset =- 1;
         }
         if (j == 0)
         {    
            offset = 1;
         }
         
         nokia_imprime_memoria();
         nokia_borra_memoria();
         
         delay_ms(30);
      }
      
      memcpy(Temp, logodspic, sizeof(logodspic));
      nokia_bitmap(7, 29, Temp, 71, 19);
   
      memcpy(Temp, logonokia, sizeof(logonokia));
      nokia_bitmap(0, 0, Temp, 83, 15);
   
      memcpy(Temp, nocturno, sizeof(nocturno));
      nokia_bitmap(20, 18, Temp, 63, 7);
   
      nokia_imprime_memoria();
   
      a = 0x0C;
   
      for (i = 0; i < 10; i++) 
      {
         switch (a) 
         {
            case 0x0c:
               nokia_write_command(0x0D);   // Modo normal
               a = 0x0D;
            
               delay_ms(200);
            
               break;
         
            case 0x0D:
               nokia_write_command(0x0C);   // Modo inverso
               a = 0x0C;
            
               delay_ms(200);
            
               break;
         }
      }

      nokia_borra_memoria();
   
      for (l = 40; l > -64; l--)
      {
         nokia_texto(0, l, NORMAL, str14);
         nokia_texto(0, l + 8, NORMAL, str15);
         nokia_texto(0, l + 16, NORMAL, str16);
         nokia_texto(0, l + 24, NORMAL, str17);
         nokia_texto(0, l + 32, NORMAL, str18);
         nokia_texto(0, l + 40, NORMAL, str19);
         nokia_texto(0, l + 48, NORMAL, str20);
         nokia_texto(20, l + 56, NORMAL, str21);
         nokia_texto(10, l + 64, NEGRITA, str22);
         
         nokia_imprime_memoria();
          delay_ms(1000);
         nokia_borra_memoria();
         
        
      }   
   }
}



