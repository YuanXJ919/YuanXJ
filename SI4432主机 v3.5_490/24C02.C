#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C126 16383
#define EepType AT24C02  //定义本程序中使用的EepRom为24C02 

//-----------------------------------------------------------------//
//-----------------------24CxxInit I2C初始化-----------------------//
//-----------------------------------------------------------------//
void Init24Cxx(void)
{
   P3SEL&=~(I2_SDA+I2_CLK+TEST_OUT+I2_WP);
   P3DIR|=(I2_SDA+I2_CLK+TEST_OUT+I2_WP); //定义数据、时钟端口方向,定义电源引脚、写保护引脚端口方向
   P3OUT&=~(I2_SDA+I2_CLK); //写保护
   P3OUT|=TEST_OUT;//EepROM电源供电
   P3OUT&=~I2_WP;
}
//--------------------------------------------------------------//
//-----------------------I2C总线启动---------------------------//
//-------------------------------------------------------------//
void I2C_Start(void)
{
  EepSda_Out();
  P3OUT|=I2_SDA;
  P3OUT|=I2_CLK;
  __delay_cycles(2);
  P3OUT&=~I2_SDA;
  __delay_cycles(2);
  P3OUT&=~I2_CLK;
}
//--------------------------------------------------------------//
//----------------------I2C总线停止----------------------------//
//--------------------------------------------------------------//
void I2C_Stop(void)
{
  EepSda_Out();
  P3OUT&=~I2_SDA;
  P3OUT&=~I2_CLK;
  __delay_cycles(2);
  P3OUT|=I2_CLK;
  P3OUT|=I2_SDA;
  __delay_cycles(2);
  
}
//---------------------I2C等待应答信号------------------------//
//-----出口参数：1：接收应答失败 0：接收应答成功--------------//
//------------------------------------------------------------//
BYTE I2C_Wait_Ack(void)
{
 // WDTCTL = WDT_ARST_1000;//喂狗
  BYTE ucErrTime=0;
  EepSda_In();
  P3OUT|=I2_SDA;
  __delay_cycles(20);
  P3OUT|=I2_CLK;
  __delay_cycles(20);
  while(P3IN&I2_SDA)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      I2C_Stop();
      return 1;
    }
  }
  P3OUT&=~I2_CLK;
  return 0;
}
//-------------------------------------------------------------//
//--------------------I2C产生应答-----------------------------//
//------------------------------------------------------------//
void I2C_Ack(void)
{
  EepSda_Out();
  P3OUT&=~I2_CLK;
  P3OUT&=~I2_SDA;
  __delay_cycles(2);
  P3OUT|=I2_CLK;
  __delay_cycles(2);
  P3OUT&=~I2_CLK;
}
//-------------------------------------------------------------//
//--------------------I2C不产生应答--------------------------//
//-------------------------------------------------------------//
void I2C_NAck(void)
{
  EepSda_Out();
  P3OUT&=~I2_CLK;
  P3OUT|=I2_SDA;
  __delay_cycles(2);
  P3OUT|=I2_CLK;
  __delay_cycles(2);
  P3OUT&=~I2_CLK;
}
//---------------------I2C发送一字节数据---------------------//
//------------入口参数：Data 需要发送的一字节数据------------//
//-----------------------------------------------------------//
void I2C_Send_Byte(BYTE Data)
{
  BYTE i;
  EepSda_Out();
  P3OUT&=~I2_CLK;
  for(i=0;i<8;i++)
  {
    if(Data&0x80) P3OUT|=I2_SDA;
    else P3OUT&=~I2_SDA;
    Data<<=1;
    __delay_cycles(2);
    P3OUT|=I2_CLK;
    __delay_cycles(20);
    P3OUT&=~I2_CLK;
    __delay_cycles(20);
  }
}
//--------------------I2C接收一字节数据-----------------------//
//入口参数：Ack：0不产生应答 1产生应答  出口参数：Data 接收到的一字节数据 
//------------------------------------------------------------//
BYTE I2C_Receive_Byte(BYTE Ack)
{
  BYTE i,Data;
  EepSda_In();
  for(i=0;i<8;i++)
  {
    P3OUT&=~I2_CLK;
    __delay_cycles(2);
    P3OUT|=I2_CLK;
    Data<<=1;
    if(P3IN&I2_SDA) Data|=0x01;
    __delay_cycles(2);
  }
  if(!Ack) I2C_NAck();
  else I2C_Ack();
  return Data;
}
//-------------------24Cxx在指定地址发送一字节数据---------------------//
//-----入口参数：address:指定地址 DataToWrite:需要发送的数据-----------//
//---------------------------------------------------------------------//
void AT24CxxSendOneByte(WORD address,BYTE DataToWrite)
{
  Init24Cxx();
  //WDTCTL = WDT_ARST_1000;//喂狗
  I2C_Start();
  I2C_Start();
  //Dly1mS(10);
  if(EepType>AT24C16)
  {
    I2C_Send_Byte(0xA0);
    I2C_Wait_Ack();
    I2C_Send_Byte(address>>8);
  }
  else
  {
    I2C_Send_Byte(0xA0+((address/256)<<1));
  }
  I2C_Wait_Ack();
  I2C_Send_Byte(address%256);
  I2C_Wait_Ack();
  I2C_Send_Byte(DataToWrite);
  I2C_Wait_Ack();
  I2C_Stop();
  delay_10us(100);
  P3OUT&=~(I2_SDA+I2_CLK);
  P3OUT&=~I2_WP;
  P2OUT&=~TEST_OUT;
  
}
//-------------------24Cxx在指定地址接收一字节数据---------------------//
//入口参数：address 需要读取数据地址 出口参数 DataToRead 读出的数据
//---------------------------------------------------------------------//
BYTE AT24CxxReadOneByte(WORD address)
{
  BYTE DataToRead;
  Init24Cxx();
 // WDTCTL = WDT_ARST_1000;//喂狗
  I2C_Start();
  if(EepType>AT24C16)
  {
    I2C_Send_Byte(0xA0);
    I2C_Wait_Ack();
    I2C_Send_Byte(address>>8);
  }
  else
  {
    I2C_Send_Byte(0xA0+((address/256)<<1));
  }
  I2C_Wait_Ack();
  I2C_Send_Byte(address%256);
  I2C_Wait_Ack();
  I2C_Start();
  I2C_Send_Byte(0xA1);
  I2C_Wait_Ack();
  DataToRead=I2C_Receive_Byte(1);
  I2C_Stop();
  delay_10us(100);
  P3OUT&=~(I2_SDA+I2_CLK);
  P3OUT&=~I2_WP;
  P3OUT&=~TEST_OUT;
  
  return DataToRead;
}
//--------------------在24Cxx指定地址读出指定个数的数据-------------------//
//入口参数：address 开始读出数据的地址 NUM 读出数据的个数 出口参数：*pBuffer 读出的存储数据
//------------------------------------------------------------------------//
void RD24Cxx(WORD address,BYTE *pBuffer,BYTE NUM)
{
  while(NUM)
  {
    *pBuffer++=AT24CxxReadOneByte(address++);
    NUM--;
  }
}
//--------------------在24Cxx指定地址写入指定个数的数据--------------------//
//入口参数：address 指定的地址 *pBuffer 需要写入的数据 NUM 需要写入的数据的个数 出口参数：无
//-------------------------------------------------------------------------//
void WR24Cxx(WORD address,BYTE *pBuffer,BYTE NUM)
{
  while(NUM)
  {
    AT24CxxSendOneByte(address,*pBuffer);
    address++;
    pBuffer++;
    NUM--;
  }
}
//-----------------------检测24Cxx----------------------------------//
//------------入口参数：无 出口参数：0 检测成功 1检测不成功---------//
//------------------------------------------------------------------//
BYTE AT24CxxCheck(void)
{
  Init24Cxx();
  //WDTCTL = WDT_ARST_1000;//喂狗
  BYTE temp;
  temp=AT24CxxReadOneByte(255);
  if(temp==0xAA) return 0;
  else
  {
    AT24CxxSendOneByte(255,0xAA);
    temp=AT24CxxReadOneByte(255);
    if(temp==0xAA) return 0;
  }
  return 1;
}