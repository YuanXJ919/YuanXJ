
//**********************************************
//-----------SI4432 SPI PIN -----------
#define  SCLK     BIT6            //p1OUT---p1.6
#define  nSEL     BIT6            //p3OUT---p3.6
#define  SDI      BIT5            //p1IN---p1.5
#define  SDO      BIT4            //p1OUT---p1.4

//---------SI4432 INT and CONRAL FOR  MCU-------
#define  nIRQ     BIT4            //p2IN---p2.4
#define  SDN      BIT7            //p3OUT---p3.7

//---------SI4432 INT PIN FOR MCU-----------
#define  GPIO0    BIT1            //p1OUT---p1.1
#define  GPIO1    BIT2            //p1OUT---p1.2
#define  GPIO2    BIT3            //p1OUT---p1.3

//-----------24c02--------------------
#define  TEST_OUT   BIT4          //P3OUT---P3.4
#define  I2_WP      BIT2          //P3OUT---P3.2
#define  I2_SDA     BIT1          //P3IN_AND_OUT---3.1
#define  I2_CLK     BIT0          //P3OUT---P3.0

//------------------电池电压检测-----------------
#define  AD_A0     BIT0       //P1IN---p1.0 

//--------------控制电机部分-------------
#define  DRIVER    BIT3      //P3OUT---P3.3
#define  LED       BIT3      //P2OUT---P2.3

//-----------------键盘部分--------------
#define  KEY1     BIT1       //P2IN---P2.1
#define  KEY2     BIT2       //P2IN---P2.2
#define  KEY3     BIT0       //P2IN---P2.0

//----------------电源控制------------
#define  PWR_TEST     BIT5      //P2IN---P2.5
#define  PWR_EN   BIT5     //P3OUT---P3.5




//------------------unused pin------------------
#define  P1_unused      (~(AD_AO+GPIO0+GPIO1+GPIO2+SDI+SDO+SCLK))
#define  P2_unused      (~(KEY3+KEY1+KEY2+LED+ nIRQ+PWR_TEST))
#define  P3_unused      (~(I2_CLK+I2_SDA+I2_WP+DRIVER+TEST_OUT+PWR_EN+nSEL+SDN))

#define EepSda_In()  {P3DIR&=~I2_SDA;}  //定义数据端口方向In
#define EepSda_Out() {P3DIR|=I2_SDA;}  //定义数据端口方向Out

#define keyin1 (P2IN & 0x04)
#define keyin2 (P2IN & 0x02)
#define keyin3 (P2IN & 0x01)


