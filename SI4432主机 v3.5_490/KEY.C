//延时MS
void DelayMs(WORD MsNum)
{
        BYTE Ti;
	for(;MsNum>0;MsNum--) 
		for(Ti=0;Ti<100;Ti++);
}
unsigned char KeyScan(void)//键盘扫描
{

	if(KeyMark==1)
	{
		if(keyin1!=0X04)
		{
			DelayMs(20);
			if(keyin1!=0X04)//K1生效
			{
                            Cn=0;
                            while(keyin1!=0X04)
                            {
   			        DelayMs(10);
                                Cn++;
                                if(Cn>100)break;
                            }
   			    if(Cn>100)//K1生效
   			    {
   
   				KeyMark=0;
   				return(11);
   			    }
                            else
                            {
   				KeyMark=0;
                                return(1);
                            }
			}
                }            
		else if(keyin2!=0X02)
		{
		        DelayMs(20);
			if(keyin2!=0X02)//K2生效
			{
                            Cn=0;
                            while(keyin2!=0X02)
                            {
   			        DelayMs(10);
                                Cn++;
                                if(Cn>100)break;
                            }
   			    if(Cn>100)//K1生效
   			    {
   
   				KeyMark=0;
   				return(22);
   			    }
                            else
                            {
   				KeyMark=0;
                                return(2);
                            }
			}
		}
		else if(keyin3!=0X01)
		{
			DelayMs(20);
			if(keyin3!=0X01)//K1生效
			{
                            Cn=0;
                            while(keyin3!=0X01)
                            {
   			        DelayMs(10);
                                Cn++;
                                if(Cn>100)break;
                            }
   			    if(Cn>100)//K1生效
   			    {
   
   				KeyMark=0;
   				return(33);
   			    }
                            else
                            {
   				KeyMark=0;
                                return(3);
                            }
			}
		}
		else
		{
			return(0);
		}
	}
	else
	{
		if((keyin1==0X04)&&(keyin2==0X02)&&(keyin3==0X01))//释放键盘
		{
			DelayMs(20);
			if((keyin1==0X04)&&(keyin2==0X02)&&(keyin3==0X01)) KeyMark=1;
		}
		return(0);
	}
}