////////////////////////////////////////////////////////////////////////
//18F4550Trainer_V1.0 Bootloader - www.microingenia.com     16/02/2010//
//                                                                    //
//Bootloader based on CCS one. Power your Trainer with PIN_C0 to low  //
//and use MiEBootloader Software to burn your code!                   //
//                                                                    //
//Released under the cc by-nc-sa 3.0 License.                         //
//http://creativecommons.org/licenses/by-nc-sa/3.0/                   //
////////////////////////////////////////////////////////////////////////
#include "config18F4550Trainer.c"

#define LOADER_ISR 0x28
#build(interrupt=LOADER_ISR)

#define _bootloader
#include "usb\usb_bootloader.h"
#include "usb\usb_cdc.h"

#org 0x08,0x17
void high_isr(void) 
{
   if (bit_test(g_InBootloader,0))
   {
    #ASM
     goto LOADER_ISR
    #ENDASM
   }
   else
   {
    #ASM
     goto APPLICATION_ISR
    #ENDASM
   }
}

#org 0x18,0x27
void low_isr(void) 
{
   if (bit_test(g_InBootloader,0))
   {
    #ASM
     goto LOADER_ISR+0x10
    #ENDASM
   }
   else
   {
    #ASM
     goto APPLICATION_ISR+0x10
    #ENDASM
   }
}

#define ROM_BLOCK_INVALID -1

int32 rom_block_start = ROM_BLOCK_INVALID;

#define EEPROM_ERASE_SIZE  getenv("FLASH_ERASE_SIZE")
int8 rom_block[EEPROM_ERASE_SIZE];

void rom_w_flush(void)
{
   if (rom_block_start != ROM_BLOCK_INVALID)
   {
      erase_program_eeprom(rom_block_start);
   
      write_program_memory(rom_block_start, rom_block, sizeof(rom_block));
      
      rom_block_start = ROM_BLOCK_INVALID;
   }
}

void rom_w_block(int32 location, char *src, int16 size)
{
   int32 block_start;
   int16 i,num;

   block_start = location & (~((int32)EEPROM_ERASE_SIZE-1));
      
   i = location - block_start;

   while (size) 
   {
      if (block_start != rom_block_start)
      {
         rom_w_flush();
         
         rom_block_start = block_start;
         
         read_program_memory(block_start, rom_block, sizeof(rom_block));
      }   

      if (size>(EEPROM_ERASE_SIZE-i)) num=EEPROM_ERASE_SIZE-i;
      else num=size;

      memcpy(&rom_block[i], src, num);

      src += num;
      block_start += EEPROM_ERASE_SIZE;
      i = 0;
      size -= num;
   }
}

void rom_w(int32 location, char *src, int16 size)
{
   rom_w_block(location, src, size);
   rom_w_flush();
}

#define BUFFER_LEN_LOD 64

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

unsigned int8 atoi_b16(char *s) 
{  
   char c;
   unsigned int8 result = 0;
   int i;

   for (i=0; i<2; i++,s++)  
   {
      c = *s;
      if (c >= 'A')
         result = 16*result + c - 'A' + 10;
      else
         result = 16*result + c - '0';         
   }

   return(result);
}

void load_program(void)
{
   int1  do_ACKLOD, done=FALSE;
   int8  checksum, line_type;
   int16 l_addr,h_addr=0;
   int8 to;
   int32 addr;
   int8  dataidx, i, count;
   int8  data[32];
   int  buffidx;
   char buffer[BUFFER_LEN_LOD];
   
   while (!done)
   {
      usb_task();
      
      if (!usb_cdc_kbhit())
         continue;
         
      buffidx = 0;
      to = 250;
      do 
      {
         if (!usb_cdc_kbhit())
         {
            delay_ms(1);
            to--;
            if (!to)
               break;
         }
         else
            to = 250;
         i = usb_cdc_getc();
         buffer[buffidx++] = i;
      } while ( (i != 0x0D) && (i != 0x0A) && (buffidx <= BUFFER_LEN_LOD) );
            
      if (!to)
         continue;

      usb_cdc_putc(XOFF);

      do_ACKLOD = TRUE;

      if (buffer[0] == ':') 
      {
         count = atoi_b16 (&buffer[1]);

         l_addr = make16(atoi_b16(&buffer[3]),atoi_b16(&buffer[5]));

         line_type = atoi_b16 (&buffer[7]);

         addr = make32(h_addr,l_addr);

         checksum = 0;
         for (i=1; i<(buffidx-3); i+=2)
            checksum += atoi_b16 (&buffer[i]);
         
         checksum = 0xFF - checksum + 1;

         if (checksum != atoi_b16 (&buffer[buffidx-3]))
            do_ACKLOD = FALSE;
         else
         {
            if (line_type == 1) 
               done = TRUE;
            else if (line_type == 4)
               h_addr = make16(atoi_b16(&buffer[9]), atoi_b16(&buffer[11]));
            else if ((line_type == 0) && (addr >= (int32)APPLICATION_START) && (addr < ((int32)APPLICATION_END)))
            {
               for (i = 9,dataidx=0; i < buffidx-3; i += 2)
                  data[dataidx++]=atoi_b16(&buffer[i]);
                       
               rom_w_block(addr, data, count);
            }
         }
      }

      if (do_ACKLOD) usb_cdc_putc (ACKLOD);

      usb_cdc_putc(XON);
   }

   rom_w_flush();

   usb_cdc_putc (ACKLOD);
   usb_cdc_putc(XON);
   delay_ms(100);
   #zero_ram
   reset_cpu();
}

void main(void) 
{
   if(!input(BTLDR))
   {
      while(!input(BTLDR));
      OFF(LEDR);
      g_InBootloader = TRUE;
      usb_cdc_init();
      usb_init();
      while(!usb_enumerated());
      load_program();
   }
   g_InBootloader = FALSE;
   
   #ASM
      goto APPLICATION_START
   #ENDASM
}
