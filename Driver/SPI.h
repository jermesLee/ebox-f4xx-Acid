#include <stm32f4xx.h>

/* SPI�����ٶ�����*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

void SPI_Configuration(void);
void SPI_SetSpeed(u8 SpeedSet);
u8 SPI_ReadWriteByte(u8 TxData);                //SPI���߶�дһ���ֽ�
