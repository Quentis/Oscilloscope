#ifndef USB_RXTX_H_
#define USB_RXTX_H_

extern uint32_t USB_RxTx_rxFifoElementNum;

#define   USB_RX_FIFO_SIZE          1024
#define   USB_RX_OVERRUN_ACK        do{usb_rx_overrun = 0;}while(0)

void USB_RxTx_sendChar(uint8_t ch);
void USB_RxTx_sendString(char str[]);
void USB_RxTx_pushChar(uint8_t ch);
uint8_t USB_RxTx_getChar(void);

#endif /* USB_RXTX_H_ */
