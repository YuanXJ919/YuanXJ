#define  BYTE   unsigned char
#define  WORD   unsigned int 
#define  BYTE   unsigned char
#define  WORD   unsigned int

#define VOL_MODE      0x00
#define DATE_MODE     0x55
//*************************************************//
#define		_FALSE	0x00
#define		_TRUE	0x01


//BYTE meter1[6]={0x68,0x00,0x01,0x68,0xD1,0x16};
//
//BYTE txBuffer_SI4432[6]={0};    //10-01-07
//BYTE rxBuffer_SI4432[6]={0};	//���ջ���
//BYTE rx_length,i;
//BYTE KeyBuff;
//BYTE KeyMark;
//BYTE Cn;
//BYTE Time_flag=0;
//BYTE RX_flag=0;
//BYTE LIFlag=1;
//BYTE Second_flag=0;
//BYTE CS=0;
//BYTE Temp=0;
//BYTE search_flag=0;
//BYTE Working_flag=0;//�������ڼ�������ʱ�ı�־�����ڹ���״̬�������ܽ���������������
//
//WORD count=0;
//WORD search_count=0;
//WORD Second_10count=0;
//WORD Tx_count=0;
//WORD Si4432_tx_count=0;
//WORD KeyCount; 
//WORD Temp_Count=0;

        

//*************************************************//
//EEPROM(24C02)�ĵ�ַ���壺��ַ��Χ000H-0FFH
/********************************************************/
//----------------��һ���ܵ�ַ----------
#define     EEPAddr1      1  
//----------------�ڶ����ܵ�ַ----------
#define     EEPAddr2      3 
//----------------�������ܵ�ַ----------
#define     EEPAddr3      5 
//----------------��һ���ܵ�ַ----------
#define     EEPAddr4      7  
//----------------�ڶ����ܵ�ַ----------
#define     EEPAddr5      9 
//----------------�������ܵ�ַ----------
#define     EEPAddr6      11 
//-----------------���豸��ַ-----------
#define     EEPAddr       20

struct stru_io_frame_head
{
	BYTE fir68;
	BYTE frame_c;
	BYTE scd68;
        BYTE CS;
        BYTE END;
};









