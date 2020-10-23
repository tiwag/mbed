/* ***************************************************************
    Autor:  Zorn    Datum: 24.9.2020
    Thema:  Implementation - drive onboard led
****************************************************************** */
#include "ledout.h"

DigitalOut led1(LED1);

void ledout ( int state)
{
    led1 = state;
}

void ledtoggle ( void )
{
    led1 = !led1;
}


