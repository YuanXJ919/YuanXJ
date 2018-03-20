/************************************************************************************
*  Copyright (c) 20XX,西安xxxxxxx公司
*            All rights reserved.
*
* 文件名称： MAIN.C
* 文件标识： none
* 适用器件： msp430g2433
* 
* 摘 要：
* 
* 当前版本：V3.2
* 作 者：xxx
* 完成日期：20xx年xx月xx日
*
*************************************************************************************/
//修改记录
//2013.12.13,时间10s到，但设备继续工作，进行调整
//2014.1.7,小李要求加开关P2.1(I/O)，轻药感除以10，重药感除以20,设定P2.1为高电平时除以10，低电平时除以20。
//2014.2.23:
//1、添加正在工作标志，主机接收到正确命令是置1，时间到后就清0，为1时表示正在工作状态，不再接收接收器发来的命令。
//2、将10s定时改为使用Timer_B；
//3、加看门狗功能；
//2014.3.10:10结束后重新初始化SPI，保证无线模块正常工作，解决可能死机问题。

#include "include.h"
//#include "KEY.C"
//#include "24C02.C"
//#include "protocol.C"

//#include "SI4432.C"
//#include "public.C"
//#include "PORT2_INT.C"
//#include "TimerA0_INT.C"

void Si4432_Second_count(void);
void Si4432_tx_data(void);
void Si4432_rx_data(void);
void Second_time(void);

#define DEVICE_ADDR    222     //设备ID
#define keyin2 (P2IN & 0x02)
#define debug
//=================================================================//
void Delay20Ms_1M(BYTE MsTime)
{
  WORD i;
  
  while (MsTime--){     //delay 20Ms
    for (i=0;i<1820;i++) _NOP();
    //WDTCTL = WDT_MRST_32;  //喂狗 
  }    
}
//=================================================================//
void Cfg32k(void)
{
    BYTE i;
    BCSCTL1 &= ~XTS;         //低频模式
    do 
    {
	IFG1 &= ~OFIFG;                    // 清除OSCFault标志
	for (i = 0x20; i > 0; i--);                
    }
    while ((IFG1 & OFIFG) == OFIFG);      // 如果OSCFault =1  
}

void PORT_Init(void)
{
 
  P1SEL |= BIT0;
  
  //P2SEL |= KEY3;          //KEY3 INPUT  P2.0
  P2DIR &= ~KEY3;  
  //P2SEL |= KEY1;          //KEY1 INPUT  P2.1
  P2DIR &= ~KEY1;   
  //P2SEL |= KEY2;          //KEY2 INPUT  P2.2
  P2DIR &= ~KEY2;
  P2DIR &= ~nIRQ;
  
  P1SEL &= ~GPIO0;         //GPIO0 OUTPUT
  P1DIR |=  GPIO0;
  P1OUT |=  GPIO0; 

  
  P1SEL &= ~GPIO1;         //GPIO1 OUTPUT
  P1DIR |=  GPIO1;
  P1OUT |=  GPIO1; 
  
  P1SEL &= ~GPIO2;         //GPIO2 OUTPUT
  P1DIR |=  GPIO2;
  P1OUT |=  GPIO2; 
  
  P1SEL &= ~SDO;            //SDO INPUT
  P1DIR &= ~SDO; 
  
  P1SEL &= ~SDI;            //SDI OUTPUT
  P1DIR |= SDI;
  P1OUT |=  SDI;
  
  P1SEL &= ~SCLK;          //SCLK OUTPUT
  P1DIR |=  SCLK;
  P1OUT |=  SCLK;
  
  P3SEL &= ~nSEL;           //nSEL OUTPUT
  P3DIR |=  nSEL;
  P3OUT |=  nSEL;
  
  P3SEL &= ~SDN;           //SDN OUTPUT
  P3DIR |=  SDN;
  P3OUT |=  SDN;
  
  P2SEL &= ~LED;
  P2DIR |= LED;
  P2OUT |= LED;
  

  P3SEL &= ~DRIVER;         //PWR_EN OUTPUT
  P3DIR |=  DRIVER;
  P3OUT &= ~DRIVER;
  
  P2REN |= nIRQ;     //打开上拉，电路板上无上拉电阻，触发边沿有高电平到低电平
  P2IE |= nIRQ;      //PWR_TEST INPUT中断处理 
  P2IES |= nIRQ;     //高电平表示下降沿触发
  P2IFG &= ~nIRQ;

}

void Init_TimerA0(void)
{
   /*TACTL = TASSEL_1 + TACLR ; //ACLK=32768Hz 
   CCTL0 = CCIE; //
   CCR0 = 8192;   //  定时250mS
   TACTL |= MC_1; //增计数  */
   TACTL = TACLR+MC1+TAIE;
   
}

void Init_Device(void)
{
        RF_SDN_1;//芯片将完全关闭并且寄存器内容将全部丢失，重新配置寄存器数据
        delay_ms(10);
        RF_SDN_0;
        delay_ms(200);
}

BYTE LIFlag=1;
BYTE Working_flag=0;//主机正在计数工作时的标志，正在工作状态，不接受接收器发来的数据
BYTE rx_length,i;
BYTE CS=0;

void main(void)
{
      //WDTCTL = WDT_ADLY_1_9;                    // WDT 1.9ms, ACLK, interval timer
      //IE1 &= ~WDTIE;                             // Close WDT interrupt
      WDTCTL = WDTPW + WDTHOLD;
//      Delay20Ms_1M(20);   //大约延时 1s
      //----- MCLK=500K SMCLK=500K------------c-------------------------------
      //BCSCTL1= CALBC1_16MHZ;        
      //DCOCTL = CALDCO_16MHZ;       //XT2OFF,LFXT1 mode=Low frequency mode,Divider for ACLK=1,	
      //BCSCTL2= DIVM0 +DIVS0;      //MCLK =SMCLK=1/2 DCO=1/2MHz
      BCSCTL1= CALBC1_8MHZ;      
      DCOCTL = CALDCO_8MHZ;
//      BCSCTL1= CALBC1_12MHZ;      
//      DCOCTL = CALDCO_12MHZ;      
//      BCSCTL1= CALBC1_16MHZ;      
//      DCOCTL = CALDCO_16MHZ;      
//      BCSCTL1= CALBC1_1MHZ;      
//      DCOCTL = CALDCO_1MHZ;      
//      Cfg32k();     
      //Init_TimerA0();
      PORT_Init();
      if(keyin2==0x02) LIFlag = 1;
        else LIFlag = 0;      
      //P2OUT |= LED;
      Init_Device();   
      //根据不同的射频参数初始化射频模块；
      RF_init();
      RFSetRxMode();
      P2OUT &= ~LED;
//      WDTCTL = WDT_ARST_1000;//设置看门狗
       _EINT();
      while(1)
      {
//          WDTCTL = WDTPW + WDTCNTCL;
          //Si4432_rx_data();
          Si4432_tx_data();
          //Second_time();
//          WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL;//喂狗
//          WDTCTL = WDTPW + WDTCNTCL;
      }     
} 

void Second_time(void)
{
     if(Time_flag==1){//10s定时到做如下处理
          TAR = 0;
          //count = TAR;//读计数值
          //IE1 &= ~WDTIE;
          TA1CTL = TACLR + MC_0;
          TACTL = TACLR + MC_0;
          search_flag=0;//2013.12.13,时间10s到，但设备继续工作，
          Time_flag = 0;
          search_count = 0;
          Working_flag = 0;//清除工作标志
          RF_init();//2013.3.10,解决无线模块可能不中断问题
          //Init_Device();
          RFSetRxMode();//2013.12.13,时间10s到，接收器立即按键启动主机工作时，主机有可能不能准确接收命令，加此语句
      }
}
void Si4432_tx_data(void)
{
    if(search_flag==1){  
          search_flag=0;
          TACTL = TACLR+MC1;
          CCTL0 = CCIE;
          //CCR0=10;
          if(LIFlag) CCR0=10;else CCR0=20;
          MemClear_rxBuffer();
          copy_tx();
          txBuffer_SI4432[1] = DEVICE_ADDR;
          txBuffer_SI4432[2] = 0x01;
          CS = calculate_cs(txBuffer_SI4432, 4);
          txBuffer_SI4432[4] = calculate_cs(txBuffer_SI4432, 4); 

          //不使能中断
//          P2IE &= ~nIRQ;      //PWR_TEST INPUT中断处理 

          RFFIFOSendData(6, txBuffer_SI4432);

          //清楚中断标志位，使能中断
//          P2IFG &= ~nIRQ;
//          P2IE |= nIRQ;      //PWR_TEST INPUT中断处理 
          
          MemClear_rxBuffer();
          //reset the RX FIFO
       //SpiWriteRegister(0x08, 0x02);			//write 0x02 to the Operating Function Control 2 register
       //SpiWriteRegister(0x08, 0x00);			//write 0x00 to the Operating Function Control 2 register
       //RFSetRxMode();
          
    }
        
}
void Si4432_rx_data(void)
{
     BYTE control_byte;
     if((RX_flag==1)&&(Working_flag==0))
     {  
	RX_flag=0;
        RFSetIdleMode();                          //设置模块处于空闲模式，处理收到的数据包，不继续接收数据
	if((ItStatus1 & 0x01) == 0x01 )          //判断是否由于CRC校验出错引发的中断；在RFSetIdleMode中已经读出了中断状态寄存器的值
	{   SpiWriteRegister(0x07, 0x01);
       	    SpiWriteRegister(0x08, 0x02);	  //如果是CRC校验出错，那么接收FIFO复位,write 0x02 to the Operating Function Control 2 register
            SpiWriteRegister(0x08, 0x00);        //write 0x00 to the Operating Function Control 2 register
            
	}
	if((ItStatus1 & 0x02) == 0x02 )          //判断是否是数据包已经被正确接收。
        {
	    rx_length = SpiReadRegister(0x4B);	  //数据包已经被正确接收，读取收到的数据包长度,read the Received Packet Length register
//	    for(i=0;i < rx_length;i++)
            //_DINT();
	    for(i=0;i < 6;i++)
	    {
		rxBuffer_SI4432[i] = SpiReadRegister(0x7F);		//read the FIFO Access register
	    }
            //_EINT();
	    SpiWriteRegister(0x05, 0x03); 				//write 0x03 to the Interrupt Enable 1 register
	    SpiWriteRegister(0x06, 0x00); 				//write 0x00 to the Interrupt Enable 2 register
	    ItStatus1 = SpiReadRegister(0x03);				//read the Interrupt Status1 register
	    ItStatus2 = SpiReadRegister(0x04);				//read the Interrupt Status2 register	
            CS = calculate_cs(rxBuffer_SI4432, 4);
            if((rxBuffer_SI4432[0] == 0x68) && (rxBuffer_SI4432[1] == DEVICE_ADDR)
               && (rxBuffer_SI4432[3]==0x68) && (rxBuffer_SI4432[5]==0x16)
                   && (rxBuffer_SI4432[4] == CS))
            {
                 
                  control_byte = rxBuffer_SI4432[2];
                  switch(control_byte)
                  {
                     case 0xC1:
                       {
                             //IE1 |= WDTIE;// Enable WDT interrupt
                             //设置Timer_B
                             TA1CCR0 = 10000;//定时0.01s
                             TA1CCTL0 = CCIE;
                             //TA1CTL = TASSEL_2 + MC_1 + ID_3 + TAIE; // SMCLK, up mode
                             TA1CTL = TASSEL_2 + MC_1 + ID_3;
                             
                             //设置Timer_A
                             //TACTL = TACLR+MC1+TAIE;
                             TACTL = TACLR+MC_1;//+TAIE;//2013.12.13，MC1=0，MC0=1
                             CCTL0 = CCIE;
                             //CCR0=10;
                             if(LIFlag) CCR0=10;else CCR0=20;
                             
                             Working_flag = 1;//接收到正确命令后开始工作，不再接收命令了
                       }break;
                    
                     default: //命令码无效不应答 
                             MemClear_rxBuffer();
                  } 
                  
            }  
         }  		        
      //reset the RX FIFO
       SpiWriteRegister(0x08, 0x02);			//write 0x02 to the Operating Function Control 2 register
       SpiWriteRegister(0x08, 0x00);			//write 0x00 to the Operating Function Control 2 register
       RFSetRxMode();//发射结束后，执行此句，改为接收模式
       
    } 
}


