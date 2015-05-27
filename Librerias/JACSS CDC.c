#include "usb/usb_cdc.h"

#define usbAvailable() usb_cdc_kbhit()
#define usbRead()      usb_cdc_getc()
#define usbWrite       usb_cdc_putc

void usbInit()
{
   usb_init();
   usb_wait_for_enumeration();
}

