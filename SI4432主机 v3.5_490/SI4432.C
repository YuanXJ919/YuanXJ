#include "include.h"

#define RR		0x00
#define WR		0x80
#define  TX1_RX0	SpiWriteRegister(0x0e, 0x01)	  // 发射状态的天线开关定义
#define  TX0_RX1	SpiWriteRegister(0x0e, 0x02)	  // 接收状态的天线开关定义
#define  TX0_RX0	SpiWriteRegister(0x0e, 0x00)      // 非发射，接收状态的天线开关定义

unsigned char ItStatus1,ItStatus2,ItStatus3,ItStatus4;
BYTE rxBuffer_SI4432[6]={0};	//接收缓冲
BYTE txBuffer_SI4432[6]={0};
BYTE meter1[6]={0x68,0x00,0x01,0x68,0xD1,0x16};
BYTE RX_flag=0;
//********************************************************************************//
//* 函数名:					 delay_10us（）										  //
//* 函数功能：                 延时10us                                           //
//********************************************************************************//
//void delay_10us(char n)
//{
//	int i;
//	while(n--)
//	for(i=0;i<5;i++);
//}

//void delay_ms(unsigned int ms)
//{
//	unsigned int i;
//	unsigned char j;
//	for(i=0;i<ms;i++)
//	{
//		for(j=0;j<200;j++);
//		for(j=0;j<102;j++);
//	} 
//}
//********************************************************************************//
//* 函数名:		SPI_Init(void)						             
//* 函数功能：          SPI初始化函数                                     
//********************************************************************************//
void SPI_Init(void)
{
	RF4432_SEL_1;    
	RF4432_SCLK_0;
}
//********************************************************************************//
//* 函数名:		SPI_Read(void)						             
//* 函数功能：          SPI单字节读取函数                                   /
//********************************************************************************//
unsigned char SPI_Read(void)
{

	unsigned char i,rxdata;
	rxdata = 0x00;
	for (i = 0;i < 8;i++)
	{
		rxdata = rxdata<<1;
		RF4432_SCLK_0;
		if (RF4432_SDO)	//读取最高位，保存至最末尾，通过左移位完成整个字节
		{
			rxdata |= 0x01;
		}
//		delay_10us(2);
                delay_us(20);	
		RF4432_SCLK_1;
//		delay_10us(2);
                delay_us(20);	
	 }
	 return rxdata;

}
//********************************************************************************//
//* 函数名:	SPI_Write(unsigned char txdata)					          
//* 函数功能：  SPI单字节写入函数                                   
//********************************************************************************//
void SPI_Write(unsigned char txdata)
{
	unsigned char i;
	for (i = 0;i < 8;i++)
	{
		RF4432_SCLK_0;
		if ((txdata&0x80)==0x80)	//总是发送最高位
		{
		    RF4432_SDI_1;
		}
		else
		{
	  	    RF4432_SDI_0;
		}
		txdata = txdata<<1;
//		delay_10us(2);	
                delay_us(20);
		RF4432_SCLK_1;
//		delay_10us(2);	
                delay_us(20);
	}
}
//********************************************************************************//
//* 函数名:     RF4432_ReadReg(unsigned char  addr)			         
//* 函数功能：  RF4432寄存器读取函数                                
//********************************************************************************//
unsigned char  SpiReadRegister(unsigned char  addr)
{
	unsigned char value;
        _DINT();
	RF4432_SEL_0;  
	SPI_Write(addr|RR);      
	value = SPI_Read();          
	RF4432_SEL_1;         
        _EINT();
	return value;
}
//********************************************************************************//
//* 函数名:	RF4432_WriteReg( )  			                  
//* 函数功能：  RF4432寄存器写入函数                             
//********************************************************************************//
void  SpiWriteRegister(unsigned char  addr, unsigned char value)
{
        _DINT();
	RF4432_SEL_0;                  
	SPI_Write(addr|WR);      
	SPI_Write(value);          
	RF4432_SEL_1;                  
        _EINT();
}





// ----------------------------------------------------------------------
// 无线模块初始化
// ----------------------------------------------------------------------
void RF_init(void)
{
    static unsigned char j;
    
    SpiWriteRegister(0x07, 0x80);//SW reset
    while ( RF4432_IRQ == 1);
    ItStatus1 = SpiReadRegister(0x03);													//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);													//read the Interrupt Status2 register

        
    SpiWriteRegister(0x06, 0x80);	
    SpiWriteRegister(0x07, 0x01);															//write 0x80 to the Operating & Function Control1 register 
    //SpiWriteRegister(0x09, 0x64);		//云宝
    //SpiWriteRegister(0x09, 0x7f);     //思为
    SpiWriteRegister(0x09, 0x78);

    SpiWriteRegister(0x0A, 0x05);  //关闭低频输出
    SpiWriteRegister(0x0B, 0xEA);  //GPIO 0 当做普通输出口
    SpiWriteRegister(0x0C, 0xEA);  //GPIO 1 当做普通输出口
    SpiWriteRegister(0x0D, 0xF4);  //GPIO 2 输出收到的数据

    SpiWriteRegister(0x70, 0x24);  //0x2c);  // 下面的设置根据Silabs 的Excel
    SpiWriteRegister(0x1D, 0x40);  // 使能 afc
	
    ///*/ 1.2K bps setting
    SpiWriteRegister(0x1C, 0x16);	
    SpiWriteRegister(0x20, 0x83);

        SpiWriteRegister(0x21, 0xc0);  //
	SpiWriteRegister(0x22, 0x13);  // 
	SpiWriteRegister(0x23, 0xa9);  //
	SpiWriteRegister(0x24, 0x00);  //
	SpiWriteRegister(0x25, 0x04);  //
	
	SpiWriteRegister(0x2A, 0x14);
	SpiWriteRegister(0x6E, 0x09);
	SpiWriteRegister(0x6F, 0xd5);


        SpiWriteRegister(0x6D, 0x07);  //07 设置为最大功率发射
        SpiWriteRegister(0x0D, 0xFC);  //GPIO 2 信道空闲指示(常态为低，忙为高)
	SpiWriteRegister(0x79, 0x00);   // 不需要跳频
	SpiWriteRegister(0x7A, 0x00);   // 不需要跳频
    
	SpiWriteRegister(0x71, 0x22);  // 发射不需要 CLK，FiFo ， FSK模式			
	SpiWriteRegister(0x72, 0x30);  // 频偏为 30KHz
	
	SpiWriteRegister(0x73, 0x00);   // 没有频率偏差
	SpiWriteRegister(0x74, 0x00);   // 没有频率偏差
	
	SpiWriteRegister(0x75, 0x53);  // 频率设置 433.5
	SpiWriteRegister(0x76, 0x57);  // 
	SpiWriteRegister(0x77, 0x80);
	//SpiWriteRegister(0x75, 0x60);  // 频率设置 490
	//SpiWriteRegister(0x76, 0x7d);  // 
	//SpiWriteRegister(0x77, 0x00);												
        //test
//#ifdef debug
	j = SpiReadRegister(0x75);  // 频率设置 490
	j = SpiReadRegister(0x76);  // 
	j = SpiReadRegister(0x77);
        if(j>0)
        {
	SpiWriteRegister(0x34, 0x0C);															//write 0x0A to the Preamble Length register
        }
//#endif
	/*set the packet structure and the modulation type*/
	//set the preamble length to 10bytes 
	SpiWriteRegister(0x34, 0x0C);															//write 0x0A to the Preamble Length register
	//set preamble detection threshold to 20bits
	SpiWriteRegister(0x35, 0x2A); 														//write 0x2A to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	SpiWriteRegister(0x33, 0x02);															//write 0x02 to the Header Control2 register    
	
	//Set the sync word pattern to 0x2DD4
	SpiWriteRegister(0x36, 0x2D);															//write 0x2D to the Sync Word 3 register
	SpiWriteRegister(0x37, 0xD4);															//write 0xD4 to the Sync Word 2 register

	//enable the TX & RX packet handler and CRC-16 (IBM) check
	SpiWriteRegister(0x30, 0x8D);															//write 0x8D to the Data Access Control register
	//Disable the receive header filters
        SpiWriteRegister(0x32, 0x00);
	
	/*enable receiver chain*/
	SpiWriteRegister(0x07, 0x05);															//write 0x05 to the Operating Function Control 1 register
	//Enable two interrupts: 
	// a) one which shows that a valid packet received: 'ipkval'
	// b) second shows if the packet received with incorrect CRC: 'icrcerror' 
	SpiWriteRegister(0x05, 0x03); 														//write 0x03 to the Interrupt Enable 1 register
	SpiWriteRegister(0x06, 0x00); 														//write 0x00 to the Interrupt Enable 2 register
	//read interrupt status registers to release all pending interrupts
	ItStatus1 = SpiReadRegister(0x03);													//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);
}



//------------------------------------------------------------------------------------------------
// Function Name :RFSetRxMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于接收模式下
//
//-----------------------------------------------------------------------------------------------
void RFSetRxMode(void)
{  
	//read interrupt status registers to release all pending interrupts
	ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register

	TX0_RX1;		// 设置天线开关
	
	/*enable receiver chain*/
	SpiWriteRegister(0x07, 0x05); 
}

//------------------------------------------------------------------------------------------------
// Function Name :RFSetTxMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于发射状态模式下
//
//-----------------------------------------------------------------------------------------------
void RFSetTxMode(void)
{
	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);	        //read the Interrupt Status2 register

    TX1_RX0;		//设置天线开关的方向

	/*enable transmitter*/
	//The radio forms the packet and send it automatically.
	SpiWriteRegister(0x07, 0x09);
}

//------------------------------------------------------------------------------------------------
// Function Name :RFSetIdleMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在空闲状态下。当模块不需要接收数据也不需要发射数据的时候可以调用该函数以便省电；
//
//-----------------------------------------------------------------------------------------------
void RFSetIdleMode(void)
{
	//The radio will work in idle mode
	SpiWriteRegister(0x07, 0x01);			//write 0x09 to the Operating Function Control 1 register
	
	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
}
//------------------------------------------------------------------------------------------------
// Function Name :RFFIFOSendData
//
// Return Value : None
// Parameters   : uint8 length: the length of the content which will be sent out. This value should
//                not more than 64bytes because the FIFO is 64bytes. If it is bigger than 64, the
//                content should be sent out for more times according to the FIFO empty interrupt.
//                uint8 *payload: point to the content.
// Notes        : In FIFO mode, send out one packet data.
//                调用该函数可以把数据写入模块的FIFO中并把数据发送出去；数据长度不能大于64个字节；
//
//-----------------------------------------------------------------------------------------------
void RFFIFOSendData(unsigned char length, unsigned char *payload)
{
	unsigned char i;
        
	RFSetIdleMode();
	//turn on the Tx LED to show the packet transmission
	//P2OUT |= LED;
        
	/*SET THE CONTENT OF THE PACKET*/
	//set the length of the payload.	
	SpiWriteRegister(0x3E, length);

	//fill the payload into the transmit FIFO
	for(i = 0; i < length; i++)
	{
		SpiWriteRegister(0x7F, payload[i]);	       
	}

	//Disable all other interrupts and enable the packet sent interrupt only.
	//This will be used for indicating the successfull packet transmission for the MCU
	SpiWriteRegister(0x05, 0x04);			//write 0x04 to the Interrupt Enable 1 register	
	SpiWriteRegister(0x06, 0x00);			//write 0x00 to the Interrupt Enable 2 register	

	RFSetTxMode();
    
	/*wait for the packet sent interrupt*/
	//The MCU just needs to wait for the 'ipksent' interrupt.
	//等待发射完成中断的到来，如果中断采用非查询方式，则本程序到这里可以结束，但必须在中断服务程序中读取状态寄存器0x03和0x04，以便释放中断
	while(RF4432_IRQ);
        
	//read interrupt status registers to release the interrupt flags
	ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
        //P2OUT &= ~LED;
}