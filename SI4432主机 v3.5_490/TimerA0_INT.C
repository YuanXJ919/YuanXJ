#include "include.h"

BYTE Time_flag=0;
BYTE search_flag=0;
WORD search_count=0;

// Watchdog Timer interrupt service routine
/*#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
        search_count++;
        if(search_count==5260)
        {
            Time_flag = 1;
            search_count=0;

        }
  
      
}*/
//**********************************************************//
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{  

      search_flag=1;     
      //Tx_count++;//2013.12.13,此语句无用
}
// ----------------------------------------------------------------------
// Interrupt service routine for Timer A1 channel 0
// ----------------------------------------------------------------------
#pragma vector = TIMER1_A0_VECTOR
__interrupt void T1A0_ISR (void)
{
    search_count++;
        if(search_count>=1000)//10s到
        {
            Time_flag = 1;
            search_count=0;

        }    
}
