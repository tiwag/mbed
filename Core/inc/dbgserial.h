/* ***************************************************************
    Autor:  Zorn    Datum: 24.9.2020
    Thema:  Header - Debug over serial-USB (sUSB)
****************************************************************** */
#ifndef ZO_DBGSERIAL_H
#define ZO_DBGSERIAL_H

#include "mbed.h"

/*  debugging:  1 .. enable, 0 .. disable   */
#define DEBUG 1

/*  list of supported baud rates for sUSB
    110, 300, 600, 1200, 2400, 4800, 9600, 14400,
    19200, 38400, 57600, 115200, 230400, 460800, 921600
    use this baudrate with your terminal program    */
#define DBGBAUDRATE 115200

/*  echo rxvd char to pc:  1 .. enable, 0 .. disable   */
#define DBGECHO 1

extern Mutex stdio_mutex;
extern RawSerial pc; // use MBED os 5.14.2

/*  recvd char from rx_usb_interrupt */
extern char cdbgrx;
extern bool bdbgrx;

#define MAXMSG 20
extern char cpDbgMsg[]; // contains last message
extern int  iDbgMsgCnt;
extern bool bDbgCmd;
extern int  iDbgCmd;


/*  prototypes */
void dbgini ( char* message );  // initialise sUSB
void dboc ( char c );       // debug out char
void dbos ( char* cp );     // debug out string
void dboi ( int i );        // debug out int
void dbof ( float f );      // debug out float

#endif

