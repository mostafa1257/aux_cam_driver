#include "msp430.h"
#include <stdint.h>
#include "COMMON.h"
/*---------------------------------*/
/*UART Perephiral Macros*/
/*---------------------------------*/
#define UART_A0     0U              
#define UART_A1     1U

#define MAX_BUFFER_SIZE     8U

#define BAUD_9600_16MHZ       1666U
#define BAUD_19200_16MHZ      833U
#define BAUD_38400_16MHZ      416U
#define BAUD_56000_16MHZ      285U
#define BAUD_115200_16MHZ     138U
#define BAUD_128000_16MHZ     125U
#define BAUD_256000_16MHZ     62U
/*---------------------------------*/
/*APIs Declaration*/
/*---------------------------------------------------------------------------------------------------------------*/
extern E_STATUS UART_Init(uint8_t port,uint32_t baud,uint8_t config_0,uint8_t config_1,uint8_t interrupt_config);
extern E_STATUS UART_Transmit(uint8_t port,uint8_t data[],uint8_t tx_length);
extern E_STATUS UART_Recieve(uint8_t port,uint8_t rx_length);

