#include "include.h"
#pragma vector = PORT2_VECTOR
__interrupt void POWER_OFF(void)
{
    if(P2IFG&nIRQ)
    {
         RX_flag =1; 
         P2IFG &= ~nIRQ;
    }
}
    
