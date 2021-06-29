#include "msp430.h"
#include <stdint.h>
#include <stdio.h>
#include "AUXCAM.h"
#include "COMMON.h"
#include "UART.h"

uint8_t CMD_SYSTEM_RESET[SYSTEM_RESET_COMMAND_LENGTH] = {REQUEST_SIGN,SERIAL_NUMBER,SYSTEM_RESET,0x00U};
uint8_t CMD_GEN_VERSION[GEN_VERSION_COMMAND_LENGTH]   = {REQUEST_SIGN,SERIAL_NUMBER,GEN_VERSION,0x00U};
uint8_t CMD_FBUF_CTRL[FBUF_CTRL_COMMAND_LENGTH]       = {REQUEST_SIGN,SERIAL_NUMBER,FBUF_CTRL,0x01U,'\0'};      // null is replaced by control flag (1 byte) index[4]
uint8_t CMD_GET_FBUF_LEN[GET_FBUF_LEN_COMMAND_LENGTH] = {REQUEST_SIGN,SERIAL_NUMBER,GET_FBUF_LEN,0x01U,'\0'};   // null is replaced by fbuf type (1 byte)    index[4]

uint8_t CMD_READ_FBUF[]    = {REQUEST_SIGN,SERIAL_NUMBER,READ_FBUF,0x0CU,'\0',0x0AU, // null is replaced by fbuf type(1 byte)     index[4]
'\0','\0','\0','\0', // null is replaced by starting address (4 bytes) indices [6,7,8,9]
'\0','\0','\0','\0', // null is replaced by data length(4 bytes)       indices [10,11,12,13]
'\0','\0'};          // null is replaced by delay(2 bytes)             indices [14,15]
                                                                                                                                          
                                                                                                                                         
E_STATUS CAMERA_INIT(uint8_t uart_port,uint32_t camera_baud)
{
  static E_STATUS CAMERAINIT_status;
  
  if((uart_port == UART_A0 || uart_port == UART_A1) && camera_baud == 115200U)
  {
    HS_UART_PORT = uart_port;
    
    CAMERAINIT_status |= UART_INIT(HS_UART_PORT,BAUD_115200_16MHZ,0x00U,0x80U,UART_INTERRUPT_ENABLE_RX);
    
    return CAMERAINIT_status;
  }
  else
  {
    CAMERAINIT_status = E_NOK;
  }
  return CAMERAINIT_status;
}
                                                                                                                                         
E_STATUS CAMERA_RESET(void)
{
  static E_STATUS CAMERARESET_status;
  
  CAMERARESET_status |= UART_TRANSMIT(HS_UART_PORT,CMD_SYSTEM_RESET,SYSTEM_RESET_COMMAND_LENGTH);
  CAMERARESET_status |= UART_RECIEVE(HS_UART_PORT,RES_SYSTEM_RESET,SYSTEM_RESET_RESPONSE_LENGTH);
  
  return CAMERARESET_status;
}

E_STATUS CAMERA_GET_VERSION(void)
{
  static E_STATUS CAMERAGETVERSION_status;
  
  CAMERAGETVERSION_status |= UART_TRANSMIT(HS_UART_PORT,CMD_GEN_VERSION,GEN_VERSION_COMMAND_LENGTH);
  CAMERAGETVERSION_status |= UART_RECIEVE(HS_UART_PORT,RES_GEN_VERSION,GEN_VERSION_RESPONSE_LENGTH);
  
  return CAMERAGETVERSION_status;
}

E_STATUS CAMERA_FBUF_CTRL(uint8_t control_flag)
{
  static E_STATUS CAMERAFBUFCTRL_status;
  
  if(control_flag == FBUF_STOP_CURRENT_FRAME || 
     control_flag == FBUF_STOP_NEXT_FRAME    ||
     control_flag == FBUF_RESUME_FRAME       ||
     control_flag == FBUF_STEP_FRAME)
  {
    CMD_FBUF_CTRL[4] = control_flag;
    
    CAMERAFBUFCTRL_status |= UART_TRANSMIT(HS_UART_PORT,CMD_FBUF_CTRL,FBUF_CTRL_COMMAND_LENGTH);
    CAMERAFBUFCTRL_status |= UART_RECIEVE(HS_UART_PORT,RES_FBUF_CTRL,FBUF_CTRL_RESPONSE_LENGTH);
    
    return CAMERAFBUFCTRL_status;
  }
  else
  {
    CAMERAFBUFCTRL_status = E_NOK;
  }
  return CAMERAFBUFCTRL_status;
}

E_STATUS CAMERA_GET_FBUF_LEN(uint8_t fbuf_type)
{
  static E_STATUS CAMERAGETFBUFLEN_status;
  
  if(fbuf_type == FBUF_TYPE_CURRENT_FRAME || fbuf_type == FBUF_TYPE_NEXT_FRAME)
  {
    CMD_GET_FBUF_LEN[4] = fbuf_type;
    
    CAMERAGETFBUFLEN_status |= UART_TRANSMIT(HS_UART_PORT,CMD_GET_FBUF_LEN,GET_FBUF_LEN_COMMAND_LENGTH);
    CAMERAGETFBUFLEN_status |= UART_RECIEVE(HS_UART_PORT,RES_GET_FBUF_LEN,GET_FBUF_LEN_RESPONSE_LENGTH);
    
    BUFFER_SIZE =  RES_GET_FBUF_LEN[5];
    BUFFER_SIZE <<= 8U;
    BUFFER_SIZE |= RES_GET_FBUF_LEN[6];
    BUFFER_SIZE <<= 8U;
    BUFFER_SIZE |= RES_GET_FBUF_LEN[7];
    BUFFER_SIZE <<= 8U;
    BUFFER_SIZE |= RES_GET_FBUF_LEN[8];
    
    return CAMERAGETFBUFLEN_status;
  }
  else
  {
    CAMERAGETFBUFLEN_status = E_NOK;
  }
  return CAMERAGETFBUFLEN_status;
}

E_STATUS CAMERA_READ_FBUF(uint8_t fbuf_type)
{
  static E_STATUS CAMERAREADFBUF_status;
  
  if(fbuf_type == FBUF_TYPE_CURRENT_FRAME || fbuf_type == FBUF_TYPE_NEXT_FRAME)
  {
    static uint32_t address_to_read;
    while(address_to_read < BUFFER_SIZE + FBUF_BYTES_TO_READ)
    {
      CMD_READ_FBUF[4] = fbuf_type;
      
      CMD_READ_FBUF[6] = (address_to_read >> 24U) & 0xFFU;
      CMD_READ_FBUF[7] = (address_to_read >> 16U) & 0xFFU;
      CMD_READ_FBUF[8] = (address_to_read >>  8U) & 0xFFU;
      CMD_READ_FBUF[9] =  address_to_read         & 0xFFU;
      
      CMD_READ_FBUF[13] = FBUF_BYTES_TO_READ;
      
      CMD_READ_FBUF[14] = 1U;
      CMD_READ_FBUF[15] = 0U;
      
      CAMERAREADFBUF_status |= UART_TRANSMIT(HS_UART_PORT,CMD_READ_FBUF,READ_FBUF_COMMAND_LENGTH);
      CAMERAREADFBUF_status |= UART_RECIEVE(HS_UART_PORT,RES_READ_FBUF,5 + FBUF_BYTES_TO_READ + 5);
      
      address_to_read += FBUF_BYTES_TO_READ;
      
      CAMERAREADFBUF_status |= UART_TRANSMIT(UART_A1,RES_READ_FBUF,5 + FBUF_BYTES_TO_READ + 5);
    }
    
    return CAMERAREADFBUF_status;
  }
  else
  {
    CAMERAREADFBUF_status = E_NOK;
  }
  return CAMERAREADFBUF_status;
}