#include "msp430.h"
#include <stdint.h>
#include <stdio.h>
#include "AUXCAM.h"
#include "COMMON.h"
#include "UART.h"
#include <string.h>

void delay()
{
  double x;
  for(x = 0;x<50000;x++)
  {}
}

int main(void) {
  DCOCTL = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;

  WDTCTL = WDTPW + WDTHOLD;
  
  static E_STATUS MAIN_STATUS;
  
  MAIN_STATUS = UART_Init(UART_A0,BAUD_115200_16MHZ,0x00,0x80,0x01);

  uint8_t get_version[] = {0x56,0x00,0x11,0x00};
  
  
while(!MAIN_STATUS)
  {
    delay();
    MAIN_STATUS |= UART_Transmit(UART_A0,get_version,4);
    MAIN_STATUS |= UART_Recieve(UART_A0,16);
  }   
  return 0;
}
