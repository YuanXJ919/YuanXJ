#include "include.h"

//========================================================
void MemCopy(BYTE  *src, BYTE  *des, BYTE Len)//复制数据
{
  BYTE i;
  for(i=Len;i>0;i--) {*des++=*src++;}
}
//=======================================================
void MemCopy_overturn(BYTE  *src, BYTE  *des, BYTE Len)//反向复制数据
{
  BYTE i;
  for(i=Len;i>0;i--) {*(des+i-1) =*src++;}
}
//=======================================================
void MemClear_rxBuffer_SI4432()
{
  BYTE i;
  for(i=0;i<7;i++)
  {
    rxBuffer_SI4432[i]=0x00;
  }
}
//========================================================
//********************************************************
//*******************************************************************
BYTE GetCs(BYTE  *StartPt,BYTE Len,BYTE m)
{
  BYTE i,j;
  BYTE  *pt;

  j=0;
  pt = StartPt+m; 
  for(i=0;i<Len;i++) j += * pt++;
  return(j);
}




