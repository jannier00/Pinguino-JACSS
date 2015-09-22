/////////////////////////////////////////////////////////////////////////
////                          pic24_usb.c                            ////
////                                                                 ////
//// Hardware layer for CCS's USB library.  See pic24_usb.h more     ////
//// documentation about the PIC24 hardware layer.                   ////
////                                                                 ////
//// This file is part of CCS's PIC USB driver code.  See USB.H      ////
//// for more documentation and a list of examples.                  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// Version History:                                                ////
////                                                                 ////
//// Nov 13th, 2009:                                                 ////
////  usb_disable_endpoint() won't touch BD status registers for     ////
////     endpoints that aren't allocated.                            ////
////                                                                 ////
//// March 5th, 2009:                                                ////
////   Cleanup for Wizard.                                           ////
////   PIC24 Initial release.                                        ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2009 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#IFNDEF __PIC24_USB_C__
#DEFINE __PIC24_USB_C__

#INCLUDE "usb\usb.h"

#if USB_EP15_TX_SIZE || USB_EP15_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  15
#elif USB_EP14_TX_SIZE || USB_EP14_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  14
#elif USB_EP13_TX_SIZE || USB_EP13_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  13
#elif USB_EP12_TX_SIZE || USB_EP12_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  12
#elif USB_EP11_TX_SIZE || USB_EP11_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  11
#elif USB_EP10_TX_SIZE || USB_EP10_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  10
#elif USB_EP9_TX_SIZE || USB_EP9_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  9
#elif USB_EP8_TX_SIZE || USB_EP8_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  8
#elif USB_EP7_TX_SIZE || USB_EP7_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  7
#elif USB_EP6_TX_SIZE || USB_EP6_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  6
#elif USB_EP5_TX_SIZE || USB_EP5_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  5
#elif USB_EP4_TX_SIZE || USB_EP4_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  4
#elif USB_EP3_TX_SIZE || USB_EP3_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  3
#elif USB_EP2_TX_SIZE || USB_EP2_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  2
#elif USB_EP1_TX_SIZE || USB_EP1_RX_SIZE
 #define USB_LAST_DEFINED_ENDPOINT  1
#else
 #define USB_LAST_DEFINED_ENDPOINT  0
#endif

#define USB_CONTROL_REGISTER_SIZE   ((USB_LAST_DEFINED_ENDPOINT+1)*8)

#define USB_DATA_BUFFER_NEEDED (USB_EP0_TX_SIZE+USB_EP0_RX_SIZE+USB_EP1_TX_SIZE+\
                           USB_EP1_RX_SIZE+USB_EP2_TX_SIZE+USB_EP2_RX_SIZE+\
                           USB_EP3_TX_SIZE+USB_EP3_RX_SIZE+USB_EP4_TX_SIZE+\
                           USB_EP4_RX_SIZE+USB_EP5_TX_SIZE+USB_EP5_RX_SIZE+\
                           USB_EP6_TX_SIZE+USB_EP6_RX_SIZE+USB_EP7_TX_SIZE+\
                           USB_EP7_RX_SIZE+USB_EP8_TX_SIZE+USB_EP8_RX_SIZE+\
                           USB_EP9_TX_SIZE+USB_EP9_RX_SIZE+USB_EP10_TX_SIZE+\
                           USB_EP10_RX_SIZE+USB_EP11_TX_SIZE+USB_EP11_RX_SIZE+\
                           USB_EP12_TX_SIZE+USB_EP12_RX_SIZE+USB_EP13_TX_SIZE+\
                           USB_EP13_RX_SIZE+USB_EP14_TX_SIZE+USB_EP14_RX_SIZE+\
                           USB_EP15_TX_SIZE+USB_EP15_RX_SIZE)

#if ((USB_DATA_BUFFER_NEEDED+USB_CONTROL_REGISTER_SIZE) > getenv("RAM"))
 #error You are trying to allocate more memory for endpoints than the PIC can handle
#endif

//reserve the control space needed
union
{
   struct
   {
      unsigned int16 BDnSTo;
      unsigned int16 BDnADo;
      unsigned int16 BDnSTi;
      unsigned int16 BDnADi;
   } ep[USB_LAST_DEFINED_ENDPOINT+1];
   char buffer[USB_CONTROL_REGISTER_SIZE];
} usb_endpoint_control_registers;
#locate usb_endpoint_control_registers=0xA00
#error/warning usb_endpoint_control_registers needs to be positioned at the start of a 512 byte boundry

union
{
   struct
   {
      unsigned int8 ep0_rx_buffer[USB_MAX_EP0_PACKET_LENGTH];
      unsigned int8 ep0_tx_buffer[USB_MAX_EP0_PACKET_LENGTH];
         
      //these buffer definitions needed for CDC library
     #if USB_EP1_RX_SIZE
      unsigned int8 ep1_rx_buffer[USB_EP1_RX_SIZE];
     #endif
     #if USB_EP1_TX_SIZE
      unsigned int8 ep1_tx_buffer[USB_EP1_TX_SIZE];
     #endif
     #if USB_EP2_RX_SIZE
      unsigned int8 ep2_rx_buffer[USB_EP2_RX_SIZE];
     #endif
     #if USB_EP2_TX_SIZE
      unsigned int8 ep2_tx_buffer[USB_EP2_TX_SIZE];
     #endif
   };
   unsigned int8 general[USB_DATA_BUFFER_NEEDED];
} g_USBDataBuffer;
#define USB_DATA_BUFFER_LOCATION &g_USBDataBuffer.general[0]

#define usb_ep0_rx_buffer  g_USBDataBuffer.ep0_rx_buffer
#define usb_ep0_tx_buffer  g_USBDataBuffer.ep0_tx_buffer

//these buffer definitions needed for CDC library
#define usb_ep1_rx_buffer g_USBDataBuffer.ep1_rx_buffer
#define usb_ep1_tx_buffer g_USBDataBuffer.ep1_tx_buffer
#define usb_ep2_rx_buffer g_USBDataBuffer.ep2_rx_buffer
#define usb_ep2_tx_buffer g_USBDataBuffer.ep2_tx_buffer

#define debug_usb(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
#define debug_putc(c)

#define debug_display_ram(x,y)

/*
void debug_putc(char c) {putc(c);}
#define debug_usb printf
*/

/*
void debug_display_ram(unsigned int8 len, int8 *ptr) {
   unsigned int8 max=16;
   debug_usb(debug_putc,"%U - ",len);
   if (max>len) {max=len;}
   while(max--) {
      debug_usb(debug_putc,"%X",*ptr);
      len--;
      ptr++;
   }
   if (len) {debug_usb(debug_putc,"...");}
}
*/

//#define debug_putc putc_tbe

//if you are worried that the PIC is not receiving packets because a bug in the
//DATA0/DATA1 synch code, you can set this to TRUE to ignore the DTS on
//receiving.
#ifndef USB_IGNORE_RX_DTS
 #define USB_IGNORE_RX_DTS FALSE
#endif

#ifndef USB_IGNORE_TX_DTS
 #define USB_IGNORE_TX_DTS FALSE
#endif

//if you enable this it will keep a counter of the 6 possible errors the
//pic can detect.  disabling this will save you ROM, RAM and execution time.
#if !defined(USB_USE_ERROR_COUNTER)
   #define USB_USE_ERROR_COUNTER FALSE
#endif

#define USB_PING_PONG_MODE_OFF   0  //no ping pong
#define USB_PING_PONG_MODE_E0    1  //ping pong endpoint 0 only
#define USB_PING_PONG_MODE_ON    2  //ping pong all endpoints

//NOTE - PING PONG MODE IS NOT SUPPORTED BY CCS!
#if !defined(USB_PING_PONG_MODE)
   #define USB_PING_PONG_MODE USB_PING_PONG_MODE_OFF
#endif

#if (USB_PING_PONG_MODE==USB_PING_PONG_MODE_OFF)
 #define EP_BDxST_O(x)    usb_endpoint_control_registers.ep[x].BDnSTo
 #define EP_BDxADR_O(x)   usb_endpoint_control_registers.ep[x].BDnADo
 #define EP_BDxST_I(x)    usb_endpoint_control_registers.ep[x].BDnSTi
 #define EP_BDxADR_I(x)   usb_endpoint_control_registers.ep[x].BDnADi
#else
#error Right now this driver only supports no ping pong
#endif

#define __BDST_UOWN     0x8000   //set = buffer control, clear = cpu control
#define __BDST_DTS      0x4000   //set = data1 packet, clear = data0 packet
#define __BDST_DTSEN    0x0800   //set = dts enabled, clear = disabled
#define __BDST_BSTALL   0x0400   //set = buffer stalled

#if USB_USE_ERROR_COUNTER
   int ERROR_COUNTER[6];
#endif

//---pic24 memory locations
#word UOTGIR  =  0x480
#word UOTGIE  =  0x482
#word UOTGCON =  0x486
#word UPWRC   =  0x488
#word UIR     =  0x48A
#word UIE     =  0x48C
#word UEIR    =  0x48E
#word UEIE    =  0x490
#word USTAT   =  0x492
#word UCON    =  0x494
#word UADDR   =  0x496
#word UBDTP1  =  0x498
#word UFRML   =  0x49A
#word UFRMH   =  0x49C
#word UCFG1   =  0x4A6
#word UCFG2   =  0x4A8
#define  UEP0_LOC 0x4AA

int16 UEP_SFR[16];
#locate UEP_SFR=UEP0_LOC

#define UEP(x) UEP_SFR[x]

#define UIE_BIT_URST    0
#define UIE_BIT_UERR    1
#define UIE_BIT_SOF     2
#define UIE_BIT_TRN     3
#define UIE_BIT_IDLE    4
#define UIE_BIT_STALL   7

#BIT UIE_SOF = UIE.UIE_BIT_SOF
#BIT UIE_STALL = UIE.UIE_BIT_STALL
#BIT UIE_IDLE = UIE.UIE_BIT_IDLE
#BIT UIE_TRN = UIE.UIE_BIT_TRN
#BIT UIE_UERR = UIE.UIE_BIT_UERR
#BIT UIE_URST = UIE.UIE_BIT_URST

#define UOTGIE_BIT_ACTV 4

#BIT UOTGIE_ACTV=UOTGIE.UOTGIE_BIT_ACTV

#BIT UIR_SOF = UIR.UIE_BIT_SOF
#BIT UIR_STALL = UIR.UIE_BIT_STALL
#BIT UIR_IDLE = UIR.UIE_BIT_IDLE
#BIT UIR_TRN = UIR.UIE_BIT_TRN
#BIT UIR_UERR = UIR.UIE_BIT_UERR
#BIT UIR_URST = UIR.UIE_BIT_URST

#BIT UOTGIR_ACTV=UOTGIR.UOTGIE_BIT_ACTV

#ifndef BIT
#define BIT(x) (1<<x)
#endif

#define __USB_UIF_RESET    BIT(UIE_BIT_URST)
#define __USB_UIF_ERROR    BIT(UIE_BIT_UERR)
#define __USB_UIF_TOKEN    BIT(UIE_BIT_TRN)
#define __USB_UIF_IDLE     BIT(UIE_BIT_IDLE)
#define __USB_UIF_STALL    BIT(UIE_BIT_STALL)
#define __USB_UIF_SOF      BIT(UIE_BIT_SOF)

#if USB_USE_ERROR_COUNTER
 #define STANDARD_INTS __USB_UIF_STALL|__USB_UIF_IDLE|__USB_UIF_TOKEN|__USB_UIF_ERROR|__USB_UIF_RESET
#else
 #define STANDARD_INTS __USB_UIF_STALL|__USB_UIF_IDLE|__USB_UIF_TOKEN|__USB_UIF_RESET
#endif

//#bit UOTGCON_DPPULUP = UOTGCON.7
//#bit UOTGCON_DPMULUP = UOTGCON.6

#bit UCON_SE0=UCON.6
#bit UCON_PKTDIS=UCON.5
#bit UCON_USBEN=UCON.0
#bit UCON_RESUME=UCON.2
#bit UCON_PPBRST=UCON.1

#bit UPWRC_USBPWR=UPWRC.0
#bit UPWRC_SUSPND=UPWRC.1
#define UCON_SUSPND UPWRC_SUSPND

//See UEPn (0xF70-0xF7F)
#define ENDPT_DISABLED   0x00   //endpoint not used
#define ENDPT_IN_ONLY   0x04    //endpoint supports IN transactions only
#define ENDPT_OUT_ONLY   0x08    //endpoint supports OUT transactions only
#define ENDPT_CONTROL   0x0C    //Supports IN, OUT and CONTROL transactions - Only use with EP0
#define ENDPT_NON_CONTROL 0x1C  //Supports both IN and OUT transactions

#define __UEP_EPHSK  0x01

//Define the states that the USB interface can be in
enum {USB_STATE_DETACHED=0, USB_STATE_ATTACHED=1, USB_STATE_POWERED=2, USB_STATE_DEFAULT=3,
    USB_STATE_ADDRESS=4, USB_STATE_CONFIGURED=5} usb_state=0;

#define USTAT_IN_E0        8
#define USTAT_OUT_SETUP_E0 0

#define __USB_UCFG1_UTEYE   0x80
#define __USB_UCFG1_UOEMON  0x40

#define __USB_UCFG2_PUVBUS  0x10 //microchip calls this USB_PULLUP_ENABLE
#define __USB_UCFG2_UTRDIS  0x01 //microchip calls this USB_EXTERNAL_TRANSCEIVER

//#define __UCFG1_VAL_ENABLED__ USB_PING_PONG_MODE
#define __UCFG1_VAL_ENABLED__ USB_PING_PONG_MODE
#define __UCFG2_VAL_ENABLED__ __USB_UCFG2_PUVBUS

#define __UCFG1_VAL_DISABLED__   0
#define __UCFG2_VAL_DISABLED__ 0

int8 __setup_0_tx_size;

//interrupt handler, specific to PIC24 peripheral only
void usb_handle_interrupt(void);
void usb_isr_rst(void);
void usb_isr_uerr(void);
void usb_isr_sof(void);
void usb_isr_activity(void);
void usb_isr_uidle(void);
void usb_isr_tok_dne(void);
void usb_isr_stall(void);
void usb_init_ep0_setup(void);

//following functions standard part of CCS PIC USB driver, and used by usb.c
unsigned int16 usb_get_packet_buffer(int8 endpoint, int8 *ptr, unsigned int16 max);

static void usb_clear_trn(void);

static void usb_clear_isr_flag(int16 *sfr, int8 bit);
static void usb_clear_isr_reg(int16 *sfr);

//// BEGIN User Functions:

// see usb_hw_layer.h for documentation
int1 usb_kbhit(int8 en)
{
   return((UEP(en) != ENDPT_DISABLED) && ((EP_BDxST_O(en) & __BDST_UOWN) == 0));
}

// see usb_hw_layer.h for documentation
int1 usb_tbe(int8 en)
{
   return((UEP(en)!=ENDPT_DISABLED) && ((EP_BDxST_I(en) & __BDST_UOWN) == 0));
}

// see usb_hw_layer.h for documentation
void usb_detach(void)
{
   UCON = 0;  //disable USB hardware
   UIE = 0;   //disable USB interrupts
   usb_clear_isr_reg(&UEIR);
   usb_clear_isr_reg(&UIR);   
   UOTGIE = 0;
   UPWRC_USBPWR = 1;
   UCFG1 = __UCFG1_VAL_DISABLED__;
   UCFG2 = __UCFG2_VAL_DISABLED__;
   UCON_PPBRST = 1;
   UCON_PPBRST = 0;
   //UOTGCON_DPPULUP = 0;
   //set_tris_g(get_tris_g() | 0x0C); //set d+/d- pins to input //pin_g2 and pin_g3
   usb_state=USB_STATE_DETACHED;
   UADDR = 0;
   
   memset(&UEP_SFR[0], 0x00, sizeof(UEP_SFR));
   
   memset(&usb_endpoint_control_registers, 0x00, sizeof(usb_endpoint_control_registers));
   
   UBDTP1 = &usb_endpoint_control_registers.buffer[0] >> 8;
   usb_token_reset();              //clear the chapter9 stack
   UCON_PKTDIS=0;
   //__usb_kbhit_status=0;

   /*
   UCON = 0;  //disable USB hardware
   UIE = 0;   //disable USB interrupts
   usb_state = USB_STATE_DETACHED;
   */
}

// see usb_hw_layer.h for documentation
void usb_init_cs(void) 
{
   /* microchip
      UCON = 0;
      UIE = 0;
      UEIE = 0;
   usb_clear_isr_reg(&UEIR);
   usb_clear_isr_reg(&UIR);
   
   UPWRC_USBPWR = 1;
   
   UBDTP1 = &usb_endpoint_control_registers >> 8;

   UCON_PPBRST = 1;
   UCON_PPBRST = 0;

   UADDR = 0;
   
   memset(&UEP_SFR[0], 0x00, sizeof(UEP_SFR));
   
   memset(&usb_endpoint_control_registers, 0x00, sizeof(usb_endpoint_control_registers));

   UEP(0) = ENDPT_CONTROL | __UEP_EPHSK;   

   while (UIR_TRN) 
   {
      usb_clear_trn();
   }

   UCON_PKTDIS = 0;  //enable
   
   usb_state = USB_STATE_ATTACHED;      // Defined in usbmmap.c & .h
   */

   usb_detach();
   
   /* not sure where this came from
   usb_token_reset();
    UCON = 0;
    UIE = 0;                                // Mask all USB interrupts
    UPWRC_USBPWR = 0;
    UCFG1 = __UCFG1_VAL_ENABLED__;
    UCFG2 = __UCFG2_VAL_ENABLED__;
    //UOTGCON_DPPULUP = 1;
    while (!UCON_USBEN) UCON_USBEN = 1;                     // Enable module & attach to bus
    UPWRC_USBPWR = 1;
    usb_state = USB_STATE_ATTACHED;      // Defined in usbmmap.c & .h
    */
}

// see usb_hw_layer.h for documentation
static void usb_attach(void) {
   usb_token_reset();
    UCON = 0;
    UIE = 0;                                // Mask all USB interrupts
    UCFG1 = __UCFG1_VAL_ENABLED__;
    UCFG2 = __UCFG2_VAL_ENABLED__;
    while (!UCON_USBEN) {UCON_USBEN = 1;}  // Enable module & attach to bus
    usb_state = USB_STATE_ATTACHED;      // Defined in usbmmap.c & .h
}

// see usb_hw_layer.h for documentation
void usb_task(void) {
   if (usb_attached()) {
      if (UCON_USBEN==0) {
         debug_usb(debug_putc, "\r\n\nUSB TASK: ATTACH");
         usb_attach();
         //delay_ms(50);
      }
   }
   else {
      if (UCON_USBEN==1)  {
         debug_usb(debug_putc, "\r\n\nUSB TASK: DE-ATTACH");
         usb_detach();
      }
   }

   if ((usb_state == USB_STATE_ATTACHED)&&(!UCON_SE0)) 
   {
      usb_clear_isr_reg(&UIR);
      UIE=0;
      enable_interrupts(INT_USB);
      enable_interrupts(INTR_GLOBAL);
      UIE=__USB_UIF_IDLE | __USB_UIF_RESET;  //enable IDLE and RESET USB interrupt
      usb_state=USB_STATE_POWERED;
      debug_usb(debug_putc, "\r\n\nUSB TASK: POWERED");
   }
}

// see usb_hw_layer.h for documentation
void usb_init(void) 
{
   usb_init_cs();

   do 
   {
      usb_task();
   } while (usb_state != USB_STATE_POWERED);
}

// see pic24_usb.h for documentation
int1 usb_flush_in(int8 endpoint, unsigned int16 len, USB_DTS_BIT tgl) 
{
   int16 oldBD, newBD = 0;
   
   debug_usb(debug_putc,"\r\nPUT %X %U %LU %LX", endpoint, tgl, len, EP_BDxST_I(endpoint));

   if (usb_tbe(endpoint)) 
   {
      newBD = len;

      debug_display_ram(len, EP_BDxADR_I(endpoint));

     #if USB_IGNORE_TX_DTS
      newBD |= __BDST_UOWN;
     #else
      if (tgl == USB_DTS_TOGGLE) 
      {
         oldBD = EP_BDxST_I(endpoint);
         if ((oldBD & __BDST_DTS) == __BDST_DTS)   //(bit_test(i,14))
            tgl=USB_DTS_DATA0;  //was DATA1, goto DATA0
         else
            tgl=USB_DTS_DATA1;  //was DATA0, goto DATA1
      }
      else if (tgl == USB_DTS_USERX) 
      {
         oldBD = EP_BDxST_O(endpoint);
         if ((oldBD & __BDST_DTS) == __BDST_DTS)   //(bit_test(i,14))
            tgl = USB_DTS_DATA1;
         else
            tgl = USB_DTS_DATA0;
      }
      if (tgl == USB_DTS_DATA1)
         newBD |= __BDST_UOWN | __BDST_DTS | __BDST_DTSEN;  //DATA1, UOWN
      else //if (tgl == USB_DTS_DATA0) 
         newBD |= __BDST_UOWN | __BDST_DTSEN; //DATA0, UOWN
     #endif

      debug_usb(debug_putc," %X", newBD);

      EP_BDxST_I(endpoint) = newBD;//save changes
      
      return(1);
   }
    else {
         debug_usb(debug_putc,"\r\nPUT ERR");
    }
   return(0);
}

// see usb_hw_layer.h for documentation.
int1 usb_put_packet(int8 endpoint, int8 * ptr, unsigned int16 len, USB_DTS_BIT tgl) 
{
   int8 * buff_add;    

   if (usb_tbe(endpoint)) 
   {
      buff_add=EP_BDxADR_I(endpoint);
      memcpy(buff_add, ptr, len);     
      
      return(usb_flush_in(endpoint, len, tgl));
   }
   else 
   {
      debug_usb(debug_putc,"\r\nPUT ERR");
   }

   return(0);
}

/// END User Functions


/// BEGIN Hardware layer functions required by USB.C

// see pic24_usb.h for documentation
void usb_flush_out(int8 endpoint, USB_DTS_BIT tgl) 
{
   int16 oldBD, newBD;
   int16 len;

     #if USB_IGNORE_RX_DTS
      if (tgl == USB_DTS_STALL) 
      {
         debug_usb(debug_putc, '*');
         EP_BDxST_I(endpoint) = __BDST_UOWN | __BDST_BSTALL;
         EP_BDxST_O(endpoint) = __BDST_UOWN | __BDST_BSTALL;
         return;
      }
      else
         newBD = __BDST_UOWN;
     #else
      oldBD = EP_BDxST_O(endpoint);
      if (tgl == USB_DTS_TOGGLE) 
      {
         if ((oldBD & __BDST_DTS) == __BDST_DTS)   //(bit_test(i,14))
            tgl = USB_DTS_DATA0;  //was DATA1, goto DATA0
         else
            tgl  =USB_DTS_DATA1;  //was DATA0, goto DATA1
      }
      if (tgl == USB_DTS_STALL) 
      {
         newBD = __BDST_UOWN | __BDST_BSTALL;
         EP_BDxST_I(endpoint) = __BDST_UOWN | __BDST_BSTALL; //stall both in and out endpoints
      }
      else if (tgl == USB_DTS_DATA1) 
      {
         newBD = __BDST_UOWN | __BDST_DTS | __BDST_DTSEN;  //DATA1, UOWN
      }
      else //if (tgl == USB_DTS_DATA0) 
      {
         newBD = __BDST_UOWN | __BDST_DTSEN; //DATA0, UOWN
      }
     #endif

   //bit_clear(__usb_kbhit_status,endpoint);

   len=usb_ep_rx_size[endpoint];

   newBD |= len;

   EP_BDxST_O(endpoint) = newBD;
}

// see pic24_usb.h for documentation
unsigned int16 usb_rx_packet_size(int8 endpoint) 
{
   return(EP_BDxST_O(endpoint) & 0x03FF);
}

/*******************************************************************************
/* usb_get_packet_buffer(endpoint, *ptr, max)
/*
/* Input: endpoint - endpoint to get data from
/*        ptr - where to save data to local PIC RAM
/*        max - max amount of data to receive from buffer
/*
/* Output: the amount of data taken from the buffer.
/*
/* Summary: Gets a packet of data from the USB buffer and puts into local PIC RAM.
/*          Does not mark the endpoint as ready for more data.  Once you are
/*          done with data, call usb_flush_out() to mark the endpoint ready
/*          to receive more data.
/*
/********************************************************************************/
unsigned int16 usb_get_packet_buffer(int8 endpoint, int8 *ptr, unsigned int16 max) 
{
   int8 *al;
   unsigned int16 i;

   al = EP_BDxADR_O(endpoint);
   i = usb_rx_packet_size(endpoint);

   //printf("\r\nUSB GET PACKET ST:%LX %U I=%LX MAX=%LX %X%X ", EP_BDxST_O(endpoint), endpoint, i, max, al[0], al[1]);

   if (i<max) {max=i;}
   
   if (max > 1)   
      memcpy(ptr,al,max);
   else if (max == 1)
      *ptr = *al;

   //printf("%X%X\r\n", ptr[0], ptr[1]);

   return(max);
}

// see usb_hw_layer.h
unsigned int16 usb_get_packet(int8 endpoint, int8 * ptr, unsigned int16 max) 
{
   max=usb_get_packet_buffer(endpoint,ptr,max);
   usb_flush_out(endpoint, USB_DTS_TOGGLE);

   return(max);
}

// see usb_hw_layer.h
void usb_stall_ep(int8 endpoint) 
{
   int1 direction;
   
   direction=bit_test(endpoint,7);
   endpoint&=0x7F;
   
   if (direction) 
   {
      EP_BDxST_I(endpoint) = __BDST_UOWN | __BDST_BSTALL;
   }
   else 
   {
      EP_BDxST_O(endpoint) = __BDST_UOWN | __BDST_BSTALL;
   }
}

// see usb_hw_layer.h for documentation
void usb_unstall_ep(int8 endpoint) 
{
   int1 direction;

   direction=bit_test(endpoint,7);
   endpoint&=0x7F;
   
   if (direction) {
      #if USB_IGNORE_RX_DTS
      EP_BDxST_I(endpoint) = __BDST_UOWN;
      #else
      EP_BDxST_I(endpoint) = __BDST_UOWN | __BDST_DTSEN;
      #endif
   }
   else {
      EP_BDxST_O(endpoint) = 0x00;
   }
}

// see usb_hw_layer.h for documentation
int1 usb_endpoint_stalled(int8 endpoint) 
{
   int1 direction;
   int16 st;
   
   direction = bit_test(endpoint,7);
   endpoint &= 0x7F;
   
   if (direction) 
   {
      st = EP_BDxST_I(endpoint);
   }
   else 
   {
      st = EP_BDxST_O(endpoint);
   }
   
   return(
            ((st & __BDST_UOWN) == __BDST_UOWN) && 
            ((st & __BDST_UOWN) == __BDST_BSTALL)
         );
}

// see usb_hw_layer.h for documentation
void usb_set_address(int8 address) 
{
   UADDR = address;
   
   if (address)
      usb_state = USB_STATE_ADDRESS;
   else
      usb_state = USB_STATE_POWERED;
}

// see usb_hw_layer.h for documentation
void usb_set_configured(int8 config) 
{
   unsigned int8 en;
   unsigned int16 addy;
   int8 new_uep;
   unsigned int16 len;
   int16 newBD;
   
      if (config == 0) 
      {  //if config=0 then set addressed state
         usb_state = USB_STATE_ADDRESS;
         usb_disable_endpoints();
      }
      else 
      {
         usb_state = USB_STATE_CONFIGURED; //else set configed state
         addy = USB_DATA_BUFFER_LOCATION + (2*USB_MAX_EP0_PACKET_LENGTH); //skip ep0
         for (en=1; en<16; en++) 
         {
            usb_disable_endpoint(en);         
            new_uep = 0;
            if (usb_ep_rx_type[en] != USB_ENABLE_DISABLED) 
            {
               new_uep = ENDPT_OUT_ONLY;
               
               len = usb_ep_rx_size[en];
               
               EP_BDxADR_O(en) = addy;
               addy += usb_ep_rx_size[en];
               
               #if USB_IGNORE_RX_DTS
                  newBD = __BDST_UOWN;
               #else
                  newBD = __BDST_UOWN | __BDST_DTSEN;
               #endif
               
               newBD |= len;
               EP_BDxST_O(en) = newBD;
            }
            if (usb_ep_tx_type[en] != USB_ENABLE_DISABLED) 
            {
               new_uep |= ENDPT_IN_ONLY;
               
               EP_BDxADR_I(en) = addy;
               addy += usb_ep_tx_size[en];
               
               EP_BDxST_I(en) = __BDST_DTS;
            }
            
            if (new_uep == (ENDPT_IN_ONLY | ENDPT_OUT_ONLY))
               new_uep = ENDPT_NON_CONTROL;
               
            if (usb_ep_tx_type[en] != USB_ENABLE_ISOCHRONOUS)
               new_uep |= __UEP_EPHSK;
               
            UEP(en)=new_uep;
         }
      }
}

/// END Hardware layer functions required by USB.C


/// BEGIN USB Interrupt Service Routine

static void usb_clear_trn(void)
{
   usb_clear_isr_flag(&UIR, UIE_BIT_TRN);
   delay_cycles(6);
}

/*******************************************************************************
/* usb_handle_interrupt()
/*
/* Summary: Checks the interrupt, and acts upon event.  Processing finished
/*          tokens is the majority of this code, and is handled by usb.c
/*
/* NOTE: If you wish to change to a polling method (and not an interrupt method),
/*       then you must call this function rapidly.  If there is more than 10ms
/*       latency the PC may think the USB device is stalled and disable it.
/*       To switch to a polling method, remove the #int_usb line above this fuction.
/*       Also, goto usb_init() and remove the code that enables the USB interrupt.
/********************************************************************************/
#int_usb
void usb_isr(void) 
{
   int8 TRNAttempts = 0;
   
   debug_putc('.');
   
   if (usb_state==USB_STATE_DETACHED) return;   //should never happen, though
   if (UIR || UOTGIR) 
   {
      //debug_usb(debug_putc,"\r\n\n[%X%X%U] ",(int8)UIR, (int8)UOTGIR, UCON_SUSPND);
      if (UOTGIR_ACTV && UOTGIE_ACTV) {usb_isr_activity();}  //activity detected.  (only enable after sleep)

      if (UCON_SUSPND) return;

      if (UIR_STALL && UIE_STALL) {usb_isr_stall();}        //a stall handshake was sent

      if (UIR_UERR && UIE_UERR) {usb_isr_uerr();}          //error has been detected

      if (UIR_URST && UIE_URST) {usb_isr_rst();}        //usb reset has been detected

      if (UIR_IDLE && UIE_IDLE) {usb_isr_uidle();}        //idle time, we can go to sleep
      if (UIR_SOF && UIE_SOF) {usb_isr_sof();}
      //if (UIR_STALL && UIE_STALL) {usb_isr_stall();}        //a stall handshake was sent

      do
      {
         if (UIR_TRN && UIE_TRN) {
            usb_isr_tok_dne();
            //UIR_TRN=0;    // clear the token done interrupt., 0x190.3
         }    //a token has been detected (majority of isrs)
         else
            break;
      }
      while (TRNAttempts++ < 4);
   }
}

//SOF interrupt not handled.  user must add this depending on application
void usb_isr_sof(void) 
{
   debug_usb(debug_putc,"\r\nSOF");
   usb_clear_isr_flag(&UIR, UIE_BIT_SOF);
}

// see usb_hw_layer.h for documentation
void usb_disable_endpoint(int8 en) 
{
   UEP(en)=ENDPT_DISABLED;
   
   if (usb_endpoint_is_valid(en))
   {
      EP_BDxST_O(en) = 0;   //clear state, deque if necessary      
      EP_BDxST_I(en) = 0;   //clear state, deque if necessary
   }
}

// see usb_hw_layer.h for documentation
void usb_disable_endpoints(void) 
{
   unsigned int8 i;
   
   for (i=1;i<16;i++)
      usb_disable_endpoint(i);
      
   //__usb_kbhit_status=0;
}

/*******************************************************************************
/* usb_isr_rst()
/*
/* Summary: The host (computer) sent us a RESET command.  Reset USB device
/*          and token handler code to initial state.
/*
/********************************************************************************/
void usb_isr_rst(void) {
   debug_usb(debug_putc,"R");

   usb_clear_isr_reg(&UEIR);
   usb_clear_isr_reg(&UIR);
   UEIE = 0x9F;
   UIE = STANDARD_INTS;

   UADDR=0;

   usb_token_reset();

   usb_disable_endpoints();
  
   UEP(0) = ENDPT_CONTROL | __UEP_EPHSK;

   while (UIR_TRN) 
   {
      usb_clear_trn();
   }

   UCON_PKTDIS=0; //SIE token and packet processing enabled

   usb_init_ep0_setup();

   usb_state=USB_STATE_DEFAULT; //put usb mcu into default state
}

/*****************************************************************************
/* usb_init_ep0_setup()
/*
/* Summary: Configure EP0 to receive setup packets
/*
/*****************************************************************************/
void usb_init_ep0_setup(void) 
{
   int16 newBD;
   
   newBD = USB_MAX_EP0_PACKET_LENGTH;
   EP_BDxADR_O(0) = USB_DATA_BUFFER_LOCATION;
  #if USB_IGNORE_RX_DTS
   newBD |= __BDST_UOWN;
  #else
   newBD |= __BDST_UOWN | __BDST_DTSEN;   //give control to SIE, DATA0, data toggle synch on
  #endif
   EP_BDxST_O(0) = newBD;

   EP_BDxST_I(0) = 0;
   EP_BDxADR_I(0) = USB_DATA_BUFFER_LOCATION + (int16)USB_MAX_EP0_PACKET_LENGTH;
}

/*******************************************************************************
/* usb_isr_uerr()
/*
/* Summary: The USB peripheral had an error.  If user specified, error counter
/*          will incerement.  If having problems check the status of these 8 bytes.
/*
/* NOTE: This code is not enabled by default.
/********************************************************************************/
void usb_isr_uerr(void) {
#if USB_USE_ERROR_COUNTER
   int ints;
#endif

   debug_usb(debug_putc,"E %X ",UEIR);

#if USB_USE_ERROR_COUNTER

   ints=UEIR & UEIE; //mask off the flags with the ones that are enabled

   if ( bit_test(ints,0) ) { //increment pid_error counter
      debug_usb(debug_putc,"PID ");
      ERROR_COUNTER[0]++;
   }

   if ( bit_test(ints,1) ) {  //increment crc5 error counter
      debug_usb(debug_putc,"CRC5 ");
      ERROR_COUNTER[1]++;
   }

   if ( bit_test(ints,2) ) {  //increment crc16 error counter
      debug_usb(debug_putc,"CRC16 ");
      ERROR_COUNTER[2]++;
   }

   if ( bit_test(ints,3) ) {  //increment dfn8 error counter
      debug_usb(debug_putc,"DFN8 ");
      ERROR_COUNTER[3]++;
   }

   if ( bit_test(ints,4) ) {  //increment bto error counter
      debug_usb(debug_putc,"BTO ");
      ERROR_COUNTER[4]++;
   }

   if ( bit_test(ints,7) ) { //increment bts error counter
      debug_usb(debug_putc,"BTS ");
      ERROR_COUNTER[5]++;
   }
#endif

   usb_clear_isr_reg(&UEIR);
   usb_clear_isr_flag(&UIR, UIE_BIT_UERR);
}

/*******************************************************************************
/* usb_isr_uidle()
/*
/* Summary: USB peripheral detected IDLE.  Put the USB peripheral to sleep.
/*
/********************************************************************************/
void usb_isr_uidle(void) 
{
   debug_usb(debug_putc,"I");

   UOTGIE_ACTV=1;   //enable activity interrupt flag.
   usb_clear_isr_flag(&UIR, UIE_BIT_IDLE);
   //UCON_SUSPND=1; //set suspend. we are now suspended
}


/*******************************************************************************
/* usb_isr_activity()
/*
/* Summary: USB peripheral detected activity on the USB device.  Wake-up the USB
/*          peripheral.
/*
/********************************************************************************/
void usb_isr_activity(void) 
{
   debug_usb(debug_putc,"A");

   UCON_SUSPND=0; //turn off low power suspending
   UOTGIE_ACTV=0; //clear activity interupt enabling
   usb_clear_isr_flag(&UOTGIR, UOTGIE_BIT_ACTV);
}

/*******************************************************************************
/* usb_isr_stall()
/*
/* Summary: Stall handshake detected.
/*
/********************************************************************************/
void usb_isr_stall(void) 
{
   debug_usb(debug_putc,"S");
   
   
   if (bit_test(UEP(0),1)) {
      usb_init_ep0_setup();
      bit_clear(UEP(0),1);
   }
   usb_clear_isr_flag(&UIR, UIE_BIT_STALL);
}

/*******************************************************************************
/* usb_isr_tok_dne()
/*
/* Summary: A Token (IN/OUT/SETUP) has been received by the USB peripheral.
/*          If a setup token on EP0 was received, run the chapter 9 code and
/*          handle the request.
/*          If an IN token on EP0 was received, continue transmitting any
/*          unfinished requests that may take more than one packet to transmit
/*          (if necessary).
/*          If an OUT token on any other EP was received, mark that EP as ready
/*          for a usb_get_packet().
/*          Does not handle any IN or OUT tokens on EP0.
/*
/********************************************************************************/
void usb_isr_tok_dne(void) 
{
   int8 en;

   en=USTAT>>4;

   debug_usb(debug_putc,"T ");
   debug_usb(debug_putc,"%X ", USTAT);

   if ((USTAT & 0x00F8) == USTAT_OUT_SETUP_E0) 
   {
      //new out or setup token in the buffer
      int16 pidKey;
      
      debug_usb(debug_putc,"%LX ", EP_BDxST_O(0));
      
      pidKey = EP_BDxST_O(0) & (int16)0x3C00;  //save PID
      pidKey >>= 10;
      
      EP_BDxST_O(0) &= (int16)0x43FF;  //clear pid, prevent bdstal/pid confusion
      usb_clear_trn();
      
      if (pidKey == PID_SETUP) 
      {
         EP_BDxST_I(0)=0;   // return the in buffer to us (dequeue any pending requests)

         debug_usb(debug_putc,"%LU ", EP_BDxST_O(0) & 0x3FF);
         debug_display_ram(EP_BDxST_O(0) & 0x3FF, usb_ep0_rx_buffer);

         usb_isr_tok_setup_dne();

         UCON_PKTDIS=0;       // UCON,PKT_DIS ; Assuming there is nothing to dequeue, clear the packet disable bit

         //if setup_0_tx_size==0xFF - stall ep0 (unhandled request)
         //if setup_0_tx_size==0xFE - get EP0OUT ready for a data packet, leave EP0IN alone
         //else setup_0_tx_size=size of response, get EP0OUT ready for a setup packet, mark EPOIN ready for transmit
         if (__setup_0_tx_size==0xFF)
            usb_flush_out(0,USB_DTS_STALL);
         else 
         {
            usb_flush_out(0,USB_DTS_TOGGLE);
            if (__setup_0_tx_size!=0xFE)
               usb_flush_in(0,__setup_0_tx_size,USB_DTS_USERX);

         }
         //why was this here?
         //UCON_PKTDIS=0;       // UCON,PKT_DIS ; Assuming there is nothing to dequeue, clear the packet disable bit
      }
      else if (pidKey == PID_OUT) 
      {
         usb_isr_tok_out_dne(0);
         usb_flush_out(0,USB_DTS_TOGGLE);
         if ((__setup_0_tx_size!=0xFE)&&(__setup_0_tx_size!=0xFF))
               usb_flush_in(0,__setup_0_tx_size,USB_DTS_DATA1);   //send response (usually a 0len)
      }
   }
   else if ((USTAT & 0x00F8) == USTAT_IN_E0) 
   {
      //pic -> host transfer completed
      EP_BDxST_I(0) = EP_BDxST_I(0) & 0x4300;   //clear up any BDSTAL confusion
      usb_clear_trn();
      __setup_0_tx_size=0xFF;
      usb_isr_tok_in_dne(0);
      if (__setup_0_tx_size!=0xFF)
         usb_flush_in(0,__setup_0_tx_size,USB_DTS_TOGGLE);
      //else
      //   usb_init_ep0_setup();
   }
   else 
   {
      if (!bit_test(USTAT, 3)) 
      {
         EP_BDxST_O(en) = EP_BDxST_O(en) & 0x43FF;   //clear up any BDSTAL confusion
         usb_clear_trn();
         usb_isr_tok_out_dne(en);
      }
      else 
      {
         EP_BDxST_I(en) = EP_BDxST_I(en) & 0x4300;   //clear up any BDSTAL confusion
         usb_clear_trn();
         usb_isr_tok_in_dne(en);
      }
   }
}

// see usb_hw_layer.h for documentation
void usb_request_send_response(unsigned int8 len) {__setup_0_tx_size=len;}
void usb_request_get_data(void) {__setup_0_tx_size=0xFE;}
void usb_request_stall(void) {__setup_0_tx_size=0xFF;}


/// END USB Interrupt Service Routine

static void usb_clear_isr_flag(int16 *sfr, int8 bit)
{
   *sfr = (int16)1 << bit;
}

static void usb_clear_isr_reg(int16 *sfr)
{
   *sfr = 0xFFFF;
}

#ENDIF

