 /* 
      Nokia 3310 LCD Demo. Codes are partially modified from Michel 
      Bavin's CCS C Codes for Nokia 3310 Graphical LCD 
      (LPH7779) routines version 3 from http://users.skynet.be/bk317494/ 
      
   Connection: 
   - get the pinout of the 3310 screen online 
   - how the 3310 pins are connected to the PIC pin are defined by the define statements below: 
    
       #define SCK          PIN_D7               // Connections between PIC and LCD. 
      #define SDI          PIN_D6 
      #define D_C          PIN_D5 
      #define _SCE         PIN_D4 
      #define _RES         PIN_D3 
    
   The above shows that by default, pins D3 to D7 are used by this code. 
   You can change by modifying the #define statements below. 
    
   The 480-byte large array "Table" is used to define the font used for the display. 
   The comment on each line of the code for declaring the array shows the character that line defines. 
   DO NOT MESS WITH THIS TABLE AT ALL UNLESS YOU NEED TO CHANGE THE STYLE WITH WHICH A PARTICULAR CHARACTER IS DISPLAYED 
    
   Contrastup and contrastdown increase or reduce the contrast by a small amount respectively each time they are called 
    
   Initlcd must be called before any other thing is done with or to the lcd. It needs to be called only once close to the beginning of your code. 
   There is no need to call it again unless program execution resets for whatever reason 
    
    
   Writecomm, Writedata,Clockdata : DO NOT these functions. They are called by some other functions. 
    
   Clearam clears the entire screen 
    
   Cursorxy places the cursor at a screen location. Note that the cursor itself is invisible. Use Cursor XY to 
   determine where the next character you write will appear. 
    
   Putlcdchar allows you to write a single character to the LCD at the present cursor location. Character written with a foreground of 
   black and a background of white. 
    
   Putlcdcharinv  writes a single character to the LCD. Difference is that the character is written with a foreground of white on a background of black. 
    
   putstr and putstrinv both write an array of chacters to the LCD. outstr writes with a forground of black on white while putstrinv 
   writes with white on a black background. 
   */ 

// Structures used by LCD 
#define SCK          PIN_D7               // Connections between PIC and LCD. 
#define SDI          PIN_D6 
#define D_C          PIN_D5 
#define _SCE         PIN_D4 
#define _RES         PIN_D3 

int lcdcontrast = 216;           //Contrast initially set at 216 

void initlcd(void);               // Initializes the LCD. 
void writecom(char);            // Writes a command. 
void writedata(char);            // Writes data to DDRAM to illuminate the pixels. 
void clockdata(char);            // Clocks in data into the PCD8544 controller. 
void clearram(void);            // Erase the DDRAM contents. 
void cursorxy(char,char);         // Position cursor to x,y. 
void putlcdchar(char);               // Write one character. 
void putlcdcharinv(char);               // Write one character. 
void putstr(char *message);         // Writes a string of characters. 
void putstrinv(char *message);         // Writes a string of characters. 

const char table[480] = {  0x00,0x00,0x00,0x00,0x00,// 20 space      ASCII table for NOKIA LCD: 96 rows * 5 bytes= 480 bytes 
                     0x00,0x00,0x5f,0x00,0x00,   // 21 !         Note that this is the same set of codes for character you 
                     0x00,0x07,0x00,0x07,0x00,   // 22 "         would find on a HD44780 based character LCD. :) 
                     0x14,0x7f,0x14,0x7f,0x14,   // 23 #         Also, given the size of the LCD (84 pixels by 48 pixels), 
                     0x24,0x2a,0x7f,0x2a,0x12,   // 24 $         the maximum number of characters per row is only 14. :) 
                     0x23,0x13,0x08,0x64,0x62,   // 25 % 
                     0x36,0x49,0x55,0x22,0x50,   // 26 & 
                     0x00,0x05,0x03,0x00,0x00,   // 27 '      KAY: 480 bytes above obtained only from fact that the table has 96 elements 
                     0x00,0x1c,0x22,0x41,0x00,   // 28 (           and not from any physical limitation of the LCD itself 
                     0x00,0x41,0x22,0x1c,0x00,   // 29 ) 
                     0x14,0x08,0x3e,0x08,0x14,   // 2a * 
                     0x08,0x08,0x3e,0x08,0x08,   // 2b + 
                     0x00,0x50,0x30,0x00,0x00,   // 2c , 
                     0x08,0x08,0x08,0x08,0x08,   // 2d - 
                     0x00,0x70,0x70,0x70,0x00,   // 2e .   // changed from 0x00,0x60,0x60,0x00,0x00 on 27/02/09 
                     0x20,0x10,0x08,0x04,0x02,   // 2f / 
                     0x3e,0x51,0x49,0x45,0x3e,   // 30 0    //start 
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
                     0x02,0x04,0x08,0x10,0x20,   // 5c Yen Currency Sign 
                     0x00,0x41,0x41,0x7f,0x00,   // 5d ] 
                     0x04,0x02,0x01,0x02,0x04,   // 5e ^ 
                     0x40,0x40,0x40,0x40,0x40,   // 5f _ 
                     0x00,0x01,0x02,0x04,0x00,   // 60 ` 
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
                     0x00,0x08,0x36,0x41,0x00,   // 7b < 
                     0x00,0x00,0x7f,0x00,0x00,   // 7c | 
                     0x00,0x41,0x36,0x08,0x00,   // 7d > 
                     0x10,0x08,0x08,0x10,0x08,   // 7e Right Arrow    -> 
                     0x78,0x46,0x41,0x46,0x78};   // 7f Left Arrow   <- 

//////////////////////////////////////////////////////////////////////// 
void contrastup(void) 
{ 
   //write 000100    001 
   //000100 in ANY mode sets the mode (value of H and V etc) 
   if(lcdcontrast<250) 
   { 
      writecom(0x21);      // Activate Chip and H=1.        bit PD=0, V=0, H=1  // 
      lcdcontrast=lcdcontrast+5; 
      writecom(lcdcontrast);      // Set LCD Voltage to about 7V.         //Kay changed from C2 
      writecom(0x20);      // Horizontal addressing and H=0. 
    } 

} 

//////////////////////////////////////////////////////////////////////// 
void contrastdown(void) 
{ 
   if(lcdcontrast>132) 
   { 
      writecom(0x21);      // Activate Chip and H=1.        bit PD=0, V=0, H=1  // 
      lcdcontrast=lcdcontrast-5; 
      writecom(lcdcontrast);      // Set LCD Voltage to about 7V.         //Kay changed from C2 
      writecom(0x20);      // Horizontal addressing and H=0. 
    } 
} 

//////////////////////////////////////////////////////////////////////// 
void initlcd(void) 
{ 
   output_high(_RES);      // Set _RES HIGH. 
   output_high(_SCE);      // Disable Chip. 
   output_low(_RES);       // Reset the LCD. 
   //delay_ms(100);          // Wait 100ms. 
   output_high(_RES);      // Awake LCD from RESET state. 

   writecom(0x21);      // Activate Chip and H=1.        bit PD=0, V=0, H=1  // 
   writecom(lcdcontrast);      // Set LCD Voltage to about 7V.         //Kay changed from C2 
   writecom(0x13);      // Adjust voltage bias. 
   writecom(0x20);      // Horizontal addressing and H=0.      //Can change to vertical mode to draw screen, but hope no stress associated 
                                                               //H doesn't need to be set to 1 any more unless we need to change Bias or LCD Voltage 
                                                               //to set to vertical address mode, writecom(0x22) 
   writecom(0x09);      // Activate all segments. 
   clearram();          // Erase all pixel on the DDRAM. 
   writecom(0x08);      // Blank the Display. 
   writecom(0x0C);      // Display Normal. 
   cursorxy(0,0);       // Cursor Home. 
} 
//////////////////////////////////////////////////////////////////////// 
void writecom(char command_in) 
{ 
   output_low(D_C);              // Select Command register. 
   output_low(_SCE);             // Select Chip. 
   clockdata(command_in);        // Clock in command bits. 
   output_high(_SCE);            // Deselect Chip. 
} 
//////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////// 
void writedata(char data_in) 
{ 
   output_high(D_C);             // Select Data register. 
   output_low(_SCE);             // Select Chip. 
   clockdata(data_in);           // Clock in data bits. 
   output_high(_SCE);            // Deselect Chip. 
} 
//////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////// 
void clockdata(char bits_in) 
{ 
   int bitcnt; 
   for (bitcnt=8; bitcnt>0; bitcnt--) 
      { 
      output_low(SCK); 
            // Set Clock Idle level LOW. 
         if ((bits_in&0x80)==0x80) {output_high(SDI);}      // PCD8544 clocks in the MSb first. 
         else {output_low(SDI);} 
       output_high(SCK);                        // Data is clocked on the rising edge of SCK. 
                 //ADDED BY KAY 
      bits_in=bits_in<<1;                  // Logical shift data by 1 bit left. 
      } 
} 
//////////////////////////////////////////////////////////////////////// 
// CLEARRAM: clears the RAM by writing blanks to every single RAM location 
// Each RAM location controls 8 vertical pixels 
//////////////////////////////////////////////////////////////////////// 
void clearram(void) 
{ 
   int16 ddram; 
   cursorxy(0,0);                                 // Cursor Home. 
   for (ddram=700;ddram>0;ddram--)   {writedata(0x00);}      // 6*84 = 504 DDRAM addresses.       //changed to 600 to troubleshoot 
} 
//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 
// CURSORXY: Uses appropriate Command to take "cursor" to right RAM address. 
// This is probably done by specifying a RAM address to go to 
//////////////////////////////////////////////////////////////////////// 

void cursorxy(char x, char y) 
{ 
   writecom(0x40|(y&0x07));   // Y axis 
   writecom(0x80|(x&0x7f));   // X axis 
} 
//////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////// 
void putlcdchar(char character) 
{ 
   int16 tablept; 
   int16 characterint; 
   int count; 
   int char_column; 

   if ((character<0x20)||(character>0x7f)) {return;}// Exit function if character is not found. 
   characterint=character; 
   tablept = ((5*characterint) - 160);                  // Point to the columns of the character in the table. 

   for (count=5;count>0;count--)                     // Draw the columns to print the character. 
   { 
      char_column = table[tablept]; 
      writedata(char_column); 
      tablept++; 
   } 
   writedata(0x00);                              // 1 pixel spacing per character. 
} 

//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 
void putlcdcharinv(char character) 
{ 
   int16 tablept; 
   int16 characterint; 
   int count; 
   int char_column; 

   if ((character<0x20)||(character>0x7f)) {return;}// Exit function if character is not found. 
   characterint=character; 
    tablept = ((5*characterint) - 160);                   // Point to the columns of the character in the table. 

   for (count=5;count>0;count--)                     // Draw the columns to print the character. 
   { 
      char_column = table[tablept]^0xFF; 
      writedata(char_column); 
      tablept++; 
   } 
   writedata(0xFF);                              // 1 pixel spacing per character. 
} 
//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 
void putstr(char *message) 
{ 
 // int i=0; 
   while(*message) {putlcdchar(*message++);}                        // Points to one ASCII to be written one at a time. 
// while((i<30)&&(message[i])) 
// { 
 // putchar(message[i]); 
 // i++; 
 //  } 
  } 
//////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////// 
void putstrinv(char *message) 
{ 
   while(*message) {putlcdcharinv(*message++);}                        // Points to one ASCII to be written one at a time. 
} 
//////////////////////////////////////////////////////////////////////// 
