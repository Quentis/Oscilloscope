#include "usbd_cdc_vcp.h"
#include "USB_RxTx.h"

/*USB Tx Functions*/

extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

void USB_RxTx_sendChar(uint8_t ch){
  APP_Rx_Buffer[APP_Rx_ptr_in] = ch;
  APP_Rx_ptr_in++;
  if(APP_Rx_ptr_in == APP_RX_DATA_SIZE) APP_Rx_ptr_in = 0;
}

void USB_RxTx_sendString(char str[]){
  uint32_t i;
  for(i = 0;str[i];i++){
    APP_Rx_Buffer[APP_Rx_ptr_in] = str[i];
    APP_Rx_ptr_in++;
    if(APP_Rx_ptr_in == APP_RX_DATA_SIZE) APP_Rx_ptr_in = 0;
  }
}

/*USB Rx Functions*/

uint8_t usb_rx_fifo[USB_RX_FIFO_SIZE];
uint32_t usb_rx_fifoLastIn = 0;
uint32_t usb_rx_fifoFirstOut = 0;
uint32_t USB_RxTx_rxFifoElementNum = 0;
uint8_t usb_rx_overrun = 0;

void USB_RxTx_pushChar(uint8_t ch){
  if(!usb_rx_overrun){
    usb_rx_fifoLastIn = (usb_rx_fifoLastIn + 1) % USB_RX_FIFO_SIZE;
    if(usb_rx_fifoLastIn != usb_rx_fifoFirstOut){
      usb_rx_fifo[usb_rx_fifoLastIn] = ch;
      USB_RxTx_rxFifoElementNum++;
    } else {
      usb_rx_overrun = 1;
    }
  }
}

uint8_t USB_RxTx_getChar(void){
  if(USB_RxTx_rxFifoElementNum){
    usb_rx_fifoFirstOut = (usb_rx_fifoFirstOut + 1) % USB_RX_FIFO_SIZE;
    USB_RxTx_rxFifoElementNum--;
  }
  return usb_rx_fifo[usb_rx_fifoFirstOut];
}
