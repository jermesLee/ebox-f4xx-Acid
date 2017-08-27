#include "TFT.h"
#include "FONT.h"
#include "stdlib.h"  
#include "function.h"
	 
					 
//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


void TFT_FSMC_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+------------------+
 | PD0  <-> FSMC_D2  | PE0  <-> FSMC_NBL0 | PF0 <-> FSMC_A0  | PG0 <-> FSMC_A10 |
 | PD1  <-> FSMC_D3  | PE1  <-> FSMC_NBL1 | PF1 <-> FSMC_A1  | PG1 <-> FSMC_A11 |
 | PD4  <-> FSMC_NOE | PE2  <-> FSMC_A23  | PF2 <-> FSMC_A2  | PG2 <-> FSMC_A12 |
 | PD5  <-> FSMC_NWE | PE3  <-> FSMC_A19  | PF3 <-> FSMC_A3  | PG3 <-> FSMC_A13 |
 | PD8  <-> FSMC_D13 | PE4  <-> FSMC_A20  | PF4 <-> FSMC_A4  | PG4 <-> FSMC_A14 |
 | PD9  <-> FSMC_D14 | PE5  <-> FSMC_A21  | PF5 <-> FSMC_A5  | PG5 <-> FSMC_A15 |
 | PD10 <-> FSMC_D15 | PE6  <-> FSMC_A22  | PF12 <-> FSMC_A6 | PG9 <-> FSMC_NE2 |
 | PD11 <-> FSMC_A16 | PE7  <-> FSMC_D4   | PF13 <-> FSMC_A7 |------------------+
 | PD12 <-> FSMC_A17 | PE8  <-> FSMC_D5   | PF14 <-> FSMC_A8 |
 | PD13 <-> FSMC_A18 | PE9  <-> FSMC_D6   | PF15 <-> FSMC_A9 |
 | PD14 <-> FSMC_D0  | PE10 <-> FSMC_D7   |------------------+
 | PD15 <-> FSMC_D1  | PE11 <-> FSMC_D8   |
 +-------------------| PE12 <-> FSMC_D9   |
                     | PE13 <-> FSMC_D10  |
                     | PE14 <-> FSMC_D11  |
                     | PE15 <-> FSMC_D12  |
                     +--------------------+
*/

  /* FSMC NE1 configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
/*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime = 3;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 6;
  p.FSMC_BusTurnAroundDuration = 1;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /*!< Enable FSMC Bank1_SRAM2 Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
} 



//д�Ĵ���
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_REG=LCD_Reg;  
	LCD_RAM=LCD_RegValue;	    		 
}	   
//���Ĵ���
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_REG=LCD_Reg;    //д��Ҫ���ļĴ�����  
  return (LCD_RAM);
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_REG=R202;
}	 
//LCDдGRAM
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_RAM=RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}		 
//��ȡ��ĳ�����ɫֵ	 
//x:0~239
//y:0~319
//����ֵ:�˵����ɫ
// u16 LCD_ReadPoint(u16 x,u16 y)
// {
// 	u16 t;	
// 	if(x>=LCD_W||y>=LCD_H)return 0;//�����˷�Χ,ֱ�ӷ���		   
// 	LCD_SetCursor(x,y);
// 	LCD_WR_REG(R202);       //ѡ��GRAM��ַ 
// 	GPIOB->CRL=0X88888888; //PB0-7  ��������
// 	GPIOB->CRH=0X88888888; //PB8-15 ��������
// 	GPIOB->ODR=0XFFFF;     //ȫ�������
// #ifdef LCD_FAST_IO //����IO
// 	LCD_RS_SET;
// 	LCD_CS_CLR;
// 	//��ȡ����(��GRAMʱ,��Ҫ��2��)
// 	LCD_RD_CLR;					   
// 	LCD_RD_SET;
// 	delay_us(2);//FOR 9320,��ʱ2us					   
// 	//dummy READ
// 	LCD_RD_CLR;					   
// 	delay_us(2);//FOR 8989,��ʱ2us					   
// 	LCD_RD_SET;
// 	t=DATAIN;  
// 	LCD_CS_SET;
// #else
// 	LCD_RS=1;
// 	LCD_CS=0;
// 	//��ȡ����(��GRAMʱ,��Ҫ��2��)
// 	LCD_RD=0;					   
//  	LCD_RD=1;
//  	//dummy READ
// 	LCD_RD=0;					   
//  	LCD_RD=1;
// 	t=DATAIN;  
// 	LCD_CS=1;  
// #endif 
// 	GPIOB->CRL=0X33333333; //PB0-7  �������
// 	GPIOB->CRH=0X33333333; //PB8-15 �������
// 	GPIOB->ODR=0XFFFF;    //ȫ�������  
// 	if(DeviceCode==0X4531||DeviceCode==0X8989)return t;//4531/8989����IC
// 	else return LCD_BGR2RGB(t);
// }
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(R7, 0x0100); //26��ɫ��ʾ����
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
#ifdef USE_HORIZONTAL
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Ypos);
		LCD_WriteReg(0X4F, 319-Xpos);
    }else
	{
		LCD_WriteReg(R200, Ypos);
		LCD_WriteReg(R201, 399-Xpos);
	}							   
#else
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Xpos);
		LCD_WriteReg(0X4F, Ypos);
    }else
	{
		LCD_WriteReg(R200, Xpos);
		LCD_WriteReg(R201, Ypos);
	}						    
#endif
}  
//����
//x:0~239
//y:0~319
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_REG=R202;//��ʼд��GRAM
	LCD_RAM=POINT_COLOR; 
} 	 
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_Init(void)
{
	TFT_FSMC_Init();
	Delayms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	Delayms(50); // delay 50 ms 	
 	LCD_WriteReg(0x000,0x0000);
 	LCD_WriteReg(0x000,0x0000);
 	LCD_WriteReg(0x000,0x0000);
 	LCD_WriteReg(0x000,0x0000);
	Delayms(15);

	LCD_WriteReg(0x008,0x0808);

	LCD_WriteReg(0x010,0x0010);
	
	LCD_WriteReg(0x400,0x6200);
	
	LCD_WriteReg(0x300,0x0c06);//GAMMA
	LCD_WriteReg(0x301,0x9d0f);
	LCD_WriteReg(0x302,0x0b05);
	LCD_WriteReg(0x303,0x1217);
	LCD_WriteReg(0x304,0x3333);
	LCD_WriteReg(0x305,0x1712);
	LCD_WriteReg(0x306,0x950b);	
	LCD_WriteReg(0x307,0x0f0d);
	LCD_WriteReg(0x308,0x060c); 
	LCD_WriteReg(0x309,0x0000);

	LCD_WriteReg(0x011,0x0202);
	LCD_WriteReg(0x012,0x0101);
	LCD_WriteReg(0x013,0x0001);

	LCD_WriteReg(0x007,0x0001);
	LCD_WriteReg(0x100,0x0730);//BT,AP 0x0330
	LCD_WriteReg(0x101,0x0237);//DC0,DC1,VC
	LCD_WriteReg(0x103,0x2b00);//VDV	//0x0f00
	LCD_WriteReg(0x280,0x4000);//VCM
	LCD_WriteReg(0x102,0x81b0);//VRH,VCMR,PSON,PON
	Delayms(15);

	LCD_WriteReg(0x001,0x0100);
	LCD_WriteReg(0x002,0x0100);
	LCD_WriteReg(0x003,0x1028);//1030
	LCD_WriteReg(0x009,0x0001);
	
	LCD_WriteReg(0x0C,0x0000);	//MCU interface 
//  	LCD_WriteReg(0x0C,0x0110);	//RGB interface 18bit
//   LCD_WriteReg(0x0C,0x0111);	//RGB interface 16bit
//   LCD_WriteReg(0x0C,0x0020);	//VSYNC interface

	LCD_WriteReg(0x090,0x8000);
	LCD_WriteReg(0x00f,0x0000);

	LCD_WriteReg(0x210,0x0000);
	LCD_WriteReg(0x211,0x0ef);//ef
	LCD_WriteReg(0x212,0x0000);
	LCD_WriteReg(0x213,0x018f);//18f

	LCD_WriteReg(0x500,0x0000);
	LCD_WriteReg(0x501,0x0000);
	LCD_WriteReg(0x502,0x005f);
	LCD_WriteReg(0x401,0x0001);
	LCD_WriteReg(0x404,0x0000);
	Delayms(15);	
	LCD_WriteReg(0x007,0x0100);
	Delayms(15);
	LCD_WriteReg(0x200,0x0000);
	LCD_WriteReg(0x201,0x0000);

//	LCD_LED=1;//��������	 
	LCD_Clear(BLUE);
}  		  
  
//��������
//Color:Ҫ���������ɫ
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<96000;index++)
	{
		LCD_RAM=(Color);    
	}
}  
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
#ifdef USE_HORIZONTAL
	xlen=yend-ysta+1;	   
	for(i=xsta;i<=xend;i++)
	{
	 	LCD_SetCursor(i,ysta);      //���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_RAM=(color);//���ù��λ�� 	  
	}
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{
	 	LCD_SetCursor(xsta,i);      //���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_RAM=(color);//���ù��λ�� 	    
	}
#endif						  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  
#ifdef USE_HORIZONTAL
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 
#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#endif 
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ2������
//x,y:�������
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,0);
        x+=8;
        p++;
    }  
}






























