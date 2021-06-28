/*-------------------------------------*/
/*Error Codes*/
/*-------------------------------------*/
#define CMD_EXECUTE_RIGHT          0x00

#define CMD_NOT_SENT               0x01
#define DATA_LENGTH_ERROR          0x02
#define DATA_FORMAT_ERROR          0x03
#define CMD_CANNOT_EXECUTE_NOW     0x04
#define CMD_EXECUTION_WRONG        0x05
/*-------------------------------------*/
/*Protocol Signs*/
/*---------------------------*/
#define REQUEST_SIGN     0x56
#define RETURN_SIGN      0x76
#define SERIAL_NUMBER    0x00
/*---------------------------*/
/*Response Length*/
/*---------------------------------------*/
#define GET_VERSION_REPONSE_LENGTH     16
#define FBUF_CTRL_RESPONSE_LENGTH      5
#define GET_BUF_LEN_LENGTH             9
/*--------------------------------------*/
/*Commands*/
/*---------------------------*/
#define GEN_VERSION      0x11
#define SYSTEM_RESET     0x26
#define READ_DATA        0x30
#define WRITE_DATA       0x31
#define GET_FBUF_LEN     0x34
#define FBUF_CTRL        0x36
/*---------------------------*/
/*Command Data*/
/*--------------------------------------*/
#define FBUF_TYPE_CURReNT_FRAME     0x00
#define FBUF_TYPE_NEXT_FRAME        0x01
#define FBUF_STOP_CURRENT_FRAME     0x00
#define FBUF_STOP_NEXT_FRAME        0x01
#define FBUF_RESUME_FRAME           0x02
#define FBUF_STEP_FRAME             0x03
/*--------------------------------------*/