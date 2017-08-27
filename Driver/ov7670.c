#include "ov7670.h"
#include "delay.h"
#include <stm32f4xx.h>
#include "TFT.h"
void Cam_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	DCMI_InitTypeDef DCMI_InitStructure;
  	DMA_InitTypeDef  DMA_InitStructure;

  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI 
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD |
													 RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��DCMI��GPIOʱ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//MCO1:PA8
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	     
    RCC_MCO2Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_HSYNC 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D5 			  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_VSYNC 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_D6 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D7 			  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D0 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_D1 			  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI);//DCMI_D2 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D3 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_D4 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8| GPIO_Pin_9; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 

  	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
  	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  	DCMI_Init(&DCMI_InitStructure); 
  	DMA_DeInit(DMA2_Stream1);
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  	DMA_InitStructure.DMA_Memory0BaseAddr = FSMC_LCD_ADDRESS;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_BufferSize = 1;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

u8 OV7670_Init(void)
{
  	u8 i;
  	Cam_Init();
	SCCB_Init();
	OV_Reset();
	delay_ms(5);
  	for(i=0;i<OV7670_REG_NUM;i++)
  	{
    	if(OV_WriteReg(OV7670_reg[i][0],OV7670_reg[i][1]))return 1;
  	}
	return 0; 
}
void OV7660_Config_Window(unsigned int startx,unsigned int starty,unsigned int width, unsigned int height);
void Cam_Start(void)
{
	OV7660_Config_Window(272,16,400,240);
//	LCD_SetCursor(0,0);
	  LCD_REG=R202;//��ʼд��GRAM 	
  	DMA_Cmd(DMA2_Stream1, ENABLE); 
  	DCMI_Cmd(ENABLE); 
  	DCMI_CaptureCmd(ENABLE); 
}	
void OV7660_Config_Window(unsigned int startx,unsigned int starty,unsigned int width, unsigned int height)
{
	unsigned int endx=(startx+width);
	unsigned int endy=(starty+(height<<1));
	unsigned char  temp_reg1,temp_reg2;
	unsigned char  temp;
	
	OV_ReadReg(0x03,&temp_reg1);
	temp_reg1&=0xC0;
	OV_ReadReg(0x32,&temp_reg2);
	temp_reg2&=0xC0;
	
	// Horizontal
	temp=temp_reg2|((endx&0x7)<<3)|(startx&0x7);
	OV_WriteReg(0x32,temp);
	temp=(startx&0x7F8)>>3;
	OV_WriteReg(0x17,temp);
	temp=(endx&0x7F8)>>3;
	OV_WriteReg(0x18,temp);
	
	// Vertical
	temp=temp_reg1|((endy&0x7)<<3)|(starty&0x7);
	OV_WriteReg(0x03, temp );
	temp=(starty&0x7F8)>>3;
	OV_WriteReg(0x19, temp );
	temp=(endy&0x7F8)>>3;
	OV_WriteReg(0x1A, temp );
}

/////////////////////////////////////
void SCCB_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;  //SCCB_SIC:PE2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//SCCB_SID:PE3
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void SCCB_SID_OUT(void)//����SCCB_SIDΪ���
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
void SCCB_SID_IN(void)//����SCCB_SIDΪ����
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void SCCB_Start(void)
{
    SCCB_SID_H();     //�����߸ߵ�ƽ
    delay_us(50);
    SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
    delay_us(50);
}

void SCCB_Stop(void)
{
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_H();	
    delay_us(50);  
    SCCB_SID_H();	
    delay_us(50);  
}

void noAck(void)
{	
	SCCB_SID_H();	
	delay_us(50);	
	SCCB_SIC_H();	
	delay_us(50);	
	SCCB_SIC_L();	
	delay_us(50);	
	SCCB_SID_L();	
	delay_us(50);
}

u8 SCCB_Write(u8 m_data)
{
	u8 j,tem;

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if((m_data<<j)&0x80)SCCB_SID_H();
		else SCCB_SID_L();
		delay_us(50);
		SCCB_SIC_H();	
		delay_us(50);
		SCCB_SIC_L();	
		delay_us(50);
	}
	delay_us(10);
	SCCB_DATA_IN;
	delay_us(50);
	SCCB_SIC_H();	
	delay_us(10);
	if(SCCB_SID_STATE)tem=0;//SDA=1����ʧ��
	else tem=1;//SDA=0���ͳɹ�������1
	SCCB_SIC_L();	
	delay_us(50);	
    SCCB_DATA_OUT;
	return tem;  
}

u8 SCCB_Read(void)
{
	u8 read,j;
	read=0x00;
	
	SCCB_DATA_IN;
	delay_us(50);
	for(j=8;j>0;j--) //ѭ��8�ν�������
	{		     
		delay_us(50);
		SCCB_SIC_H();
		delay_us(50);
		read=read<<1;
		if(SCCB_SID_STATE)read=read+1; 
		SCCB_SIC_L();
		delay_us(50);
	}	
    SCCB_DATA_OUT;
	return read;
}

//дOV7670�Ĵ���
u8 OV_WriteReg(u8 regID, u8 regDat)
{
	SCCB_Start();//����SCCB ���߿�ʼ��������
	if(SCCB_Write(0x42)==0)//д��ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 1;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//������ID
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 2;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regDat)==0)//д���ݵ�������
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 3;//���󷵻�
	}
  	SCCB_Stop();//����SCCB ����ֹͣ��������	
  	return 0;//�ɹ�����
}

//��OV7660�Ĵ���
u8 OV_ReadReg(u8 regID, u8 *regDat)
{
	//ͨ��д�������üĴ�����ַ
	SCCB_Start();
	if(SCCB_Write(0x42)==0)//д��ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 1;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//������ID
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 2;//���󷵻�
	}
	SCCB_Stop();//����SCCB ����ֹͣ��������	
	delay_us(10);	
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	if(SCCB_Write(0x43)==0)//����ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 3;//���󷵻�
	}
	delay_us(10);
  	*regDat=SCCB_Read();//���ض�����ֵ
  	noAck();//����NACK����
  	SCCB_Stop();//����SCCB ����ֹͣ��������
  	return 0;//�ɹ�����
}

void OV_Reset(void)
{
	OV_WriteReg(0x12,0x80);
}

u8 OV_ReadID(void)
{
	u8 temp;
	OV_ReadReg(0x0b,&temp);
  	return temp;
}
