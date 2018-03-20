#include "include.h"
/***********************************************************************************
* Function	: calculate_cs
* Description	: 计算效验和
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
BYTE calculate_cs(BYTE *buf, BYTE len)
{
    BYTE i = 0;
    BYTE cs = 0;

    for(i = 0; i < len; i++)
    {
        cs += buf[i];
    }
    return(cs);
}

//内存拷贝
void Memcpy(unsigned char *dec, unsigned char *src, unsigned char n)
{
	while(n--)
	{
		*dec++ = *src++;
	}
}

//内存清零
void MemClear_rxBuffer(void)
{
  BYTE i;
  for(i=0;i<=6;i++)
  {
    txBuffer_SI4432[i]=0x00;
  }
}

void copy_tx(void)
{
    Memcpy(txBuffer_SI4432, meter1, 6);  
}
