///////////////////////////////////////////////////////////////////////////
////                             usb.c                                 ////
////                                                                   ////
//// Standard USB request and token handler code.                      ////
////                                                                   ////
//// This file is part of CCS's PIC USB driver code.  See USB.H        ////
//// for more documentation and a list of examples.                    ////
////                                                                   ////
//// The majority of this code is called and used by the interrupt     ////
//// generated by the hardware level, and therefore it is not meant    ////
//// to be called by the user.  The functions that are meant to be     ////
//// called by the user (init, put packet, get packet, etc) are        ////
//// documented in USB.H.                                              ////
////                                                                   ////
////   *************************  NOTE  **************************     ////
////    This code will not create a multiple configuration device.     ////
////    If you wish to create a multiple configuration device then you ////
////    will have to modify these drivers.                             ////
////                                                                   ////
////   *************************  NOTE  **************************     ////
////   This code does not support Get_Idle or Set_Idle HID-specific    ////
////   requests.  These requests are optional.  If you want to support ////
////   these requests you must provide the code yourself.  See         ////
////   usb_isr_tkn_setup_ClassInterface() if you wish to add this      ////
////   support.                                                        ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// If you wish to provide your own USB peripheral hardware layer, it ////
//// must must provide the API as described in usb_hw_layer.h.         ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Version History:                                                  ////
////                                                                   ////
////  April 2nd, 2012                                                  ////
////     Initial version of CDC+HID composite device support           ////
////                                                                   ////
//// June 8th, 2011:                                                   ////
////  Problem with usb_gets() and usb_puts() and specifying timeout=0  ////
////     (for no timeout) resolved.                                    ////
////                                                                   ////
//// September 2nd, 2010:                                              ////
////  Problem with descriptors larger than 127 bytes on 16-bit PICs    ////
////     (dsPIC, PIC24) resolved.                                      ////
////                                                                   ////
//// April 28th, 2010:                                                 ////
////  USB_CON_SENSE_PIN replaced with USB_CABLE_IS_ATTACHED()          ////
////     macro.  If USB_CON_SENSE_PIN is defined, it will create       ////
////     USB_CABLE_IS_ATTACHED() macro for you (for backwards          ////
////     compatibility).                                               ////
////                                                                   ////
//// November 13th, 2009:                                              ////
////  usb_endpoint_is_valid() has an extra check.                      ////
////                                                                   ////
//// June 9th, 2009:                                                   ////
////  Some problems with Vista and fast hubs/hosts fixed on PIC18.     ////
////                                                                   ////
//// March 5th, 2009:                                                  ////
////   Cleanup for Wizard.                                             ////
////   PIC24 Initial release.                                          ////
////                                                                   ////
//// Dec 14, 2007:                                                     ////
////  usb_kbhit() moved to device driver.                              ////
////                                                                   ////
//// July 13th, 2005:                                                  ////
////  usb_puts() packet_size and this_packet_len changed to 16bits.    ////
////  usb_gets() len, packet_size and this_packet_len to 16bits.       ////
////                                                                   ////
//// June 20th, 2005:                                                  ////
////  Initial 18fxx5x release.                                         ////
////  usb_kbhit() removed, usb_kbhit() now implemented in hardware     ////
////    layer.                                                         ////
////                                                                   ////
//// May 13th, 2005:                                                   ////
////  Beta release, only works with 18Fxx5x hardware layer.            ////
////  Now supports multiple interfaces (many defines in descriptors    ////
////   will have to be changed, see examples)                          ////
////  TODO: alot of indexing and length handling for descriptors is    ////
////   only 8bit, so make sure all descriptor tables are less than     ////
////   256 bytes long.                                                 ////
////                                                                   ////
//// Apr 21st, 2005:                                                   ////
////  Initial Alpha Release with PIC18Fxx5x support.  (ONLY TESTED     ////
////   WITH 18F4550)                                                   ////
////  usb_puts() doesn't need max packet size as a paremeter, uses     ////
////   usb_ep_tx_size[] defined in usb.h                               ////
////  usb_puts() timeout parameter now in ms, not seconds.             ////
////  USB Stack no longer buffers incoming data.  If there is data to  ////
////   get usb_kbhit(en) will return TRUE and the data will sit in     ////
////   the endpoint buffer until you usb_get_packet(), which will      ////
////   then free the endpoint buffer for more data.  This affects      ////
////   routines such as usb_gets() and usb_kbhit().                    ////
////  usb_gets() no longer reads buffered data (see above note),       ////
////   now it reads multiple packets in the same way usb_puts()        ////
////   writes multiple packets                                         ////
////  usb_kbhit() is hardware specific, so has been moved to hardware  ////
////   layer.                                                          ////
////                                                                   ////
//// Nov 11th, 2004:                                                   ////
////  No longer includes wrong descriptor header.                      ////
////                                                                   ////
//// June 24th, 2004:                                                  ////
////  Optimization and cleanup.                                        ////
////                The following definitions changed:                 ////
////  USB_EPx_TX_ENABLE and USB_EPx_RX_ENABLE have changed. See usb.h  ////
////  USB_CONFIG_DESCRIPTORS[] removed                                 ////
////  USB_CONFIG_DESC_LEN changed to USB_DESC_CONFIG_LEN               ////
////  USB_INTERFACE_DESC_LEN changed to USB_DESC_INTERFACE_LEN         ////
////  USB_CLASS_DESC_LEN changed to USB_DESC_CLASS_LEN                 ////
////  USB_ENDPOINT_DESC_LEN changed to USB_DESC_ENDPOINT_LEN           ////
////  USB_CONFIG_DESC_KEY changed to USB_DESC_CONFIG_TYPE              ////
////  USB_INTERFACE_DESC_KEY changed to USB_DESC_INTERFACE_TYPE        ////
////  USB_CLASS_DESC_KEY changed to USB_DESC_CLASS_TYPE                ////
////  USB_ENDPOINT_DESC_KEY changed to USB_DESC_ENDPOINT_TYPE          ////
////  USB_STRING_X[] arrays removed, see USB_STRING_DESC[] and         ////
////                                 USB_STRING_DESC_OFFSET[]          ////
////  dev_req, curr_config, status_device and getdesc_type global      ////
////        variables moved into struct USB_stack_status               ////
////                                                                   ////
//// December 5th, 2003: Fixed a potential bug where descriptors are   ////
////         evenly dividable by 8 (MAX_EP0_PACKET_SIZE)               ////
////                                                                   ////
//// October 15th, 2003: Support for boot protocol added.              ////
////         Set USB_HID_BOOT_PROTOCOL to TRUE to support this.        ////
////         The array hid_protocol[] saves which protocol mode each   ////
////         interface is in.  It is your applications job to send     ////
////         data that either fit the boot protocol or HID protocol.   ////
////                                                                   ////
//// May 6th, 2003: Fixed a potential stack overflow using PCM         ////
////                                                                   ////
//// October 28th, 2002: Problem with usb_puts and timeout fixed.      ////
////                                                                   ////
//// October 28th, 2002: Typo fixed in get_next_string_character(),    ////
////                     although it didn't cause any serious problems ////
////                                                                   ////
//// October 25th, 2002: Another change to usb_puts() was made to fix  ////
////                     problems with multiple packet messages        ////
//// October 29th, 2002: Fixed a problem with multiple packet string   ////
////                     descriptors that require a 0 len packet to    ////
////                     end message.                                  ////
////                                                                   ////
//// October 23rd, 2002: usb_puts() will bomb out of error quicker     ////
////                                                                   ////
//// August 2nd, 2002: Initial Public Release                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2005 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#IFNDEF __USB_DRIVER__
#DEFINE __USB_DRIVER__

#if defined(__DO_DEBUG_USB)
   #define debug_usb printf
   void debug_putc(char c) {uart_putc(c);}
   void debug_display_ram(unsigned int8 len, unsigned int8 *ptr) {
      unsigned int8 max=16;
      debug_usb(debug_putc,"[%U - ",len);
      //if (max>len) {max=len;}
      max = len;
      while(max--) {
         debug_usb(debug_putc,"%02X",*ptr);
         len--;
         ptr++;
      }
      if (len) {debug_usb(debug_putc,"...");}
      debug_usb(debug_putc,"] ");
   }
#else
   #define debug_usb(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
   #define debug_putc(c)
   #define debug_display_ram(x,y)
#endif

#include <usb.h>

#if defined(__PIC16_USB_H__)
 #include <pic_usb.c>
#endif

#if defined(__PIC18_USB_H__)
 #include <pic18_usb.c>
#endif

#if defined(__PIC24_USB_H__)
 #include <pic24_usb.c>
#endif

#if defined(__USBN960X_H__)
 #include <usbn960x.c>
#endif


#IFNDEF __USB_HARDWARE__
   #ERROR You must include USB hardware driver.
#ENDIF

#IFNDEF __USB_DESCRIPTORS__
   #ERROR You must include USB descriptors.
#ENDIF

TYPE_USB_STACK_STATUS USB_stack_status;

unsigned int8 USB_address_pending;                        //save previous state because packets can take several isrs
unsigned int16 usb_getdesc_ptr; unsigned int16 usb_getdesc_len;             //for reading string and config descriptors

#IF USB_HID_BOOT_PROTOCOL
unsigned int8 hid_protocol[USB_NUM_HID_INTERFACES];
#ENDIF

void usb_put_0len_0(void);
void usb_match_registers(int8 endpoint, int16 *status, int16 *buffer, int8 *size);

void usb_isr_tkn_setup_StandardEndpoint(void);
void usb_isr_tkn_setup_StandardDevice(void);
void usb_isr_tkn_setup_StandardInterface(void);
#IF USB_HID_DEVICE
   void usb_isr_tkn_setup_ClassInterface(void);
#ENDIF
void usb_Get_Descriptor(void);
void usb_copy_desc_seg_to_ep(void);
void usb_finish_set_address(void);

int8 USB_Interface[USB_MAX_NUM_INTERFACES];              //config state for all of our interfaces, NUM_INTERFACES defined with descriptors

/// BEGIN User Functions

// see usb.h for documentation
int1 usb_enumerated(void)
{
   return(USB_stack_status.curr_config);
}

// see usb.h for documentation
void usb_wait_for_enumeration(void) 
{
   while (USB_stack_status.curr_config == 0) {restart_wdt();}
}

// see USB.H for documentation
int1 usb_puts(unsigned int8 endpoint, int8 * ptr, unsigned int16 len, unsigned int8 timeout) {
   unsigned int16 i=0;
   int1 res;
   unsigned int16 this_packet_len;
   unsigned int16 packet_size;
   unsigned int32 timeout_1us;

   packet_size = usb_ep_tx_size[endpoint];
   
   //printf("\r\nUSB PUTS %U LEN=%LU MAX_PACK=%LU\r\n", endpoint, len, packet_size);

   //send data packets until timeout or no more packets to send
   while (i < len) 
   {
      timeout_1us = (int32)timeout*1000;
      if ((len - i) > packet_size) {this_packet_len = packet_size;}
      else {this_packet_len = len-i;}
      //putc('*');
      do 
      {
         res = usb_put_packet(endpoint, ptr + i, this_packet_len, USB_DTS_TOGGLE);   //send 64 byte packets
         //putc('.');
         if (!res)
         {
            delay_us(1);
            //delay_ms(500);
            timeout_1us--;
         }
      } while (!res && (!timeout || timeout_1us));
      i += packet_size;
   }


   //send 0len packet if needed
   if (i==len) {
      timeout_1us=(int32)timeout*1000;
      do {
         res = usb_put_packet(endpoint,0,0,USB_DTS_TOGGLE);   //indicate end of message
         if (!res) {
            delay_us(1);
            timeout_1us--;
         }
      } while (!res && (!timeout || timeout_1us));
   }

   return(res);
}

// see usb.h for documentation
unsigned int16 usb_gets(unsigned int8 endpoint, int8 * ptr, unsigned int16 max, unsigned int16 timeout) {
   unsigned int16 ret=0;
   unsigned int16 to;
   unsigned int16 len;
   unsigned int16 packet_size;
   unsigned int16 this_packet_max;

   packet_size=usb_ep_rx_size[endpoint];

   do {
      if (packet_size < max) {this_packet_max=packet_size;} else {this_packet_max=max;}
      to=0;
      do {
         if (usb_kbhit(endpoint)) {
            len=usb_get_packet(endpoint,ptr,this_packet_max);
            ptr+=len;
            max-=len;
            ret+=len;
            break;
         }
         else {
            to++;
            delay_ms(1);
         }
      } while (to!=timeout);
   } while ((len == packet_size) && (!timeout || (to!=timeout)) && max);

   return(ret);
}

/// END User Functions


/// BEGIN USB Token, standard and HID request handler (part of ISR)

// see usb.h for documentation
void usb_token_reset(void) 
{
   unsigned int i;

   usb_getdesc_len = 0;
   
   for (i=0;i<USB_MAX_NUM_INTERFACES;i++) 
      USB_Interface[i] = 0;   //reset each interface to default

  #IF USB_HID_BOOT_PROTOCOL
   for (i=0;i<USB_NUM_HID_INTERFACES; i++)
      hid_protocol[i] = 1;
  #endif

  #if USB_CDC_DEVICE
   usb_cdc_init();
  #endif

   USB_stack_status.curr_config = 0;      //unconfigured device

   USB_stack_status.status_device = 1;    //previous state.  init at none
   USB_stack_status.dev_req = NONE;       //previous token request state.  init at none
}

//send a 0len packet to endpoint 0 (optimization)
//notice that this doesnt return the status
#define usb_put_0len_0() usb_request_send_response(0)

/**************************************************************
/* usb_endpoint_is_valid(endpoint)
/*
/* Input: endpoint - endpoint to check.
/*                   bit 7 is direction (set is IN, clear is OUT)
/*
/* Output: TRUE if endpoint is valid, FALSE if not
/*
/* Summary: Checks the dynamic configuration to see if requested
/*          endpoint is a valid endpoint.
/***************************************************************/
int1 usb_endpoint_is_valid(unsigned int8 endpoint) 
{
   int1 direction;
   
   direction = bit_test(endpoint,7);
   
   endpoint &= 0x7F;
   
   if (endpoint > 16)
      return(FALSE);
   
   if (direction) { //IN
      return(usb_ep_tx_type[endpoint] != USB_ENABLE_DISABLED);
   }
   else {   //OUT
      return(usb_ep_rx_type[endpoint] != USB_ENABLE_DISABLED);
   }
}

// see usb.h for documentation
void usb_isr_tok_in_dne(unsigned int8 endpoint) {
   if (endpoint==0) {
      if (USB_stack_status.dev_req == GET_DESCRIPTOR) {usb_copy_desc_seg_to_ep();} //check this, we are missing report descriptor?
      else if (USB_stack_status.dev_req == SET_ADDRESS) {usb_finish_set_address();}
   }
  #if USB_CDC_DEVICE
  else if (endpoint==USB_CDC_DATA_IN_ENDPOINT) { //see ex_usb_serial.c example and usb_cdc.h driver
      usb_isr_tok_in_cdc_data_dne();
  }
  #endif
}

// see usb.h for documentation
void usb_isr_tok_out_dne(unsigned int8 endpoint)
{
   //TODO:
   if (endpoint==0) {
     debug_usb(debug_putc,"TOUT 0 ");
     #if USB_CDC_DEVICE
      usb_isr_tok_out_cdc_control_dne();
     //#else   //REMOVED JUN/9/2009
     //usb_init_ep0_setup();
     #endif
   }
  #if USB_CDC_DEVICE
   else if (endpoint==USB_CDC_DATA_OUT_ENDPOINT) { //see ex_usb_serial.c example and usb_cdc.h driver
      usb_isr_tok_out_cdc_data_dne();
   }
  #endif
   //else {
   //   bit_set(__usb_kbhit_status,endpoint);
   //}
}


//---- process setup message stage -----------//

// see usb.h for documentation
void usb_isr_tok_setup_dne(void) 
{
   USB_stack_status.dev_req=NONE; // clear the device request..

   switch(usb_ep0_rx_buffer[0] & 0x7F) {

      case 0x00:  //standard to device
         debug_usb(debug_putc," d");
         usb_isr_tkn_setup_StandardDevice();
         break;

      case 0x01:  //standard to interface
         debug_usb(debug_putc," i");
         usb_isr_tkn_setup_StandardInterface();
         break;

      case 0x02:  //standard to endpoint
         debug_usb(debug_putc," e");
         usb_isr_tkn_setup_StandardEndpoint();
         break;
         
#IF USB_HID_DEVICE || USB_CDC_DEVICE
      case 0x21:  //class specific request.  the only class this driver supports is HID
         debug_usb(debug_putc, " class");
        #if USB_HID_DEVICE && USB_CDC_DEVICE
         if (usb_ep0_rx_buffer[4] == USB_HID_INTERFACE)
         {
            usb_isr_tkn_setup_ClassInterface();
         }
         else
         {
            usb_isr_tkn_cdc();
         }
        #elif USB_HID_DEVICE
         usb_isr_tkn_setup_ClassInterface();
        #else
         usb_isr_tkn_cdc();
        #endif
         break;
#endif


      //TODO: IF YOU WANT VENDOR SPECIFC REQUEST SUPPORT YOU MUST ADD IT HERE

      default:
         usb_request_stall();
         break;
   }
}

/**************************************************************
/* usb_isr_tkn_setup_StandardDevice()
/*
/* Input: usb_ep0_rx_buffer[1] == bRequest
/*
/* Summary: bmRequestType told us it was a Standard Device request.
/*          bRequest says which request.  Only certain requests are valid,
/*          if a non-valid request was made then return with an Wrong-Statue (IDLE)
/*
/* Part of usb_isr_tok_setup_dne()
/***************************************************************/
void usb_isr_tkn_setup_StandardDevice(void) {
   switch(usb_ep0_rx_buffer[1]) {

      case USB_STANDARD_REQUEST_GET_STATUS:  //0
            debug_usb(debug_putc,"GS");
            usb_ep0_tx_buffer[0]=USB_stack_status.status_device;
            usb_ep0_tx_buffer[1]=0;
            usb_request_send_response(2);
            break;

      case USB_STANDARD_REQUEST_CLEAR_FEATURE:  //1
            if (usb_ep0_rx_buffer[2] == 1) {
               debug_usb(debug_putc,"CF");
               USB_stack_status.status_device &= 1;
               usb_put_0len_0();
            }
            else
               usb_request_stall();
            break;

      case USB_STANDARD_REQUEST_SET_FEATURE: //3
            if (usb_ep0_rx_buffer[2] == 1) {
               debug_usb(debug_putc,"SF");
               USB_stack_status.status_device |= 2;
               usb_put_0len_0();
            }
            else
               usb_request_stall();
            break;

      case USB_STANDARD_REQUEST_SET_ADDRESS: //5
            debug_usb(debug_putc,"SA");
            USB_stack_status.dev_req=SET_ADDRESS; //currently processing set_address request
            USB_address_pending=usb_ep0_rx_buffer[2];
            #ifdef __USBN__   //NATIONAL part handles this differently than pic16c7x5
            USB_stack_status.dev_req=NONE; //currently processing set_address request
            usb_set_address(USB_address_pending);
            USB_stack_status.curr_config=0;   // make sure current configuration is 0
            #endif
            usb_put_0len_0();
            break;

      case USB_STANDARD_REQUEST_GET_DESCRIPTOR: //6
            debug_usb(debug_putc,"GD");
            usb_Get_Descriptor();
            break;

      case USB_STANDARD_REQUEST_GET_CONFIGURATION: //8
            debug_usb(debug_putc,"GC");
            usb_ep0_tx_buffer[0]=USB_stack_status.curr_config;
            usb_request_send_response(1);
            break;

      case USB_STANDARD_REQUEST_SET_CONFIGURATION: //9
            if (usb_ep0_rx_buffer[2] <= USB_NUM_CONFIGURATIONS) {
               USB_stack_status.curr_config=usb_ep0_rx_buffer[2];
               usb_set_configured(usb_ep0_rx_buffer[2]);
               debug_usb(debug_putc,"SC%U", USB_stack_status.curr_config);               
               usb_put_0len_0();
            }
            break;

      default:
            usb_request_stall();
            break;
   }
}

/**************************************************************
/* usb_isr_tkn_setup_StandardInterface()
/*
/* Input: usb_ep0_rx_buffer[1] == bRequest
/*
/* Summary: bmRequestType told us it was a Standard Interface request.
/*          bRequest says which request.  Only certain requests are valid,
/*          if a non-valid request was made then return with an Wrong-Statue (IDLE)
/*
/* Part of usb_isr_tok_setup_dne()
/***************************************************************/
void usb_isr_tkn_setup_StandardInterface(void) {
   unsigned int8 curr_config;

   curr_config=USB_stack_status.curr_config;

   switch (usb_ep0_rx_buffer[1]) {
      case USB_STANDARD_REQUEST_GET_STATUS:
            debug_usb(debug_putc,"GS");
            usb_ep0_tx_buffer[0]=0;
            usb_ep0_tx_buffer[1]=0;
            usb_request_send_response(2);
            break;

      case USB_STANDARD_REQUEST_GET_INTERFACE:
            if ( curr_config && (usb_ep0_rx_buffer[4] < USB_NUM_INTERFACES[curr_config-1]) ) {   //book says only supports configed state
               debug_usb(debug_putc,"GI");
               usb_ep0_tx_buffer[0]=USB_Interface[usb_ep0_rx_buffer[4]];//our new outgoing byte
               usb_request_send_response(1);; //send byte back
            }
            else
               usb_request_stall();
            break;

      case USB_STANDARD_REQUEST_SET_INTERFACE:
            if (curr_config) { //if configured state
               debug_usb(debug_putc,"SI");
               USB_Interface[usb_ep0_rx_buffer[4]]=usb_ep0_rx_buffer[2];
               usb_put_0len_0();
            }
            else
               usb_request_stall();
            break;

#IF USB_HID_DEVICE
      case USB_STANDARD_REQUEST_GET_DESCRIPTOR:
            debug_usb(debug_putc,"GDh");
            usb_Get_Descriptor();
            break;
#endif

//      case USB_STANDARD_REQUEST_CLEAR_FEATURE:
//      case USB_STANDARD_REQUEST_SET_FEATURE:
//                let default take care of these, goto wrongstate
      default:
            usb_request_stall();
            break;
   }
}

/**************************************************************
/* usb_isr_tkn_setup_StandardEndpoint()
/*
/* Input: usb_ep0_rx_buffer[1] == bRequest
/*
/* Summary: bmRequestType told us it was a Standard Endpoint request.
/*          bRequest says which request.  Only certain requests are valid,
/*          if a non-valid request was made then return with an Wrong-Statue (IDLE)
/*
/* Part of usb_isr_tok_setup_dne()
/***************************************************************/
void usb_isr_tkn_setup_StandardEndpoint(void) {
   if (usb_endpoint_is_valid(usb_ep0_rx_buffer[4])) {
      switch(usb_ep0_rx_buffer[1]) {

         case USB_STANDARD_REQUEST_CLEAR_FEATURE:
               debug_usb(debug_putc,"CF");
               usb_unstall_ep(usb_ep0_rx_buffer[4]);
               usb_put_0len_0();
               break;

         case USB_STANDARD_REQUEST_SET_FEATURE:
                     debug_usb(debug_putc,"SF");
                     usb_stall_ep(usb_ep0_rx_buffer[4]);
                     usb_put_0len_0();
                     break;

         case USB_STANDARD_REQUEST_GET_STATUS:
               debug_usb(debug_putc,"GS");
               usb_ep0_tx_buffer[0]=0;
               usb_ep0_tx_buffer[1]=0;
               if (usb_endpoint_stalled(usb_ep0_rx_buffer[4])) {
                  usb_ep0_tx_buffer[0]=1;
               }
               usb_request_send_response(2);
               break;

         default:
            usb_request_stall();
            break;
      }
   }
}

/**************************************************************
/* usb_isr_tkn_setup_ClassInterface()
/*
/* Input: usb_ep0_rx_buffer[1] == bRequest
/*
/* Summary: bmRequestType told us it was a Class request.  The only Class this drivers supports is HID.
/*          bRequest says which request.  Only certain requests are valid,
/*          if a non-valid request was made then return with an Wrong-Statue (IDLE)
/*
/* Part of usb_isr_tok_setup_dne()
/* Only compiled if HID_DEVICE is TRUE
/***************************************************************/
#IF USB_HID_DEVICE
void usb_isr_tkn_setup_ClassInterface(void) {
   switch(usb_ep0_rx_buffer[1]) {

    #IF USB_HID_BOOT_PROTOCOL
      case USB_HID_REQUEST_GET_PROTOCOL:  //03
            debug_usb(debug_putc,"GP");
            usb_ep0_tx_buffer[0]=hid_protocol[usb_ep0_rx_buffer[4]];
            usb_request_send_response(1);
            break;
    #ENDIF

    #IF USB_HID_BOOT_PROTOCOL
      case USB_HID_REQUEST_SET_PROTOCOL:  //0b
            debug_usb(debug_putc,"SP");
            hid_protocol[usb_ep0_rx_buffer[4]]=usb_ep0_rx_buffer[2];
            usb_put_0len_0(); //send 0len packet69
            break;
    #ENDIF

   #IF USB_HID_IDLE
      case USB_HID_REQUEST_SET_IDLE:   //0a
         #error TODO: if you want to support SET_IDLE, add code here
   #ENDIF

   #IF USB_HID_IDLE
      case USB_HID_REQUEST_GET_IDLE:   //02
         #error TODO: if you want to support GET_IDLE, add code here
   #ENDIF

      default:
            usb_request_stall();
            break;
   }
}
#ENDIF

/**************************************************************
/* usb_Get_Descriptor()
/*
/* Input: usb_ep0_rx_buffer[3] == wValue, which descriptor we want
/*        usb_ep0_rx_buffer[6,7] == Max length the host will accept
/*
/* Summary: Checks to see if we want a standard descriptor (Interface, Endpoint, Config, Device, String, etc.),
/*          or a class specific (HID) descriptor.  Since some pics (especially the PIC167x5) doesn't have
/*          pointers to constants we must simulate or own by setting up global registers that say
/*          which constant array to deal with, which position to start in this array, and the length.
/*          Once these globals are setup the first packet is sent.  If a descriptor takes more than one packet
/*          the PC will send an IN request to endpoint 0, and this will be handled by usb_isr_tok_in_dne()
/*          which will send the rest of the data.
/*
/* Part of usb_isr_tok_setup_dne()
/***************************************************************/
void usb_Get_Descriptor() {
   usb_getdesc_ptr=0;
   USB_stack_status.getdesc_type=USB_GETDESC_CONFIG_TYPE;

   switch(usb_ep0_rx_buffer[3]) {
      case USB_DESC_DEVICE_TYPE:    //1
            usb_getdesc_len=USB_DESC_DEVICE_LEN;
            USB_stack_status.getdesc_type=USB_GETDESC_DEVICE_TYPE;
            break;

      //windows hosts will send a FF max len and expect you to send all configs without asking for them individually.
      case USB_DESC_CONFIG_TYPE:   //2
            usb_getdesc_len=USB_TOTAL_CONFIG_LEN;
            break;

      case USB_DESC_STRING_TYPE: //3
            USB_stack_status.getdesc_type=USB_GETDESC_STRING_TYPE;
            usb_getdesc_ptr=USB_STRING_DESC_OFFSET[usb_ep0_rx_buffer[2]];
            usb_getdesc_len=USB_STRING_DESC[usb_getdesc_ptr];
            break;

      //case USB_DESC_DEVICE_QUALIFIER_TYPE:   //0x06
      // this is needed so host can find out about differences about this
      // devices ability to handle full speed verses fast speed.
      // since we don't support fast speed then we ignore this.
      //      break;

#IF USB_HID_DEVICE || USB_CDC_DEVICE
      case USB_DESC_CLASS_TYPE:  //0x21
            //TODO does this work for multiple interfaces or multiple languages?
            //usb_getdesc_ptr=USB_CLASS_DESCRIPTORS[0][usb_ep0_rx_buffer[4]][usb_ep0_rx_buffer[2]];
            usb_getdesc_ptr=USB_CLASS_DESCRIPTORS[0][0][usb_ep0_rx_buffer[2]];
            if (usb_getdesc_ptr!=0xFF) {
               usb_getdesc_len=USB_CONFIG_DESC[usb_getdesc_ptr];
               break;
            }
            else {
               usb_request_stall();
               return;
            }
#endif

#IF USB_HID_DEVICE
      case USB_DESC_HIDREPORT_TYPE: //0x22
            usb_getdesc_ptr=USB_CLASS_SPECIFIC_DESC_LOOKUP[0][usb_ep0_rx_buffer[4]];
            if (usb_getdesc_ptr !=0xFF) {
               USB_stack_status.getdesc_type=USB_GETDESC_HIDREPORT_TYPE;
               usb_getdesc_len=USB_CLASS_SPECIFIC_DESC_LOOKUP_SIZE[0][usb_ep0_rx_buffer[4]];
               break;
            }
            else {
               usb_request_stall();
               return;
            }
#endif

      default:
            usb_request_stall();
            return;
   }
   if (usb_ep0_rx_buffer[7]==0) {
      if (usb_getdesc_len > usb_ep0_rx_buffer[6])
         usb_getdesc_len = usb_ep0_rx_buffer[6];
   }
   USB_stack_status.dev_req=GET_DESCRIPTOR;
   usb_copy_desc_seg_to_ep();
}

/**************************************************************
/* usb_finish_set_address()
/*
/* Input: USB_address_pending holds the address we were asked to set to.
/*
/* Summary: Sets the address.
/*
/* This code should only be run on the PIC USB peripheral, and not the
/* National peripheral.
/*
/* Part of usb_isr_tok_setup_dne()
/***************************************************************/
 void usb_finish_set_address() {
   debug_usb(debug_putc," FSA ");
   USB_stack_status.curr_config=0;   // make sure current configuration is 0

   #ifdef __PIC__
   USB_stack_status.dev_req=NONE;  // no request pending
   usb_set_address(USB_address_pending);
   #endif
}

////////////////////////////////////////////////////////////////////////////
///
/// The following function retrieve data from constant arrays.  This may
/// look un-optimized, but remember that you can't create a pointer to
/// a constant array.
///
///////////////////////////////////////////////////////////////////////////
void usb_copy_desc_seg_to_ep(void) {
   unsigned int i=0;
   char c;
   
   //debug_usb(debug_putc, "!%LX! ", &usb_ep0_tx_buffer[0]);

   while ((usb_getdesc_len)&&(i<USB_MAX_EP0_PACKET_LENGTH))
   {
      switch(USB_stack_status.getdesc_type) {
         case USB_GETDESC_CONFIG_TYPE:
            c=USB_CONFIG_DESC[usb_getdesc_ptr];
            break;

        #IF USB_HID_DEVICE
         case USB_GETDESC_HIDREPORT_TYPE:
            c=USB_CLASS_SPECIFIC_DESC[usb_getdesc_ptr];
            break;
        #endif

         case USB_GETDESC_STRING_TYPE:
            c=USB_STRING_DESC[usb_getdesc_ptr];
            break;

         case USB_GETDESC_DEVICE_TYPE:
            c=USB_DEVICE_DESC[usb_getdesc_ptr];
            //debug_usb(debug_putc, "-%X- ", c);
            break;
      }
      usb_getdesc_ptr++;
      usb_getdesc_len--;
      usb_ep0_tx_buffer[i++]=c;
   }

   if ((!usb_getdesc_len)&&(i!=USB_MAX_EP0_PACKET_LENGTH)) {
         USB_stack_status.dev_req = NONE;
   }

   usb_request_send_response(i);
}

#ENDIF
