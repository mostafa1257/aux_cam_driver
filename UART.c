#include "msp430.h"
#include <stdint.h>
#include <stdio.h>
#include "AUXCAM.h"
#include "COMMON.h"
#include "UART.h"

E_STATUS UART_INIT(uint8_t port,uint32_t baud,uint8_t config_0,uint8_t config_1,uint8_t interrupt_config)
{ 
  static E_STATUS UARTINIT_status;
  if((port == UART_A0 || port == UART_A1) && (
                           (baud == BAUD_9600_16MHZ)    || 
                           (baud == BAUD_19200_16MHZ)   || 
                           (baud == BAUD_38400_16MHZ)   || 
                           (baud == BAUD_56000_16MHZ)   || 
                           (baud == BAUD_115200_16MHZ)  || 
                           (baud == BAUD_128000_16MHZ)  || 
                           (baud == BAUD_256000_16MHZ)))
  {
    if(port == UART_A0 && (
                           (baud == BAUD_9600_16MHZ)    || 
                           (baud == BAUD_19200_16MHZ)   || 
                           (baud == BAUD_38400_16MHZ)   || 
                           (baud == BAUD_56000_16MHZ)   || 
                           (baud == BAUD_115200_16MHZ)  || 
                           (baud == BAUD_128000_16MHZ)  || 
                           (baud == BAUD_256000_16MHZ)))
    {
      UCA0CTL0 |= config_0;
      UCA0CTL1 |= config_1;
      UCA0BR0 = baud;
      P3SEL |= BIT4;
      P3SEL |= BIT5;
      IE2 |= interrupt_config;
      UCA0CTL1 &= ~UCSWRST;
      
      UARTINIT_status = E_OK;
    }
    else if(port == UART_A1 && (
                           (baud == BAUD_9600_16MHZ)    || 
                           (baud == BAUD_19200_16MHZ)   || 
                           (baud == BAUD_38400_16MHZ)   || 
                           (baud == BAUD_56000_16MHZ)   || 
                           (baud == BAUD_115200_16MHZ)  || 
                           (baud == BAUD_128000_16MHZ)  || 
                           (baud == BAUD_256000_16MHZ)))
    {
      UCA1CTL0 |= config_0;
      UCA1CTL1 |= config_1;
      UCA1BR0 = baud;
      P3SEL |= BIT6;
      P3SEL |= BIT7;
      IE2 |= interrupt_config;
      UCA1CTL1 &= ~UCSWRST;
      
      UARTINIT_status = E_OK;
    }
    else
    {
      UARTINIT_status = E_NOK;
    }
  } 
    else
    {
      UARTINIT_status = E_NOK;
    }
    return UARTINIT_status;
  }

E_STATUS UART_TRANSMIT(uint8_t port,uint8_t tx_data[],uint8_t tx_length)
{
  static E_STATUS UARTTRANSMIT_status;
  if((port == UART_A0 || port == UART_A1) && tx_length <= MAX_BUFFER_SIZE)
  {
    uint8_t tx_index = 0U;
    if(port == UART_A0)
    {
      while(tx_index < tx_length)
      {
        UCA0TXBUF = tx_data[tx_index];
        while((UCA0STAT & 0x01U) == 0x01U){}
      
        tx_index++;
      }
      UARTTRANSMIT_status = E_OK;
    }
    else if(port == UART_A1)
    {
      while(tx_index < tx_length)
      {
        UCA1TXBUF = tx_data[tx_index];
        while((UCA1STAT & 0x01U) == 0x01U){}
      
        tx_index++;
      }
      UARTTRANSMIT_status = E_OK;
    }
    else
    {
      UARTTRANSMIT_status = E_NOK;
    }
  }
    else
    {
      UARTTRANSMIT_status = E_NOK;
    }
  return UARTTRANSMIT_status;
}

E_STATUS UART_RECIEVE(uint8_t port,uint8_t response[],uint8_t rx_length)
{
  static E_STATUS UARTRECIEVE_status;
  if((port == UART_A0 || port == UART_A1) && rx_length != 0)
  {
    uint8_t rx_index = 0U;
    if(port == UART_A0)
    {
      while(rx_index < rx_length)
      {
        while(!(IFG2&UCA0RXIFG)){}
        
        IFG2 &= ~0x01U;
        response[rx_index] = UCA0RXBUF;
        rx_index++;
      }
      UARTRECIEVE_status = E_OK;
    }
    else if(port == UART_A1)
    {
      while(rx_index < rx_length)
      {
        while(!(IFG2&UCA1RXIFG)){}
        
        IFG2 &= ~0x01U;
        response[rx_index] = UCA1RXBUF;
        rx_index++;
      }
      UARTRECIEVE_status = E_OK;
    }
    else
    {
      UARTRECIEVE_status = E_NOK;
    }
  }
  else
  {
    UARTRECIEVE_status = E_NOK;
  }
  return UARTRECIEVE_status;
}