#include "msp430.h"
#include <stdint.h>
#include "UART.h"
#include "COMMON.h"

uint8_t rxbuf[16];

extern E_STATUS UART_Init(uint8_t port,uint32_t baud,uint8_t config_0,uint8_t config_1,uint8_t interrupt_config)
{
  static E_STATUS UART_Init_Status;
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
      
      UART_Init_Status = E_OK;
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
      
      UART_Init_Status = E_OK;
    }
    else
    {
      UART_Init_Status = E_NOK;
    }
  } 
    else
    {
      UART_Init_Status = E_NOK;
    }
    return UART_Init_Status;
  }

extern E_STATUS UART_Transmit(uint8_t port,uint8_t data[],uint8_t tx_length)
{
  static E_STATUS UART_Transmit_Status;
  if((port == UART_A0 || port == UART_A1) && tx_length <= MAX_BUFFER_SIZE)
  {
    uint8_t tx_index = 0;
    if(port == UART_A0)
    {
      while(tx_index < tx_length)
      {
        UCA0TXBUF = data[tx_index];
        while((UCA0STAT & 0x01) == 0x01){}
      
        tx_index++;
      }
      UART_Transmit_Status = E_OK;
    }
    else if(port == UART_A1)
    {
      while(tx_index < tx_length)
      {
        UCA1TXBUF = data[tx_index];
        while((UCA1STAT & 0x01) == 0x01){}
      
        tx_index++;
      }
      UART_Transmit_Status = E_OK;
    }
    else
    {
      UART_Transmit_Status = E_NOK;
    }
  }
    else
    {
      UART_Transmit_Status = E_NOK;
    }
  return UART_Transmit_Status;
}

extern E_STATUS UART_Recieve(uint8_t port,uint8_t rx_length)
{
  static E_STATUS UART_Recieve_Status;
  if((port == UART_A0 || port == UART_A1) && rx_length != 0)
  {
    uint8_t rx_index = 0;
    if(port == UART_A0)
    {
      while(rx_index < rx_length)
      {
        while(!(IFG2&UCA0RXIFG)){}
        
        IFG2 &= ~0x01;
        rxbuf[rx_index] = UCA0RXBUF;
        rx_index++;
      }
      UART_Recieve_Status = E_OK;
    }
    else if(port == UART_A1)
    {
      while(rx_index < rx_length)
      {
        while(!(IFG2&UCA1RXIFG)){}
        
        IFG2 &= ~0x01;
        rxbuf[rx_index] = UCA1RXBUF;
        rx_index++;
      }
      UART_Recieve_Status = E_OK;
    }
    else
    {
      UART_Recieve_Status = E_NOK;
    }
  }
  else
  {
    UART_Recieve_Status = E_NOK;
  }
  return UART_Recieve_Status;
}