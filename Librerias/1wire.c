// (C) copyright 2003 j.d.sandoz / jds-pic !at! losdos.dyndns.org 

// released under the GNU GENERAL PUBLIC LICENSE (GPL) 
// refer to http://www.gnu.org/licenses/gpl.txt 

// This program is free software; you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version. 

// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 

// You should have received a copy of the GNU General Public License 
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

/***********************1Wire Class***********************/ 
/*Description: This class handles all communication */ 
/* between the processor and the 1wire */ 
/* sensors. 
/*********************************************************/ 

/*-------1-wire definitions-------*/ 
#define ONE_WIRE_PIN PIN_A0 

/*******************1-wire communication functions********************/ 

/************onewire_reset*************************************************/ 
/*This function initiates the 1wire bus */ 
/* */ 
/*PARAMETERS: */ 
/*RETURNS: */ 
/*********************************************************************/ 

void onewire_reset()  // OK if just using a single permanently connected device 
{ 
 output_low(ONE_WIRE_PIN); 
 delay_us( 500 ); // pull 1-wire low for reset pulse 
 output_float(ONE_WIRE_PIN); // float 1-wire high 
 delay_us( 500 ); // wait-out remaining initialisation window. 
 output_float(ONE_WIRE_PIN); 
} 

/*********************** onewire_write() ********************************/ 
/*This function writes a byte to the sensor.*/ 
/* */ 
/*Parameters: byte - the byte to be written to the 1-wire */ 
/*Returns: */ 
/*********************************************************************/ 

void onewire_write(int data) 
{ 
 int count; 

 for (count=0; count<8; ++count) 
 { 
  output_low(ONE_WIRE_PIN); 
  delay_us( 2 ); // pull 1-wire low to initiate write time-slot. 
  output_bit(ONE_WIRE_PIN, shift_right(&data,1,0)); // set output bit on 1-wire 
  delay_us( 60 ); // wait until end of write slot. 
  output_float(ONE_WIRE_PIN); // set 1-wire high again, 
  delay_us( 2 ); // for more than 1us minimum. 
 } 
} 

/*********************** read1wire() *********************************/ 
/*This function reads the 8 -bit data via the 1-wire sensor. */ 
/* */ 
/*Parameters: */ 
/*Returns: 8-bit (1-byte) data from sensor */ 
/*********************************************************************/ 

int onewire_read() 
{ 
 int count, data; 

 for (count=0; count<8; ++count) 
 { 
  output_low(ONE_WIRE_PIN); 
  delay_us( 2 ); // pull 1-wire low to initiate read time-slot. 
  output_float(ONE_WIRE_PIN); // now let 1-wire float high, 
  delay_us( 8 ); // let device state stabilise, 
  shift_right(&data,1,input(ONE_WIRE_PIN)); // and load result. 
  delay_us( 120 ); // wait until end of read slot. 
 } 

 return( data ); 
} 
