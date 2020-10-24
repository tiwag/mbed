/* ***************************************************************
    Autor:  Zorn    Datum: 30.9.2020
    Thema:  Implementation - Debug over serial-USB (sUSB)
****************************************************************** */
#include "dbgserial.h"

Mutex stdio_mutex;          // lock shared resource RawSerial
RawSerial pc(USBTX, USBRX); // tx, rx, use MBED os 5.14.2
char cdbgrx;                // last received character within rx-interrupt
bool bdbgrx;                // flag received character to user program

int iDbgMsgCnt = 0;
char cpDbgMsg[MAXMSG+1];  // contains last message

bool bDbgCmd = false;       // new input available
int iDbgCmd = 0;

void dbgrxint(void) {
  bdbgrx = true;
  cdbgrx = pc.getc();

  if ((MAXMSG) == iDbgMsgCnt) {
    cdbgrx = '\r';
  }

  cpDbgMsg[iDbgMsgCnt] = cdbgrx;
  iDbgMsgCnt++;
  cpDbgMsg[iDbgMsgCnt] = 0;

  if (DBGECHO) {
    pc.putc(cdbgrx);
  }
  if ('\r' == cdbgrx) {
    bDbgCmd = true;
    iDbgMsgCnt = 0;
    if (DBGECHO) {
      pc.putc('\n');
    }
  }
}

void dbgini(char *cp) {
  pc.baud(DBGBAUDRATE);
  bdbgrx = false;
  pc.attach(&dbgrxint);
  dbos(cp);
}

void dboc(char c) {
// debug out char
#if DEBUG
  stdio_mutex.lock();
  pc.putc(c);
  stdio_mutex.unlock();
#endif
}

void dbos(char *cp) {
// debug out string
#if DEBUG
  stdio_mutex.lock();
  pc.printf("%s", (char *)cp);
  stdio_mutex.unlock();
#endif
}

void dboi(int i) {
// debug out int
#if DEBUG
  stdio_mutex.lock();
  pc.printf(" %5d ", i);
  stdio_mutex.unlock();
#endif
}

void dbof(float f) {
// debug out float
#if DEBUG
  stdio_mutex.lock();
  pc.printf(" %5.5f ", f);
  stdio_mutex.unlock();
#endif
}
