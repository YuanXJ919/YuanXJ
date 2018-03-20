
/***************************************************************************************
****************************************************************************************
* FILE		: com_fun.c
* Description	: 公用函数
*			  
* Copyright (c) 2011 by FLAG.ELE.CO. All Rights Reserved.
* 
* History:
* Version		Name       		Date			Description
   0.1		jinlei  	2011/07/22	Initial Version
   
****************************************************************************************
****************************************************************************************/
#include "var.h"
#include "com_fun.h"
#include "driver.h"
/***********************************************************************************
** Function Name:	byte_bin2bcd
** Input Parameters:    HEX码
** Output Parameters:   BCD码
** Description:  单字节HEX码转BCD码
***********************************************************************************/
 uint8 byte_bin2bcd(uint8 hex)
{
	uint8 bcd = 0;
	
	bcd = hex / 10;
	bcd = bcd % 10;
	return((bcd << 4) + (hex % 10));
}
/***********************************************************************************
** Function Name:	byte_bcd2bin
** Input Parameters:    BCD码
** Output Parameters:   HEX码
** Description:  单字节BCD码转HEX码
***********************************************************************************/
 uint8 byte_bcd2bin(uint8 bcd)
{
  return((bcd >> 4) * 10 + (bcd & 0x0F));
}
/***********************************************************************************
* Function	: bin_to_bcd
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
  void bin_to_bcd(uint8 *s, uint8 num)
{
  uint8 i;
    for(i = 0; i < num; i++)
    {
      *(s + i) = ((*(s + i) / 10)<<4) + (*(s + i) % 10);
    }
}
/***********************************************************************************
* Function	: bcd_to_bin
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
 void bcd_to_bin(uint8 *s, uint8 num)
{
  uint8 i;
    for(i = 0; i < num; i++)
    {
      *(s + i) = (*(s + i)>>4) * 10 + (*(s + i) & 0x0f);
    }
}


/***********************************************************************************
* Function	: bcd_add
* Description	: bcd 加法
* Input Para	: uint8 *DataOneDptr,uint8 *DataTwoDptr,uint8 DataLend
* Output Para	: uint8 *DataOneDptr
* Return Value: Sum返回溢出标志(最高1位)
***********************************************************************************/
uint8 bcd_add(uint8 *DataOneDptr,uint8 *DataTwoDptr,uint8 DataLend)
{
	uint8	One, Two, Tmp;	//中间变量
	uint8	i = 0;
	uint16	Sum = 0;			//考虑CY位故取整型
	
	while(DataLend--) 				//不能DataLend--或--DataLend
	{
		One = DataOneDptr[i];//取出正确的被加数
		Two = DataTwoDptr[i];//取出正确的加数
		Sum = One + Two + Sum;		//二进制求和(注意上次低位向高位的进位)
		Tmp = (One & 0xf0) + (Two & 0xf0);	//为半进位做准备

		if ((Tmp != (Sum & 0xf0)) || ((Sum & 0x0f) > 9)) 
		{
			Sum += 6;				//BCD码低4位调整
		}

		if (Sum >= 0xa0)
		{   
			Sum += 0x60;			//BCD码高4位调整
		}
		DataOneDptr[i] = Sum;//只存入低8位
		Sum >>= 8;//保留高8位做为下次低位向高位的进位
		i++;
	}
	return Sum;//返回溢出标志(最高1位)
}

/***********************************************************************************
* Function	: bcd_p
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
void bcd_p(uint8 s[], uint8 num)
{
  uint8 i = 0;
    
    s[0] = 0x9a - s[0];
    for(i = 1; i < num; i++)
    {
      s[i] = 0x99 - s[i];
    }
}
/***********************************************************************************
* Function	: bcd_sub
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
void bcd_sub(uint8 l[], uint8 s[], uint8 num)
{
  uint8 ss[5];
    
    memcpy(ss, s, num);
    bcd_p(ss, num);
    bcd_add(l, ss, num);
}
/***********************************************************************************
* Function	: memsub
* Description	:  内存数据减法操作 
* Input Para	: uint8 buf[] ，uint8 addend，uint8 cnt
* Output Para	: uint8 buf[] 
* Return Value: NONE
***********************************************************************************/
extern void memsub(uint8 buf[], uint8 addend, uint8 cnt)
{
    uint8 i = 0;

    for (i = 0; i < cnt; i++)
    {
        buf[i] -= addend;
    }
}
/***********************************************************************************
* Function	: memadd
* Description	:  内存数据加法操作
* Input Para	: uint8 buf[] ，uint8 addend，uint8 cnt
* Output Para	: uint8 buf[] 
* Return Value: NONE
***********************************************************************************/
extern void  memadd(uint8 buf[], uint8 addend, uint8 cnt)
{
    uint8 i = 0;

    for (i = 0; i < cnt; i++)
    {
        buf[i] += addend;
    }
}
/***********************************************************************************
* Function	: memcpy
* Description	: 内存数据拷贝
* Input Para	: uint8 from[]  uint8 cnt
* Output Para	: uint8 to[]
* Return Value: 
***********************************************************************************/
extern void memcpy(uint8 to[],uint8 from[],uint8 cnt)
{
    uint8 i=0;
    for(i=0;i<cnt;i++)
    	{
    	    to[i]=from[i];
    	}
}
/***********************************************************************************
* Function	: memmove
* Description	: 内存数据移动
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
extern void memmove(uint8 buff[],uint8 cnt,uint8 len)
{
     uint8 i=0;
     for(i=0;i<len-cnt;i++)
     	{
     	    buff[len-1-i]=buff[len-cnt-1-i];
     	}
}
/***********************************************************************************
* Function	: memset
* Description	: 设置内存数据
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
extern void memset(uint8 buff[],uint8 data,uint8 num)
{
    uint8 i=0;
    for(i=0;i<num;i++)
    	{
    	   buff[i]=data;
    	}
}
/***********************************************************************************
* Function	: memchr
* Description	: 设置内存查找
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
extern uint8*memchr(uint8 buff[],uint8 data,uint8 len)
{
   uint8 i=0;
   for(i=0;i<len;i++)
   	{
   	    if(buff[i]==data)  return (uint8*)(buff+i);
   	}
   return NULL;
}
/***********************************************************************************
* Function	: memcmp
* Description	: 内存数据比较
* Input Para	: 
* Output Para	: 
* Return Value:  buf1>buf2   1  ；buf1=buf2   0 ；buf1<buf2  -1
***********************************************************************************/
extern int8 memcmp(uint8 buf1[], uint8 buf2[], uint8 cnt)      
{                                                                                           
    uint8 i=0;                                                                       
    	for(i=0;i<cnt;)
    		{
    			i++;
    			if( buf1[cnt-i]>buf2[cnt-i] )    return 1;
                    if( buf1[cnt-i]<buf2[cnt-i] )    return -1;
    		}
    	return 0;
}
/***********************************************************************************
* Function	: calculate_cs
* Description	: 计算效验和
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
extern uint8 calculate_cs(uint8 *buf, uint8 len)
{
    uint8 i = 0;
    uint8 cs = 0;

    for(i = 0; i < len; i++)
    {
        cs += buf[i];
    }
    return(cs);
}
/***********************************************************************************
* Function	: check_BCD_format
* Description	: 检查BCD 数据格式
* Input Para	: uint8 buf[], uint8 cnt, uint8 limit_min, uint8 limit_max
* Output Para	: 
* Return Value: TRUE or FLASE
***********************************************************************************/
uint8 check_BCD_format(uint8 buf[], uint8 cnt, uint8 limit_min, uint8 limit_max)
{
	uint8 i = 0;
	
	for (i = 0; i < cnt; i++)
	{
        if (((buf[i] & 0x0F) > 0x09) || ((buf[i] & 0xF0) > 0x90) || (buf[i] > limit_max) || (buf[i] < limit_min))
        {
            return(_FALSE);
        }
	}
	return(_TRUE);
}

/***********************************************************************************
* Function	: buffer_cs_check
* Description	: 检查cs效验
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
uint8 buffer_cs_check(uint8 *buff, uint8 n)
{
	uint8	i;
	uint8	cs = *buff;
	uint8	cs_cnt = 0;
	
	if(n<2)	return	_FALSE;
	
	for(i=0; i<n; i++)
	{
		cs_cnt	+= *buff++;
	}
	if(cs_cnt == (uint8)(cs<<1))
	{
		return	_TRUE;
	}
	else
	{
		return	_FALSE;
	}
}


/***********************************************************************************
* Function	: buffer_cs_set
* Description	: 设置校验和，第一字节为校验和
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
void buffer_cs_set(uint8 *buff, uint8 n)
{
	uint8 *p;
	uint8 i;
	uint8 cs=0;
	
	if(n<2)	return;
	
	p	= buff++;
	
	for(i=0; i<n-1; i++)
	{
		cs	+= *buff++;
	}
	
	*p	= cs;
}

/***********************************************************************************
* Function	: IsEqual
* Description	: 判断 相等
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
unsigned char IsEqual(unsigned char *src1,unsigned char * src2,unsigned char len)
{
    do{
        if(*src1!=*src2)
        {
            return(ERROR);
        }
        src1++;
        src2++;
    }while(--len);
    return(OK);
}
/***********************************************************************************
* Function	: TimeCompare
* Description	: 时间比较函数
* Input Para	:
* Output Para	: 
* Return Value: 时间相等1    buff1<buff2  0  buff1>buff2  2 
***********************************************************************************/
uint8 TimeCompare( uint8 *buff1, uint8 *buff2, uint8 len )
{
    if( IsEqual( &buff1[0], &buff2[0], len ) == OK ) return TIME_EQUAL;
    while(len--)
    {
     if(buff1[len] > buff2[len])
        {
        return TIME_LARGER;
        }
      else if(buff1[len] < buff2[len])
        {
        return TIME_LITTER;      
        }
    }
    return ERROR;
}
/****************************************************************************
*	Module:	bcd_add_1
*	Arg:
*	Ret:  bcd 加 1
***************************************************************************/
void bcd_add_1(uint8 *s)
{
    if((*s & 0x0f) == 9)
    {
      *s = (*s & 0xf0) + 0x10;
    }
    else
      (*s)++;
}
/****************************************************************************
*	Module:	bcd_sub_1
*	Arg:
*	Ret:  bcd 减 1
***************************************************************************/

void bcd_sub_1(uint8 *s)
{
    if((*s & 0x0f) == 0)
    {
      *s = (*s | 0x09) - 0x10 ;
    }
    else
      (*s)--;
}
/***********************************************************************************
** Function Name:	bcd2byte_to_hex16
** Input Parameters:  BCD码 
** Output Parameters:    uint32位 HEX码
** Description: 2字节BCD码 转 HEX码
***********************************************************************************/
extern uint16 bcd2byte_to_hex16(uint8 bcd[])
{
	uint16 s;
	  s=(((bcd[0] >> 4) * 10 )+ (bcd[0]& 0x0F));
	  s+=(((bcd[1] >> 4) * 10 )+ (bcd[1]& 0x0F))*100;
	return s;
}
/***********************************************************************************
** Function Name:	bcd4byte_to_hex32
** Input Parameters:  BCD码 
** Output Parameters:    uint32位 HEX码
** Description: 4字节BCD码 转 HEX码
***********************************************************************************/
extern uint32 bcd4byte_to_hex32(uint8 bcd[])
{
	uint32 s,bcd_2,bcd_3;
	bcd_2=bcd[2];
	bcd_3=bcd[3];
	  s=(((bcd[0] >> 4) * 10 )+ (bcd[0]& 0x0F));
	  s+=(((bcd[1] >> 4) * 10 )+ (bcd[1]& 0x0F))*100;
	  s+=(((bcd_2 >> 4) * 10 )+ (bcd_2& 0x0000000F))*10000;
	  s+=(((bcd_3 >> 4) * 10 )+ (bcd_3& 0x0000000F))*1000000;
	return s;
}
/***********************************************************************************
** Function Name:	uint16_to_bcd
** Input Parameters:    uint16位 HEX码
** Output Parameters:   BCD码
** Description:  2字节HEX码转BCD码
***********************************************************************************/
extern void uint16_to_bcd(uint16 s, uint8 *num)
{
  num[0] = s%100;
  num[1] = (s % 10000)/100;
  bin_to_bcd(num,2);  
}
/***********************************************************************************
** Function Name:	uint32_to_bcd
** Input Parameters:    uint32位 HEX码
** Output Parameters:   BCD码
** Description:  3字节HEX码转BCD码num[0] 为低位
***********************************************************************************/
extern void uint32_to_bcd(uint32 s, uint8 *num)
{
  num[0] = s%100;
  num[1] = (s % 10000)/100;
  num[2] = s / 10000;
  bin_to_bcd(num,3);  
}
/***********************************************************************************
** Function Name:	uint32_to_bcd4byte
** Input Parameters:    uint32位 HEX码
** Output Parameters:   BCD码
** Description:  HEX码转BCD码num[0] 为低位
***********************************************************************************/
extern void uint32_to_bcd4byte(uint32 s, uint8 *num)
{
  num[0] = s%100;
  num[1] = (s % 10000)/100;
  num[2] = s / 10000;
  num[3] = s / 1000000;
  bin_to_bcd(num,4);  
}

/***********************************************************************************
* Function	: move_data_type_to_hex
* Description	: 3byte uint8  to uint32
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
 uint32 move_data_type_to_hex(uint8 *from)
{uint32 result;
    result=from[2];
    result<<=8;
    result+=from[1];
    result<<=8;
    result+=from[0];
    return result;
}

/***********************************************************************************
* Function	: mult_data
* Description	: a*b/c
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
 uint16 mult_data(uint32 a,uint32 b,uint32 num)
{
    uint16 result;
    uint32 c;
    c=a*b;
    c=c/num;
    result=c;
    return result;
}

/***********************************************************************************
* Function	: turn_data
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value: 
***********************************************************************************/
 void turn_data(uint8 *from,uint8 *to,uint8 num)
{
    uint8 i=0;
    while(num--)
    	{
    	    to[i]=from[num];
    	    i++;
    	}
}
 
