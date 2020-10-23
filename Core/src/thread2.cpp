
/* ***************************************************************
    Autor:  Zorn    Datum: 30.9.2020
    Thema:  thread demo template
            Debug-Ausgaben über sUSB.
****************************************************************** */
#include "dbgserial.h"
#include "thread2.h"

// externals
extern Thread thTh2;
extern volatile bool bTh2_run;

// locals
int  iSleep_ms;         // demo for passing an argument to thread
bool bRun = false;      // flags to lock run / stop messages to one time output
bool bStop = false;


// functions
void th2_run ( int* arg )
{
    if( false == bRun ) {
        dbos((char*)"\r\nTHREAD 2 RUNNING\r\n" );
        bRun = true;
    }
    dbos ((char*)">> T2: arg ");
    dboi (*arg);
    dbos ((char*)">> sleep_ms ");
    dboi (iSleep_ms);
    dbos ((char*)"\r\n");

    iSleep_ms += 11;
    if ( iSleep_ms > 399 ) {
        th2state();
        iSleep_ms = 333 ;
    }
    bStop = false;
}

void th2_stop ( void )
{
    if( false == bStop ) {
        dbos((char*)"\r\nTHREAD 2 STOPPED\r\n" );
        bStop = true;
    }
    bRun = false;
}

void th2callback ( int *arg )
{
    dbos((char*)"\r\nTHREAD 2 STARTED\r\n" );
    iSleep_ms = *arg;

    while(1) {
        if(bTh2_run) {
            th2_run(arg);
        } else {
            th2_stop();
        }
        ThisThread::sleep_for(iSleep_ms);
    }
}

void th2state(void)
{
    stdio_mutex.lock();
    pc.printf("\r\nThisThread_id %x >> T2 State %d \r\n", (unsigned int)ThisThread::get_id(),thTh2.get_state());
    stdio_mutex.unlock();
}



