#include "msp430.h"
#include <stdint.h>
#include <stdio.h>
#include "AUXCAM.h"
#include "COMMON.h"
#include "UART.h"

void delay()
{
  int x;
  for(x = 0;x<5000;x++)
  {}
}

int main(void) {
  
  static E_STATUS MAIN_status;
  
  DCOCTL = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;
  WDTCTL = WDTPW + WDTHOLD;
  
  MAIN_status = CAMERA_INIT(UART_A0,115200U);
  MAIN_status |= UART_INIT(UART_A1,BAUD_115200_16MHZ,0x00U,0x80U,UART_INTERRUPT_ENABLE_TX);
  
  while(!MAIN_status)
  {
      delay();
      MAIN_status |= CAMERA_GET_VERSION();
      delay();
      MAIN_status |= CAMERA_FBUF_CTRL(FBUF_STOP_CURRENT_FRAME);
      delay();
      MAIN_status |= CAMERA_GET_FBUF_LEN(FBUF_TYPE_CURRENT_FRAME); 
      delay();
      MAIN_status |= CAMERA_READ_FBUF(FBUF_TYPE_CURRENT_FRAME);
  }
    return 0;
}
