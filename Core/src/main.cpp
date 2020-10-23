/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "platform/mbed_thread.h"
/* ***************************************************************
    Autor:  Zorn    Datum: 13.10.2020
    Thema:  mbed-os-v5.14.2
                template für neue Projekte
            Debug-Ausgaben über sUSB.
            Serial resource Mutex-locked.
            input commands demo for int, double and char[]
****************************************************************** */
#include "main.h"

Thread thTh2;
int iTh2arg = 333;
volatile bool bTh2_run = false;

/////////////////////////////////////////////////////////////////////
int main() // this is Thread 1
{
  dbgini(RESETMSG);
  pc.printf("SystemCoreClock = %lf MHz\r\n", (double)SystemCoreClock / 1E6);
  pc.printf("HAL_RCC_GetHCLKFreq = %lf MHz\r\n",
            (double)HAL_RCC_GetHCLKFreq() / 1E6);

  thTh2.start(callback(th2callback, &iTh2arg));

  int dt = 100; // delta t [ms]
  Timer systimer;
  systimer.start();

  enum COMMANDS { NONE = 0, FREQU, PWMDC, COMMENT };

  double freq = 3.1415926;
  int dutycycle = 23;
  char comment[20] = "template app";

  while (1) {
    if ((NONE == iDbgCmd) &&
        bdbgrx) { // character received but not within Cmd input mode
      bdbgrx = false;
      iDbgMsgCnt = 0;
      switch (cdbgrx) {
      case '+': // dt+
        if (dt < DTMAX)
          dt += 100;
        dboi(dt);
        break;
      case '-': // dt-
        if (dt > DTMIN)
          dt -= 100;
        dboi(dt);
        break;
      case 't': // print state of thread2
        th2state();
        break;
      case 'e': // print actual values
        pc.printf("\r\n");
        pc.printf("frequency  = %lf\r\n", freq);
        pc.printf("dutycycle  = %d\r\n", dutycycle);
        pc.printf("comment    = \"%s\"\r\n", comment);
        break;
      case 's': // run / stop thread 2
        bTh2_run = (bTh2_run == true) ? false : true;

        break;
      case 'f':
        pc.printf("\r\nactual frequency = %lf, input new frequency = ", freq);
        iDbgCmd = FREQU;
        break;
      case 'd':
        pc.printf("\r\nactual duty cycle  = %d, input new duty cycle = ",
                  dutycycle);
        iDbgCmd = PWMDC;
        break;
      case 'c':
        pc.printf("\r\nactual comment = \"%s\", input new comment = ", comment);
        iDbgCmd = COMMENT;
        break;
      }
    }

    if (true == bDbgCmd) { // Cmd input done
      switch (iDbgCmd) {
      case FREQU:
        sscanf(cpDbgMsg, "%lf", &freq);
        pc.printf("new frequency = %lf\r\n", freq);
        break;
      case PWMDC:
        sscanf(cpDbgMsg, "%d", &dutycycle);
        pc.printf("new duty cycle = %d\r\n", dutycycle);
        break;
      case COMMENT:
        for (int j = 0; j < 20; j++) {
          char c = cpDbgMsg[j];
          if ('\r' == c)
            c = 0;
          comment[j] = c;
          if (0 == c)
            break;
        }
        pc.printf("   new comment = \"%s\"\r\n", comment);
        break;
      case NONE:
      default:
        pc.printf("\r\navailable commands:\r\n");
        pc.printf(" + - s t e d f c \r\n");
      }
      bDbgCmd = false; // Cmd processed
      iDbgCmd = NONE;
    }

    if (systimer.read_ms() > dt) {
      systimer.reset();
      ledtoggle();
    }
  }
}
