////////////////////////////////////////////////////////////////////////////////
// DESCRIPCIÓN DE LAS FUNCIONES
//
// void nokia_write_dorc(byte bytefornokia)              USO INTERNO
// void nokia_write_data(byte bytefornokia_data)         USO INTERNO
// void nokia_printchar(byte caracter)                   IMPRIME EL CARACTER
// void nokia_write_command(byte bytefornokia_command)   MANDA EL COMANDO
// void nokia_gotoxy(byte xnokia, byte ynokia)           SE POSICIONA EN X/Y
// void nokia_clean_ddram(void)                          LIMPIA LA PANTALLA
// void Lcd_init(void)                                   INICIALIZA LA LCD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// CONFIGURACION DE LA CONEXION
//
// Conectamos la LCD al PORTB en los pines indicados
//
////////////////////////////////////////////////////////////////////////////////
#DEFINE Pin_Nok_SClk PIN_A0
#DEFINE Pin_Nok_SDa  PIN_A1
#DEFINE Pin_Nok_DC   PIN_A2
#DEFINE Pin_Nok_CS   PIN_A3
#DEFINE Pin_Nok_Res  PIN_A4
// FIN CONFIGURACION

#define NEGRITA      0  // 7x8
#define COMPRIMIDA   1  // 3x6
#define NORMAL       2  // 5x7

#define NEGRO        1
#define BLANCO       0

#define SOLIDO       1
#define TRANSPARENTE 0

#define ANCHOPANTALLA   84
#define ALTOPANTALLA    48
////////////////////////////////////////////////////////////////////////////////

/*
Tabla de Instrucciones de la LCD. Siempre con D/C=0

0010 0xyz         // X=1 -> Driver en OFF
                     Y=1 -> Direccionamiento Vertical
                     Z=1 -> Comandos Extendidos

Modo de pantalla
0000 1001   0x09   // Entera Negra
0000 1000   0x08   // Entera Blanco
0000 1100   0x0c   // Modo Normal
0000 1101   0x0d   // Vídeo Inverso

Control de Temperatura
0100 0100   0x44  // Coeficiente de temperatura 0
0100 0101   0x45  // Coeficiente de temperatura 1
0100 0110   0x46  // Coeficiente de temperatura 2
0100 0111   0x47  // Coeficiente de temperatura 3

Bias (Contraste?)
0001 0xxx         // Establece el valor para Bias

Vop
1xxx xxxx         // Establece el valor para Vop
*/

char MemoriaVideo[504];

// F1-ñ -> 0xFC,0x09,0x05,0x05,0xF8
// D1-Ñ ->       
int8 const ASCII_TABLE[490]={
                  0x00,0x00,0x00,0x00,0x00,   // 20 space
                  0x00,0x00,0x5f,0x00,0x00,   // 21 !
                  0x00,0x07,0x00,0x07,0x00,   // 22 "
                  0x14,0x7f,0x14,0x7f,0x14,   // 23 #
                  0x24,0x2a,0x7f,0x2a,0x12,   // 24 $
                  0x23,0x13,0x08,0x64,0x62,   // 25 %
                  0x36,0x49,0x55,0x22,0x50,   // 26 &
                  0x00,0x05,0x03,0x00,0x00,   // 27 '
                  0x00,0x1c,0x22,0x41,0x00,   // 28 (
                  0x00,0x41,0x22,0x1c,0x00,   // 29 )
                  0x14,0x08,0x3e,0x08,0x14,   // 2a *
                  0x08,0x08,0x3e,0x08,0x08,   // 2b +
                  0x00,0x50,0x30,0x00,0x00,   // 2c ,
                  0x08,0x08,0x08,0x08,0x08,   // 2d -
                  0x00,0x60,0x60,0x00,0x00,   // 2e .
                  0x20,0x10,0x08,0x04,0x02,   // 2f /
                  0x3e,0x51,0x49,0x45,0x3e,   // 30 0
                  0x00,0x42,0x7f,0x40,0x00,   // 31 1
                  0x42,0x61,0x51,0x49,0x46,   // 32 2
                  0x21,0x41,0x45,0x4b,0x31,   // 33 3
                  0x18,0x14,0x12,0x7f,0x10,   // 34 4
                  0x27,0x45,0x45,0x45,0x39,   // 35 5
                  0x3c,0x4a,0x49,0x49,0x30,   // 36 6
                  0x01,0x71,0x09,0x05,0x03,   // 37 7
                  0x36,0x49,0x49,0x49,0x36,   // 38 8
                  0x06,0x49,0x49,0x29,0x1e,   // 39 9
                  0x00,0x36,0x36,0x00,0x00,   // 3a :
                  0x00,0x56,0x36,0x00,0x00,   // 3b ;
                  0x08,0x14,0x22,0x41,0x00,   // 3c <
                  0x14,0x14,0x14,0x14,0x14,   // 3d =
                  0x00,0x41,0x22,0x14,0x08,   // 3e >
                  0x02,0x01,0x51,0x09,0x06,   // 3f ?
                  0x32,0x49,0x79,0x41,0x3e,   // 40 @
                  0x7e,0x11,0x11,0x11,0x7e,   // 41 A
                  0x7f,0x49,0x49,0x49,0x36,   // 42 B
                  0x3e,0x41,0x41,0x41,0x22,   // 43 C
                  0x7f,0x41,0x41,0x22,0x1c,   // 44 D
                  0x7f,0x49,0x49,0x49,0x41,   // 45 E
                  0x7f,0x09,0x09,0x09,0x01,   // 46 F
                  0x3e,0x41,0x49,0x49,0x7a,   // 47 G
                  0x7f,0x08,0x08,0x08,0x7f,   // 48 H
                  0x00,0x41,0x7f,0x41,0x00,   // 49 I
                  0x20,0x40,0x41,0x3f,0x01,   // 4a J
                  0x7f,0x08,0x14,0x22,0x41,   // 4b K
                  0x7f,0x40,0x40,0x40,0x40,   // 4c L
                  0x7f,0x02,0x0c,0x02,0x7f,   // 4d M
                  0x7f,0x04,0x08,0x10,0x7f,   // 4e N
                  0x3e,0x41,0x41,0x41,0x3e,   // 4f O ////////////////////////////////                  
                  0x7f,0x09,0x09,0x09,0x06,   // 50 P
                  0x3e,0x41,0x51,0x21,0x5e,   // 51 Q
                  0x7f,0x09,0x19,0x29,0x46,   // 52 R
                  0x46,0x49,0x49,0x49,0x31,   // 53 S
                  0x01,0x01,0x7f,0x01,0x01,   // 54 T
                  0x3f,0x40,0x40,0x40,0x3f,   // 55 U
                  0x1f,0x20,0x40,0x20,0x1f,   // 56 V
                  0x3f,0x40,0x38,0x40,0x3f,   // 57 W
                  0x63,0x14,0x08,0x14,0x63,   // 58 X
                  0x07,0x08,0x70,0x08,0x07,   // 59 Y
                  0x61,0x51,0x49,0x45,0x43,   // 5a Z
                  0x00,0x7f,0x41,0x41,0x00,   // 5b [
                  0x02,0x04,0x08,0x10,0x20,   // 5c
                  0x00,0x41,0x41,0x7f,0x00,   // 5d
                  0x04,0x02,0x01,0x02,0x04,   // 5e
                  0x40,0x40,0x40,0x40,0x40,   // 5f
                  0x00,0x01,0x02,0x04,0x00,   // 60
                  0x20,0x54,0x54,0x54,0x78,   // 61 a
                  0x7f,0x48,0x44,0x44,0x38,   // 62 b
                  0x38,0x44,0x44,0x44,0x20,   // 63 c
                  0x38,0x44,0x44,0x48,0x7f,   // 64 d
                  0x38,0x54,0x54,0x54,0x18,   // 65 e
                  0x08,0x7e,0x09,0x01,0x02,   // 66 f
                  0x0c,0x52,0x52,0x52,0x3e,   // 67 g
                  0x7f,0x08,0x04,0x04,0x78,   // 68 h
                  0x00,0x44,0x7d,0x40,0x00,   // 69 i
                  0x20,0x40,0x44,0x3d,0x00,   // 6a j
                  0x7f,0x10,0x28,0x44,0x00,   // 6b k
                  0x00,0x41,0x7f,0x40,0x00,   // 6c l
                  0x7c,0x04,0x18,0x04,0x78,   // 6d m
                  0x7c,0x08,0x04,0x04,0x78,   // 6e n
                  0x38,0x44,0x44,0x44,0x38,   // 6f o
                  0x7c,0x14,0x14,0x14,0x08,   // 70 p
                  0x08,0x14,0x14,0x18,0x7c,   // 71 q
                  0x7c,0x08,0x04,0x04,0x08,   // 72 r
                  0x48,0x54,0x54,0x54,0x20,   // 73 s
                  0x04,0x3f,0x44,0x40,0x20,   // 74 t
                  0x3c,0x40,0x40,0x20,0x7c,   // 75 u
                  0x1c,0x20,0x40,0x20,0x1c,   // 76 v
                  0x3c,0x40,0x30,0x40,0x3c,   // 77 w
                  0x44,0x28,0x10,0x28,0x44,   // 78 x
                  0x0c,0x50,0x50,0x50,0x3c,   // 79 y
                  0x44,0x64,0x54,0x4c,0x44,   // 7a z
                  0x00,0x08,0x36,0x41,0x00,   // 7b
                  0x00,0x00,0x7f,0x00,0x00,   // 7c
                  0x00,0x41,0x36,0x08,0x00,   // 7d
                  0x10,0x08,0x08,0x10,0x08,   // 7e
                  0x78,0x46,0x41,0x46,0x78,  // 7F
                  0x7C,0x09,0x05,0x05,0x78,  // 80 -> ñ
                  0x7E,0x05,0x09,0x11,0x7E   // 81 -> Ñ
                  };   // 7f
  
  
char const Fuente_3_6 [195]={   /* System 3x6 (char #32 to #96) */
          0x00,0x00,0x00, /* Espace   0x20 */
          0x00,0x5C,0x00, /* ! */
          0x0C,0x00,0x0C, /* " */
          0x7C,0x28,0x7C, /* # */
          0x7C,0x44,0x7C, /* 0x */
          0x24,0x10,0x48, /* % */
          0x28,0x54,0x08, /* & */
          0x00,0x0C,0x00, /* ' */
          0x38,0x44,0x00, /* ( */
          0x44,0x38,0x00, /* ) */
          0x20,0x10,0x08, /* // */
          0x10,0x38,0x10, /* + */
          0x80,0x40,0x00, /* , */
          0x10,0x10,0x10, /* - */
          0x00,0x40,0x00, /* . */
          0x20,0x10,0x08, /* / */
          0x38,0x44,0x38, /* 0   0x30 */
          0x00,0x7C,0x00, /* 1 */
          0x64,0x54,0x48, /* 2 */
          0x44,0x54,0x28, /* 3 */
          0x1C,0x10,0x7C, /* 4 */
          0x4C,0x54,0x24, /* 5 */
          0x38,0x54,0x20, /* 6 */
          0x04,0x74,0x0C, /* 7 */
          0x28,0x54,0x28, /* 8 */
          0x08,0x54,0x38, /* 9 */
          0x00,0x50,0x00, /* : */
          0x80,0x50,0x00, /* ; */
          0x10,0x28,0x44, /* < */
          0x28,0x28,0x28, /* = */
          0x44,0x28,0x10, /* > */
          0x04,0x54,0x08, /* ? */
          0x38,0x4C,0x5C, /* @   0x40 */
          0x78,0x14,0x78, /* A */
          0x7C,0x54,0x28, /* B */
          0x38,0x44,0x44, /* C */
          0x7C,0x44,0x38, /* D */
          0x7C,0x54,0x44, /* E */
          0x7C,0x14,0x04, /* F */
          0x38,0x44,0x34, /* G */
          0x7C,0x10,0x7C, /* H */
          0x00,0x7C,0x00, /* I */
          0x20,0x40,0x3C, /* J */
          0x7C,0x10,0x6C, /* K */
          0x7C,0x40,0x40, /* L */
          0x7C,0x08,0x7C, /* M */
          0x7C,0x04,0x7C, /* N */
          0x7C,0x44,0x7C, /* O */
          0x7C,0x14,0x08, /* P   0x50 */
          0x38,0x44,0x78, /* Q */
          0x7C,0x14,0x68, /* R */
          0x48,0x54,0x24, /* S */
          0x04,0x7C,0x04, /* T */
          0x7C,0x40,0x7C, /* U */
          0x3C,0x40,0x3C, /* V */
          0x7C,0x20,0x7C, /* W */
          0x6C,0x10,0x6C, /* X */
          0x1C,0x60,0x1C, /* Y */
          0x64,0x54,0x4C, /* Z */
          0x7C,0x44,0x00, /* [ */
          0x08,0x10,0x20, /* \ */
          0x44,0x7C,0x00, /* ] */
          0x08,0x04,0x08, /* ^ */
          0x80,0x80,0x80, /* _ */
          0x04,0x08,0x00 /* `   0x60 */
};

char const Fuente_7_8 [685]={   /* System 7x8 (char #32 to #128) */

          0,   0,   0,   0,   0,   0,   0, //' '
          0,   6,  95,  95,   6,   0,   0, //'!'
          0,   7,   7,   0,   7,   7,   0, //'"'
         20, 127, 127,  20, 127, 127,  20, //'#'
         36,  46, 107, 107,  58,  18,   0, //'$'
         70, 102,  48,  24,  12, 102,  98, //'%'
         48, 122,  79,  93,  55, 122,  72, //'&'
          4,   7,   3,   0,   0,   0,   0, //'''
          0,  28,  62,  99,  65,   0,   0, //'('
          0,  65,  99,  62,  28,   0,   0, //')'
          8,  42,  62,  28,  28,  62,  42, //'*'
          8,   8,  62,  62,   8,   8,   0, //'+'
          0, 128, 224,  96,   0,   0,   0, //','
          8,   8,   8,   8,   8,   8,   0, //'-'
          0,   0,  96,  96,   0,   0,   0, //'.'
         96,  48,  24,  12,   6,   3,   1, //'/'
         62, 127, 113,  89,  77, 127,  62, //'0'
         64,  66, 127, 127,  64,  64,   0, //'1'
         98, 115,  89,  73, 111, 102,   0, //'2'
         34,  99,  73,  73, 127,  54,   0, //'3'
         24,  28,  22,  83, 127, 127,  80, //'4'
         39, 103,  69,  69, 125,  57,   0, //'5'
         60, 126,  75,  73, 121,  48,   0, //'6'
          3,   3, 113, 121,  15,   7,   0, //'7'
         54, 127,  73,  73, 127,  54,   0, //'8'
          6,  79,  73, 105,  63,  30,   0, //'9'
          0,   0, 102, 102,   0,   0,   0, //':'
          0, 128, 230, 102,   0,   0,   0, //';'
          8,  28,  54,  99,  65,   0,   0, //'<'
         36,  36,  36,  36,  36,  36,   0, //'='
          0,  65,  99,  54,  28,   8,   0, //'>'
          2,   3,  81,  89,  15,   6,   0, //'?'
         62, 127,  65,  93,  93,  31,  30, //'@'
         124, 126,  19,  19, 126, 124,  0, //'A'
         65, 127, 127,  73,  73, 127,  54, //'B'
         28,  62,  99,  65,  65,  99,  34,  //'C'
         65, 127, 127,  65,  99,  62,  28, //'D'
         65, 127, 127,  73,  93,  65,  99, //'E'
         65, 127, 127,  73,  29,   1,   3, //'F'
         28,  62,  99,  65,  81, 115, 114, //'G'
         127, 127,   8,   8, 127, 127,  0, //'H'
          0,  65, 127, 127,  65,   0,   0, //'I'
         48, 112,  64,  65, 127,  63,   1, //'J'
         65, 127, 127,   8,  28, 119,  99, //'K'
         65, 127, 127,  65,  64,  96, 112, //'L'
         127, 127,  14,  28,  14, 127,127, //'M'
         127, 127,   6,  12,  24, 127,127, //'N'
         28,  62,  99,  65,  99,  62,  28, //'O'
         65, 127, 127,  73,   9,  15,   6, //'P'
         30,  63,  33, 113, 127,  94,   0, //'Q'
         65, 127, 127,   9,  25, 127, 102, //'R'
         38, 111,  77,  89, 115,  50,   0, //'S'
          3,  65, 127, 127,  65,   3,   0, //'T'
         127, 127,  64,  64, 127, 127,   0, //'U'
         31,  63,  96,  96,  63,  31,   0, //'V'
         127, 127,  48, 24,  48, 127, 127, //'W'
         67, 103,  60,  24,  60, 103,  67, //'X'
          7,  79, 120, 120,  79,   7,   0, //'Y'
         71,  99, 113,  89,  77, 103, 115, //'Z'
          0, 127, 127,  65,  65,   0,   0, //'['
          1,   3,   6,  12,  24,  48,  96, //'\'
          0,  65,  65, 127, 127,   0,   0, //']'
          8,  12,   6,   3,   6,  12,   8, //'^'
         128, 128, 128, 128, 128,128, 128, //'_'
          0,   0,   3,   7,   4,   0,   0, //'`'
         32, 116,  84,  84,  60, 120,  64, //'a'
         65, 127,  63,  72,  72, 120,  48, //'b'
         56, 124,  68,  68, 108,  40,   0, //'c'
         48, 120,  72,  73,  63, 127,  64, //'d'
         56, 124,  84,  84,  92,  24,   0, //'e'
         72, 126, 127,  73,   3,   2,   0, //'f'
         56, 188, 164, 164, 252, 120,   0, //'g'
         65, 127, 127,   8,   4, 124, 120, //'h'
          0,  68, 125, 125,  64,   0,   0, //'i'
         96, 224, 128, 128, 253, 125,   0, //'j'
         65, 127, 127,  16,  56, 108,  68, //'k'
          0,  65, 127, 127,  64,   0,   0, //'l'
         120, 124,  28, 56,  28, 124, 120, //'m'
         124, 124,  4,   4, 124, 120,   0, //'n'
         56, 124,  68,  68, 124,  56,   0, //'o'
         0, 252, 252, 164,   36,  60,  24, //'p'
         24,  60,  36, 164, 248, 252, 132, //'q'
         68, 124, 120,  76,   4,  28,  24, //'r'
         72,  92,  84,  84, 116,  36,   0, //'s'
          0,   4,  62, 127,  68,  36,   0, //'t'
         60, 124,  64,  64,  60, 124,  64, //'u'
         28,  60,  96,  96,  60,  28,   0, //'v'
         60, 124, 112,  56, 112, 124,  60, //'w'
         68, 108,  56,  16,  56, 108,  68, //'x'
         60, 188, 160, 160, 252, 124,   0, //'y'
         76, 100, 116,  92,  76, 100,   0, //'z'
          8,   8,  62, 119,  65,  65,   0, //'{'
          0,   0,   0, 119, 119,   0,   0, //'|'
         65,  65, 119,  62,   8,   8,   0, //'}'
          2,   3,   1,   3,   2,   3,   1, //'~'
         255, 29, 129,129,  129, 129, 255, //''
         14, 159, 145, 177, 251,  74,   0  //'ç'
};

char const Fuente_5_7 [490]={
                         0x00,0x00,0x00,0x00,0x00,   // 20 space
                  0x00,0x00,0x5f,0x00,0x00,   // 21 !
                  0x00,0x07,0x00,0x07,0x00,   // 22 "
                  0x14,0x7f,0x14,0x7f,0x14,   // 23 #
                  0x24,0x2a,0x7f,0x2a,0x12,   // 24 $
                  0x23,0x13,0x08,0x64,0x62,   // 25 %
                  0x36,0x49,0x55,0x22,0x50,   // 26 &
                  0x00,0x05,0x03,0x00,0x00,   // 27 '
                  0x00,0x1c,0x22,0x41,0x00,   // 28 (
                  0x00,0x41,0x22,0x1c,0x00,   // 29 )
                  0x14,0x08,0x3e,0x08,0x14,   // 2a *
                  0x08,0x08,0x3e,0x08,0x08,   // 2b +
                  0x00,0x50,0x30,0x00,0x00,   // 2c ,
                  0x08,0x08,0x08,0x08,0x08,   // 2d -
                  0x00,0x60,0x60,0x00,0x00,   // 2e .
                  0x20,0x10,0x08,0x04,0x02,   // 2f /
                  0x3e,0x51,0x49,0x45,0x3e,   // 30 0
                  0x00,0x42,0x7f,0x40,0x00,   // 31 1
                  0x42,0x61,0x51,0x49,0x46,   // 32 2
                  0x21,0x41,0x45,0x4b,0x31,   // 33 3
                  0x18,0x14,0x12,0x7f,0x10,   // 34 4
                  0x27,0x45,0x45,0x45,0x39,   // 35 5
                  0x3c,0x4a,0x49,0x49,0x30,   // 36 6
                  0x01,0x71,0x09,0x05,0x03,   // 37 7
                  0x36,0x49,0x49,0x49,0x36,   // 38 8
                  0x06,0x49,0x49,0x29,0x1e,   // 39 9
                  0x00,0x36,0x36,0x00,0x00,   // 3a :
                  0x00,0x56,0x36,0x00,0x00,   // 3b ;
                  0x08,0x14,0x22,0x41,0x00,   // 3c <
                  0x14,0x14,0x14,0x14,0x14,   // 3d =
                  0x00,0x41,0x22,0x14,0x08,   // 3e >
                  0x02,0x01,0x51,0x09,0x06,   // 3f ?
                  0x32,0x49,0x79,0x41,0x3e,   // 40 @
                  0x7e,0x11,0x11,0x11,0x7e,   // 41 A
                  0x7f,0x49,0x49,0x49,0x36,   // 42 B
                  0x3e,0x41,0x41,0x41,0x22,   // 43 C
                  0x7f,0x41,0x41,0x22,0x1c,   // 44 D
                  0x7f,0x49,0x49,0x49,0x41,   // 45 E
                  0x7f,0x09,0x09,0x09,0x01,   // 46 F
                  0x3e,0x41,0x49,0x49,0x7a,   // 47 G
                  0x7f,0x08,0x08,0x08,0x7f,   // 48 H
                  0x00,0x41,0x7f,0x41,0x00,   // 49 I
                  0x20,0x40,0x41,0x3f,0x01,   // 4a J
                  0x7f,0x08,0x14,0x22,0x41,   // 4b K
                  0x7f,0x40,0x40,0x40,0x40,   // 4c L
                  0x7f,0x02,0x0c,0x02,0x7f,   // 4d M
                  0x7f,0x04,0x08,0x10,0x7f,   // 4e N
                  0x3e,0x41,0x41,0x41,0x3e,   // 4f O
                        0x7f,0x09,0x09,0x09,0x06,   // 50 P
                  0x3e,0x41,0x51,0x21,0x5e,   // 51 Q
                  0x7f,0x09,0x19,0x29,0x46,   // 52 R
                  0x46,0x49,0x49,0x49,0x31,   // 53 S
                  0x01,0x01,0x7f,0x01,0x01,   // 54 T
                  0x3f,0x40,0x40,0x40,0x3f,   // 55 U
                  0x1f,0x20,0x40,0x20,0x1f,   // 56 V
                  0x3f,0x40,0x38,0x40,0x3f,   // 57 W
                  0x63,0x14,0x08,0x14,0x63,   // 58 X
                  0x07,0x08,0x70,0x08,0x07,   // 59 Y
                  0x61,0x51,0x49,0x45,0x43,   // 5a Z
                  0x00,0x7f,0x41,0x41,0x00,   // 5b [
                  0x02,0x04,0x08,0x10,0x20,   // 5c
                  0x00,0x41,0x41,0x7f,0x00,   // 5d
                  0x04,0x02,0x01,0x02,0x04,   // 5e
                  0x40,0x40,0x40,0x40,0x40,   // 5f
                  0x00,0x01,0x02,0x04,0x00,   // 60
                  0x20,0x54,0x54,0x54,0x78,   // 61 a
                  0x7f,0x48,0x44,0x44,0x38,   // 62 b
                  0x38,0x44,0x44,0x44,0x20,   // 63 c
                  0x38,0x44,0x44,0x48,0x7f,   // 64 d
                  0x38,0x54,0x54,0x54,0x18,   // 65 e
                  0x08,0x7e,0x09,0x01,0x02,   // 66 f
                  0x0c,0x52,0x52,0x52,0x3e,   // 67 g
                  0x7f,0x08,0x04,0x04,0x78,   // 68 h
                  0x00,0x44,0x7d,0x40,0x00,   // 69 i
                  0x20,0x40,0x44,0x3d,0x00,   // 6a j
                  0x7f,0x10,0x28,0x44,0x00,   // 6b k
                  0x00,0x41,0x7f,0x40,0x00,   // 6c l
                  0x7c,0x04,0x18,0x04,0x78,   // 6d m
                  0x7c,0x08,0x04,0x04,0x78,   // 6e n
                  0x38,0x44,0x44,0x44,0x38,   // 6f o
                  0x7c,0x14,0x14,0x14,0x08,   // 70 p
                  0x08,0x14,0x14,0x18,0x7c,   // 71 q
                  0x7c,0x08,0x04,0x04,0x08,   // 72 r
                  0x48,0x54,0x54,0x54,0x20,   // 73 s
                  0x04,0x3f,0x44,0x40,0x20,   // 74 t
                  0x3c,0x40,0x40,0x20,0x7c,   // 75 u
                  0x1c,0x20,0x40,0x20,0x1c,   // 76 v
                  0x3c,0x40,0x30,0x40,0x3c,   // 77 w
                  0x44,0x28,0x10,0x28,0x44,   // 78 x
                  0x0c,0x50,0x50,0x50,0x3c,   // 79 y
                  0x44,0x64,0x54,0x4c,0x44,   // 7a z
                  0x00,0x08,0x36,0x41,0x00,   // 7b
                  0x00,0x00,0x7f,0x00,0x00,   // 7c
                  0x00,0x41,0x36,0x08,0x00,   // 7d
                  0x10,0x08,0x08,0x10,0x08,   // 7e
                  0x78,0x46,0x41,0x46,0x78,  // 7F
                       0x7C,0x09,0x05,0x05,0x78,  // 80 -> ñ
                       0x7E,0x05,0x09,0x11,0x7E   // 81 -> Ñ
                     };   // 7f  
  
float const senos[46] = {0,0.0348,0.0697,0.1045,0.1391,0.1736,0.2079,0.2419,
                  0.2756,0.309,0.342,0.3746,0.4067,0.4383,0.4694,0.5,0.5299,
                  0.5591,0.5877,0.6156,0.6427,0.6691,0.6946,0.7193,0.7431,
                  0.766,0.788,0.809,0.829,0.848,0.866,0.8829,0.8987,0.9135,
                  0.9271,0.9396,0.951,0.9612,0.9702,0.9781,0.9848,0.9902,
                  0.9945,0.9975,0.9993,1};

float const cosenos[46] = {1,0.9993,0.9975,0.9945,0.9902,0.9848,0.9781,0.9702,
                  0.9612,0.951,0.9396,0.9271,0.9135,0.8987,0.8829,0.866,
                  0.848,0.829,0.809,0.788,0.766,0.7431,0.7193,0.6946,0.6691,
                  0.6427,0.6156,0.5877,0.5591,0.5299,0.5,0.4694,0.4383,
                  0.4067,0.3746,0.342,0.309,0.2756,0.2419,0.2079,0.1736,
                  0.1391,0.1045,0.0697,0.0348,0};  
  
void nokia_clean_ddram(void);

////////////////////////////////////////////////////////////////////////////////
// Envía en serie todos los bits del byte pasado
////////////////////////////////////////////////////////////////////////////////
void nokia_write_dorc(int8 bytefornokia)
{
   char caa;
   
   for (caa = 0; caa < 8; caa++)
   {
      output_low(Pin_Nok_Sclk);
      
      delay_us(2);
            
      if ((bytefornokia & 0b10000000) == 0) 
      {
         output_low(Pin_Nok_Sda);
      }
      else 
      {
         output_high(Pin_Nok_sda);
      }       
      
      output_high(Pin_Nok_Sclk);
      
      bytefornokia <<= 1;
   }
}

////////////////////////////////////////////////////////////////////////////////
// Prepara a la LCD para que guarde el byte en la RAM (para que lo muestre
////////////////////////////////////////////////////////////////////////////////
void nokia_write_data(int8 bytefornokia_data)
{   
   output_high(Pin_Nok_DC);
   
   output_low(Pin_Nok_CS);    //chip enabled
   nokia_write_dorc(bytefornokia_data);   // Rutina que manda en serie los bits
   output_high(Pin_Nok_CS);   // chip disabled
}


////////////////////////////////////////////////////////////////////////////////
// Escribe un comando en el nokia. Para los comandos se activa D/C
////////////////////////////////////////////////////////////////////////////////
void nokia_write_command(int8 bytefornokia_command)
{
   output_low(Pin_Nok_DC);   // byte is a command it is read with the eight SCLK pulse
   
   output_low(Pin_Nok_CS);   // chip enabled
   nokia_write_dorc(bytefornokia_command);
   output_high(Pin_Nok_CS);   // chip disabled
}


////////////////////////////////////////////////////////////////////////////////
// Escribe en la LCD el caracter pasado
////////////////////////////////////////////////////////////////////////////////
void nokia_printchar(int8 caracter)
{
   int8 char_row, chardata;
   int16 charpos;

   if (caracter == 0xF1) caracter = 0x80;
   else if (caracter == 0xD1) caracter = 0x81;
   else if (caracter == '\f') {nokia_clean_ddram(); return;}
   else if (caracter < 0x20) return;
   else if (caracter > 0x7f) return;

   for (char_row = 0; char_row < 5; char_row++)    // Son 5 Bytes por caracter Por eso este bucle
   {            
      charpos = ((int16)((caracter & 0xFF) - 0x20) * 5) + char_row;
     chardata = ASCII_TABLE[charpos];
   
      nokia_write_data(chardata);                    // Manda el caracter
   }

   nokia_write_data(0x00);                           // Mandamos 1 espacio Blanco
}


////////////////////////////////////////////////////////////////////////////////
// Posicionamiento en la pantalla
////////////////////////////////////////////////////////////////////////////////
void nokia_gotoxy(int8 xnokia, int8 ynokia)      // Nokia LCD 3310 Position cursor
{
      nokia_write_command(0x40 | (ynokia & 0x07));   // Y axe initialisation: 0100 0yyy
      nokia_write_command(0x80 | (xnokia & 0x7f));   // X axe initialisation: 1xxx xxxx
}


////////////////////////////////////////////////////////////////////////////////
// Limpiamos la RAM (la pantalla)
////////////////////////////////////////////////////////////////////////////////
void nokia_borra_pantalla(void)
{
   int16 ddram;  //cambiar por la del rtc del tio eso

   nokia_gotoxy(0,0);                  // Vamos al principio
   
   for (ddram = 504; ddram > 0; ddram--) // bucle estableciendo cada byte a 0x00
   {   
      nokia_write_data(0x00);
   }
}

////////////////////////////////////////////////////////////////////////////////
// Vuelca la memoria de video sobre la pantalla
////////////////////////////////////////////////////////////////////////////////
void nokia_imprime_memoria(void)
{
int16 ddram;  

   nokia_gotoxy(0,0);                  // Vamos al principio
   for (ddram=0;ddram<504;ddram++) {   
      nokia_write_data(MemoriaVideo[ddram]);
   }
}

////////////////////////////////////////////////////////////////////////////////
// Vacía la memoria de video 
////////////////////////////////////////////////////////////////////////////////
void nokia_borra_memoria(void)
{
int16 ddram;  
   for (ddram=0;ddram<504;ddram++)
      MemoriaVideo[ddram]=0;
}

////////////////////////////////////////////////////////////////////////////////
// Pinta un punto en la memoria 
////////////////////////////////////////////////////////////////////////////////
void nokia_punto (int8 x, int8 y, int1 color)
{
   int16 ddram; 
   
   ddram = x + (((int16)(y / 8) * 84));
   
   if (color)
       MemoriaVideo[ddram] = MemoriaVideo[ddram] | (1 << (y % 8));
   else
       MemoriaVideo[ddram] = MemoriaVideo[ddram] & (0xFF - (1 << (y % 8)));
}

////////////////////////////////////////////////////////////////////////////////
// Pinta una línea en la memoria 
////////////////////////////////////////////////////////////////////////////////
void nokia_linea (int8 x1, int8 y1, int8 x2, int8 y2, int1 color)
{
   int8 TramoX, TramoY,MenorX,MenorY,MayorX,MayorY,TramoMax,i,xx,yy;
   float OffsetX, OffsetY;

   if (y1>y2)
      {MenorY=y2;MayorY=y1;OffsetY=-1;}
   else
      {MenorY=y1;MayorY=y2;OffsetY=1;}

   if (y1==y2) OffsetY=0;

   if (x1>x2)
      {MenorX=x2;MayorX=x1;OffsetX=-1;}
   else
      {MenorX=x1;MayorX=x2;OffsetX=1;}

   if (x1==x2) OffsetX=0;

   TramoX = MayorX-MenorX+1;
   TramoY = MayorY-MenorY+1;

   if (TramoX>TramoY)
      {
         OffsetY = (float)TramoY / (float)TramoX * OffsetY;
         TramoMax=TramoX;
      }
   else
      {
         OffsetX = (float)TramoX / (float)TramoY * OffsetX;
         TramoMax=TramoY;
      }

   for (i=0;i<TramoMax;i++)
      {
         xx=x1 + OffsetX * i;
         yy=y1 + OffsetY * i;
         if ((xx<ANCHOPANTALLA) && (yy<ALTOPANTALLA) && (xx>=0) && (yy>=0))
            nokia_punto (xx,yy, color);
      }
}


////////////////////////////////////////////////////////////////////////////////
// Pinta un rectángulo
////////////////////////////////////////////////////////////////////////////////
void nokia_rectangulo (int8 x1, int8 y1, int8 x2, int8 y2, int1 estilo, int1 color)
{
   int16 i;

   nokia_linea (x1,y1,x2,y1,color);
   nokia_linea (x1,y2,x2,y2,color);
   nokia_linea (x2,y1,x2,y2,color);
   nokia_linea (x1,y1,x1,y2,color);

   if (estilo)
      if (y1<y2)
         for (i=y1;i<y2;i++)
            nokia_linea (x1,i,x2,i,color);
      else
         for (i=y1;i>y2;i--)
            nokia_linea (x1,i,x2,i,color);
}

////////////////////////////////////////////////////////////////////////////////
// Calcula el arcocoseno de un número
////////////////////////////////////////////////////////////////////////////////
float arcocoseno(int8 perimetro, int8 radio)
{
   char j = 45;
   
   while ((j > 0) && ((senos[j] * (float)radio) >= perimetro))
   {
      j--;
   }
   
   return (cosenos[j] * (float)radio * 1.16666); // 1.16666 es el factor de correción de la pantalla
}
         
////////////////////////////////////////////////////////////////////////////////
// Pinta un círculo
////////////////////////////////////////////////////////////////////////////////
void nokia_circulo(int8 x, int8 y, int8 radio, int1 estilo, int1 color)
{
   int8 i;
   float xx, antxx=0;

   for (i = (y - radio); i <= y; i++)
   {
      xx = arcocoseno(y - i, radio);
         
      if (i > (y - radio))
      {
         nokia_linea(x + (int8)xx, i, x + (int8)antxx, i - 1, NEGRO);
         nokia_linea(x - (int8)xx, i, x - (int8)antxx, i - 1, NEGRO);
         nokia_linea(x + (int8)xx, 2 * y - 1 - i, x + (int8)antxx, 2 * y - i, NEGRO);
         nokia_linea(x - (int8)xx, 2 * y - 1 - i, x - (int8)antxx, 2 * y - i, NEGRO);
      }
      
      if (estilo)
      {
         nokia_linea(x + (int8)xx, i - 1, x - (int8)xx, i - 1, NEGRO);
         nokia_linea(x + (int8)xx, 2 * y - 1 - i, x - (int8)xx, 2 * y - 1 - i, NEGRO);
      }
      
      antxx = xx;
   }
}

////////////////////////////////////////////////////////////////////////////////
// Pinta un bitmap
////////////////////////////////////////////////////////////////////////////////
void nokia_bitmap (signed int8 x, signed int8 y, int8 *bitmap, int8 ancho, int8 alto)
{
   int16 ind_bitmap, ind_memoria, xx, yy, desplazamiento;

   for (yy = 0; yy < ((alto >> 3) + ((alto % 8) > 0)); yy++)
   {
      for (xx = 0; xx < ancho; xx++)
      {
         if (((xx + x) < 84) && ((xx + x) > 0))
         {
            ind_memoria = yy * 84 + xx + x + (int16)(y >> 3) * 84;
            ind_bitmap = yy * (int16)ancho + xx;
            desplazamiento = y % 8;
            MemoriaVideo[ind_memoria] = MemoriaVideo[ind_memoria] | (bitmap[ind_bitmap] << desplazamiento);
            
            if (desplazamiento)
            {
               MemoriaVideo[ind_memoria + 84] = MemoriaVideo[ind_memoria + 84] | ((bitmap[ind_bitmap] >> (8 - desplazamiento)) & (0xFF >> (8 - desplazamiento)));
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
// Escribe gráficamente texto en la LCD
////////////////////////////////////////////////////////////////////////////////
void nokia_texto(signed int8 x, signed int8 y, int8 fuente, char *caracter)
{
   int16 ind_bitmap, ind_memoria, xx, desplazamiento;
   int8 letra, alto, ancho, EscribirByte;

   if (fuente == 1)
   {
      ancho = 3; 
      alto = 6;
   }
   else if (fuente == 2)
   {
      ancho = 5;
      alto = 7;
   }
   else
   {
      ancho = 7;
      alto = 8;
   }

   letra = *caracter;
   
   while (letra != 0)
   {
      for (xx = 0; xx < ancho; xx++)
      {
         if (((xx + x) < 84) && ((xx + x) >= 0) && (y >= 0)&& (y <= 40))
         {
            ind_memoria = xx + x + (int16)(y >> 3) * 84;
            ind_bitmap = (int16)(letra - 0x20) * ancho + xx;
            desplazamiento = y % 8;

            if (fuente == 1)
            {    
               EscribirByte = Fuente_3_6[ind_bitmap];
            }
            else if (fuente == 2)
            {
               EscribirByte = Fuente_5_7[ind_bitmap];
            }
            else
            {
               EscribirByte = Fuente_7_8[ind_bitmap];
            }
                   
            MemoriaVideo[ind_memoria] = MemoriaVideo[ind_memoria] | (EscribirByte << desplazamiento);
                  
            if (desplazamiento)
            {
               MemoriaVideo[ind_memoria + 84] = MemoriaVideo[ind_memoria + 84] | ((EscribirByte >> (8 - desplazamiento)) & (0xFF >> (8 - desplazamiento)));                        
            }
         }
      }
            
      caracter++;
      letra = *caracter;
      x = x + ancho + 1;
   }
}

////////////////////////////////////////////////////////////////////////////////
// Inicialización de la pantalla
////////////////////////////////////////////////////////////////////////////////
void Lcd_init(void) 
{   
   delay_us(200);       // Pequeño delay de seguridad
   
   output_high(Pin_Nok_DC);            // Destino de Bytes -> RAM
   output_high(Pin_Nok_CS);            // Desactivamos el Driver para que se inicialice
   
   delay_ms(10);
   
   output_low(Pin_Nok_Res);            // Y damos un pulso de reset según indica el datasheet   
   delay_ms(250);         // durante 1us.  
   output_high(Pin_Nok_Res);
   
   nokia_write_command(0b00100001);   // Activamos los comandos extendidos
   nokia_write_command(0x90);   // Definimos el Vop. Establecerá la tensión interna del cristal (su contraste)
   nokia_write_command(0x13);   // bias 13. Min 10 Max 17
   nokia_write_command(0b00100000);    // Modo horizontal, de izq. a dcha. Se incrementa la posición automáticamente
   nokia_write_command(0x09);   // all on
   
   delay_ms(50);
   
   nokia_borra_pantalla();        // Limpiamos la RAM para que no haya basura en la LCD
   
   delay_ms(10);
   
   nokia_write_command(0b00001100);   // mod control normal change
}
