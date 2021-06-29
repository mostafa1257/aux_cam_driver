#include "msp430.h"
#include <stdint.h>
#include <stdio.h>
#include "COMMON.h"
#include "UART.h"
/*--------------------------------------*/
/*Error Codes*/
/*--------------------------------------*/
#define CMD_EXECUTE_RIGHT          0x00U

#define CMD_NOT_SENT               0x01U
#define DATA_LENGTH_ERROR          0x02U
#define DATA_FORMAT_ERROR          0x03U
#define CMD_CANNOT_EXECUTE_NOW     0x04U
#define CMD_EXECUTION_WRONG        0x05U
/*--------------------------------------*/
/*Protocol Signs*/
/*----------------------------*/
#define REQUEST_SIGN      0x56U
#define RESPONSE_SIGN     0x76U
#define SERIAL_NUMBER     0x00U
/*----------------------------*/
/*Command Lengths*/
/*----------------------------------------*/
#define SYSTEM_RESET_COMMAND_LENGTH     4U
#define GEN_VERSION_COMMAND_LENGTH      4U
#define FBUF_CTRL_COMMAND_LENGTH        5U
#define GET_FBUF_LEN_COMMAND_LENGTH     5U
#define READ_FBUF_COMMAND_LENGTH        16U
/*----------------------------------------*/
/*Response Lengths*/
/*------------------------------------------*/
#define SYSTEM_RESET_RESPONSE_LENGTH     4U
#define GEN_VERSION_RESPONSE_LENGTH      16U
#define FBUF_CTRL_RESPONSE_LENGTH        5U
#define GET_FBUF_LEN_RESPONSE_LENGTH     9U
/*-----------------------------------------*/
/*Commands*/
/*----------------------------*/
#define GEN_VERSION      0x11U
#define SYSTEM_RESET     0x26U
#define READ_DATA        0x30U
#define WRITE_DATA       0x31U
#define READ_FBUF        0x32U
#define GET_FBUF_LEN     0x34U
#define FBUF_CTRL        0x36U
/*----------------------------*/
/*FBUF Types*/
/*---------------------------------------*/
#define FBUF_TYPE_CURRENT_FRAME     0x00U
#define FBUF_TYPE_NEXT_FRAME        0x01U
/*----------------------------*/
/*Control Flags*/
/*---------------------------------------*/
#define FBUF_STOP_CURRENT_FRAME     0x00U
#define FBUF_STOP_NEXT_FRAME        0x01U
#define FBUF_RESUME_FRAME           0x02U
#define FBUF_STEP_FRAME             0x03U
/*---------------------------------------*/
/*Request Commands*/
/*-----------------------------------------------------------*/
static uint8_t CMD_SYSTEM_RESET[SYSTEM_RESET_COMMAND_LENGTH];
static uint8_t CMD_GEN_VERSION[GEN_VERSION_COMMAND_LENGTH];
static uint8_t CMD_FBUF_CTRL[FBUF_CTRL_COMMAND_LENGTH];  
static uint8_t CMD_GET_FBUF_LEN[GET_FBUF_LEN_COMMAND_LENGTH]; 
static uint8_t CMD_READ_FBUF[];    
/*-----------------------------------------------------------*/
/*Response Buffers*/
/*------------------------------------------------------------*/
static uint8_t RES_SYSTEM_RESET[SYSTEM_RESET_RESPONSE_LENGTH];
static uint8_t RES_GEN_VERSION[GEN_VERSION_RESPONSE_LENGTH];
static uint8_t RES_FBUF_CTRL[FBUF_CTRL_RESPONSE_LENGTH];
static uint8_t RES_GET_FBUF_LEN[GET_FBUF_LEN_RESPONSE_LENGTH];
/*------------------------------------------------------------*/
/*System Variables*/
/*---------------------------*/
static uint8_t  HS_UART_PORT;
static uint16_t BUFFER_SIZE;
/*---------------------------*/
/*APIs Declaration*/
/*------------------------------------------------------------------*/
extern E_STATUS CAMERA_INIT(uint8_t uart_port,uint32_t camera_baud);
extern E_STATUS CAMERA_RESET(void);
extern E_STATUS CAMERA_GET_VERSION(void);
extern E_STATUS CAMERA_FBUF_CTRL(uint8_t control_flag);
extern E_STATUS CAMERA_GET_FBUF_LEN(uint8_t fbuf_type);
extern E_STATUS CAMERA_READ_FBUF(uint8_t fbuf_type,uint32_t start_address,uint8_t data_length[4],uint8_t delay[2]);
